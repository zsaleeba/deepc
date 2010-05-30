#ifndef DCLEXERCLASS_H
#define DCLEXERCLASS_H

class ParseTree;
class DataType;
class Compiler;
class DCLexer;

#include "preproc.h"
#include "dcparser.hpp"


class DCLexer : public yyFlexLexer
{
    // the preprocessor we call to get input
    PreProcessor PreProc;

			
    // point to yylval (provided by Bison in overloaded yylex)
    yy::DCParser::semantic_type *yylval;
    
    // pointer to yylloc (provided by Bison in overloaded yylex)
    yy::DCParser::location_type *yylloc;
    
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

    //
    // an externally visible version of yylex() with yylval and yylloc
    //
    int yylex(yy::DCParser::semantic_type *p_yylval, yy::DCParser::location_type *p_yylloc)
    {
        yylval = p_yylval;
        yylloc = p_yylloc;
        return yylex();
    }
};

#endif /* DCLEXERCLASS_H */
