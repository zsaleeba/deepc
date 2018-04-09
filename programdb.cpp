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
#include "sourcefile.h"
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
        throw ProgramDbException(std::string("mdb_env_create: ") + mdb_strerror(rc));

    rc = mdb_env_set_mapsize(env_, 1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
    if (rc)
        throw ProgramDbException(std::string("mdb_env_set_mapsize: ") + mdb_strerror(rc));

    rc = mdb_env_open(env_, filename.c_str(), 0, 0664);
    if (rc)
        throw ProgramDbException(std::string("mdb_env_open: ") + mdb_strerror(rc));

    rc = mdb_env_set_maxdbs(env_, 32);
        throw ProgramDbException(std::string("mdb_env_set_maxdbs: ") + mdb_strerror(rc));

    // Open the transaction we'll use to open the databases.
    MDB_txn *txn = nullptr;
    rc = mdb_txn_begin(env_, nullptr, 0, &txn);
    if (rc)
        throw ProgramDbException(std::string("mdb_txn_begin: ") + mdb_strerror(rc));

    // Open the databases.
    rc = mdb_dbi_open(txn, "SourceFiles", MDB_INTEGERKEY | MDB_CREATE, &sourceFilesDbi_);
    if (rc)
    {
        mdb_txn_abort(txn);
        throw ProgramDbException(std::string("mdb_dbi_open(SourceFiles): ") + mdb_strerror(rc));
    }

    rc = mdb_dbi_open(txn, "SourceFileIdsByFilename", MDB_CREATE, &sourceFilesIdsByFilenameDbi_);
    if (rc)
    {
        mdb_txn_abort(txn);
        throw ProgramDbException(std::string("mdb_dbi_open(SourceFileIdsByFilename): ") + mdb_strerror(rc));
    }

    // Close the transaction without closing the databases.
    rc = mdb_txn_commit(txn);
    if (rc)
        throw ProgramDbException(std::string("mdb_txn_commit: ") + mdb_strerror(rc));

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


bool ProgramDb::getSourceFileByFileId(unsigned int fileId, std::shared_ptr<SourceFile> *source)
{
    // Open the transaction.
    MDB_txn *txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc)
        throw ProgramDbException(std::string("can't get source file by file id, mdb_txn_begin: ") + mdb_strerror(rc));

    // Get the record.
    MDB_val k;
    MDB_val v;
    k.mv_size = sizeof(unsigned int);
    k.mv_data = reinterpret_cast<void *>(&fileId);
    rc = mdb_get(txn, sourceFilesDbi_, &k, &v);
    if (rc)
    {
        mdb_txn_abort(txn);

        if (rc == MDB_NOTFOUND)
        {
            // Not found.
            return false;
        }
        else
        {
            // Failed.
            throw ProgramDbException(std::string("can't get source file by file id ") + std::to_string(fileId) + ": " + mdb_strerror(rc));
        }
    }

    // Convert the binary form into a SourceFile.
    const fb::SourceFile *sf = fb::GetSourceFile(v.mv_data);
    *source = std::make_shared<SourceFile>(sf->filename()->str(), sf->source()->str(), SourceFile::TimeDate(sf->modified()->seconds(), sf->modified()->nanoseconds()));

    mdb_txn_abort(txn);
    return true;
}


bool ProgramDb::getSourceFileModifiedTimeByFileId(unsigned int fileId, SourceFile::TimeDate *modified)
{
    // Open the transaction.
    MDB_txn *txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc)
        throw ProgramDbException(std::string("can't get source file by file id, mdb_txn_begin: ") + mdb_strerror(rc));

    // Get the record.
    MDB_val k;
    MDB_val v;
    k.mv_size = sizeof(unsigned int);
    k.mv_data = reinterpret_cast<void *>(&fileId);
    rc = mdb_get(txn, sourceFilesDbi_, &k, &v);
    if (rc)
    {
        mdb_txn_abort(txn);

        if (rc == MDB_NOTFOUND)
        {
            // Not found.
            return false;
        }
        else
        {
            // Failed.
            throw ProgramDbException(std::string("can't get source file by file id ") + std::to_string(fileId) + ": " + mdb_strerror(rc));
        }
    }

    // Convert the binary form into a SourceFile.
    const fb::SourceFile *sf = fb::GetSourceFile(v.mv_data);
    *modified = SourceFile::TimeDate(sf->modified()->seconds(), sf->modified()->nanoseconds());

    mdb_txn_abort(txn);
    return true;
}


bool ProgramDb::getSourceFileIdByFilename(const std::string &filename, unsigned int *fileId)
{
    // Open the transaction.
    MDB_txn *txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc)
        throw ProgramDbException(std::string("can't get source file id by filename, mdb_txn_begin: ") + mdb_strerror(rc));

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
        return true;
    }

    // Errors fall through to here.
    *fileId = 0;
    mdb_txn_abort(txn);

    if (rc == MDB_NOTFOUND)
    {
        return false;
    }
    else
    {
        throw ProgramDbException(std::string("can't get source file id by filename ") + filename + ": " + mdb_strerror(rc));
    }
}


bool ProgramDb::addSourceFile(std::shared_ptr<SourceFile> source, unsigned int *fileId)
{
    // Encode the SourceFile data.
    flatbuffers::FlatBufferBuilder builder(source->fileName().length() + source->sourceText().length() + 1024);
    auto filenameStr = builder.CreateString(source->fileName());
    auto sourceStr = builder.CreateString(source->sourceText());
    fb::SourceModified modified(source->modified().seconds(), source->modified().nanoseconds());
    fb::CreateSourceFile(builder, filenameStr, sourceStr, &modified);

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
        throw ProgramDbException(std::string("can't add source file, mdb_txn_begin: ") + mdb_strerror(rc));
    }

    // See if it already exists.
    MDB_val fileKey;
    MDB_val fileVal;
    fileKey.mv_size = source->fileName().length();
    fileKey.mv_data = reinterpret_cast<void *>(const_cast<char *>(source->fileName().data()));
    rc = mdb_get(txn, sourceFilesIdsByFilenameDbi_, &fileKey, &fileVal);
    if (rc != 0 && rc != MDB_NOTFOUND)
    {
        // Error getting it.
        mdb_txn_abort(txn);
        throw ProgramDbException(std::string("can't add source file, mdb_get(") + source->fileName() + "): " + mdb_strerror(rc));
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
            mdb_txn_abort(txn);
            throw ProgramDbException(std::string("can't add source file, mdb_cursor_open(") + source->fileName() + "): " + mdb_strerror(rc));
        }

        // Get the last entry in the SourceFiles db.
        MDB_val numKey;
        MDB_val numData;
        rc = mdb_cursor_get(cursor, &numKey, &numData, MDB_LAST);
        if (rc)
        {
            mdb_txn_abort(txn);
            throw ProgramDbException(std::string("can't add source file, mdb_cursor_get(") + source->fileName() + "): " + mdb_strerror(rc));
        }

        // Increment the file id.
        unsigned int *numDataIdPtr = reinterpret_cast<unsigned int *>(numKey.mv_data);
        (*numDataIdPtr)++;
        *fileId = *numDataIdPtr;

        // Store the name to id lookup.
        rc = mdb_put(txn, sourceFilesIdsByFilenameDbi_, &fileKey, &numData, MDB_NODUPDATA);
        if (rc)
        {
            mdb_txn_abort(txn);
            throw ProgramDbException(std::string("can't add source file, mdb_put(") + std::to_string(*fileId) + "): " + mdb_strerror(rc));
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
        mdb_txn_abort(txn);
        throw ProgramDbException(std::string("can't add source file, mdb_put(") + std::to_string(*fileId) + "," + source->fileName() + "): " + mdb_strerror(rc));
    }

    rc = mdb_txn_commit(txn);
    if (rc)
    {
        mdb_txn_abort(txn);
        throw ProgramDbException(std::string("can't add source file, mdb_txn_commit(") + source->fileName() + "): " + mdb_strerror(rc));
    }

    return true;
}


}  // namespace deepC
