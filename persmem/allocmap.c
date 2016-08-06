#include <stdio.h>
#include <string.h>

#include "persmem.h"
#include "persmem_internal.h"


/* Size constants. */
#define BITS_PER_BYTE 8
#define BLOCK_STATUS_PER_UINT32 (sizeof(uint32_t) * BITS_PER_BYTE)


/*
 * The alloc map is a compact store of each memory block's status.
 * The status can be:
 *     0 - unallocated
 *     1 - allocated
 *
 * This data is arranged in a certain pattern to make it easier to expand
 * the bitmap to twice its size. These expansions occur whenever we double
 * the size of our persistent storage space.
 *
 * Just as the persistent storage space can be split down through a 
 * hierarchy of sizes the alloc map represents the possible statuses
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
 * NAME:        persmemAllocMapGet
 * ACTION:      Get the status of a memory block from the alloc map.
 * PARAMETERS:  pmAllocMap *map  - the alloc map to use.
 *              unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              size_t index   - the index of the block in the set of all blocks of that level.
 * RETURNS:     bool - the status of the block. free / allocated / split.
 */

bool persmemAllocMapGet(pmAllocMap *map, unsigned level, size_t index)
{
    size_t mapStart = (1 << level) - 1;
    size_t mapStepped = index << (level + 1);
    size_t mapOffset = mapStart + mapStepped;
    size_t wordOffset = mapOffset >> 5;
    size_t bitOffset = mapOffset & 0x1f;

    return (map->bitmap[wordOffset] >> bitOffset) & 0x01;
}


/*
 * NAME:        persmemAllocMapSet
 * ACTION:      Set the status of a memory block in the alloc map.
 * PARAMETERS:  pmAllocMap *map  - the alloc map to use.
 *              unsigned level - the level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 *              size_t index   - the index of the block in the set of all blocks of that level.
 *              bool           - the status of the block. allocated / unallocated.
 */

void persmemAllocMapSet(pmAllocMap *map, unsigned level, size_t index, bool allocated)
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
 * NAME:        persmemAllocMapFindLevel
 * ACTION:      Find the level at which the given memory is allocated. The size of the
 *              memory can be inferred from this.
 *
 *              We know that our block starts on a power of two boundary so we can
 *              skip past the leading non-zero address bits where it's branching
 *              down the tree. The block might be allocated anywhere from the largest
 *              block whch has all zeroes in the addresses below it down to the minimum
 *              block size. We stop when we find one with the "allocated" bit set.
 *
 * PARAMETERS:  pmAllocMap *map - the alloc map to use.
 *              size_t offset   - the memory offset from the start to find.
 * RETURNS:     int             - the allocated level if positive. -1 if not allocated.
 *                               0 = the lowest level, ie. the smallest block size
 */

unsigned persmemAllocMapFindLevel(pmAllocMap *map, size_t offset)
{
    int level;

    size_t levelOffset = offset >> PERSMEM_MIN_ALLOC_BITSIZE;
    int setBit = ffs(levelOffset);
    size_t index = levelOffset >> setBit;

    for (level = setBit; level >= 0; level--)
    {
        if (persmemAllocMapGet(map, level, index))
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
 * NAME:        persmemAllocMapGetBlockAllocLevel
 * ACTION:      Get the allocation size level of a alloc map given the pool's size level.
 * PARAMETERS:  unsigned level - the pool's level in the bitmap.
 *                               0 = the lowest level, ie. the smallest block size
 * RETURNS:     unsigned - the block size level of the entire alloc map.
 */

unsigned persmemAllocMapGetBlockAllocLevel(unsigned level)
{
    if (level >= 6)
    {
        /*
         * The alloc map needs (2 * 2 ^ level) bits. That's one bit for each
         * of the minimum sized blocks at the lowest level. Then half as many
         * for the next level up and so on which gives 2x this number total.
         * 
         * The "+ 1" is for 2 * size.
         * The "- PERSMEM_MIN_ALLOC_BITSIZE" gives the number of bytes per level.
         * The "- 3" gives the number of bits per byte.
         */
        return level - PERSMEM_MIN_ALLOC_BITSIZE - 3 + 1;
    }
    else
    {
        return 0;
    }
}
