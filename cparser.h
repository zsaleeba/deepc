#ifndef DEEPC_CPARSER_H
#define DEEPC_CPARSER_H


#include <string>
#include <memory>


namespace deepC
{


// Forward declarations.
class ProgramDb;
class CompileArgs;


class CParser
{
private:
    std::shared_ptr<ProgramDb>  pdb_;
    const CompileArgs          &args_;
    const std::string          &sourceFileName_;
    
public:
    CParser(std::shared_ptr<ProgramDb> pdb, const CompileArgs &args, const std::string &sourceFileName);
};


}

#endif // DEEPC_CPARSER_H
