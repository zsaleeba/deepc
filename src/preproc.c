/* preproc.c */
#include <string.h>

#include "deepc.h"


/*
 * NAME:        PreProcInit
 * ACTION:      Initialise the preprocessor to parse a file
 * PARAMETERS:  ParseState *Parser - the parser
 *              const char *FileName - the file to start parsing, NULL for console input
 * RETURNS:     void - program fails on error
 */
 
void PreProcInit(ParseState *Parser, const char *FileName)
{
    if (FileName == NULL)
    {
        /* start reading from the console */
        Parser->IsInteractive = TRUE;
        Parser->InputStream = NULL;
    }
    else
    {
        /* start reading a file */
        Parser->InputStream = fopen(FileName, "r");
        if (Parser->InputStream == NULL)
            ProgramFail(Parser, "can't open file '%s'", FileName);
    
        Parser->SourceFileName = ShareStr(FileName);
    }
}


/*
 * NAME:        PreProcClose
 * ACTION:      Release any resources used by the preprocessor
 * PARAMETERS:  ParseState *Parser - the parser
 */
 
void PreProcClose(ParseState *Parser)
{
    if (!Parser->IsInteractive)
        fclose(Parser->InputStream);
}


/*
 * NAME:        PreProcRead
 * ACTION:      Called by the lexer to get pre-processed input from the pre-processor
 * PARAMETERS:  ParseState *Parser - the parser
 *              char *ReadBuf - the buffer to put the data we're reading in
 *              int MaxSize - the size of the ReadBuf
 * RETURNS:     int - the number of characters read or 0 at EOF
 */
 
int PreProcRead(ParseState *Parser, char *ReadBuf, int MaxSize)
{
    int ReturnValue;
    
    if (Parser->IsInteractive)
    {
        /* read a line from the console */
        if (fgets(ReadBuf, MaxSize, stdin) == NULL)
            ReturnValue = 0;
        else
            ReturnValue = strlen(ReadBuf);
    }
    else
    {
        /* read a block of data from the input file */
        ReturnValue = fread(ReadBuf, 1, MaxSize, Parser->InputStream);
    }
        
    /* let yylex() know if we had a read error */
    if (ReturnValue < 0)
        ReturnValue = 0;

    return ReturnValue;
}
