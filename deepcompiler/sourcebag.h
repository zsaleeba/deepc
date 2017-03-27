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

#ifndef DEEPC_SOURCEBAG_H
#define DEEPC_SOURCEBAG_H

#include <map>
#include <memory>
#include <mutex>

namespace DeepC
{

    // Forward declarations.
    class SourceFile;
    
    //
    // A container to keep track of all the source files. This loads the files
    // and keeps track of them so we don't need to reload them.
    //
    
    class SourceBag
    {
        // A map of all the source files.
        std::map<std::string, std::shared_ptr<SourceFile> > sources_;
        std::mutex sourcesMutex_;
        
    private:
        // Load a source file.
        
    public:
        // Constructor for the source bag.
        SourceBag();
        
        // Get a source file, either by loading it or just returning it.
        std::shared_ptr<SourceFile> get(const std::string &sourceName);
        
        // Singleton method.
        SourceBag &instance()
        {
            static SourceBag inst;
            return inst;
        }
    };

}

#endif // DEEPC_SOURCEBAG_H
