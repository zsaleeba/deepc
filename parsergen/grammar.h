#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <stdbool.h>


//
// A generic grammar node.
//

typedef struct
{
    int i;
} GrammarNode;


//
// Defines a grammar's parse tree.
//

typedef struct 
{
    GrammarNode *root;
    char *errMsg;
} Grammar;


// Prototypes.
void GrammarInit(Grammar *grammar);
void GrammarClose(Grammar *grammar);
bool GrammarRead(Grammar *grammar, const char *fileName);
const char *GrammarGetError(Grammar *grammar);

#endif // GRAMMAR_H
