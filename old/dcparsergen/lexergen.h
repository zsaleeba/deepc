#ifndef LEXERGEN_H
#define LEXERGEN_H

#include <stdbool.h>

#include "grammar.h"

typedef struct 
{
    Grammar grammar;
    char *errMsg;
} LexerGen;

// Prototypes.
void LexerGenInit(LexerGen *lgen);
void LexerGenClose(LexerGen *lgen);
bool LexerGenReadGrammar(LexerGen *lgen, const char *fileName);
const char *LexerGenGetError(LexerGen *lgen);

#endif // LEXERGEN_H
