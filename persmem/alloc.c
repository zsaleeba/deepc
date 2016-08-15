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
    for (level = needLevel; level <= pm->c->mapLevel; level++)
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


/*
 * NAME:        persmemFreeBlock
 * ACTION:      Frees a block of memory at a particular level.
 *              Checks for a free buddy memory block that this freed block
 *              can be coalesced with. The same operation is then done at
 *              the next level up so large blocks can be coalesced.
 * PARAMETERS:  PersMem *pm - the PersMem to use.
 *              unsigned level - the block's level. The size can be inferred
 *                  from this.
 *              size_t offset - the offset at the given level of the block.
 *                  If the pool was split into blocks of the given level this
 *                  would be the nth block from the start of the pool.
 */

void persmemFreeBlock(PersMem *pm, unsigned level, size_t offset)
{
    /* Coalesce this block as much as possible. */
    bool readyToFree = false;
    while (!readyToFree && level < pm->c->mapLevel - 1)
    {
        /* Can we free this block's buddy? */
        size_t buddyOffset = offset ^ 0x1;
        void  *buddyMem = PERSMEM_MEM_FROM_LEVEL_OFFSET(pm, level, buddyOffset);
        
        if (persmemFreeListFind(&pm->c->freeList[level], buddyMem))
        {
            /* Buddy existed. We're coalescing this block so remove it. */
            persmemFreeListRemove(&pm->c->freeList[level], buddyMem);

            /* Move up a level and try again. */
            level++;
        }
        else
        {
            /* Can't coalesce with our buddy. Just move on to free it. */
            readyToFree = true;
        }
    }
    
    /* Now free it for real. */
    void  *mem = PERSMEM_MEM_FROM_LEVEL_OFFSET(pm, level, offset);
    persmemFreeListInsert(&pm->c->freeList[level], mem);
}


/*
 * NAME:        persmemAllocPossible
 * ACTION:      Checks if it's possible to allocate a specific sized block
 *              of memory.
 * PARAMETERS:  PersMem *pm - the PersMem to use.
 *              unsigned needLevel - the level to allocate at.
 * RETURNS:     bool - true if it's possible to allocate the block.
 */

bool persmemAllocPossible(PersMem *pm, unsigned level)
{
    unsigned i;

    for (i = level; i < pm->c->mapLevel; i++)
    {
        if (pm->c->freeList[i].size > 0)
            return true;
    }

    return false;
}
