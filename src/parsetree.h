#ifndef PARSETREE_H
#define PARSETREE_H

#include <stdio.h>
#include <string>

class ParseTree;
class DataType;
class Compiler;

#include "dcparser.hpp"

using namespace std;


/* parse trees */
enum Cmd
{
    CmdFail = 0,
    CmdAdd,
    CmdAddressOf,
    CmdArrayIndex,
    CmdAssign,
    CmdAssignInitialiser,
    CmdBitwiseAnd,
    CmdBitwiseExor,
    CmdBitwiseNot,
    CmdBitwiseOr,
    CmdBreak,
    CmdCallFunction,
    CmdCase,
    CmdCast,
    CmdCommaExpression,
    CmdConstant,
    CmdContinue,
    CmdDataType,
    CmdDeclaration,
    CmdDereference,
    CmdDivide,
    CmdDoWhile,
    CmdEmpty,
    CmdFor,
    CmdFunctionDef,
    CmdGoto,
    CmdIdentifier,
    CmdIndexDesignator,
    CmdInitialiser,
    CmdIsEqual,
    CmdIsGreaterThan,
    CmdIsGreaterThanOrEqual,
    CmdIsLessThan,
    CmdIsLessThanOrEqual,
    CmdIsNotEqual,
    CmdLabel,
    CmdLogicalAnd,
    CmdLogicalNot,
    CmdLogicalOr,
    CmdModulus,
    CmdMultiply,
    CmdNegate,
    CmdPositive,
    CmdPostDecrement,
    CmdPostIncrement,
    CmdPreDecrement,
    CmdPreIncrement,
    CmdReturn,
    CmdShiftLeft,
    CmdShiftRight,
    CmdSizeof,
    CmdSizeofType,
    CmdStructElement,
    CmdSubtract,
    CmdSwitch,
    CmdTernaryOperator,
    CmdWhile,
    
    /* nodes used only when creating parse trees */
    CmdDeclarator,
    CmdElementDesignator,
    CmdTypedInitialiser,
    
    CmdMax      /* not a real command - just the last one */
};


/* parse tree arguments */
union ParseTreeArgs
{
    //Value *Val;                 /* value argument for CmdConstant constants */
    //string Identifier;          /* identifier when doing CmdIdentifier */
    DataType *DType;            /* data type when doing CmdType */
    //ParseTree *Arg[MAX_PARSE_TREE_ARGS]; /* sub-trees for command parameters */
};

class ParseTree
{
    enum Cmd Command;               /* what type of command this node represents */
    //ParseTree *ListNext;            /* the next tree node in a linked list of statements */
    //ParseTree *ListLast;            /* the last tree node in a linked list of statements */

    /* arguments to this node */
    //int NumArgs;                    /* the number of arguments */
    //union ParseTreeArgs a;    
};

#endif /* PARSETREE_H */
