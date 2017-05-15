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

#include "sourcediff.h"


namespace deepC
{

//
// A container to keep track of all the source files. This loads the files
// and keeps track of them so we don't need to reload them.
//

// Constructor for the source bag.
SourceDiff::SourceDiff(const std::string &filename) : filename_(filename)
{
}




}  // namespace deepC
