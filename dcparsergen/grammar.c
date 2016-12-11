#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#include "grammar.h"
#include "util.h"


// Prototypes.
void GrammarInit(Grammar *grammar)
{
    memset(grammar, 0, sizeof(*grammar));
    grammar->fileName = NULL;
    grammar->src = NULL;
    grammar->defList = NULL;
    grammar->errMsg = NULL;
    grammar->lineNo = 0;
}


void GrammarClose(Grammar *grammar)
{
    FreeStr(&grammar->src);
    FreeStr(&grammar->errMsg);
    
    GrammarFreeDefinitions(grammar->defList);
    grammar->defList = NULL;
}


bool GrammarRead(Grammar *grammar, const char *fileName)
{
    // Read the file.
    grammar->fileName = fileName;
    if (!ReadFile(&grammar->src, &grammar->errMsg, fileName))
        return false;
    
    // Go through it line by line.
    grammar->lineNo = 1;
    char *line;
    char *nextLine = grammar->src;
    while (ReadLine(&line, &nextLine))
    {
        // Remove comments.
        char *commentPos = strstr(line, "//");
        if (commentPos != NULL)
        {
            *commentPos = 0;
        }
        
        // Remove trailing whitespace.
        StringStripEnd(line);
        
        // Skip empty lines.
        if (line[0] != 0)
        {
            // Is it a definition line?
            if (isalnum(line[0]))
            {
                // Parse a definition.
                if (!GrammarParseDefinition(grammar, line))
                    return false;
            }
            else if (isspace(line[0]))
            {
                // Parse an option.
                if (!GrammarParseOption(grammar, line))
                    return false;
            }
            else
            {
                // Unexpected character.
                AllocSprintf(&grammar->errMsg, "unexpected character in %s line %d: '%s'", fileName, grammar->lineNo, line);
                return false;
            }
        }

        grammar->lineNo++;
    }
    
    return true;
}


bool GrammarParseDefinition(Grammar *grammar, char *line)
{
    AllocSprintf(&grammar->errMsg, "unimplemented");
    return false;
}


bool GrammarParseOption(Grammar *grammar, char *line)
{
    AllocSprintf(&grammar->errMsg, "unimplemented");
    return false;
}


bool GrammarParseItem(Grammar *grammar, char *str)
{
    AllocSprintf(&grammar->errMsg, "unimplemented");
    return false;
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
