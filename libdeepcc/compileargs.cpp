#include <unistd.h>

#include "compileargs.h"


namespace deepC
{


CompileArgs::CompileArgs() :
    optimisationLevel_(0),
    performLink_(true),
    outputDebugSymbols_(false),
    programDbFileName_("%HOME%/.deepc/%TARGET%/%TARGET%.pdb"),
    pwd_(getenv("HOME"))
{
}


void CompileArgs::substituteVariables()
{
    substituteStr(&programDbFileName_);
}


void CompileArgs::substituteStr(std::string *str)
{
    replaceStr(str, "%HOME%", pwd_);

    std::string target = target_;
    if (target.empty())
    {
        target = "default";
    }

    replaceStr(str, "%TARGET%", target);
}


void CompileArgs::replaceStr(std::string *str, const std::string &from, const std::string &to)
{
    size_t start_pos = 0;
    while((start_pos = str->find(from, start_pos)) != std::string::npos)
    {
        str->replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

} // namespace deepC
