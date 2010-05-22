#include <stdio.h>

#include "deepc.h"


/* 
 * NAME:        Initialise
 * ACTION:      Initialise any structures before we start
 */
 
void Initialise()
{
    ShareStrInit();
    DataTypeInit();
    CodeGenInit();
}


/*
 * NAME:        Cleanup
 * ACTION:      Free any resources before we leave
 * PARAMETERS:  ParseState *Parser - a parser structure to clean up if we have one
 */
 
void Cleanup(ParseState *Parser)
{
    if (Parser != NULL)
        ParserClose(Parser);

    DataTypeCleanup();
    ShareStrClose();
}


int main(int argc, char **argv)
{
    printf("deepc v 0.1\n");
    
    return 0;
}
