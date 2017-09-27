/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                      Source file collection                        ***
 ***                                                                    ***
 ***                                         - Zik Saleeba              ***
 ***                                           2017-03-26               ***
 ***                                                                    ***
 **************************************************************************/

#include <map>
#include <memory>
#include <mutex>

#include "sourcefile.h"
#include "allsources.h"


namespace deepC
{

//
// A container to keep track of all the source files. This loads the files
// and keeps track of them so we don't need to reload them.
//

// Constructor for the source bag.
AllSources::AllSources()
{
}

// Get a source file, either by loading it or just returning it.
std::shared_ptr<SourceFile> AllSources::get(const std::string &sourceName)
{
    std::lock_guard<std::mutex> locker(sourcesMutex_);

    auto found = sources_.find(sourceName);
    if (found != sources_.end())
    {
        return (*found).second;
    }
    else
    {
        // Load the file and return it.

    }

    return nullptr;
}

}  // namespace deepC
