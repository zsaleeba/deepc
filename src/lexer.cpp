/* lexer.c */
#include <ctype.h>

#include "deepc.h"
#include "dclexer.h"


/*
 * NAME:        LexerInit
 * ACTION:      Initialise the lexer
 * PARAMETERS:  ParseState *Parser - the parser
 *              void **LexerPtr - a pointer to where the lexer pointer is stored
 */
 
void LexerInit(ParseState *Parser, void **LexerPtr)
{
    DCParser_lex_init(LexerPtr);
    DCParser_set_extra(Parser, *LexerPtr);
}


/*
 * NAME:        LexerClose
 * ACTION:      Close the lexer and free any resources used
 * PARAMETERS:  void *Lexer - an anonymous pointer to the lexer
 */
 
void LexerClose(void *Lexer)
{
    DCParser_lex_destroy(Lexer);
}


/*
 * NAME:        LexerCheckType
 * ACTION:      Check if an identifier is a data type
 * PARAMETERS:  ParseState *Parser - the parser
 *              const char **Dest - where to put the result
 *              const char *SrcText - the identifier
 * RETURNS:     enum yytokentype - eg. IDENTIFIER etc. 
 */
 
int LexerCheckType(ParseState *Parser, const char **Dest, const char *SrcText)
{
    *Dest = ShareStr(SrcText);
    
    if (DataTypeGetTypedef(SrcText) != NULL)
        return TYPE_NAME;
    else
        return IDENTIFIER;
}


/*
 * NAME:        LexerIntegerConstant
 * ACTION:      Lex a decimal number
 * PARAMETERS:  ParseState *Parser - the parser
 *              ParseTree **Dest - where to put the result
 *              const char *SrcText - the identifier
 *              int Base - what base the number is in
 * RETURNS:     enum yytokentype - CONSTANT
 */
 
int LexerIntegerConstant(ParseState *Parser, ParseTree **Dest, const char *SrcText, int Base)
{
    char *EndPos;
    Value *NewVal = DataTypeAllocValue(TypeUInt);
    NewVal->v.UInt = strtoul(SrcText, &EndPos, Base);
    
    if (*EndPos != '\0')
        ProgramFail(Parser, "int parse error");
        
    *Dest = ParseTreeConstant(NewVal);
    return CONSTANT;
}


/*
 * NAME:        LexerFloatConstant
 * ACTION:      Lex a floating point/double number
 * PARAMETERS:  ParseState *Parser - the parser
 *              ParseTree **Dest - where to put the result
 *              const char *SrcText - the identifier
 * RETURNS:     enum yytokentype - CONSTANT
 */
 
int LexerFloatConstant(ParseState *Parser, ParseTree **Dest, const char *SrcText)
{
    char *EndPos;
    Value *NewVal = DataTypeAllocValue(TypeDouble);
    NewVal->v.FPDouble = strtod(SrcText, &EndPos);
    
    if (*EndPos != '\0')
        ProgramFail(Parser, "float parse error");
        
    *Dest = ParseTreeConstant(NewVal);
    return CONSTANT;
}


/*
 * NAME:        LexerReadDigits
 * ACTION:      Lex a number from a numeric escaped character
 * PARAMETERS:  const char **Str - where to find the digits. Will advance the pointer.
 * RETURNS:     unsigned char - the character
 */
 
unsigned char LexerReadDigits(const char **Str, int Base, int MaxDigits)
{
    int CharsRead = 0;
    unsigned char ThisChar;
    unsigned char MadeChar = 0;
    int Finished = FALSE;
    
    while (CharsRead < MaxDigits && !Finished)
    {
        ThisChar = **Str;
        
        if (isdigit(ThisChar))
            MadeChar = MadeChar * Base + (ThisChar - '0');
            
        else if (Base > 10 && ThisChar >= 'a' && ThisChar <= 'a' + (Base - 10))
            MadeChar = MadeChar * Base + (ThisChar - 'a' + 10);

        else
            Finished = TRUE;

        if (!Finished)
        {
            (*Str)++;
            CharsRead++;
        }
    }
    
    return MadeChar;
}


/*
 * NAME:        LexerReadEscapedCharacter
 * ACTION:      Lex a single escaped character from a character or string constant
 * PARAMETERS:  const char **Str - where to find the character. Will advance the pointer.
 * RETURNS:     int - the character
 */
 
int LexerReadEscapedCharacter(const char **Str)
{
    unsigned char ThisChar = **Str;
    
    (*Str)++;
    
    if (ThisChar != '\\')
        return ThisChar;
    else
    {
        /* an escaped character */
        ThisChar = **Str;
        (*Str)++;

        switch (ThisChar)
        {
            /* escaped characters */
            case 'a': return '\a';
            case 'b': return '\b';
            case 'f': return '\f';
            case 'n': return '\n';
            case 'r': return '\r';
            case 't': return '\t';
            case 'v': return '\v';

            /* hex ASCII character */
            case 'x':
                return LexerReadDigits(Str, 16, 2);
                
            default:
                if (isdigit(ThisChar))
                {
                    /* octal ASCII character */
                    return LexerReadDigits(Str, 8, 3);
                }
                else
                    return ThisChar;
        }
    }
}


/*
 * NAME:        LexerCharConstant
 * ACTION:      Lex a character constant
 * PARAMETERS:  ParseState *Parser - the parser
 *              ParseTree **Dest - where to put the result
 *              const char *SrcText - the identifier
 * RETURNS:     enum yytokentype - CONSTANT
 */
 
int LexerCharConstant(ParseState *Parser, ParseTree **Dest, const char *SrcText)
{
    int CharValue = 0;
    int SingleChar;
    int CharsRead = 0;
    Value *NewVal = DataTypeAllocValue(TypeUInt);
    
    if (*SrcText != '\'')
        ProgramFail(Parser, "wide character constants aren't supported yet");
    
    /* get multiple characters until the character constant is over */
    SrcText++;
    while (*SrcText != '\'' && *SrcText != '\0')
    {
        if (CharsRead >= (int)(sizeof(int)/sizeof(char)))
            ProgramFail(Parser, "you have too many characters in this character constant");

        /* get a character */            
        SingleChar = LexerReadEscapedCharacter(&SrcText);
        
        /* add it to our total value (may be a multi-character constant) */
        CharValue = (CharValue << 8) | SingleChar;
        CharsRead++;
    }
    
    if (CharsRead == 0)
        ProgramFail(Parser, "there's nothing in here");
    
    /* return the result */
    NewVal->v.UInt = CharValue;
    *Dest = ParseTreeConstant(NewVal);
    return CONSTANT;
}


/*
 * NAME:        LexerStringConstant
 * ACTION:      Lex a string constant
 * PARAMETERS:  ParseState *Parser - the parser
 *              ParseTree **Dest - where to put the result
 *              const char *SrcText - the string
 * RETURNS:     enum yytokentype - CONSTANT
 */
 
int LexerStringConstant(ParseState *Parser, ParseTree **Dest, const char *SrcText)
{
    Value *NewVal;
    char *TmpStr;
    int CharCount = 0;
    const char *SharedStr;
    
    if (*SrcText != '\"')
        ProgramFail(Parser, "wide character constants aren't supported yet");
    
    /* get characters until the string constant is over */
    TmpStr = (char *)CheckedAlloc(strlen(SrcText));
    SrcText++;
    while (*SrcText != '\"' && *SrcText != '\0')
    {
        /* get a character */            
        TmpStr[CharCount] = LexerReadEscapedCharacter(&SrcText);
        CharCount++;
    }
    
    /* null-terminate the string */
    TmpStr[CharCount] = '\0';
    CharCount++;
    
    /* return the result */
    SharedStr = ShareStr2(TmpStr, CharCount);
    free(TmpStr);
    NewVal = DataTypeAllocValue(TypeCharArray);
    NewVal->v.Array.Size = CharCount;
    NewVal->v.Array.Data = (void *)SharedStr;
    *Dest = ParseTreeConstant(NewVal);
    return CONSTANT;
}


/*
 * NAME:        LexerIdentifier
 * ACTION:      Lex an identifier
 * PARAMETERS:  ParseState *Parser - the parser
 *              ParseTree **Dest - where to put the result
 *              const char *SrcText - the identifier
 * RETURNS:     enum yytokentype - CONSTANT
 */
 
int LexerIdentifier(ParseState *Parser, const char **Dest, const char *SrcText)
{
    return IDENTIFIER;
}
