#ifndef DEEPC_PERSMEM_H
#define DEEPC_PERSMEM_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/* Opaque structure for the persistent memory handle. */
typedef struct _persmemControl persmemControl;

/* Persistent memory handle info. */
typedef struct
{
    persmemControl *c;
    int             fd;
    void           *masterStruct;   /* The user's master structure which points to the rest of their storage. */
    bool            readOnly;       /* True if the file is open read only. */
    bool            wasCreated;     /* Will be set to true if there was no persistent store file so we're starting from scratch. */
} PersMem;


/* Open or create a persistency file. */
PersMem *pmopen(const char *path, bool writable, bool createIfMissing, size_t masterStructSize);
int pmclose(PersMem *pm);

/* Standard malloc style interface. To use these do persmen_default_pool = pmopen(const char *path); */
#define pmalloc(size) pmmalloc(persmem_default_pool, (size))
#define pfree(mem) pmfree(persmem_default_pool, (mem))
#define pcalloc(nmemb, size) pmcalloc(persmem_default_pool, (nmemb), (size))
#define prealloc(mem, size) pmrealloc(persmem_default_pool, (mem), (size))

/* For use when handling multiple persistent memory files at the same time. */
void *pmmalloc(PersMem *pm, size_t size);
void  pmfree(PersMem *pm, void *mem);
void *pmcalloc(PersMem *pm, size_t nmemb, size_t size);
void *pmrealloc(PersMem *pm, void *mem, size_t size);

/* Global variable used by pmalloc() and friends. */
extern void *persmem_default_pool;

#endif /* DEEPC_PERSMEM_H */

