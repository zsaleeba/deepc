#ifndef COMPILER_H
#define COMPILER_H

#include <string>

using namespace std;


// forward declarations
class ParseTree;
class DCLexer;
namespace yy {
    class DCParser;
};


//
// The form of the result, object file, exe file or an internal 
// representation which we can interpret
//
enum CompilationStyle
{
    CSObject,
    CSExe,
    CSInternal
};


//
// The top level object of the compiler. This calls everything else.
//

class Compiler
{
    /* lexer */
    string mSourceFileName;
    yy::DCParser *Parser;
    DCLexer *Lexer;
    
    /* parser result */
    ParseTree *Tree;

    //
    // NAME:        Init
    // ACTION:      Initialise the compiler
    // PARAMETERS:  string SourceFileName - the name of the file to parse
    //
    void Init(string SourceFileName);
    
    //
    // NAME:        Cleanup
    // ACTION:      Free any storage or resources used by the compiler
    //
    void Cleanup();

public:
    //
    // Constructor and Destructor
    //
    Compiler();    
    ~Compiler();
    
    //
    // NAME:        Compile
    // ACTION:      Compile a file. Calls 
    // PARAMETERS:  string SourceFileName - the source file to compile, empty for console input
    //              string DestFileName - the destination file name to store the result in
    //              CompilationStyle ResultStyle - the form of the result, object file, 
    //                  exe file or an internal representation which we can interpret
    // RETURNS:     bool - true on success, false otherwise
    //
    bool Compile(string SourceFileName, string DestFileName, CompilationStyle ResultStyle);
    
    // accessors
    //YYLTYPE *Location() { return mLocation; };
    
    // singleton
    static Compiler &instance() { static Compiler LocalInstance; return LocalInstance; };
};

#endif /* COMPILER_H */
