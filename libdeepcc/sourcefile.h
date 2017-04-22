/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                            Source file                             ***
 ***                                                                    ***
 ***                                         - Zik Saleeba              ***
 ***                                           2017-03-26               ***
 ***                                                                    ***
 **************************************************************************/

#ifndef DEEPC_SOURCEFILE_H
#define DEEPC_SOURCEFILE_H

#include <map>
#include <memory>
#include <mutex>

#include "cbtree.h"


namespace deepC
{

//
// A container to keep track of all the source files. This loads the files
// and keeps track of them so we don't need to reload them.
//

class SourceFile
{
    // The source code.
    const char *source_;
    size_t len_;

    // An index of lines in the source code.
    //chain<std::string> lines_;

public:
    // Constructor for the source bag.
    SourceFile();
};

}  // namespace deepC

#endif // DEEPC_SOURCEFILE_H
