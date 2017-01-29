/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                       Parse tree generator                         ***
 ***                                                                    ***
 ***                                         - Zik Saleeba 2017         ***
 ***                                                                    ***
 **************************************************************************/

#ifndef CTREE_H
#define CTREE_H

class CTree
{
public:
    enum class NodeType
    {
        None,
        FunctionDefinition
    };

private:
    NodeType nodeType_;

public:
    CTree(NodeType nt) : nodeType_(NodeType::None) {}

    NodeType getNodeType() const { return nodeType_; }
};


#endif // CTREE_H
