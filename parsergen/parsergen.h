#ifndef PARSERGEN_H
#define PARSERGEN_H

#include <stdbool.h>

#include "grammar.h"

typedef struct 
{
    Grammar grammar;
    char *errMsg;
} ParserGen;

// Prototypes.
void ParserGenInit(LexerGen *lgen);
void ParserGenClose(LexerGen *lgen);
bool ParserGenReadGrammar(LexerGen *lgen, const char *fileName);
const char *ParserGenGetError(LexerGen *lgen);

#endif // PARSERGEN_H
