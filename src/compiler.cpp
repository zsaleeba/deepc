#include "compiler.h"


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
//

void Compiler::Init()
{
}


//
// NAME:        Cleanup
// ACTION:      Free any storage or resources used by the compiler
//

void Compiler::Cleanup()
{
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
    Init();
    
    // free resources
    Cleanup();
    
    return true;
}
