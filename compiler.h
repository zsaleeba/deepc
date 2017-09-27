#ifndef COMPILER_H
#define COMPILER_H

#include "compileargs.h"
#include "programdb.h"


// Forward declarations.
class Preprocessor;
class Lexer;
class Parser;


//
// The Compiler class is used to compile all the files we need to compile.
// It exists for the duration of the program so it keeps a single instance
// of the program database open even if multiple files are compiled.
//

class Compiler
{
private:
    // A single instance of program database class is used throughout the run.
    ProgramDb     pdb_;

    // An instance of the lexer and parser are created when compiling each file.
    Preprocessor *preProc_;
    Lexer        *lexer_;
    Parser       *parser_;

public:
    Compiler();

    bool preprocess(const CompileArgs &args, const std::string sourceFileName);
    bool compile(const CompileArgs &args, const std::string sourceFileName);
};

#endif // COMPILER_H
