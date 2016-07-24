#ifndef PERSMEM_INTERNAL_H
#define PERSMEM_INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/* File format attributes. */
#define PERSMEM_MAGIC "persmem^"
#define PERSMEM_FORMAT_VERSION_MAJOR 1
#define PERSMEM_FORMAT_VERSION_MINOR 0
#define PERSMEM_FORMAT_INITIAL_DEPTH 16
#define PERSMEM_FORMAT_INITIAL_SIZE ((1<<PERSMEM_FORMAT_INITIAL_DEPTH) * sizeof(void*) * 2)


/* The persistent memory implementation. */
struct _PersMem
{
    void   *mapAddr;
    size_t  mapSize;
    bool    readOnly;
    void   *startElement;
};


/* The buddy bitmap is allocated from an array of uint32_t.  */
typedef struct
{
    unsigned maxLevel;
    uint32_t *bitmap;
} pmBuddyMap;


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

/* File header. */
typedef struct
{
    char        magic[8];
    uint32_t    version_major;
    uint32_t    version_minor;
    void       *mapAddr;
    unsigned    depth;
    pmBuddyMap *buddyBitmap;
    void       *startElement;
    void       *freeList[32];
} pmFileHeader;


/* Prototypes. */
unsigned persmemFitToDepth(size_t i);
size_t persmemRoundUpPowerOf2(size_t i);
int persmemCreateNewFile(int fd);

/* Buddy bitmap prototypes. */
void persmemBuddySplit(pmBuddyMap *map, unsigned level, size_t index);
//void *persmemBuddyAlloc(pmBuddyMap *map, unsigned level);
//void persmemBuddyFree(pmBuddyMap *map, void *mem);

/* Free list prototypes. */
void persmemFreeListInsert(pmFreeList *fl, void *mem);
void persmemFreeListRemove(pmFreeList *fl, void *mem);
void *persmemFreeListRemoveFirst(pmFreeList *fl);

#endif /* PERSMEM_INTERNAL */

