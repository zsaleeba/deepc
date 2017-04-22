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

#include "lmdb++.h"
#include "programdb.h"


namespace deepC
{

//
// A program database. This stores intermediate information about a program
// which is used to quickly resume compilation on subsequent executions.
//

ProgramDb::ProgramDb(const std::string &filename)
{
    env_ = lmdb::env::create();
    env_.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
    env_.open(filename.c_str(), 0, 0664);
}

void ProgramDb::getSourceFiles(std::map<std::string, std::string> &sourceFiles)
{
    lmdb::txn rtxn = lmdb::txn::begin(env_, nullptr, MDB_RDONLY);
    lmdb::dbi dbi = lmdb::dbi::open(rtxn, "global");
    dbi.get(rtxn(), "sources", )
    std::string key, value;
    while (cursor.get(key, value, MDB_NEXT)) {
        std::printf("key: '%s', value: '%s'\n", key.c_str(), value.c_str());
    }
    cursor.close();
    rtxn.abort();
}

void ProgramDb::putSourceFiles(const std::map<std::string, std::string> &sourceFiles)
{
    
}


}  // namespace deepC
