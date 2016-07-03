#include "compiler.h"

#include <FlexLexer.h>

class DataType;

#include "dcparser.hpp"
#include "dclexerclass.h"


//
// Constructor and destructor
//

Compiler::Compiler()
    : Tree(NULL)
{
}
    
Compiler::~Compiler()
{
    Cleanup();
}


//
// NAME:        Init
// ACTION:      Initialise the compiler
// PARAMETERS:  string SourceFileName - the name of the file to parse
//

void Compiler::Init(string SourceFileName)
{
    Lexer = new DCLexer();
    Parser = new yy::DCParser(this, Lexer);
    
    Lexer->Init(SourceFileName);
}


//
// NAME:        Cleanup
// ACTION:      Free any storage or resources used by the compiler
//

void Compiler::Cleanup()
{
    if (Lexer != NULL)
    {
        delete Lexer;
        Lexer = NULL;
    }
    
    if (Parser != NULL)
    {
        delete Parser;
        Parser = NULL;
    }
}


//
// NAME:        Compile
// ACTION:      Compile a file
// PARAMETERS:  string SourceFileName - the source file to compile, empty for console input
//              string DestFileName - the destination file name to store the result in
//              CompilationStyle ResultStyle - the form of the result, object file, 
//                  exe file or an internal representation which we can interpret
// RETURNS:     bool - true on success, false otherwise
//

bool Compiler::Compile(string SourceFileName, string DestFileName, CompilationStyle ResultStyle)
{
    // initialise the compiler
    Init(SourceFileName);
    
    // parse the source
    bool Success = Parser->parse() == 0;
    
    // free resources
    Cleanup();
    
    return Success;
}
