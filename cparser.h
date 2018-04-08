#ifndef DEEPC_PARSER_H
#define DEEPC_PARSER_H


#include <memory>
#include <string>


namespace deepC
{


// Forward declarations.
class ProgramDb;
class CompileArgs;


class CParser
{
    std::shared_ptr<ProgramDb> pdb_;
    const CompileArgs         &args_;
    std::string                sourceFileName_;

public:
    CParser(std::shared_ptr<ProgramDb> pdb, const CompileArgs &args, const std::string &sourceFileName);
};


} namespace deepC

#endif // DEEPC_PARSER_H
