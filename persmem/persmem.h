#ifndef DEEPC_PERSMEM_H
#define DEEPC_PERSMEM_H

#include <stdlib.h>
#include <stdint.h>


/* Persistent memory handle. */
typedef struct
{
    void   *mapAddr;
    size_t  mapSize;
    bool    readOnly;
    void   *startElement;
} PERSMEM;

/* Open or create a persistency file. */
PERSMEM *pmopen(const char *path, const char *mode);
int pmclose(PERSMEM *pm);

/* Standard malloc style interface. To use these do persmen_default_pool = pmopen(const char *path); */
#define pmalloc(size) pmmalloc(persmem_default_pool, (size))
#define pfree(mem) pmfree(persmem_default_pool, (mem))
#define pcalloc(nmemb, size) pmcalloc(persmem_default_pool, (nmemb), (size))
#define prealloc(mem, size) pmrealloc(persmem_default_pool, (mem), (size))

/* For use when handling multiple persistent memory files at the same time. */
void *pmmalloc(PERSMEM *pm, size_t size);
void  pmfree(PERSMEM *pm, void *mem);
void *pmcalloc(PERSMEM *pm, size_t nmemb, size_t size);
void *pmrealloc(PERSMEM *pm, void *mem, size_t size);

/* Global variable used by pmalloc() and friends. */
extern void *persmem_default_pool;

#endif /* DEEPC_PERSMEM_H */

