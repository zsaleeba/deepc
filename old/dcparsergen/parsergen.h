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
void ParserGenInit(ParserGen *lgen);
void ParserGenClose(ParserGen *lgen);
bool ParserGenReadGrammar(ParserGen *lgen, const char *fileName);
const char *ParserGenGetError(ParserGen *lgen);

#endif // PARSERGEN_H
