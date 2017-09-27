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
SourceFile::SourceFile()
{
}


}  // namespace deepC
