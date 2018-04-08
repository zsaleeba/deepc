#include <iostream>
#include <fstream>

#include "sourcefile.h"


namespace deepC
{


SourceFile::SourceFile(const std::string &fileName, const std::string &sourceText, const struct timespec &modified) :
    fileName_(fileName),
    modified_(modified)
{
#if 0
    // Get the file modification time.
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0)
    {
        errorMessage_ = std::string("can't add source file, open(") + filename + "): " + strerror(errno);
        return false;    
    }
    
    struct stat fileInfo;
    if (fstat(fd, &fileInfo))
    {
        errorMessage_ = std::string("can't add source file, fstat(") + filename + "): " + strerror(errno);
        close(fd);
        return false;    
    }
    
    // Get the new source text.
    void *mapFile = mmap(NULL, fileInfo.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (mapFile == MAP_FAILED)
    {
        errorMessage_ = std::string("can't add source file, mmap(") + filename + "): " + strerror(errno);
        close(fd);
        return false;    
    }
#endif
    
}


} // namespace deepC
