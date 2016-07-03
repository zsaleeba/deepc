#ifndef DEEPC_DCDBFILE_H
#define DEEPC_DCDBFILE_H

#include <stdlib.h>
#include <stdint.h>

typedef enum 
{
    DDBT
} DcDbBlockType;

typedef struct
{
    DcDbBlock    *next;
    size_t        size;
    DcDbBlockType typ;
} DcDbBlock;

typedef struct 
{
    uint8_t    magic[8];
    uint32_t   version;
    uint32_t   isClean;
    void      *mapAddress;
    DcDbBlock *blockList;
    DcDbBlock *freeList;
} DcDb;

#endif /* DCDBFILE_H */
