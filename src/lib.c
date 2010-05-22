/* lib.c */
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "deepc.h"


void PrintSourceTextErrorLine(const char *FileName, const char *SourceText, int Line, int CharacterPos)
{
#if 0
    int LineCount;
    const char *LinePos;
    const char *CPos;
    int CCount;
    
    if (SourceText != NULL)
    {
        /* find the source line */
        for (LinePos = SourceText, LineCount = 1; *LinePos != '\0' && LineCount < Line; LinePos++)
        {
            if (*LinePos == '\n')
                LineCount++;
        }
        
        /* display the line */
        for (CPos = LinePos; *CPos != '\n' && *CPos != '\0'; CPos++)
            PrintCh(*CPos, &CStdOut);
            
        PrintCh('\n', &CStdOut);
        
        /* display the error position */
        for (CPos = LinePos, CCount = 0; *CPos != '\n' && *CPos != '\0' && (CCount < CharacterPos || *CPos == ' '); CPos++, CCount++)
        {
            if (*CPos == '\t')
                PrintCh('\t', &CStdOut);
            else
                PrintCh(' ', &CStdOut);
        }
    }
    else
    {
        /* assume we're in interactive mode - try to make the arrow match up with the input text */
        for (CCount = 0; CCount < CharacterPos + strlen(INTERACTIVE_PROMPT_STATEMENT); CCount++)
            PrintCh(' ', &CStdOut);
    }
#endif
    printf("^\n%s:%d: ", FileName, Line);
}


/* display the source line and line number to identify an error */
void PrintSourceErrorPrefix(ParseState *Parser)
{
    if (Parser != NULL)
    {
        if (Parser->Location == NULL)
            PrintSourceTextErrorLine(Parser->SourceFileName, NULL, 0, 0);
        else
            PrintSourceTextErrorLine(Parser->SourceFileName, NULL /*Parser->SourceText*/, Parser->Location->first_line, Parser->Location->first_column);
    }
}


/* exit with a message */
void ProgramErrorMessage(ParseState *Parser, const char *Message, ...)
{
    va_list Args;

    PrintSourceErrorPrefix(Parser);
    va_start(Args, Message);
    vprintf(Message, Args);
    va_end(Args);
    putchar('\n');
}

void ProgramFail(ParseState *Parser, const char *Message, ...)
{
    va_list Args;

    PrintSourceErrorPrefix(Parser);
    va_start(Args, Message);
    vprintf(Message, Args);
    va_end(Args);
    putchar('\n');
    
    Cleanup(Parser);
    exit(1);
}


#if 0
/* like ProgramFail() but gives descriptive error messages for assignment */
void AssignFail(struct ParseState *Parser, const char *Format, struct DataType *Type1, struct DataType *Type2, int Num1, int Num2, const char *FuncName, int ParamNo)
{
    PrintSourceErrorPrefix(Parser);
    PlatformPrintf("can't %s ", (FuncName == NULL) ? "assign" : "set");   
        
    if (Type1 != NULL)
        PlatformPrintf(Format, Type1, Type2);
    else
        PlatformPrintf(Format, Num1, Num2);
    
    if (FuncName != NULL)
        PlatformPrintf(" in argument %d of call to %s()", ParamNo, FuncName);
        
    ProgramFail(NULL, "");
}
#endif


/* printf for compiler error reporting */
void PlatformPrintf(const char *Format, ...)
{
    va_list Args;
    
    va_start(Args, Format);
    vprintf(Format, Args);
    va_end(Args);
}


/*
 * NAME:        CheckedAlloc
 * ACTION:      Allocate memory in a safe, checked way. 
 *              Also clears the memory.
 * PARAMETERS:  int Size - the amount of memory to allocate
 * RETURNS:     void * - the allocated memory
 */
 
void *CheckedAlloc(int Size)
{
    /* allocate the memory */
    void *AllocMem = malloc(Size);
    
    /* check that it worked */
    if (AllocMem == NULL)
        ProgramFail(NULL, "out of memory");
    
    /* clear the memory */
    memset(AllocMem, '\0', Size);
    
    return AllocMem;
}
