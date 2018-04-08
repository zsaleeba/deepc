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
#include <memory>
#include <lmdb.h>


namespace deepC
{


// Forward declarations.
class SourceFile;


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
    
private:
    // Load a source file.

public:
    // Constructor for the source bag.
    ProgramDb(const std::string &filename);
    ~ProgramDb();
    
    bool isOpen() const { return isOpen_; }
    
    // Source file list.
    bool getSourceFileByFileId(unsigned int fileId, std::shared_ptr<SourceFile> *source);
    bool getSourceFileIdByFilename(const std::string &filename, unsigned int *fileId);
    bool addSourceFile(std::shared_ptr<SourceFile> source, unsigned int *fileId);
};


//
// An exception thrown when the program database fails.
//

class ProgramDbException : public std::exception
{
    std::string message_;
    
public:
    ProgramDbException(const std::string &message) : message_(message) {}
    
    const char * what () const throw ()
    {
    	return message_.c_str();
    }
};


} // namespace deepC

#endif // DEEPC_PROGRAMDB_H
