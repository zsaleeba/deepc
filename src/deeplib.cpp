/* deeplib.cpp */
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>

#include "deeplib.h"

using namespace std;


void PrintSourceTextErrorLine(string FileName, string SourceText, int Line, int CharacterPos)
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
    printf("^\n%s:%d: ", FileName.c_str(), Line);
}


/* display the source line and line number to identify an error */
void PrintSourceErrorPrefix(Parser &p)
{
    if (Parser.Location == NULL)
        PrintSourceTextErrorLine(p.SourceFileName(), NULL, 0, 0);
    else
        PrintSourceTextErrorLine(p.SourceFileName(), NULL /*Parser->SourceText*/, Parser.Location()->first_line, Parser.Location()->first_column);
}


/* exit with a message */
void ProgramErrorMessage(Parser &p, const char *Message, ...)
{
    va_list Args;

    PrintSourceErrorPrefix(Parser);
    va_start(Args, Message);
    vprintf(Message, Args);
    va_end(Args);
    putchar('\n');
}

void ProgramFail(const char *Message, ...)
{
    va_list Args;

    va_start(Args, Message);
    vprintf(Message, Args);
    va_end(Args);
    putchar('\n');
    
    Cleanup(Parser);
    exit(1);
}

void ProgramFail(Parser &p, const char *Message, ...)
{
    va_list Args;

    PrintSourceErrorPrefix(p);
    va_start(Args, Message);
    vprintf(Message, Args);
    va_end(Args);
    putchar('\n');
    
    Cleanup(Parser);
    exit(1);
}


/* printf for compiler error reporting */
void PlatformPrintf(const char *Format, ...)
{
    va_list Args;
    
    va_start(Args, Format);
    vprintf(Format, Args);
    va_end(Args);
}
