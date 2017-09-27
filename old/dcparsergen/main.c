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
        fprintf(stderr, "Format: %s <lexical.pgen> <syntax.pgen>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Invoke the lexer generator.
    LexerGen lgen;
    LexerGenInit(&lgen);
    if (!LexerGenReadGrammar(&lgen, argv[1]))
    {
        fprintf(stderr, "%s\n", LexerGenGetError(&lgen));
        exit(EXIT_FAILURE);
    }
    
    // Invoke the parser generator.
    ParserGen pgen;
    ParserGenInit(&pgen);
    if (!ParserGenReadGrammar(&pgen, argv[2]))
    {
        fprintf(stderr, "%s\n", ParserGenGetError(&pgen));
        LexerGenClose(&lgen);
        exit(EXIT_FAILURE);
    }
    
    // Clean up.
    ParserGenClose(&pgen);
    LexerGenClose(&lgen);
    
    return EXIT_SUCCESS;
}
