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
#include <ctype.h>

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
    
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        AllocSprintf(errMsg, "can't return to start of '%s': %s", fileName, strerror(errno));
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
    off_t bytesRead = 0;
    while (bytesRead < fileSize)
    {
        ssize_t thisRead = read(fd, *content, fileSize);
        if (thisRead == -1)
        {
            AllocSprintf(errMsg, "can't read '%s': %s", fileName, strerror(errno));
            free(*content);
            *content = NULL;
            close(fd);
            return false;
        }
        
        bytesRead += thisRead;
    }
    
    // Clean up.
    close(fd);
    return true;
}


//
// Reads a line from a string. Uses the nextLine to iterate along the string.
// Will modify the original string. This is ok when working from a string
// read using ReadFile().
// Set nextLine to point to the start of the string at the start.
//

bool ReadLine(char **line, char **nextLine)
{
    // Are we at the end?
    if (**nextLine == 0)
    {
        *line = NULL;
        return false;
    }
    
    // Find the end of the line.
    char *lfPos = index(*nextLine, '\n');
    if (lfPos == NULL)
    {
        // We're on the last line - just return the last segment.
        *line = *nextLine;
        *nextLine += strlen(*nextLine);
    }
    else
    {
        // Found a line feed - null terminate and return this line.
        *line = *nextLine;
        *lfPos = 0;
        *nextLine = lfPos+1;
    }
    
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


//
// Strip trailing whitespace from a string.
//

void StringStripEnd(char *str)
{
    ssize_t pos = strlen(str);
    if (pos > 0)
    {
        pos--;
    }
    
    while (pos >= 0 && isspace(str[pos]))
    {
        str[pos] = 0;
        pos--;
    }
}


//
// Moves a pointer to a string past any whitespace characters.
//

void SkipWhitespace(char **pos)
{
    while (isspace(**pos))
    {
        (*pos)++;
    }
}
