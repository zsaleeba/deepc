#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <stdbool.h>
#include <stdint.h>


// Type forward declarations.
typedef struct _GrammarDefinition GrammarDefinition;
typedef struct _GrammarOption GrammarOption;
typedef struct _GrammarItem GrammarItem;


//
// A grammar definition.
//
// A grammar is a set of definitions. Each definition has a name
// and a set of options.
//

struct _GrammarDefinition
{
    GrammarDefinition *nextDefinition;
    char              *name;
    GrammarOption     *firstOption;
};


//
// An option from a definition.
//
// Each option has a set of items, each of which is required to match
// for the whole option to match.
//

struct _GrammarOption
{
    GrammarOption *nextOption;
    GrammarItem   *firstItem;
};


//
// An item from an option.
//
// Each option can be the name of a definition or a token or
// a set of characters to match on.
//

struct _GrammarItem
{
    GrammarItem *nextItem;
    bool         repeated;
    char        *definitionName;
    char        *token;
    uint8_t     *charSet;
};


//
// Defines a grammar's parse tree.
//

typedef struct 
{
    char *src;      // The source code.
    
    GrammarDefinition *defList;
    char *errMsg;
} Grammar;


// Prototypes.
void GrammarInit(Grammar *grammar);
void GrammarClose(Grammar *grammar);
bool GrammarRead(Grammar *grammar, const char *fileName);
const char *GrammarGetError(Grammar *grammar);

// Internal prototypes.
void GrammarFreeDefinitions(GrammarDefinition *def);
void GrammarFreeOptions(GrammarOption *opt);
void GrammarFreeItems(GrammarItem *item);

#endif // GRAMMAR_H
