#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <cstring>
#include <string_view>

#include "sourcefile.h"
#include "programdb.h"
#include "deeptypes.h"


namespace deepC
{


//
// Constructor for SourceFileOnFilesystem: read a file, get the modification time and contents.
//

SourceFileOnFilesystem::SourceFileOnFilesystem(const std::string &fileName) :
    SourceFile(fileName),
    fd_(-1),
    mapData_(nullptr),
    mapSize_(0)
{
    // Open the file.
    int fd = ::open(fileName.c_str(), O_RDONLY);
    if (fd < 0)
        throw SourceFileException(std::string("can't open source file ") + fileName + ": " + strerror(errno));
    
    // Get the file modification time.
    struct stat fileInfo;
    if (fstat(fd, &fileInfo))
        throw SourceFileException(std::string("can't stat() source file ") + fileName + ": " + strerror(errno));

    // Convert to a time_point.
    int64_t nanos = static_cast<int64_t>(fileInfo.st_mtim.tv_sec) * 1000000000 + fileInfo.st_mtim.tv_nsec;
    modified_ = TimePoint(std::chrono::duration_cast<Duration>(std::chrono::nanoseconds(nanos)));

    // Memory map the file.
    mapSize_ = fileInfo.st_size;
    mapData_ = mmap(NULL, mapSize_, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (mapData_ == MAP_FAILED)
        throw SourceFileException(std::string("can't mmap() source file ") + fileName + ": " + strerror(errno));

    // Create the string_view.
    sourceText_ = std::string_view(reinterpret_cast<char *>(mapData_), mapSize_);
}


//
// Destructor for SourceFileOnFilesystem.
//

SourceFileOnFilesystem::~SourceFileOnFilesystem()
{
    if (mapData_)
    {
        munmap(mapData_, mapSize_);
    }
    
    if (fd_ >= 0)
    {
        ::close(fd_);
    }
}


//
// To store this type in the database.
//

void SourceFileOnFilesystem::store(ProgramDb &pdb)
{
    pdb.putSourceFile(*this);
}


//
// Constructor for SourceFileOnDatabase.
//

SourceFileOnDatabase::SourceFileOnDatabase(std::shared_ptr<ProgramDb> pdb, const std::string &fileName) :
    SourceFile(fileName), pdb_(pdb)
{
    pdb->getSourceFileIdByFilename(fileName, &id_);
    pdb->getSourceFileByFileId(id_, this);
}


//
// Destructor for SourceFileOnDatabase.
//

SourceFileOnDatabase::~SourceFileOnDatabase()
{
}


//
// To store this type in the database.
//

void SourceFileOnDatabase::store(ProgramDb &pdb)
{
    pdb.putSourceFile(*this);
}


} // namespace deepC
