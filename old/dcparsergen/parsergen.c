#include <stdlib.h>
#include <string.h>

#include "parsergen.h"
#include "util.h"


void ParserGenInit(ParserGen *pgen)
{
    memset(pgen, 0, sizeof(*pgen));
    GrammarInit(&pgen->grammar);
    pgen->errMsg = NULL;
}


void ParserGenClose(ParserGen *pgen)
{
    GrammarClose(&pgen->grammar);
    FreeStr(&pgen->errMsg);
}


bool ParserGenReadGrammar(ParserGen *pgen, const char *fileName)
{
    if (!GrammarRead(&pgen->grammar, fileName))
    {
        pgen->errMsg = strdup(GrammarGetError(&pgen->grammar));
        return false;
    }
    
    return true;
}


const char *ParserGenGetError(ParserGen *pgen)
{
    return pgen->errMsg;
}
