#include "persmem_internal.h"


/*
 * NAME:        persmemAllocBlockFromFreeList
 * ACTION:      Allocate a block of a particular level. Splits blocks from
 *              the free list as necessary.
 * PARAMETERS:  PersMem *pm - the PersMem to use.
 *              unsigned needLevel - the level to allocate at.
 * RETURNS:     void * - a pointer to the allocated block.
 */

void *persmemAllocBlockFromFreeList(PersMem *pm, unsigned needLevel)
{
    unsigned level;

    /* Try to find a block of the smallest usable size. */
    for (level = needLevel; level <= pm->c->depth; level++)
    {
        /* Is there a block of this size in the free list? */
        void *mem = persmemFreeListRemoveFirst(&pm->c->freeList[level]);
        if (mem)
        {
            /* Got a block, do we need to cut it down to size? */
            while (level > needLevel)
            {
                /* Split this block. */
                level--;
                void *memRightHalf = mem + PERSMEM_LEVEL_BLOCK_BYTES(level);
                persmemFreeListInsert(&pm->c->freeList[level], memRightHalf);
            }

            return mem;
        }
    }

    return NULL;
}


/*
 * NAME:        persmemAllocInAllocMap
 * ACTION:      Mark memory allocated with persmemAllocBlockFromFreeList()
 *              as being allocated in the alloc map. This may be necessary
 *              if a alloc map isn't available when the initial allocation
 *              is done.
 * PARAMETERS:  PersMem *pm - the PersMem to use.
 *              void *mem - the memory which was allocated.
 *              unsigned level - the level it was allocated at.
 */

void persmemAllocMarkInAllocMap(PersMem *pm, void *mem, unsigned level)
{
    /* Mark this block as allocated in the alloc map. */
    size_t index = (mem - pm->c->mapAddr) >> (PERSMEM_MIN_ALLOC_BITSIZE + level);
    persmemAllocMapSet(pm->c->allocMap, level, index, true);
}


/*
 * NAME:        persmemAllocBlock
 * ACTION:      Allocate a block of a particular level. Levels in the alloc block
 *              hierarchy equate to power of two block sizes with the largest blocks
 *              at the top of the hierarchy with the highest level and the smallest
 *              at the bottom with the lowest level.
 *
 *              Splits blocks as necessary. Updates the free lists and alloc map.
 * PARAMETERS:  PersMem *pm - the PersMem to use.
 *              unsigned needLevel - the level to allocate at.
 * RETURNS:     void * - a pointer to the allocated block.
 */

void *persmemAllocBlock(PersMem *pm, unsigned level)
{
    /* Allocate the memory from the free list. */
    void *mem = persmemAllocBlockFromFreeList(pm, level);
    if (mem == NULL)
        return NULL;

    /* Mark this block as allocated in the alloc map. */
    persmemAllocMarkInAllocMap(pm, mem, level);

    return mem;
}


