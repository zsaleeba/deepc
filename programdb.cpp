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


//
// Get a source file from the database given its file id.
//

bool ProgramDb::getSourceFileByFileId(uint32_t fileId, SourceFileOnDatabase *source)
{
    Transaction txn(*this, false);
    MDB_val v;

    // Get the record.
    try {
        if (!txn.getById(sourceFilesDbi_, fileId, &v))
            return false;
    }
    catch (const ProgramDbException &e) {
        throw ProgramDbException(std::string("can't get source file by file id, ") + e.what());
    }

    // Convert the binary form into a SourceFile.
    const fb::SourceFile *sf = fb::GetSourceFile(v.mv_data);
    std::string_view src(sf->source()->data(), sf->source()->size());
    source->setId(fileId);
    source->setFileName(sf->filename()->str());
    source->setModified(TimePoint(Duration(sf->modified())));
    source->setSourceText(src);

    return true;
}


//
// Given the source file name, find the file id.
//

bool ProgramDb::getSourceFileIdByFilename(const std::string &filename, uint32_t *fileId)
{
    // Get the record.
    Transaction txn(*this, false);
    MDB_val v;

    try {
        if (!txn.getByString(sourceFilesIdsByFilenameDbi_, filename, &v))
            return false;
    }
    catch (const ProgramDbException &e) {
        throw ProgramDbException(std::string("can't get source file id, ") + e.what());
    }

    *fileId = *reinterpret_cast<uint32_t *>(v.mv_data);
    return true;
}


//
// Store a SourceFile in the database.
//

void ProgramDb::putSourceFile(SourceFile &source)
{
    // Encode the SourceFile data.
    flatbuffers::FlatBufferBuilder builder(source.fileName().length() + source.sourceText().length() + 1024);
    auto filenameStr = builder.CreateString(source.fileName());
    auto sourceStr = builder.CreateString(std::string(source.sourceText()));
    fb::CreateSourceFile(builder, filenameStr, sourceStr, source.modified().time_since_epoch().count());

    MDB_val v;
    v.mv_data = builder.GetBufferPointer();
    v.mv_size = builder.GetSize();

    // Do we already know the file id?
    std::lock_guard locker(sourceFilesWriteLock_);
    Transaction txn(*this, true);
    uint32_t fileId = source.id();
    if (fileId == 0)
    {
        // See if it already exists in the database.
        getSourceFileIdByFilename(source.fileName(), &fileId);
    }

    // If this file has no file id, make one.
    if (fileId == 0)
    {
        // Store the row under a new id.
        fileId = txn.addRow(sourceFilesDbi_, v);

        // Add the name to id lookup.
        txn.addStringToIdMapping(sourceFilesIdsByFilenameDbi_, source.fileName(), fileId);

        // Take note of the id.
        source.setId(fileId);
    }
    else
    {
        // Store an existing row.
        txn.putRow(sourceFilesDbi_, fileId, v);
    }

    // Commit the transaction.
    txn.commit();
}


//
// Constructor for RAII ProgramDbTransaction.
//

ProgramDb::Transaction::Transaction(ProgramDb &pdb, bool writeable) :
    txn_(nullptr),
    committed_(false)
{
    int rc = mdb_txn_begin(pdb.getEnv(), nullptr, writeable ? 0 : MDB_RDONLY, &txn_);
    if (rc)
        throw ProgramDbException(std::string("can't create transaction on program database: ") + mdb_strerror(rc));
}


//
// Destructor for RAII ProgramDbTransaction.
//

ProgramDb::Transaction::~Transaction()
{
    if (!committed_)
    {
        mdb_txn_abort(txn_);
    }
}


//
// Commit a transaction before deleting the transaction object.
//

void ProgramDb::Transaction::commit()
{
    int rc = mdb_txn_commit(txn_);
    if (rc)
    {
        mdb_txn_abort(txn_);
        throw ProgramDbException(std::string("can't commit to program database: ") + mdb_strerror(rc));
    }

    committed_ = true;
}


//
// Get an object by its database id.
//

bool ProgramDb::Transaction::getById(MDB_dbi dbi, uint32_t id, MDB_val *v)
{
    MDB_val k;
    k.mv_size = sizeof(id);
    k.mv_data = reinterpret_cast<void *>(&id);
    int rc = mdb_get(txn_, dbi, &k, v);
    if (rc)
    {
        if (rc == MDB_NOTFOUND)
        {
            // Not found.
            return false;
        }
        else
        {
            // Failed.
            throw ProgramDbException(std::string("can't get by id ") + std::to_string(id) + ": " + mdb_strerror(rc));
        }
    }

    return true;
}


//
// Get an object by its string key.
//

bool ProgramDb::Transaction::getByString(MDB_dbi dbi, const std::string &key, MDB_val *v)
{
    MDB_val k;
    k.mv_size = key.length();
    k.mv_data = reinterpret_cast<void *>(const_cast<char *>((key.data())));
    int rc = mdb_get(txn_, dbi, &k, v);
    if (rc)
    {
        if (rc == MDB_NOTFOUND)
        {
            // Not found.
            return false;
        }
        else
        {
            // Failed.
            throw ProgramDbException(std::string("can't get by key '") + key + "': " + mdb_strerror(rc));
        }
    }

    return true;
}


//
// Store an item in the database with a new unique row id.
//

uint32_t ProgramDb::Transaction::addRow(MDB_dbi dbi, const MDB_val &val)
{
    // Doesn't exist, get a new file id and add it.
    MDB_cursor *cursor = nullptr;
    int rc = mdb_cursor_open(txn_, dbi, &cursor);
    if (rc)
        throw ProgramDbException(std::string("can't create new id: ") + mdb_strerror(rc));

    // Get the last entry in the SourceFiles db.
    MDB_val numKey;
    MDB_val numData;
    rc = mdb_cursor_get(cursor, &numKey, &numData, MDB_LAST);
    if (rc)
        throw ProgramDbException(std::string("can't get last id: ") + mdb_strerror(rc));

    // Increment the file id.
    uint32_t *numDataIdPtr = reinterpret_cast<uint32_t *>(numKey.mv_data);
    (*numDataIdPtr)++;
    uint32_t id = *numDataIdPtr;

    // Write the SourceFile data.
    MDB_val key;
    key.mv_size = sizeof(id);
    key.mv_data = reinterpret_cast<void *>(id);
    rc = mdb_put(txn_, dbi, &key, const_cast<MDB_val *>(&val), 0);
    if (rc)
        throw ProgramDbException(std::string("can't add row ") + std::to_string(id) + ": " + mdb_strerror(rc));
    
    return id;
}


//
// Rewrite an item in the database given its row id.
//

void ProgramDb::Transaction::putRow(MDB_dbi dbi, uint32_t id, const MDB_val &val)
{
    MDB_val key;
    key.mv_size = sizeof(id);
    key.mv_data = reinterpret_cast<void *>(id);

    int rc = mdb_put(txn_, dbi, &key, const_cast<MDB_val *>(&val), 0);
    if (rc)
        throw ProgramDbException(std::string("can't put row ") + std::to_string(id) + ": " + mdb_strerror(rc));
}


//
// Write an item into a string to id mapping database.
// eg. filename to file id.
//

void ProgramDb::Transaction::addStringToIdMapping(MDB_dbi dbi, const std::string &str, uint32_t id)
{
    MDB_val key;
    key.mv_size = str.size();
    key.mv_data = reinterpret_cast<void *>(const_cast<char *>(str.data()));
    MDB_val val;
    val.mv_size = sizeof(id);
    val.mv_data = reinterpret_cast<void *>(id);

    int rc = mdb_put(txn_, dbi, &key, &val, 0);
    if (rc)
        throw ProgramDbException(std::string("can't put row ") + std::to_string(id) + ": " + mdb_strerror(rc));
}


}  // namespace deepC
