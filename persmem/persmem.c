#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

#include "persmem.h"
#include "persmem_internal.h"


/* Global variable used by pmalloc() and friends. */
void *persmem_default_pool = NULL;


/*
 * NAME:        pmopen
 * ACTION:      Opens a file-backed persistent memory pool.
 *              This handle can be used with the pmmalloc()/pmfree()
 *              family of allocation functions.
 *              Alternatively if only a single pool is required you
 *              can assign the return value to persmem_default_pool
 *              and use the pmalloc() / pfree() functions which have
 *              the same parameters as malloc() / free().
 *              When finished, close the pool with pmclose().
 * PARAMETERS:  const char *path - path to the file to open.
 *              bool writable - true if writable.
 *              bool createIfMissing - create the file if it doesn't exist.
 *              size_t masterStructSize - the size of the user structure to
 *                  use as a master reference to other data in the
 *                  persistent store.
 * RETURNS:     PersMem * - pointer to a newly allocated handle. This is freed
 *                  when pmclose() is called with it. NULL on error.
 */

PersMem *pmopen(const char *path, bool writable, bool createIfMissing, size_t masterStructSize)
{
    int openFlags;
    struct stat fileInfo;
    size_t fileSize;
    persmemControl pc;
    PersMem pm;

    /* Open the file. */
    memset(&pm, 0, sizeof(pm));
    if (writable)
    {
        pm.readOnly = false;
        openFlags = O_RDWR;
    }
    else
    {
        pm.readOnly = true;
        openFlags = O_RDONLY;
    }

    if (createIfMissing)
    {
        openFlags |= O_CREAT;
    }
    
    int fd = open(path, openFlags, 0777);
    if (fd < 0)
        return NULL;

    pm.fd = fd;

    /* Check the file size. */
    if (fstat(fd, &fileInfo) < 0)
        return NULL;

    fileSize = fileInfo.st_size;
    
    if (fileSize < sizeof(persmemControl))
    {
        if (createIfMissing)
        {
            /* Apparently we just created the file so we'd better create a new memory pool. */
            if (!persmemPoolInit(&pm, PERSMEM_INITIAL_LEVEL, masterStructSize))
                return NULL;
        }
        else
        {
            /* We're reading the file but it's got nothing much in it. */
            errno = EILSEQ;
            return NULL;
        }
    }
    else
    {
        /* Try to use this file. */
        if (writable)
        {
            /* It's an existing file. Increase the size to our full pool space. */
            if (fileSize < PERSMEM_LEVEL_BLOCK_BYTES(PERSMEM_INITIAL_LEVEL))
            {
                fileSize = PERSMEM_LEVEL_BLOCK_BYTES(PERSMEM_INITIAL_LEVEL);
            }
    
            /* Round the file size to the next power of two. */
            fileSize = persmemRoundUpPowerOf2(fileSize);
            if (ftruncate(fd, fileSize) < 0)
                return NULL;
        }
    
        /* Read the file header in. */
        if (read(fd, &pc, sizeof(pc)) != sizeof(pc))
        {
            /* No valid header - just reinitialise. */
            if (!persmemPoolInit(&pm, PERSMEM_INITIAL_LEVEL, masterStructSize))
                return NULL;
        }
        else
        {
            /* Check the file magic. */
            if (memcmp(pc.magic, PERSMEM_MAGIC, sizeof(pc.magic)) != 0 ||
                    pc.version_major != PERSMEM_FORMAT_VERSION_MAJOR ||
                    pc.version_minor != PERSMEM_FORMAT_VERSION_MINOR)
            {
                errno = EILSEQ;
                return NULL;
            }
        }
        
        /* Go back to the start of the file. */
        if (lseek(fd, SEEK_SET, 0) < 0)
            return NULL;
        
        /* Map it to the correct address. */
        if (!persmemMapFile(pm.fd, pm.readOnly, pc.mapAddr, pc.mapSize))
            return false;
    }

    /* Create the handle. */
    PersMem *pmh = calloc(sizeof(PersMem), 1);
    if (pmh == NULL)
        return NULL;
    
    memcpy(pmh, &pm, sizeof(PersMem));
    
    return pmh;
}


/*
 * NAME:        pmclose
 * ACTION:      Closes a persistent memory pool and frees resources.
 *              Ensures that the pool is written to the backing file.
 * PARAMETERS:  PersMem *pm - the pool to close and free.
 * RETURNS:     0 on success, -1 on failure and errno is set.
 */

int pmclose(PersMem *pm)
{
    if (munmap(pm->c, pm->c->mapSize) < 0)
        return -1;

    free(pm);

    return 0;
}


/*
 * NAME:        pmmalloc
 * ACTION:      Allocate a block of memory in a specific persistent store.
 * PARAMETERS:  PersMem *pm - the memory pool to use.
 *              size_t size - the size of the memory to allocate.
 * RETURNS:     void * - the allocated memory or NULL on error.
 */

void *pmmalloc(PersMem *pm, size_t size)
{
    void *mem;
    unsigned allocLevel;

    /* Try to allocate it from the current free lists. */
    unsigned needLevel = persmemFitToLevel(size);
    mem = persmemAllocBlock(pm, needLevel);
    if (mem)
        return mem;

    /* It's not possible to allocate from the current store. Increase the size of the pool. */
    if (needLevel >= pm->c->mapLevel)
    {
        /*
         * It's a big new allocation.
         * We need enough room for the existing data + the new alloc map + the new allocation. 
         */
        allocLevel = needLevel + 2; 
    }
    else
    {
        /* It's a smaller allocation. Just double the pool size. */
        allocLevel = pm->c->mapLevel + 1;
    }
        
    /* Resize the pool. */
    if (!persmemPoolResize(pm, allocLevel))
        return NULL;

    /* Now try again to allocate it. */
    return persmemAllocBlock(pm, allocLevel);
}


/*
 * NAME:        pmfree
 * ACTION:      Free a previously allocated block of memory from a
 *              specific persistent store.
 * PARAMETERS:  PersMem *pm - the memory pool to use.
 *              void *mem - the memory to free.
 */

void pmfree(PersMem *pm, void *mem)
{
    /* Free the block from the alloc map. */
    size_t index = mem - pm->c->mapAddr;
    unsigned level = persmemAllocMapFindLevel(pm->c->allocMap, index);
    persmemAllocMapSet(pm->c->allocMap, level, index, false);

    /* Add it to the free list. */
    persmemFreeListInsert(&pm->c->freeList[level], mem);
}


/*
 * NAME:        pmcalloc
 * ACTION:      Allocate a block of memory in a specific persistent store
 *              and clear it to zeroes.
 * PARAMETERS:  PersMem *pm - the memory pool to use.
 *              size_t nmemb - the number of items to allocate.
 *              size_t size - the size of each item to allocate.
 * RETURNS:     void * - the allocated memory or NULL on error.
 */

void *pmcalloc(PersMem *pm, size_t nmemb, size_t size)
{
    size_t allocSize = nmemb * size;
    void *mem = pmmalloc(pm, allocSize);
    memset(mem, 0, allocSize);

    return mem;
}


/*
 * NAME:        pmrealloc
 * ACTION:      Change the size of a previously allocated block of memory.
 *              May return a pointer to same memory (but resized) or may
 *              move the item to a new location and return a pointer to that.
 * PARAMETERS:  PersMem *pm - the memory pool to use.
 *              void *mem - the memory to reallocate.
 *              size_t size - the size of the memory to allocate.
 * RETURNS:     void * - the reallocated memory or NULL on error.
 */

void *pmrealloc(PersMem *pm, void *mem, size_t size)
{
    size_t  oldSize;
    size_t  newSize;
    size_t  copySize;
    void   *newMem;

    /* Is this just an alloc rather than a realloc? */
    if (!mem)
        return pmmalloc(pm, size);

    /* How big is the old memory block? */
    size_t index = mem - pm->c->mapAddr;
    unsigned oldLevel = persmemAllocMapFindLevel(pm->c->allocMap, index);

    /* What size do we need to allocate? */
    unsigned newLevel = persmemFitToLevel(size);

    /* If it still fits in the same block size just leave it alone. */
    if (oldLevel == newLevel)
        return mem;

    if (newLevel > oldLevel)
    {
        /* Increasing the allocation - locate the new block and copy the old data across. */
        oldSize = ((size_t)1) << oldLevel;
        newSize = ((size_t)1) << newLevel;
        copySize = min(oldSize, newSize);
        newMem = pmmalloc(pm, newSize);
        memcpy(newMem, mem, copySize);

        /* Free the old block. */
        pmfree(pm, mem);

        return newMem;
    }
    else
    {
        /* Reducing the allocation - split the block as many times as necessary. */
        size_t oldLevelOffset = (mem - pm->c->mapAddr) >> (oldLevel + PERSMEM_INITIAL_LEVEL);
        size_t newLevelOffset = (mem - pm->c->mapAddr) >> (newLevel + PERSMEM_INITIAL_LEVEL);;

        /* Mark it as unallocated in the alloc map at the old level. */
        persmemAllocMapSet(pm->c->allocMap, oldLevel, oldLevelOffset, false);

        /* Split off and free the end of the block as many times as necessary to reach the new size. */
        int level;
        for (level = oldLevel-1; level >= (int)newLevel; level--)
        {
            /* Release a block at this level. */
            persmemFreeListInsert(&pm->c->freeList[level], mem + PERSMEM_LEVEL_BLOCK_BYTES(level));
        }

        /* Mark it as allocated in the alloc map at the new level. */
        persmemAllocMapSet(pm->c->allocMap, oldLevel, newLevelOffset, true);

        return mem;
    }
}
