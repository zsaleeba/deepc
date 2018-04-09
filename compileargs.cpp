#include "compileargs.h"


namespace deepC
{


CompileArgs::CompileArgs() :
    optimisationLevel_(0),
    performLink_(true),
    outputDebugSymbols_(false),
    programDbFileName_("%HOME%/.deepc/%TARGET%/%TARGET%.pdb")
{
}


void CompileArgs::substituteVariables()
{
    substituteStr(&programDbFileName_);
}


void CompileArgs::substituteStr(std::string *str)
{
    XXX - std::algorithm method?
}


} // namespace deepC
