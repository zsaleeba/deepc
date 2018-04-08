#ifndef DEEPC_SOURCEFILE_H
#define DEEPC_SOURCEFILE_H

#include <ctime>
#include <string>


namespace deepC
{


class SourceFile
{
    std::string     fileName_;
    std::string     sourceText_;
    struct timespec modified_;
    
public:
    SourceFile(const std::string &fileName);
    SourceFile(const std::string &fileName, const std::string &sourceText, const struct timespec &modified);
    
    const std::string     &fileName() const   { return fileName_; }
    const std::string     &sourceText() const { return sourceText_; }
    const struct timespec &modified() const   { return modified_; }
};


} // namespace deepC

#endif // SOURCEFILE_H
