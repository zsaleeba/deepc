/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                      Lexical analyser for C                        ***
 ***                                                                    ***
 ***                                         - Zik Saleeba              ***
 ***                                           2017-03-26               ***
 ***                                                                    ***
 **************************************************************************/

#ifndef CLEXER_H
#define CLEXER_H

#include "sourcespan.h"

namespace deepC
{


//
// Tree nodes can have one of a number of node types.
//

enum class CTreeNodeType
{
    None,
    CompilationUnit,
    FunctionDefinition
};


//
// Parse trees are represented as trees of CTrees.
//

class CLexer
{
public:
    CLexer(const std::string sourceFile, const SourceSpan &span);
};

// namespace deepC.

#endif // CLEXER_H
