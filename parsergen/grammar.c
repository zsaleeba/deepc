#include <stdlib.h>

#include "grammar.h"


// Prototypes.
void GrammarInit(Grammar *grammar)
{
    memset(grammar, 0, sizeof(*grammar));
    grammar->root = NULL;
}


void GrammarClose(Grammar *grammar)
{
    if (grammar->errMsg != NULL)
    {
        free(grammar->errMsg);
        grammar->errMsg = NULL;
    }
}


bool GrammarRead(Grammar *grammar, const char *fileName);
const char *GrammarGetError(Grammar *grammar);
