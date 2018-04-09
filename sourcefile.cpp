#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>

#include "sourcefile.h"


namespace deepC
{


SourceFile::SourceFile(const std::string &fileName, const std::string &sourceText, const TimeDate &modified) :
    fileName_(fileName),
    modified_(modified),
    sourceText_(sourceText),
    needToRead_(false)
{
}


//
// Constructor to read a file and check the modification time.
//

SourceFile::SourceFile(const std::string &fileName) :
    fileName_(fileName),
    modified_(0, 0),
    needToRead_(true)
{
    // Get the file modification time.
    struct stat fileInfo;
    if (stat(fileName.c_str(), &fileInfo))
        throw SourceFileException(std::string("can't open ") + fileName + ": " + strerror(errno));

    modified_ = TimeDate(fileInfo.st_mtim.tv_sec, fileInfo.st_mtim.tv_nsec);
}


//
// Read the source text from the file.
//

void SourceFile::readSourceText()
{
    // Get the new source text.
    std::ifstream src(fileName_);
    if (!src.is_open())
        throw SourceFileException(std::string("can't open source file ") + fileName_);

    std::stringstream srcBuf;
    srcBuf << src.rdbuf();
    sourceText_ = srcBuf.str();
    needToRead_ = false;
}


} // namespace deepC
