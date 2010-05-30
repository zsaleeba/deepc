#include "dclexerclass.h"
#include "parsetree.h"
#include "datatype.h"
#include "dcparser.hpp"


DCLexer::DCLexer() 
    : yyFlexLexer()
{
}


DCLexer::~DCLexer()
{
    PreProc.Close();
}


void DCLexer::Init(string SourceFileName) 
{
    PreProc.Open(SourceFileName);
}

int DCLexer::LexerInput(char *ReadBuf, int Length)
{
    return PreProc.Read(ReadBuf, Length);
}

void LexerError(const char *Message);

//
// NAME:        DCLexer::Comment
// ACTION:      Absorb a comment
//
 
void DCLexer::Comment()
{
}

int DCLexer::Identifier()
{
    return yy::DCParser::token::IDENTIFIER;
}

int DCLexer::IntegerConstant(const char *Str, int Base)
{
    return yy::DCParser::token::CONSTANT;
}

int DCLexer::CharConstant()
{
    return yy::DCParser::token::CONSTANT;
}

int DCLexer::StringConstant()
{
    return yy::DCParser::token::STRING_LITERAL;
}
