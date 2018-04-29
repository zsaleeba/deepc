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

    rc = mdb_dbi_open(txn, "SourceFileIdsByFilename", MDB_CREATE, &sourceFileKeysDbi_);
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
// Given a Storable object with the key set, find the object id.
// Returns false if not found.
//

uint32_t ProgramDb::getId(const Storable &obj)
{
    // Create the key.
    flatbuffers::FlatBufferBuilder builder;
    obj.serialiseKey(builder);
    MDB_val key;
    key.mv_size = builder.GetSize();
    key.mv_data = reinterpret_cast<void *>(builder.GetBufferPointer());

    // Get the record.
    Transaction txn(*this, false);

    try {
        return txn.getIdByKey(getDbHandle(obj.keyDbGroup()), key);
    }
    catch (const ProgramDbException &e) {
        throw ProgramDbException(std::string("can't get id, ") + e.what());
    }
}


//
// Get an object given the database and id.
//

std::shared_ptr<Storable> ProgramDb::get(Storable::DbGroup dbg, uint32_t id)
{
    Transaction txn(*this, false);
    MDB_val val;

    try {
        // Get the record.
        if (!txn.getById(getDbHandle(dbg), id, &val))
            return nullptr;
        
        // Convert the binary form into an object.
        const fb::StoredObject *so = fb::GetStoredObject(val.mv_data);
        return Storable::create(id, *so);
    }
    catch (const ProgramDbException &e) {
        throw ProgramDbException(std::string("can't get by id ") + std::to_string(id) + ", " + e.what());
    }
}


//
// Store a SourceFile in the database.
//

void ProgramDb::put(Storable &source)
{
    std::lock_guard<std::mutex> locker(writeMutex_);
    
    // Encode the Storable item's key and content.
    MDB_val key;
    keyBuilder_.Clear();
    source.serialiseKey(keyBuilder_);
    key.mv_data = keyBuilder_.GetBufferPointer();
    key.mv_size = keyBuilder_.GetSize();

    MDB_val val;
    contentBuilder_.Clear();
    source.serialiseContent(contentBuilder_);
    val.mv_data = contentBuilder_.GetBufferPointer();
    val.mv_size = contentBuilder_.GetSize();

    MDB_dbi contentDbi = getDbHandle(source.contentDbGroup());
    MDB_dbi keyDbi     = getDbHandle(source.keyDbGroup());
    
    // Do we already know the file id?
    Transaction txn(*this, true);

    uint32_t id = source.id();
    if (id == 0)
    {
        // See if it already exists in the database.
        id = txn.getIdByKey(keyDbi, key);
    }

    // If this file has no file id, make one.
    if (id == 0)
    {
        // Store the row under a new id.
        id = txn.addRow(contentDbi, val);

        // Add the name to id lookup.
        source.setId(id);
        txn.addKeyToIdMapping(keyDbi, key, id);
    }
    else
    {
        // Store an existing row.
        txn.putRow(contentDbi, id, val);
    }

    // Commit the transaction.
    txn.commit();
}


//
// Convert a database group id to the handle for that database.
//

MDB_dbi ProgramDb::getDbHandle(Storable::DbGroup db) const
{
    switch (db)
    {
    case Storable::DbGroup::SourceFiles:    return sourceFilesDbi_;
    case Storable::DbGroup::SourceFileKeys: return sourceFileKeysDbi_;
    default:                                throw ProgramDbException("invalid db group");
    }
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
// Get an id by its key. Returns 0 if not found.
//

uint32_t ProgramDb::Transaction::getIdByKey(MDB_dbi dbi, const MDB_val &key)
{
    MDB_val val;
    int rc = mdb_get(txn_, dbi, const_cast<MDB_val *>(&key), &val);
    if (rc)
    {
        if (rc == MDB_NOTFOUND)
        {
            // Not found.
            return 0;
        }
        else
        {
            // Failed.
            throw ProgramDbException(mdb_strerror(rc));
        }
    }
    
    if (val.mv_size != sizeof(uint32_t))
        throw ProgramDbException("incorrect size object");

    return *reinterpret_cast<uint32_t *>(&val.mv_data);
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

void ProgramDb::Transaction::addKeyToIdMapping(MDB_dbi dbi, const MDB_val &key, uint32_t id)
{
    MDB_val val;
    val.mv_size = sizeof(id);
    val.mv_data = reinterpret_cast<void *>(id);

    int rc = mdb_put(txn_, dbi, const_cast<MDB_val *>(&key), &val, 0);
    if (rc)
        throw ProgramDbException(std::string("can't put mapping ") + std::to_string(id) + ": " + mdb_strerror(rc));
}


}  // namespace deepC
