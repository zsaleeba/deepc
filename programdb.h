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

#ifndef PROGRAMDB_H
#define PROGRAMDB_H

#include <map>
#include <vector>
#include <string>
#include <lmdb.h>

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
public:
    // Results from "get" calls.
    enum class GetResultCode
    {
        FOUND,
        NOT_FOUND,
        ERROR
    };

private:
    // A map of all the source files.
    MDB_env *env_;
    bool     isOpen_;
    
    // The sub-databases we plan to use.
    MDB_dbi  sourceFilesDbi_;
    MDB_dbi  sourceFilesIdsByFilenameDbi_;
    
    // An error message.
    std::string errorMessage_;

private:
    // Load a source file.

public:
    // Constructor for the source bag.
    ProgramDb(const std::string &filename);
    ~ProgramDb();
    
    bool isOpen() const { return isOpen_; }
    
    // Source file list.
    GetResultCode getSourceFileByFileId(unsigned int fileId, std::string *filename, std::string *source, SourceModified *modified);
    GetResultCode getSourceFileIdByFilename(const std::string &filename, unsigned int *fileId);
    bool addSourceFile(const std::string &filename, const std::string &source, const SourceModified &modified, unsigned int *fileId);
    
    // Error accessor.
    std::string getErrorMessage() const { return errorMessage_; }
};

#endif // PROGRAMDB_H
