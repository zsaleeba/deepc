#ifndef PERSMEM_INTERNAL_H
#define PERSMEM_INTERNAL_H

#include <stddef.h>
#include <stdint.h>


/* File format attributes. */
#define PERSMEM_MAGIC "persmem^"
#define PERSMEM_FORMAT_VERSION_MAJOR 1
#define PERSMEM_FORMAT_VERSION_MINOR 0
#define PERSMEM_FORMAT_INITIAL_DEPTH 16
#define PERSMEM_FORMAT_INITIAL_SIZE ((1<<PERSMEM_FORMAT_INITIAL_DEPTH) * sizeof(void*) * 2)


/* File header. */
typedef struct
{
    char      magic[8];
    uint32_t  version_major;
    uint32_t  version_minor;
    void     *mapAddr;
    unsigned  depth;
    void     *buddyBitmap;
    void     *nextLargerBitmap;
    void     *startElement;
    void     *freeList[32];
} persmemFileHeader;


/* Prototypes. */
unsigned persmemFitToDepth(size_t i);
size_t persmemRoundUpPowerOf2(size_t i);
int persmemCreateNewFile(int fd);

/* Buddy bitmap prototypes. */

/* Free list prototypes. */
void *persmemFreeListPopFirst(unsigned depth);

#endif /* PERSMEM_INTERNAL */

