#include <string>
#include <memory>

#include "clexer.h"
#include "programdb.h"


namespace deepC
{


CLexer::CLexer(std::shared_ptr<ProgramDb> pdb, const std::string &sourceFileName) :
    pdb_(pdb),
    sourceFileName_(sourceFileName)
{

}


} // namespace deepC
