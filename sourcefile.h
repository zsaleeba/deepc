#ifndef DEEPC_SOURCEFILE_H
#define DEEPC_SOURCEFILE_H

#include <string>


namespace deepC
{


class SourceFile
{
    std::string fileName_;
    std::string sourceText_;
    
public:
    SourceFile(const std::string &fileName, const std::string &sourceText);
};


} // namespace deepC

#endif // SOURCEFILE_H
