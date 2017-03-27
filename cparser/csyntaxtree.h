/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                       Parse tree generator                         ***
 ***                                                                    ***
 ***                                         - Zik Saleeba              ***
 ***                                           2017-03-27               ***
 ***                                                                    ***
 **************************************************************************/

#ifndef CSYNTAXTREE_H
#define CSYNTAXTREE_H

#include "sourcepos.h"


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

class CTree
{
public:
    // For convenience each node has a node type.
private:
    // What type of node this is.
    NodeType nodeType_;

    // What part of the file it represents.
    SourceSpan span_;

public:
    // Constructor.
    CTree(NodeType nt) : nodeType_(NodeType::None) {}

    // Accessors.
    NodeType getNodeType() const { return nodeType_; }
};


//
// Parse tree node for a whole file.
//

class CTreeCompilationUnit : public CTree
{
private:
    // All the items in this compilation unit.
    std::vector<std::shared_ptr<CTree>> children_;

public:
    // Constructor.
    CTreeCompilationUnit() : CTree(CTree::NodeType::CompilationUnit) {}

    // Accessors.
    const std::vector<std::shared_ptr<CTree>> getChildren() { return children_; }
    void appendChild(std::shared_ptr<CTree> child) { children_.push_back(child); }
};


//
// Parse tree node for a function definition.
//

class CTreeFunctionDefinition : public CTree
{
private:
    // All the items in this compilation unit.
    std::vector<std::shared_ptr<CTree>> children_;

public:
    // Constructor.
    CTreeCompilationUnit() : CTree(CTree::NodeType::CompilationUnit) {}

    // Accessors.
    const std::vector<std::shared_ptr<CTree>> getChildren() { return children_; }
    void appendChild(std::shared_ptr<CTree> child) { children_.push_back(child); }
};



#endif // CSYNTAXTREE_H
