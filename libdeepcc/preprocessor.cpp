#include "preprocessor.h"


namespace deepC
{


Preprocessor::Preprocessor(std::shared_ptr<ProgramDb> pdb, const CompileArgs &args, const std::string &sourceFileName) :
    pdb_(pdb),
    args_(args),
    sourceFileName_(sourceFileName)
{

}


} // namespace deepC
