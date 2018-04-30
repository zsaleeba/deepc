#ifndef DEEPC_COMPILER_H
#define DEEPC_COMPILER_H

#include <memory>

#include "compileargs.h"
#include "programdb.h"


namespace deepC
{


// Forward declarations.
class Preprocessor;
class CLexer;
class CParser;


//
// The Compiler class is used to compile all the files we need to compile.
// It exists for the duration of the program so it keeps a single instance
// of the program database open even if multiple files are compiled.
//

class Compiler
{
private:
    const CompileArgs            &args_;
    
private:
    // A single instance of program database class is used throughout the run.
    std::shared_ptr<ProgramDb>    pdb_;

    // An instance of the lexer and parser are created when compiling each file.
    std::shared_ptr<Preprocessor> preProc_;
    std::shared_ptr<CLexer>       lexer_;
    std::shared_ptr<CParser>      parser_;

private:
    // Compilation phases.
    bool preprocess(const std::string &sourceFileName);
    bool lex(const std::string &sourceFileName);
    bool parse(const std::string &sourceFileName);
    bool semantic(const std::string &sourceFileName);
    bool optimise(const std::string &sourceFileName);
    bool codegen(const std::string &sourceFileName);

public:
    Compiler(const CompileArgs &args);

    bool compile(const std::string &sourceFileName);
};


} // namespace deepC

#endif // DEEPC_COMPILER_H
