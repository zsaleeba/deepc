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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "programdb.h"
#include "sourcefile_generated.h"


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


ProgramDb::GetResultCode ProgramDb::getSourceFileByFileId(unsigned int fileId, std::string *filename, std::string *source, SourceModified *modified)
{
    // Open the transaction.
    MDB_txn *txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc)
    {
        errorMessage_ = std::string("can't get source file by file id, mdb_txn_begin: ") + mdb_strerror(rc);
        return ProgramDb::GetResultCode::ERROR;
    }
    
    // Get the record.
    MDB_val k;
    MDB_val v;
    k.mv_size = sizeof(unsigned int);
    k.mv_data = reinterpret_cast<void *>(&fileId);
    rc = mdb_get(txn, sourceFilesDbi_, &k, &v);
    if (!rc)
    {
        const SourceFile *sf = GetSourceFile(v.mv_data);
        
        if (filename)
        {
            *filename = sf->filename()->str();
        }
        
        if (source)
        {
            *source = sf->source()->str();
        }
        
        if (modified)
        {
            *modified = *sf->modified();
        }
        
        mdb_txn_abort(txn);
        return ProgramDb::GetResultCode::FOUND;
    }

    // Errors fall through to here.
    *filename = std::string();
    *modified = SourceModified();
    mdb_txn_abort(txn);
    
    if (rc == MDB_NOTFOUND)
    {
        return ProgramDb::GetResultCode::NOT_FOUND;
    }
    else
    {
        errorMessage_ = std::string("can't get source file by file id ") + std::to_string(fileId) + ": " + mdb_strerror(rc);
        return ProgramDb::GetResultCode::ERROR;
    }
}


ProgramDb::GetResultCode ProgramDb::getSourceFileIdByFilename(const std::string &filename, unsigned int *fileId)
{
    // Open the transaction.
    MDB_txn *txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc)
    {
        errorMessage_ = std::string("can't get source file id by filename, mdb_txn_begin: ") + mdb_strerror(rc);
        return ProgramDb::GetResultCode::ERROR;
    }
    
    // Get the record.
    MDB_val k;
    MDB_val v;
    k.mv_size = filename.length();
    k.mv_data = reinterpret_cast<void *>(const_cast<char *>(filename.data()));
    rc = mdb_get(txn, sourceFilesIdsByFilenameDbi_, &k, &v);
    if (!rc)
    {
        *fileId = *reinterpret_cast<unsigned int *>(v.mv_data);
        mdb_txn_abort(txn);
        return ProgramDb::GetResultCode::FOUND;
    }

    // Errors fall through to here.
    *fileId = 0;
    mdb_txn_abort(txn);
    
    if (rc == MDB_NOTFOUND)
    {
        return ProgramDb::GetResultCode::NOT_FOUND;
    }
    else
    {
        errorMessage_ = std::string("can't get source file id by filename ") + filename + ": " + mdb_strerror(rc);
        return ProgramDb::GetResultCode::ERROR;    
    }
}


bool ProgramDb::addSourceFile(const std::string &filename, const std::string &source, const SourceModified &modified, unsigned int *fileId)
{
#if 0
    // Get the file modification time.
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0)
    {
        errorMessage_ = std::string("can't add source file, open(") + filename + "): " + strerror(errno);
        return false;    
    }
    
    struct stat fileInfo;
    if (fstat(fd, &fileInfo))
    {
        errorMessage_ = std::string("can't add source file, fstat(") + filename + "): " + strerror(errno);
        close(fd);
        return false;    
    }
    
    // Get the new source text.
    void *mapFile = mmap(NULL, fileInfo.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (mapFile == MAP_FAILED)
    {
        errorMessage_ = std::string("can't add source file, mmap(") + filename + "): " + strerror(errno);
        close(fd);
        return false;    
    }
#endif
    
    // Encode the SourceFile data.
    flatbuffers::FlatBufferBuilder builder(filename.length() + source.length() + 1024);
    auto filenameStr = builder.CreateString(filename);
    auto sourceStr = builder.CreateString(source);
    CreateSourceFile(builder, filenameStr, sourceStr, &modified);

#if 0    
    // Close the file.
    if (munmap(mapFile, fileInfo.st_size) < 0)
    {
        errorMessage_ = std::string("can't add source file, munmap(") + filename + "): " + strerror(errno);
        close(fd);
        return false;    
    }
    
    if (close(fd) < 0)
    {
        errorMessage_ = std::string("can't add source file, close(") + filename + "): " + strerror(errno);
        return false;    
    }
#endif
    
    // Open the transaction.
    MDB_txn *txn;
    int rc = mdb_txn_begin(env_, nullptr, 0, &txn);
    if (rc)
    {
        errorMessage_ = std::string("can't add source file, mdb_txn_begin: ") + mdb_strerror(rc);
        return false;
    }
    
    // See if it already exists.
    MDB_val fileKey;
    MDB_val fileVal;
    fileKey.mv_size = filename.length();
    fileKey.mv_data = reinterpret_cast<void *>(const_cast<char *>(filename.data()));
    rc = mdb_get(txn, sourceFilesIdsByFilenameDbi_, &fileKey, &fileVal);
    if (rc != 0 && rc != MDB_NOTFOUND)
    {
        // Error getting it.
        errorMessage_ = std::string("can't add source file, mdb_get(") + filename + "): " + mdb_strerror(rc);
        mdb_txn_abort(txn);
        return false;
    }
    else if (rc == 0)
    {
        // Already exists, get the existing file id and update the source.
        *fileId = *reinterpret_cast<unsigned int *>(fileVal.mv_data);
    }
    else
    {
        // Doesn't exist, get a new file id and add it.
        MDB_cursor *cursor = nullptr;
        rc = mdb_cursor_open(txn, sourceFilesDbi_, &cursor);
        if (rc)
        {
            errorMessage_ = std::string("can't add source file, mdb_cursor_open(") + filename + "): " + mdb_strerror(rc);
            mdb_txn_abort(txn);
            return false;    
        }
        
        // Get the last entry in the SourceFiles db.
        MDB_val numKey;
        MDB_val numData;
        rc = mdb_cursor_get(cursor, &numKey, &numData, MDB_LAST);
        if (rc)
        {
            errorMessage_ = std::string("can't add source file, mdb_cursor_get(") + filename + "): " + mdb_strerror(rc);
            mdb_txn_abort(txn);
            return false;    
        }
        
        // Increment the file id.
        unsigned int *numDataIdPtr = reinterpret_cast<unsigned int *>(numKey.mv_data);
        (*numDataIdPtr)++;
        *fileId = *numDataIdPtr;
        
        // Store the name to id lookup.
        rc = mdb_put(txn, sourceFilesIdsByFilenameDbi_, &fileKey, &numData, MDB_NODUPDATA);
        if (rc)
        {
            errorMessage_ = std::string("can't add source file, mdb_put(") + std::to_string(*fileId) + "): " + mdb_strerror(rc);
            mdb_txn_abort(txn);
            return false;    
        }
    }

    // Write the SourceFile data.
    MDB_val sourceFileKey;
    MDB_val sourceFileVal;
    sourceFileKey.mv_size = sizeof(unsigned int);
    sourceFileKey.mv_data = reinterpret_cast<void *>(fileId);
    sourceFileVal.mv_size = builder.GetSize();
    sourceFileVal.mv_data = builder.GetBufferPointer();
    rc = mdb_put(txn, sourceFilesDbi_, &sourceFileKey, &sourceFileVal, 0);
    if (rc)
    {
        errorMessage_ = std::string("can't add source file, mdb_put(") + std::to_string(*fileId) + "," + filename + "): " + mdb_strerror(rc);
        mdb_txn_abort(txn);
        return false;    
    }
    
    rc = mdb_txn_commit(txn);
    if (rc)
    {
        errorMessage_ = std::string("can't add source file, mdb_txn_commit(") + filename + "): " + mdb_strerror(rc);
        mdb_txn_abort(txn);
        return false;    
    }
    
    return true;
}


}  // namespace deepC
