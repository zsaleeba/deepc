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

#include "sourcefile.h"
#include "deeptypes.h"


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
public:
    // Results from "get" calls.
    enum class GetResultCode
    {
        FOUND,
        NOT_FOUND,
        ERROR
    };


    //
    // An RAII wrapper for database transactions.
    //

    class Transaction
    {
    protected:
        MDB_txn *txn_;
        bool     committed_;

    public:
        explicit Transaction(ProgramDb &pdb, bool writeable);
        ~Transaction();

        void     commit();

        MDB_txn *getTxn() { return txn_; }

        bool     getById(MDB_dbi dbi, uint32_t id, MDB_val *v);
        bool     getByString(MDB_dbi dbi, const std::string &key, MDB_val *v);
        uint32_t addRow(MDB_dbi dbi, const MDB_val &val);
        void     putRow(MDB_dbi dbi, uint32_t id, const MDB_val &val);
        void     addStringToIdMapping(MDB_dbi dbi, const std::string &str, uint32_t id);
    };


private:
    // A map of all the source files.
    MDB_env *env_;
    bool     isOpen_;

    // The sub-databases we plan to use.
    MDB_dbi  sourceFilesDbi_;
    MDB_dbi  sourceFilesIdsByFilenameDbi_;

    // Write locks for each of the sub-databases;
    std::mutex sourceFilesWriteLock_;

protected:
    // Generate a new id in sourceFiles.
    uint32_t createSourceFilesId(Transaction &txn);

public:
    // Constructor for the source bag.
    ProgramDb(const std::string &filename);
    ~ProgramDb();

    bool     isOpen() const { return isOpen_; }
    MDB_env *getEnv()       { return env_; }

    // Source file access.
    bool getSourceFileByFileId(uint32_t fileId, SourceFileOnDatabase *source);
    bool getSourceFileIdByFilename(const std::string &filename, uint32_t *fileId);
    void putSourceFile(SourceFile &source);
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
