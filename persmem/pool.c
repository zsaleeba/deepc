#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#include "persmem_internal.h"


/*
 * NAME:        persmemMapFile
 * ACTION:      Memory maps the file.
 * PARAMETERS:  int fd - the file's fd to map.
 *              bool readOnly - true to map read-only.
 *              void *mapAddr - where to map it.
 *              size_t mapSize - how much data to map.
 * RETURNS:     bool - true on success.
 */

void *persmemMapFile(int fd, bool readOnly, void *mapAddr, size_t mapSize)
{
    int protFlags = PROT_READ;
    if (!readOnly)
    {
        protFlags |= PROT_WRITE;
    }

    mapAddr = mmap(mapAddr, mapSize, protFlags, MAP_SHARED | MAP_FIXED, fd, 0);
    if (mapAddr == MAP_FAILED)
    {
        perror("map failed");
        return NULL;
    }
    
    return mapAddr;
}


/*
 * NAME:        persmemPoolInit
 * ACTION:      Initialises a new memory pool, setting up all the
 *              structures required. A file header and a alloc map
 *              are allocated in the new memory pool. The size of the
 *              provided pool must be consistent with newLevel.
 *              At the point this is called the memory is not mapped
 *              so this function will adjust the file size and map
 *              the memory.
 * PARAMETERS:  PersMem *pm - the persmem to use.
 *              unsigned newLevel - the new maximum level of the pool, which
 *                  controls the new size. The size in bytes is:
 *                      2 ^ (PERSMEM_MIN_ALLOC_BITSIZE + newLevel)
 * RETURNS:     bool - true on success, false if out of memory.
 */

bool persmemPoolInit(PersMem *pm, unsigned newLevel, size_t masterStructSize)
{
    /* How large do we need it to be? */
    unsigned allocMapLevel = persmemAllocMapGetBlockAllocLevel(newLevel);
    size_t   controlBlockSize = persmemRoundUpPowerOf2(sizeof(persmemControl));
    size_t   masterBlockSize = persmemRoundUpPowerOf2(masterStructSize);
    size_t   allocMapSize = PERSMEM_LEVEL_BLOCK_BYTES(allocMapLevel);
    unsigned usedLevel = persmemFitToDepth(controlBlockSize + masterBlockSize + allocMapSize) + 1 - PERSMEM_MIN_ALLOC_BITSIZE;
    unsigned level = max(newLevel, usedLevel);

    /* Adjust the file size. */
    size_t fileSize = PERSMEM_LEVEL_BLOCK_BYTES(level);
    ftruncate(pm->fd, fileSize);

    /* Map it in so we can write the header. */
    pm->c = persmemMapFile(pm->fd, pm->readOnly, NULL, fileSize);
    if (pm->c == NULL)
        return false;

    /* Initialise the header. */
    persmemControl *pc = pm->c;
    memset(pc, 0, sizeof(persmemControl));
    memcpy(pc->magic, PERSMEM_MAGIC, sizeof(pc->magic));
    pc->version_major = PERSMEM_FORMAT_VERSION_MAJOR;
    pc->version_minor = PERSMEM_FORMAT_VERSION_MINOR;
    pc->mapAddr = pc;
    pc->depth = level;
    pc->mapSize = fileSize;

    /* Create the free list with an initial block of the entire size. */
    persmemFreeListInsert(&pc->freeList[PERSMEM_INITIAL_LEVEL], pc->mapAddr);

    /* Create an outboard alloc map since we can't allocate in the pool just yet. */
    /* Allocate the control block. This will always be at the start. */
    unsigned controlBlockLevel = persmemFitToDepth(controlBlockSize) - PERSMEM_MIN_ALLOC_BITSIZE;
    void *controlBlock = persmemAllocBlock(pm, controlBlockLevel);
    
    /* Allocate the master struct. */
    unsigned masterBlockLevel = persmemFitToDepth(masterBlockSize) - PERSMEM_MIN_ALLOC_BITSIZE;
    pm->masterStruct = persmemAllocBlock(pm, masterBlockLevel);
    pc->masterStruct = pm->masterStruct;

    /* Allocate the alloc map in persistent memory and copy across the temp one. */
    pc->allocMap = persmemAllocBlock(pm, allocMapLevel);

    /* Mark these three blocks as allocated in the alloc map. */
    persmemAllocMarkInAllocMap(pm, controlBlock, controlBlockLevel);
    persmemAllocMarkInAllocMap(pm, pm->masterStruct, masterBlockLevel);
    persmemAllocMarkInAllocMap(pm, pc->allocMap, allocMapLevel);

    return true;
}


/*
 * NAME:        persmemPoolResize
 * ACTION:      Expands or contracts the memory pool to make more or less
 *              space. Increases or reduces the size of the backing file,
 *              remaps it and then adds the new memory to the free list.
 *              It also resizes the alloc map.
 * PARAMETERS:  PersMem *pm - the persmem to use.
 *              unsigned newLevel - the new maximum level of the pool, which
 *                  controls the new size. The size in bytes is:
 *                      2 ^ (PERSMEM_MIN_ALLOC_BITSIZE + newLevel)
 * RETURNS:     bool - true on success.
 */

bool persmemPoolResize(PersMem *pm, unsigned newLevel)
{
    /* Unmap the file before we expand it. */
    if (munmap(pm->c, pm->c->mapSize) < 0)
        return false;

    /* Increase the file size. */
    size_t newSize = PERSMEM_LEVEL_BLOCK_BYTES(newLevel);
    if (ftruncate(pm->fd, newSize) < 0)
        return false;

    /* Remap the file. */
    pm->c = persmemMapFile(pm->fd, pm->readOnly, pm->c, newSize);
    if (pm->c == NULL)
        return false;

    /* Fix the free list. */
    unsigned level;
    if (newLevel > pm->c->depth)
    {
        /* Add the new storage to the free list. */
        for (level = pm->c->depth; level < newLevel; level++)
        {
            persmemFreeListInsert(&pm->c->freeList[level], pm->c->mapAddr + PERSMEM_LEVEL_BLOCK_BYTES(level));
        }
    }
    else
    {
        /* Remove trailing unused storage from the free list. */
        for (level = newLevel; level < pm->c->depth; level++)
        {
            persmemFreeListRemove(&pm->c->freeList[level], pm->c->mapAddr + PERSMEM_LEVEL_BLOCK_BYTES(level));
        }
    }

    /* Update the control structure. */
    void *oldAllocMap = pm->c->allocMap;
    unsigned oldAllocMapLevel = persmemAllocMapGetBlockAllocLevel(pm->c->depth);
    unsigned newAllocMapLevel = persmemAllocMapGetBlockAllocLevel(newLevel);
    size_t   oldAllocMapSize  = PERSMEM_LEVEL_BLOCK_BYTES(oldAllocMapLevel);
    size_t   newAllocMapSize  = PERSMEM_LEVEL_BLOCK_BYTES(newAllocMapLevel);
    pm->c->depth = newLevel;
    pm->c->mapSize = newSize;

    /* Resize the alloc map. */
    pmfree(pm, pm->c->allocMap);

    void *newAllocMap = persmemAllocBlockFromFreeList(pm, newAllocMapLevel);
    pm->c->allocMap = newAllocMap;
    if (newAllocMap != oldAllocMap)
    {
        /* It's not at the same location - we'll have to copy the old one across and clear any new space. */
        if (newAllocMapSize > oldAllocMapSize)
        {
            /* Expanded - copy and clear. */
            memcpy(newAllocMap, oldAllocMap, oldAllocMapSize);
            memset(newAllocMap + oldAllocMapSize, 0, newAllocMapSize - oldAllocMapSize);
        }
        else
        {
            /* Contracted - copy part. */
            memcpy(newAllocMap, oldAllocMap, newAllocMapSize);
        }
    }
    else if (newAllocMapSize > oldAllocMapSize)
    {
        /* Expanded in place. Just clear the end. */
        memset(newAllocMap + oldAllocMapSize, 0, newAllocMapSize - oldAllocMapSize);
    }

    /* Mark the new alloc map's space as allocated in the alloc map. Self-referential? You know it. */
    persmemAllocMarkInAllocMap(pm, newAllocMap, newAllocMapLevel);

    return true;
}


#if 0
/*
 * NAME:        persmemPoolCompact
 * ACTION:      Compacts the size of the backing file by shortening it
 *              to eliminate any unused space at the end.
 * PARAMETERS:  PersMem *pm - the persmem to use.
 */

void persmemPoolCompact(PersMem *pm)
{
}


/*
 * NAME:        persmemPoolUncompact
 * ACTION:      Takes a compacted backing file and increases the length
 *              to the next higher power of two, making the new space
 *              available to the allocator.
 * PARAMETERS:  PersMem *pm - the persmem to use.
 */

void persmemPoolUncompact(PersMem *pm)
{
}
#endif
