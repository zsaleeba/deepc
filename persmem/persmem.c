#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>

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
 *              const char *mode - open mode, similar to fopen().
 *                  "r" - read only, don't create.
 *                  "r+" - read/write, don't create.
 *                  "w+" - read/write, create if it doesn't exist.
 * RETURNS:     PersMem * - pointer to a newly allocated handle. This is freed
 *                  when pmclose() is called with it. NULL on error.
 */

PersMem *pmopen(const char *path, const char *mode)
{
    bool readOnly = false;
    bool createFile = false;
    int openFlags;
    struct stat fileInfo;
    size_t fileSize;
    void *mapAddr;
    persmemFileHeader *header;
    PersMem *handle;
    int protFlags;
    long pageSize;

    /* Open the file. */
    if (strcmp(mode, "r") == 0)
    {
        openFlags = O_RDONLY;
        readOnly = true;
    }
    else if (strcmp(mode, "r+") == 0 || strcmp(mode, "w") == 0)
    {
        openFlags = O_RDWR;
    }
    else if (strcmp(mode, "w+") == 0 || strcmp(mode, "a") == 0 || strcmp(mode, "a+") == 0)
    {
        openFlags = O_CREAT | O_RDWR;
        createFile = true;
    }
    else
    {
        errno = EINVAL;
        return NULL;
    }

    int fd = open(path, openFlags);
    if (fd < 0)
        return NULL;

    /* Find the file size. */
    if (fstat(fd, &fileInfo) < 0)
        return NULL;

    fileSize = fileInfo.st_size;

    /* Check the file size. */
    if (fileSize < sizeof(persmemFileHeader))
    {
        if (createFile)
        {
            /* Apparently we just created the file so we'd better write a header. */
            if (persmemCreateNewFile(fd) < 0)
                return NULL;
        }
        else
        {
            /* We're reading the file but it's got nothing much in it. */
            errno = EILSEQ;
            return NULL;
        }
    }
    else if (!readOnly)
    {
        /* It's an existing file. Increase the size to our full pool space. */
        if (fileSize < PERSMEM_FORMAT_INITIAL_SIZE)
            fileSize = PERSMEM_FORMAT_INITIAL_SIZE;

        /* Round the file size to the next power of two. */
        fileSize = persmemRoundUpPowerOf2(fileSize);
        if (ftruncate(fd, fileSize) < 0)
            return NULL;
    }

    /* Find the system's memory page size. */
    pageSize = sysconf(_SC_PAGE_SIZE);
    if (pageSize < 0)
    {
        pageSize = 32768;
    }

    /* Map the file header in. */
    protFlags = PROT_READ;
    if (!readOnly)
    {
        protFlags |= PROT_WRITE;
    }

    mapAddr = mmap(NULL, fileSize, protFlags, MAP_SHARED, fd, 0);
    if (mapAddr == MAP_FAILED)
        return NULL;

    /* Check the file magic. */
    header = (persmemFileHeader *)mapAddr;
    if (memcmp(header->magic, PERSMEM_MAGIC, sizeof(header->magic)) != 0 || 
            header->version_major != PERSMEM_FORMAT_VERSION_MAJOR ||
            header->version_minor != PERSMEM_FORMAT_VERSION_MINOR)
    {
        errno = EILSEQ;
        return NULL;
    }

    /* Remap it to the correct address. */
    void *mapAt = header->mapAddr;
#ifdef MREMAP_FIXED
    mapAddr = mremap(mapAddr, pageSize, fileSize, MREMAP_FIXED, mapAt);
    if (mapAddr == MAP_FAILED)
        return NULL;
#else
    if (munmap(mapAddr, fileSize) < 0)
        return NULL;

    mapAddr = mmap(mapAt, fileSize, protFlags, MAP_SHARED | MAP_FIXED, fd, 0);
    if (mapAddr == MAP_FAILED)
        return NULL;
#endif

    /* Construct the handle. */
    handle = calloc(1, sizeof(PERSMEM));
    if (!handle)
    {
        errno = ENOMEM;
        return NULL;
    }

    header = (persmemFileHeader *)mapAddr;
    handle->mapAddr = mapAddr;
    handle->mapSize = fileSize;
    handle->readOnly = readOnly;
    handle->startElement = header->startElement;

    return handle;
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
    if (munmap(pm->mapAddr, pm->mapSize) < 0)
        return -1;

    free(pm);

    return 0;
}


/* For use when handling multiple persistent memory files at the same time. */
void *pmmalloc(PersMem *pm, size_t size)
{
    void *mem;

    /* What size do we need to allocate? */
    unsigned depth = persmemFitToDepth(size);

    /* Is there a block of this size in the free list? */
    mem = persmemFreeListPopFirst(depth);
    if (mem)
        return mem;

    /* Allocate from the buddy bitmap. */
    return persmemBuddyAlloc(depth);
}

void pmfree(PersMem *pm, void *mem)
{
    /* Free the block from the buddy bitmap. */
    unsigned depth = persmemBuddyFree(mem);

    /* Add it to the free list. */
    persmemFreeListInsert(mem, depth);
}

void *pmcalloc(PersMem *pm, size_t nmemb, size_t size)
{
    size_t allocSize = nmemb * size;
    void *mem = pmmalloc(pm, allocSize);
    memset(mem, 0, allocSize);
}

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
    unsigned oldDepth = persmemBuddyGetDepth(mem);

    /* What size do we need to allocate? */
    unsigned newDepth = persmemFitToDepth(size);

    /* If it still fits in the same block size just leave it alone. */
    if (oldDepth == newDepth)
        return mem;

    /* Allocate the new block and copy the old data across. */
    oldSize = ((size_t)1) << oldDepth;
    newSize = ((size_t)1) << newDepth;
    copySize = min(oldSize, newSize);
    newMem = pmmalloc(pm, newSize);
    memcpy(newMem, mem, copySize);

    /* Free the old block. */
    pmfree(pm, mem);

    return newMem;
}
