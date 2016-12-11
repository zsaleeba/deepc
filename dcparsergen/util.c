#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "util.h"


//
// Reads an entire file into a null-terminated string.
// This string is allocated and must be freed.
// On failure returns false and sets an error message.
//

bool ReadFile(char **content, char **errMsg, const char *fileName)
{
    // Open the file.
    int fd = open(fileName, O_RDONLY);
    if (fd == -1) 
    {
        AllocSprintf(errMsg, "can't open '%s': %s", fileName, strerror(errno));
        return false;
    }
   
    // Find out how long it is.
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1)
    {
        AllocSprintf(errMsg, "can't find the size of '%s': %s", fileName, strerror(errno));
        close(fd);
        return false;
    }
    
    // Allocate space for the result.
    *content = calloc(fileSize+1, sizeof(char));
    if (*content == NULL)
    {
        AllocSprintf(errMsg, "out of memory");
        close(fd);
        return false;
    }
    
    // Read the file.
    if (read(fd, *content, fileSize) != fileSize)
    {
        AllocSprintf(errMsg, "can't read '%s': %s", fileName, strerror(errno));
        free(*content);
        *content = NULL;
        close(fd);
        return false;
    }
    
    // Clean up.
    close(fd);
    return true;
}


//
// Allocates and formats a message, also freeing any existing message.
//

void AllocSprintf(char **strp, char *fmt, ...)
{
    // Get the args.
    va_list args;
    va_start(args, fmt);
    
    // Free any existing error string.
    if (*strp != NULL)
    {
        free(*strp);
    }
    
    // Allocate and format.
    vasprintf(strp, fmt, args);
    va_end(args);
}


//
// If a string was allocated, free it and NULL the pointer.
//

void FreeStr(char **str)
{
    if (*str != NULL)
    {
        free(*str);
        *str = NULL;
    }
}
