#include <stdio.h>
#include <stdlib.h>

#include "lexergen.h"
#include "parsergen.h"


//
// The main program.
//

int main(int argc, char **argv)
{
    // Check args.
    if (argc != 3)
    {
        fprintf(stderr, "Format: %s <filename.lexer> <filename.grammar>\n");
        exit(EXIT_FAILURE);
    }
    
    // Invoke the lexer generator.
    LexerGen lgen;
    LexerGenInit(&lgen);
    if (!LexerGenReadDefinition(&lgen, argv[1]))
    {
        fprintf(stderr, "can't read %s: %s\n", argv[1], LexerGenGetError(&lgen));
        exit(EXIT_FAILURE);
    }
    
    // Invoke the parser generator.
    ParserGen pgen;
    ParserGenInit(&pgen);
    if (!ParserGenReadDefinition(&pgen, argv[2]))
    {
        fprintf(stderr, "can't read %s: %s\n", argv[2], ParserGenGetError(&pgen));
        LexerGenClose(&lgen);
        exit(EXIT_FAILURE);
    }
    
    // Clean up.
    ParserGenClose(&pgen);
    LexerGenClose(&lgen);
    
    return EXIT_SUCCESS;
}
