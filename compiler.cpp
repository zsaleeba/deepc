#include "compiler.h"
#include "programdb.h"
//#include "parser.h"
#include "clexer.h"


namespace deepC
{


//
// Constructor.
//

Compiler::Compiler(const CompileArgs &args) :
    args_(args)
{
    // A single instance of program database class is used throughout the run.
    pdb_ = std::make_shared<ProgramDb>(args.getProgramDbFileName());
}


//
// Performs the preprocessing stage of compilation.
//

bool Compiler::preprocess(const std::string &sourceFileName)
{
    return false;
}


//
// Lexical analysis.
//

bool Compiler::lex(const std::string &sourceFileName)
{
    // Create a lexer.
    lexer_ = std::make_shared<CLexer>(pdb_, sourceFileName);
    
    return false;
}


//
// Parses source code.
//

bool Compiler::parse(const std::string &sourceFileName)
{
    return false;
}


//
// Performs semantic analysis.
//

bool Compiler::semantic(const std::string &sourceFileName)
{
    return false;
}


//
// Optimises the internal representation.
//

bool Compiler::optimise(const std::string &sourceFileName)
{
    return false;
}


//
// Generates object code.
//

bool Compiler::codegen(const std::string &sourceFileName)
{
    return false;
}


//
// Compiles the whole program from start to end.
//

bool Compiler::compile(const std::string &sourceFileName)
{
    // Preprocess the source file.
    if (!preprocess(sourceFileName))
        return false;

    // Lexical analysis.
    if (!lex(sourceFileName))
        return false;

    // Parsing.
    if (!parse(sourceFileName))
        return false;

    // Semantic analysis.
    if (!semantic(sourceFileName))
        return false;

    // Optimisation.
    if (!optimise(sourceFileName))
        return false;

    // Code generation.
    if (!codegen(sourceFileName))
        return false;

    return true;
}


} // namespace deepC
