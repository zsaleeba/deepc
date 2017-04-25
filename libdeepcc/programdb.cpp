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

#include <sys/stat.h>

#include "programdb.h"
#include "programdb_generated.h"


namespace deepC
{

//
// A program database. This stores intermediate information about a program
// which is used to quickly resume compilation on subsequent executions.
//
// The program database is stored using an LMDB memory-mapped database.
// This database has several sub-databases:
//   * global         - stores top level information which relates to 
//                      overall operation.
//   * sourceFileById - keeps a list of known source file names indexed
//                      by their unique file ids.
//   * sourceText     - the complete source text of each source file,
//                      indexed by file id.
//

ProgramDb::ProgramDb(const std::string &filename) :
    env_(nullptr),
    isOpen_(false)
{
    // Open the environment.
    int rc = mdb_env_create(&env_);
    if (rc)
    {
        errorMessage_ = std::string("mdb_env_create: ") + mdb_strerror(rc);
        return;
    }
    
    rc = mdb_env_set_mapsize(env_, 1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
    if (rc)
    {
        errorMessage_ = std::string("mdb_env_set_mapsize: ") + mdb_strerror(rc);
        return;
    }
    
    rc = mdb_env_open(env_, filename.c_str(), 0, 0664);
    if (rc)
    {
        errorMessage_ = std::string("mdb_env_open: ") + mdb_strerror(rc);
        return;
    }
    
    rc = mdb_env_set_maxdbs(env_, 32);
    if (rc)
    {
        errorMessage_ = std::string("mdb_env_set_maxdbs: ") + mdb_strerror(rc);
        return;
    }
    
    // Open the transaction we'll use to open the databases.
    MDB_txn *txn = nullptr;
    rc = mdb_txn_begin(env_, nullptr, 0, &txn);
    if (rc)
    {
        errorMessage_ = std::string("mdb_txn_begin: ") + mdb_strerror(rc);
        return;
    }
    
    // Open the databases.
    rc = mdb_dbi_open(txn, "SourceFiles", MDB_INTEGERKEY | MDB_CREATE, &sourceFilesDbi_);
    if (rc)
    {
        errorMessage_ = std::string("mdb_dbi_open(SourceFiles): ") + mdb_strerror(rc);
        mdb_txn_abort(txn);
        return;
    }
    
    rc = mdb_dbi_open(txn, "SourceFileIdsByFilename", MDB_CREATE, &sourceFilesIdsByFilenameDbi_);
    if (rc)
    {
        errorMessage_ = std::string("mdb_dbi_open(SourceFileIdsByFilename): ") + mdb_strerror(rc);
        mdb_txn_abort(txn);
        return;
    }
    
    // Close the transaction without closing the databases.
    rc = mdb_txn_commit(txn);
    if (rc)
    {
        errorMessage_ = std::string("mdb_txn_commit: ") + mdb_strerror(rc);
        return;
    }

    isOpen_ = true;
}


ProgramDb::~ProgramDb()
{
    if (env_)
    {
        mdb_env_close(env_);
        env_ = nullptr;
    }
}


bool ProgramDb::getSourceFileByFileId(unsigned int fileId, bool *notFound, std::string *filename, SourceModified *modified)
{
    // Open the transaction.
    MDB_txn *txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc)
    {
        errorMessage_ = std::string("mdb_txn_begin: ") + mdb_strerror(rc);
        return false;
    }
    
    // Get the record.
    MDB_val k;
    MDB_val v;
    k.mv_size = sizeof(unsigned int);
    k.mv_data = reinterpret_cast<void *>(&fileId);
    rc = mdb_get(txn, sourceFilesDbi_, &k, &v);
    if (!rc)
    {
        *notFound = false;
        
        *filename = std::string(reinterpret_cast<char *>(v.mv_data), v.mv_size);
        mdb_txn_abort(txn);
        return true;
    }

    // Errors fall through to here.
    *filename = std::string();
    *modified = SourceModified();
    mdb_txn_abort(txn);
    
    if (rc == MDB_NOTFOUND)
    {
        *notFound = true;
        errorMessage_ = std::string("source file id ") + std::to_string(fileId) + " not found";
        return false;
    }
    else
    {
        *notFound = false;
        errorMessage_ = std::string("can't get source file id ") + std::to_string(fileId) + ": " + mdb_strerror(rc);
        return false;    
    }
}


bool ProgramDb::getSourceTextByFileId(unsigned int fileId, std::string *source)
{
    return false;
}


bool ProgramDb::getSourceFileIdByFilename(const std::string &filename, unsigned int *fileId)
{
    return false;
}


bool ProgramDb::addSourceFile(const std::string &filename, unsigned int *fileId)
{
    return false;
}


}  // namespace deepC
