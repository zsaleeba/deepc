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

#ifndef CTREE_H
#define CTREE_H

#include <memory>
#include <vector>

#include "sourcepos.h"
#include "sourcespan.h"



namespace deepC
{

//
// Parse trees are represented as trees of CTrees.
// CTrees are subclassed by all the concrete node types.
//

class CTree
{
public:
    // For convenience each node has a node type.
    enum class NodeType
    {
        None,
        CompilationUnit,
        FunctionDefinition
    };

private:
    // What type of node this is.
    NodeType nodeType_;

    // What part of the file it represents.
    SourceSpan span_;

public:
    // Constructor.
    CTree(NodeType nt, const SourceSpan &span) : nodeType_(nt), span_(span) {}

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
    CTreeCompilationUnit(const SourceSpan &span) : CTree(CTree::NodeType::CompilationUnit, span) {}

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
    CTreeFunctionDefinition(const SourceSpan &span) : CTree(CTree::NodeType::FunctionDefinition, span) {}

    // Accessors.
    const std::vector<std::shared_ptr<CTree>> getChildren() { return children_; }
    void appendChild(std::shared_ptr<CTree> child) { children_.push_back(child); }
};

} // namespace deepC.

#endif // CTREE_H
