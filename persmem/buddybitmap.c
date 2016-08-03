#include <stdio.h>
#include <string.h>

#include "persmem.h"
#include "persmem_internal.h"


/* Size constants. */
#define BITS_PER_BYTE 8
#define BLOCK_STATUS_PER_UINT32 (sizeof(uint32_t) * BITS_PER_BYTE)


/*
 * The buddy bitmap is a compact store of each memory block's status.
 * The status can be:
 *     0 - unallocated
 *     1 - allocated
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
 * NAME:        persmemBuddyBitmapGet
 * ACTION:      Get the status of a memory block from the buddy bitmap.
 * PARAMETERS:  pmBuddyMap *map  - the buddy bitmap to use.
 *              unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              size_t index   - the index of the block in the set of all blocks of that level.
 * RETURNS:     bool - the status of the block. free / allocated / split.
 */

bool persmemBuddyBitmapGet(pmBuddyMap *map, unsigned level, size_t index)
{
    size_t mapStart = (1 << level) - 1;
    size_t mapStepped = index << (level + 1);
    size_t mapOffset = mapStart + mapStepped;
    size_t wordOffset = mapOffset >> 5;
    size_t bitOffset = mapOffset & 0x1f;

    return (map->bitmap[wordOffset] >> bitOffset) & 0x01;
}


/*
 * NAME:        persmemBuddyBitmapSet
 * ACTION:      Set the status of a memory block in the buddy bitmap.
 * PARAMETERS:  pmBuddyMap *map  - the buddy bitmap to use.
 *              unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              size_t index   - the index of the block in the set of all blocks of that level.
 *              bool           - the status of the block. allocated / unallocated.
 */

void persmemBuddyBitmapSet(pmBuddyMap *map, unsigned level, size_t index, bool allocated)
{
    size_t mapStart = (1 << level) - 1;
    size_t mapStepped = index << (level + 1);
    size_t mapOffset = mapStart + mapStepped;
    size_t wordOffset = mapOffset >> 5;
    size_t bitOffset = mapOffset & 0x1f;

    uint32_t maskedWord = map->bitmap[wordOffset] & ~(1 << bitOffset);
    map->bitmap[wordOffset] = maskedWord | (allocated << bitOffset);
}


/*
 * NAME:        persmemBuddyBitmapFindLevel
 * ACTION:      Find the level at which the given memory is allocated. The size of the
 *              memory can be inferred from this.
 *
 *              We know that our block starts on a power of two boundary so we can
 *              skip past the leading non-zero address bits where it's branching
 *              down the tree. The block might be allocated anywhere from the largest
 *              block whch has all zeroes in the addresses below it down to the minimum
 *              block size. We stop when we find one with the "allocated" bit set.
 *
 * PARAMETERS:  pmBuddyMap *map - the buddy bitmap to use.
 *              size_t offset   - the memory offset from the start to find.
 * RETURNS:     int             - the allocated level if positive. -1 if not allocated.
 *                               0 = the lowest level, ie. the smallest block size
 */

unsigned persmemBuddyBitmapFindLevel(pmBuddyMap *map, size_t offset)
{
    int level;

    size_t levelOffset = offset >> PERSMEM_MIN_ALLOC_BITSIZE;
    int setBit = ffs(levelOffset);
    size_t index = levelOffset >> setBit;

    for (level = setBit; level >= 0; level--)
    {
        if (persmemBuddyBitmapGet(map, level, index))
        {
            /* It was allocated at this level. */
            return level;
        }

        /* Double the index and decrement the level to shift to the left child of this block. */
        index <<= 1;
    }

    /* Not found. */
    return -1;
}


/*
 * NAME:        persmemBuddyBitmapGetBlockAllocLevel
 * ACTION:      Get the allocation size level of a buddy bitmap given the pool's size level.
 * PARAMETERS:  unsigned level - the pool's level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 * RETURNS:     unsigned - the block size level of the entire buddy bitmap.
 */

unsigned persmemBuddyBitmapGetBlockAllocLevel(unsigned level)
{
    if (level >= 4)
        return level - 4;
    else
        return 0;
}


/*
 * NAME:        persmemBuddyBitmapSizeBytes
 * ACTION:      Get the size of a buddy bitmap given its level.
 * PARAMETERS:  unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 * RETURNS:     size_t - the number of bytes used by the buddy bitmap.
 */

size_t persmemBuddyBitmapSizeBytes(unsigned level)
{
    return sizeof(uint32_t) << persmemBuddyBitmapGetBlockAllocLevel(level);
}


#if 0
/*
 * NAME:        buddyBitmapSetFrom
 * ACTION:      Set the status of a memory block from a required status to a new status.
 * PARAMETERS:  pmBuddyMap *map  - the buddy bitmap to use.
 *              unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              size_t index   - the index of the block in the set of all blocks of that level.
 *              bool - the status of the block. free / allocated / split.
 */

static void buddyBitmapSetFrom(pmBuddyMap *map, unsigned level, size_t index, bool fromStatus, bool newStatus)
{
    /* Check that the block status is correct. */
    bool currentStatus = buddyBitmapGet(map, level, index);
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
 * NAME:        buddyAllocLevel
 * ACTION:      Split a block of memory at the given level.
 * PARAMETERS:  pmBuddyMap *map  - the buddy bitmap to use.
 *              unsigned allocLevel - the power of two level of the space
 *                  to allocate starting from 0 = 16 bytes, 
 *                  1 = 32 bytes, 2 = 64 bytes etc.
 *              size_t index   - the index of the block in the set of all blocks of that level.
 *              unsigned tryLevel - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              bool - the status of the block. free / allocated / split.
 */

static void buddyAllocLevel(pmBuddyMap *map, unsigned level, size_t index)
{
    buddyBitmapSet(map, level, index, true);
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
