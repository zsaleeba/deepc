/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                Source file difference computation                  ***
 ***                                                                    ***
 ***                                         - Zik Saleeba              ***
 ***                                           2017-05-01               ***
 ***                                                                    ***
 **************************************************************************/

#ifndef DEEPC_SOURCEDIFF_H
#define DEEPC_SOURCEDIFF_H

#include <hash_map>

#include "cbtree.h"


namespace deepC
{

//
// Computes differences between two source files. This is used to work out
// what a user's changed so we can recompile only the necessary areas.
//

class SourceDiff
{
    // Which source file to diff.
    std::string filename_;

    // A set of hashes for each line of the original file.
    std::hash<std::string, std::vector<int> > oldLineHashes_;
    std::hash<std::string, std::vector<int> > newLineHashes_;

public:
    // Constructor for the source bag.
    SourceDiff(std::string filename);
};

}  // namespace deepC

#endif // DEEPC_SOURCEDIFF_H
