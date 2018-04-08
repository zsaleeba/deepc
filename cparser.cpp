#include <string>
#include <memory>

#include "cparser.h"


namespace deepC
{


CParser::CParser(std::shared_ptr<ProgramDb> pdb, const CompileArgs &args, const std::string &sourceFileName) :
    pdb_(pdb),
    args_(args),
    sourceFileName_(sourceFileName)
{

}


} // namespace deepC
