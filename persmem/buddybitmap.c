#include <stdio.h>

#include "persmem.h"
#include "persmem_internal.h"


/* Size constants. */
#define BITS_PER_BYTE 8
#define BLOCK_STATUS_BITS 2
#define BLOCK_STATUS_MASK ((1 << BLOCK_STATUS_BITS) - 1)
#define BLOCK_STATUS_PER_UINT32 (sizeof(uint32_t) * BITS_PER_BYTE / BLOCK_STATUS_BITS)


/*
 * Each persistent memory item can have one of four statuses -
 *     unknown   - the parent block is not split so this block can't be allocated.
 *     free      - this block is available for allocation.
 *     allocated - this block is already allocated.
 *     split     - this block is split, allocation will be below in a smaller block size.
 */

enum buddyBlockStatus
{
    BBSUnknown = 0,
    BBSFree,
    BBSAllocated,
    BBSSplit
};


/* Keep these names in sync with enum buddyBlockStatus. */
static const char *buddyBlockStatusName[4] =
{
    "unknown",
    "free",
    "allocated",
    "split"
};


/*
 * The buddy bitmap is a compact store of each memory block's status.
 * The status can be:
 *     free      - the block is current not allocated.
 *     allocated - the block is allocated.
 *     split     - the block is split into two sub-blocks of half size each.
 *
 * To save space only two bits are used to store these three possibilities.
 *
 * This data is arranged in a certain pattern to make it easier to expand
 * the bitmap to twice its size. These expansions occur whenever we double
 * the size of our persistent storage space.
 *
 * Just as the persistent storage space can be split down through a 
 * hierarchy of sizes the buddy bitmap represents the possible statuses
 * of each of the possible blocks of all possible sizes. This means that
 * it has a single status for the maximum sized block, two statuses for
 * the two sub-blocks of that block, four for the sizes down, and so on.
 *
 * The bitmap is organised in an interleaved manner. The smallest sized
 * memory blocks (at level 0) comprise more than half of all the status 
 * values so every second status value in the bitmap is a level 0 status.
 * Other values are interleaved with them.
 *
 * In this diagram level 0 values are shown as "0", level 1 as "1", etc.
 * "X" indicates an unused status. Letters denote the specific data.
 *
 * +----+----+----+----+
 * + a0 | e1 | b0 | X  |
 * +----+----+----+----+
 *
 * This represents a tree like:
 * 
 *      +----+
 *      | e1 |
 *      +----+
 *     /      \
 * +----+    +----+
 * | a0 |    | b0 |
 * +----+    +----+
 * 
 * This arrangement for a level 2 bitmap gives us the two level 0 statuses
 * and one level 1 status we need. It also allows us to easily expand it
 * to double the size:
 *
 * +----+----+----+----+----+----+----+----+
 * + a0 | e1 | b0 | g2 | c0 | f1 | d0 | X  |
 * +----+----+----+----+----+----+----+----+
 *
 * This is equivalent to a tree:
 * 
 *                +----+
 *                | g2 |
 *                +----+
 *              /        \
 *      +----+              +----+         
 *      | e1 |              | f1 |         
 *      +----+              +----+         
 *     /      \            /      \        
 * +----+    +----+    +----+    +----+    
 * | a0 |    | b0 |    | c0 |    | d0 |    
 * +----+    +----+    +----+    +----+    
 * 
 * Now we have a level 3 bitmap and don't need any complex rearrangment of
 * the members to double the size.
 */


/*
 * NAME:        buddyBitmapGet
 * ACTION:      Get the status of a memory block from the buddy bitmap.
 * PARAMETERS:  pmBuddyMap *map  - the buddy bitmap to use.
 *              unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              size_t index   - the index of the block in the set of all blocks of that level.
 * RETURNS:     enum buddyBlockStatus - the status of the block. free / allocated / split.
 */

static enum buddyBlockStatus buddyBitmapGet(pmBuddyMap *map, unsigned level, size_t index)
{
    size_t mapStart = (1 << level) - 1;
    size_t mapStepped = index << (level + 1);
    size_t mapOffset = mapStart + mapStepped;
    size_t wordOffset = mapOffset / BLOCK_STATUS_PER_UINT32;
    size_t bitOffset = (mapOffset % BLOCK_STATUS_PER_UINT32) * BLOCK_STATUS_BITS;

    return (map->bitmap[wordOffset] >> bitOffset) & BLOCK_STATUS_MASK;
}


/*
 * NAME:        buddyBitmapSet
 * ACTION:      Set the status of a memory block in the buddy bitmap.
 * PARAMETERS:  pmBuddyMap *map  - the buddy bitmap to use.
 *              unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              size_t index   - the index of the block in the set of all blocks of that level.
 *              enum buddyBlockStatus - the status of the block. free / allocated / split.
 */

static void buddyBitmapSet(pmBuddyMap *map, unsigned level, size_t offset, enum buddyBlockStatus newStatus)
{
    size_t mapStart = (1 << level) - 1;
    size_t mapStepped = offset << (level + 1);
    size_t mapOffset = mapStart + mapStepped;
    size_t wordOffset = mapOffset / BLOCK_STATUS_PER_UINT32;
    size_t bitOffset = (mapOffset % BLOCK_STATUS_PER_UINT32) * BLOCK_STATUS_BITS;

    uint32_t maskedWord = map->bitmap[wordOffset] & ~(BLOCK_STATUS_MASK << bitOffset);
    map->bitmap[wordOffset] = maskedWord | (newStatus << bitOffset);
}


/*
 * NAME:        buddyBitmapSetFrom
 * ACTION:      Set the status of a memory block from a required status to a new status.
 * PARAMETERS:  pmBuddyMap *map  - the buddy bitmap to use.
 *              unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              size_t index   - the index of the block in the set of all blocks of that level.
 *              enum buddyBlockStatus - the status of the block. free / allocated / split.
 */

static void buddyBitmapSetFrom(pmBuddyMap *map, unsigned level, size_t index, enum buddyBlockStatus fromStatus, enum buddyBlockStatus newStatus)
{
    /* Check that the block status is correct. */
    enum buddyBlockStatus currentStatus = buddyBitmapGet(map, level, index);
    if (currentStatus != fromStatus)
    {
        fprintf(stderr, "persistent memory error: can't change from %s to %s since it was %s instead\n", 
                buddyBlockStatusName[fromStatus], 
                buddyBlockStatusName[newStatus], 
                buddyBlockStatusName[currentStatus]);
        
        abort();
    }
    
    /* Set it. */
    buddyBitmapSet(map, level, index, newStatus);
}


/*
 * NAME:        persmemBuddySplit
 * ACTION:      Split a block of memory at the given level.
 * PARAMETERS:  pmBuddyMap *map  - the buddy bitmap to use.
 *              unsigned allocLevel - the power of two level of the space
 *                  to allocate starting from 0 = 16 bytes, 
 *                  1 = 32 bytes, 2 = 64 bytes etc.
 *              size_t index   - the index of the block in the set of all blocks of that level.
 *              unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              enum buddyBlockStatus - the status of the block. free / allocated / split.
 */

void persmemBuddySplit(pmBuddyMap *map, unsigned level, size_t index)
{
    /* Split the block in two. */
    size_t childIndex = index << 1;
    buddyBitmapSetFrom(map, level, index, BBSFree, BBSSplit);
    buddyBitmapSetFrom(map, level-1, childIndex, BBSUnknown, BBSFree);
    buddyBitmapSetFrom(map, level-1, childIndex+1, BBSUnknown, BBSFree);
}


#if 0
/*
 * NAME:        buddyAllocLevel
 * ACTION:      Split a block of memory at the given level.
 * PARAMETERS:  pmBuddyMap *map  - the buddy bitmap to use.
 *              unsigned allocLevel - the power of two level of the space
 *                  to allocate starting from 0 = 16 bytes, 
 *                  1 = 32 bytes, 2 = 64 bytes etc.
 *              size_t index   - the index of the block in the set of all blocks of that level.
 *              unsigned tryLevel - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              enum buddyBlockStatus - the status of the block. free / allocated / split.
 */

static void *buddyAllocLevel(pmBuddyMap *map, unsigned level, size_t index)
{
    return NULL;
}


/*
 * NAME:        persmemBuddyAlloc
 * ACTION:      Allocates memory from the buddy allocator.
 *              Will split blocks as necessary to create a block of
 *              the correct size.
 * PARAMETERS:  pmBuddyMap *map - the buddy bitmap to allocate in.
 *              unsigned level - the power of two level of the space
 *                  to allocate starting from 0 = 16 bytes, 
 *                  1 = 32 bytes, 2 = 64 bytes etc.
 * RETURNS:     A pointer to the allocated memory or NULL if out of memory.
 */

void *persmemBuddyAlloc(pmBuddyMap *map, unsigned level)
{
    return pmBuddyAllocLevel(map, level, 0);
}


/*
 * NAME:        persmemBuddyFree
 * ACTION:      Frees memory from the buddy allocator.
 *              Will coalesce buddy blocks if necessary.
 * PARAMETERS:  pmBuddyMap *map - the buddy bitmap to free in.
 *              void *mem - the memory to free.
 */

void persmemBuddyFree(pmBuddyMap *map, void *mem)
{
}

#endif
