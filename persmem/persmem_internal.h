#ifndef PERSMEM_INTERNAL_H
#define PERSMEM_INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "persmem.h"


#define PERSMEM_ADDRESS_64_BITS

/* File format attributes. */
#define PERSMEM_MAGIC "persmem^"
#define PERSMEM_FORMAT_VERSION_MAJOR 1
#define PERSMEM_FORMAT_VERSION_MINOR 0

/* Allocation constants. */
#define PERSMEM_INITIAL_LEVEL        15                               /* Initially 2^15 * PERSMEM_MIN_ALLOC_SIZE will be available. */
#define PERSMEM_MAX_DEPTH            26                               /* Max 2^26 * PERSMEM_MIN_ALLOC_SIZE bytes can be allocated. */
#ifdef  PERSMEM_ADDRESS_64_BITS
#define PERSMEM_MIN_ALLOC_BITSIZE    5                                /* Max 2^26 * PERSMEM_MIN_ALLOC_SIZE bytes can be allocated. */
#define PERSMEM_MIN_ALLOC_SIZE       (1 << PERSMEM_MIN_ALLOC_BITSIZE) /* Size of a pmFreeListNode rounded up to a power of two. */
#endif
#define PERSMEM_LEVEL_BLOCK_BYTES(b) (1 << ((b) + PERSMEM_MIN_ALLOC_BITSIZE))
#define PERSMEM_MEM_FROM_LEVEL_OFFSET(pm,l,o) ((pm)->c->mapAddr + ((o) << ((l) + PERSMEM_MIN_ALLOC_BITSIZE)))

/* min/max macros. */
#ifndef min
#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))
#endif


/*
 * Free list nodes are red black tree nodes.
 */

typedef struct _pmFreeListNode pmFreeListNode;
struct _pmFreeListNode
{
    pmFreeListNode *link[2];
    bool            isRed;
};


/* The free list is implemented as a red-black tree. */
typedef struct
{
    pmFreeListNode *root; /* Top of the tree */
    size_t          size; /* Number of items (user-defined) */
} pmFreeList;


/* The alloc map is allocated from an array of uint32_t.  */
typedef struct
{
    unsigned maxLevel;
    uint32_t *bitmap;
} pmAllocMap;


/* Control structure. */
struct _persmemControl
{
    char        magic[8];
    uint32_t    version_major;
    uint32_t    version_minor;
    uint32_t    checksum;
    void       *mapAddr;
    unsigned    depth;
    size_t      mapSize;
    void       *masterStruct;
    pmFreeList  freeList[PERSMEM_MAX_DEPTH];
    pmAllocMap *allocMap;
};


/* memsize.c - memory size and access. */
unsigned persmemFitToDepth(size_t i);
size_t persmemRoundUpPowerOf2(size_t i);

/* alloc.c - block allocation. */
void *persmemAllocBlockFromFreeList(PersMem *pm, unsigned needLevel);
void  persmemAllocMarkInAllocMap(PersMem *pm, void *mem, unsigned level);
void *persmemAllocBlock(PersMem *pm, unsigned needLevel);

/* pool.c - memory pool. */
bool   persmemPoolInit(PersMem *pm, unsigned newLevel, size_t masterStructSize);
bool   persmemPoolResize(PersMem *pm, unsigned newLevel);
size_t persmemPoolUsedBytes(PersMem *pm);
void  *persmemMapFile(int fd, bool readOnly, void *mapAddr, size_t mapSize);

/* allocmap.c - block allocation map. */
void     persmemAllocMapSet(pmAllocMap *map, unsigned level, size_t index, bool allocated);
bool     persmemAllocMapGet(pmAllocMap *map, unsigned level, size_t index);
unsigned persmemAllocMapFindLevel(pmAllocMap *map, size_t index);
unsigned persmemAllocMapGetBlockAllocLevel(unsigned level);

/* freelist.c - free list implemented as a red-black tree. */
void  persmemFreeListInsert(pmFreeList *fl, void *mem);
void  persmemFreeListRemove(pmFreeList *fl, void *mem);
void *persmemFreeListRemoveFirst(pmFreeList *fl);
bool  persmemFreeListFind(pmFreeList *fl, void *mem);

#endif /* PERSMEM_INTERNAL */

