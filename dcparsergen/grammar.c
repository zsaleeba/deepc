#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "grammar.h"
#include "util.h"


// Prototypes.
void GrammarInit(Grammar *grammar)
{
    memset(grammar, 0, sizeof(*grammar));
    grammar->defList = NULL;
}


void GrammarClose(Grammar *grammar)
{
    FreeStr(&grammar->errMsg);
}


bool GrammarRead(Grammar *grammar, const char *fileName)
{
    // Read the file.
    if (!ReadFile(&grammar->src, &grammar->errMsg, fileName))
        return false;
    
    return true;
}


const char *GrammarGetError(Grammar *grammar)
{
    return grammar->errMsg;
}


void GrammarFreeDefinitions(GrammarDefinition *def)
{
    // Free it and all the following definitions.
    while (def != NULL)
    {
        GrammarDefinition *next = def->nextDefinition;
        
        FreeStr(&def->name);
        GrammarFreeOptions(def->firstOption);
        
        def = next;
    }
}


void GrammarFreeOptions(GrammarOption *opt)
{
    // Free it and all the following definitions.
    while (opt != NULL)
    {
        GrammarOption *next = opt->nextOption;
        
        GrammarFreeItems(opt->firstItem);
        
        opt = next;
    }
}


void GrammarFreeItems(GrammarItem *item)
{
    // Free it and all the following definitions.
    while (item != NULL)
    {
        GrammarItem *next = item->nextItem;
        
        FreeStr(&item->definitionName);
        FreeStr(&item->token);
        
        if (item->charSet)
        {
            free(item->charSet);
        }
        
        item = next;
    }
}
