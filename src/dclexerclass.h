#ifndef DCLEXERCLASS_H
#define DCLEXERCLASS_H

#include <FlexLexer.h>

#include "preproc.h"


class DCLexer : public yyFlexLexer
{
    // the preprocessor we call to get input
    PreProcessor PreProc;

    //
    // flex defines this method for us
    //
    int yylex();
    
    //
    // yyFlexLexer methods we're overriding
    //
    int LexerInput(char *ReadBuf, int Length);
    void LexerError(const char *Message);
    
    //
    // methods for dealing with various special lexical elements
    //
    void Comment();
    int Identifier();
    int IntegerConstant(const char *Str, int Base);
    int CharConstant();
    int StringConstant();
    
public:
    DCLexer();
    ~DCLexer();
    
    //
    // NAME:        Init
    // ACTION:      Call this before lexing to define the input source
    // PARAMETERS:  string SourceFileName - empty string for console input
    //
    void Init(string SourceFileName);
};

#endif /* DCLEXERCLASS_H */
