#include <string.h>

#include "lexergen.h"
#include "util.h"


void LexerGenInit(LexerGen *lgen)
{
    memset(lgen, 0, sizeof(*lgen));
    GrammarInit(&lgen->grammar);
    lgen->errMsg = NULL;
}

void LexerGenClose(LexerGen *lgen)
{
    GrammarClose(&lgen->grammar);
    FreeStr(&lgen->errMsg);
}

bool LexerGenReadGrammar(LexerGen *lgen, const char *fileName)
{
    if (!GrammarRead(&lgen->grammar, fileName))
    {
        lgen->errMsg = strdup(GrammarGetError(&lgen->grammar));
        return false;
    }
    
    return true;
}

const char *LexerGenGetError(LexerGen *lgen)
{
    return lgen->errMsg;
}
