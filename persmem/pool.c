#include "persmem_internal.h"

int persmemCreateNewFile(int fd)
{
    persmemFileHeader header;
    
    memset(&header, 0, sizeof(header));
    memcpy(header.magic, PERSMEM_MAGIC, 8);
    header.version_major = PERSMEM_FORMAT_VERSION_MAJOR;
    header.version_minor = PERSMEM_FORMAT_VERSION_MINOR;
    header.depth = PERSMEM_FORMAT_INITIAL_DEPTH;
    
    if (write(fd, header, sizeof(header)) < 0)
        return -1;
    
    if (ftruncate(fd, PERSMEM_FORMAT_INITIAL_SIZE) < 0)
        return -1;
    
    return 0;
}
