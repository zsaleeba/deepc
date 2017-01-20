#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#include "grammar.h"
#include "util.h"


// Defines.
#define MAX_STRING 255
#define MAX_CHARSET 256


//
// Initialise a grammar structure.
//

void GrammarInit(Grammar *grammar)
{
    memset(grammar, 0, sizeof(*grammar));
    grammar->fileName = NULL;
    grammar->firstDef = NULL;
    grammar->lastDef = NULL;
    grammar->errMsg = NULL;
    grammar->lineNo = 0;
}


void GrammarClose(Grammar *grammar)
{
    FreeStr(&grammar->errMsg);
    
    GrammarFreeDefinitions(grammar->firstDef);
    grammar->firstDef = NULL;
}


bool GrammarRead(Grammar *grammar, const char *fileName)
{
    // Read the file.
    grammar->fileName = fileName;
    FILE *srcFile = fopen(fileName, "r");
    if (srcFile == NULL)
    {
        AllocSprintf(&grammar->errMsg, "can't open '%s'", fileName);
        return false;
    }
    
    // Go through it line by line.
    grammar->lineNo = 1;
    char line[4096];
    while (fgets(line, sizeof(line), srcFile))
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
            if (isalpha(line[0]))
            {
                // Parse a definition.
                if (!GrammarParseDefinition(grammar, line))
                    goto grErrExit;
            }
            else if (isspace(line[0]))
            {
                // Parse an option.
                if (!GrammarParseOption(grammar, line))
                    goto grErrExit;
            }
            else
            {
                // Unexpected character.
                AllocSprintf(&grammar->errMsg, "unexpected character in %s line %d: '%s'", fileName, grammar->lineNo, line);
                goto grErrExit;
            }
        }

        grammar->lineNo++;
    }
    
    return true;

grErrExit:
    fclose(srcFile);
    return false;
}


//
// Parse a definition start line and create the definition.
//

bool GrammarParseDefinition(Grammar *grammar, char *line)
{
    // Create a definition node.
    GrammarDefinition *def = calloc(1, sizeof(*def));
    if (def == NULL)
    {
        AllocSprintf(&grammar->errMsg, "out of memory");
        return false;
    }

    // Add it to the end of the list.
    if (grammar->firstDef == NULL)
    {
        grammar->firstDef = def;
        grammar->lastDef = def;
    }
    else
    {
        grammar->lastDef->nextDefinition = def;
        grammar->lastDef = def;
    }

    def->nextDefinition = NULL;
    def->firstOption = NULL;
    def->lastOption = NULL;
    char *pos = line;
    def->name = GrammarParseIdentifier(&pos);

    // There should only be a colon after the identifier.
    SkipWhitespace(&pos);
    if (*pos != ':')
    {
        AllocSprintf(&grammar->errMsg, "colon expected in definition, line %d", grammar->lineNo);
        return false;
    }

    pos++;
    if (*pos != 0)
    {
        AllocSprintf(&grammar->errMsg, "unexpected text after definition, line %d", grammar->lineNo);
        return false;
    }

    return true;
}


//
// Parse a definition's option line and add the option.
//

bool GrammarParseOption(Grammar *grammar, char *line)
{
    char *pos = line;

    SkipWhitespace(&pos);

    // Create an option node.
    GrammarOption *opt = calloc(1, sizeof(*opt));
    if (opt == NULL)
    {
        AllocSprintf(&grammar->errMsg, "out of memory");
        return false;
    }

    // Add it to the end of the list.
    GrammarDefinition *def = grammar->lastDef;
    if (def == NULL)
    {
        AllocSprintf(&grammar->errMsg, "option before definition, line %d", grammar->lineNo);
        free(opt);
        return false;
    }

    if (def->firstOption == NULL)
    {
        def->firstOption = opt;
        def->lastOption = opt;
    }
    else
    {
        def->lastOption->nextOption = opt;
        def->lastOption = opt;
    }

    opt->nextOption = NULL;
    opt->firstItem = NULL;
    opt->lastItem = NULL;

    // Parse the items.
    bool ok = false;
    while (GrammarParseItem(grammar, opt, &pos, &ok))
    {
    }

    return ok;
}


//
// Parse a single item and add it to an option.
//

bool GrammarParseItem(Grammar *grammar, GrammarOption *option, char **pos, bool *ok)
{
    *ok = true;

    // Are we at the end of the line?
    SkipWhitespace(pos);
    if (**pos == 0)
    {
        // There are no more items.
        return false;
    }

    // Create an item node.
    GrammarItem *item = calloc(1, sizeof(*item));
    if (item == NULL)
    {
        AllocSprintf(&grammar->errMsg, "out of memory");
        return false;
    }

    // Add it to the end of the list.
    if (option->firstItem == NULL)
    {
        option->firstItem = item;
        option->lastItem = item;
    }
    else
    {
        option->lastItem->nextItem = item;
        option->lastItem = item;
    }

    item->nextItem = NULL;
    item->repeated = false;
    item->definitionName = NULL;
    item->token = NULL;
    item->charSet = NULL;

    // Is it repeated?
    bool repeated = false;
    char ch = **pos;
    if (ch == '[')
    {
        repeated = true;
        (*pos)++;
    }

    // What kind of token is next?
    switch (ch)
    {
    case 0:
        AllocSprintf(&grammar->errMsg, "missing item after '[' in line %d\n", grammar->lineNo);
        *ok = false;
        break;

    case '\'':
        *ok = GrammarParseString(grammar, item, pos);
        break;

    case '{':
        *ok = GrammarParseCharSet(grammar, item, pos);
        break;

    default:
        if (isalpha(ch))
        {
            // Parse a definition name.
            item->definitionName = GrammarParseIdentifier(pos);
        }
        else
        {
            // It's something unknown.
            AllocSprintf(&grammar->errMsg, "unknown character in line %d\n", grammar->lineNo);
            *ok = false;
        }
        break;
    }

    // If it was repeated, make sure we have a closing square bracket.
    if (repeated)
    {
        if (**pos != ']')
        {
            AllocSprintf(&grammar->errMsg, "expected closing ']' after repeated term in line %d\n", grammar->lineNo);
            *ok = false;
        }

        item->repeated = true;
    }

    // If we're failing out deallocate the item.
    if (!*ok)
    {
        free(item);
        return false;
    }

    return true;
}


//
// Given a pointer to the start of an identifier returns a pointer to
// a newly allocated string containing the identifier and moves the
// pointer to after the string.
//

char *GrammarParseIdentifier(char **pos)
{
    char *startPos = *pos;

    while (isalnum(**pos))
    {
        (*pos)++;
    }

    char endCh = **pos;
    **pos = 0;
    char *result = strdup(startPos);
    **pos = endCh;

    return result;
}


//
// Parses a string delimited by single quote characters.
// Handles escape sequences etc.
//

bool GrammarParseString(Grammar *grammar, GrammarItem *item, char **pos)
{
    // Skip the leading quote character.
    if (**pos == '\'')
    {
        (*pos)++;
    }

    // Gather characters until we reach a closing single quote or the end of the string.
    int ch;
    int i = 0;
    char str[MAX_STRING+1];
    while (GrammarParseCharacter(pos, &ch) && i < MAX_STRING)
    {
        str[i] = ch;
        i++;
    }

    // Null terminate the string.
    str[i] = 0;

    // Did the string finish suitably?
    if (**pos == '\'')
    {
        (*pos)++;
        item->token = strdup(str);

        return true;
    }
    else if (**pos == 0)
    {
        AllocSprintf(&grammar->errMsg, "string is missing a closing quote in line %d\n", grammar->lineNo);
        return false;
    }

    AllocSprintf(&grammar->errMsg, "string terminates strangely in line %d\n", grammar->lineNo);
    return false;
}


//
// Parses a regex-style character set definition.
//

bool GrammarParseCharSet(Grammar *grammar, GrammarItem *item, char **pos)
{
    // A bitset of allowed characters.
    uint8_t charSet[MAX_CHARSET / 8];
    bool positiveSet = true;

    // Skip the leading brace character.
    if (**pos == '{')
    {
        (*pos)++;
    }

    // Is this a positive or negative set?
    if (**pos == '^')
    {
        positiveSet = false;
        (*pos)++;
        memset(charSet, 0xff, sizeof(charSet));
    }
    else
    {
        positiveSet = true;
        memset(charSet, 0, sizeof(charSet));
    }

    // Add/remove each of the specified characters.
    int ch;
    while (**pos != '}' && **pos != 0 && GrammarParseCharacter(pos, &ch))
    {
        if (ch < MAX_CHARSET)
        {
            if (positiveSet)
            {
                // Add it to the bitset.
                charSet[ch/8] = charSet[ch/8] | (1<<(ch%8));
            }
            else
            {
                // Remove it from the bitset.
                charSet[ch/8] = charSet[ch/8] & ~(1<<(ch%8));
            }
        }
    }

    // Did the character set finish suitably?
    if (**pos != '}')
    {
        AllocSprintf(&grammar->errMsg, "character set is missing a closing brace in line %d\n", grammar->lineNo);
        return false;
    }

    (*pos)++;

    // Copy the result to the item.
    item->charSet = calloc(MAX_CHARSET / 8, sizeof(uint8_t));
    memcpy(item->charSet, charSet, MAX_CHARSET/8);

    return true;
}


//
// Parses a character from a source line including un-escaping it and
// recognising the end of the string.
//

bool GrammarParseCharacter(char **pos, int *ch)
{
    *ch = **pos;

    switch (*ch)
    {
    case '\\':
        // An escaped character.
        (*pos)++;
        *ch = **pos;
        switch (*ch)
        {
        case 0: return false;
        case 'a': *ch = '\a'; break;
        case 'b': *ch = '\b'; break;
        case 'e': *ch = '\e'; break;
        case 'f': *ch = '\f'; break;
        case 'n': *ch = '\n'; break;
        case 'r': *ch = '\r'; break;
        case 't': *ch = '\t'; break;
        case 'v': *ch = '\v'; break;
        }

        break;

    case '\'':
        // End of string.
        (*pos)++;
        return false;

    case 0:
        // End of line.
        return false;

    default:
        (*pos)++;
        break;
    }

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
