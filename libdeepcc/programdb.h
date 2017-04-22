/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                         Program database                           ***
 ***                                                                    ***
 ***                                         - Zik Saleeba              ***
 ***                                           2017-04-22               ***
 ***                                                                    ***
 **************************************************************************/

#ifndef DEEPC_PROGRAMDB_H
#define DEEPC_PROGRAMDB_H

#include <map>
#include <vector>
#include <string>

#include "lmdb++.h"

namespace deepC
{

//
// A program database. This stores intermediate information about a program
// which is used to quickly resume compilation on subsequent executions.
//
// Stored in this database:
// 
//  * a list of source files.
//  * the contents of the source files.
//  * the tokenised contents of each of the source files.
//  * an index of the top level declarations in each source file.
//  * a parse tree for each of the top level declarations.
//  * a compiled object for each of the top level declarations.

class ProgramDb
{
    // A map of all the source files.
    lmdb::env env_;

private:
    // Load a source file.

public:
    // Constructor for the source bag.
    ProgramDb(const std::string &filename);
    ~ProgramDb();
    
    // Source file list.
    void getSourceFiles(std::map<std::string, std::string> &sourceFiles);
    
};

}  // namespace deepC

#endif // DEEPC_PROGRAMDB_H
