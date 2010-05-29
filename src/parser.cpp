/* parser.c */
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "deepc.h"

#define MAX_STRING_SHOW 40
#define DEFAULT_OPTIMISATION_LEVEL 2


/* these command names are used when printing the parse tree */
/* this list must be kept in sync with enum Cmd in minic.h */
static const char *CommandName(enum Cmd Command)
{
    /* this is done as a big switch to make it easier to keep in sync with enum Cmd */
    switch (Command)
    {
        case CmdFail: case CmdMax: return "error in parse tree";
        case CmdAdd: return "+";
        case CmdAddressOf: return "addressof";
        case CmdArrayIndex: return "[]";
        case CmdAssign: return "assign";
        case CmdAssignInitialiser: return "assign initialiser";
        case CmdBitwiseAnd: return "bitwise and";
        case CmdBitwiseExor: return "^";
        case CmdBitwiseNot: return "~";
        case CmdBitwiseOr: return "|";
        case CmdBreak: return "break";
        case CmdCallFunction: return "call";
        case CmdCase: return "case";
        case CmdCast: return "cast";
        case CmdCommaExpression: return "comma expression";
        case CmdConstant: return "constant";
        case CmdContinue: return "continue";
        case CmdDataType: return "data type";
        case CmdDeclaration: return "declaration";
        case CmdDereference: return "dereference";
        case CmdDivide: return "/";
        case CmdDoWhile: return "do..while";
        case CmdEmpty: return "empty command";
        case CmdFor: return "for";
        case CmdFunctionDef: return "function definition";
        case CmdGoto: return "goto";
        case CmdIdentifier: return "identifier";
        case CmdIndexDesignator: return "index designator";
        case CmdInitialiser: return "initialiser";
        case CmdIsEqual: return "==";
        case CmdIsGreaterThan: return ">";
        case CmdIsGreaterThanOrEqual: return ">=";
        case CmdIsLessThan: return "<";
        case CmdIsLessThanOrEqual: return "<=";
        case CmdIsNotEqual: return "!=";
        case CmdLabel: return "label";
        case CmdLogicalAnd: return "&&";
        case CmdLogicalNot: return "!";
        case CmdLogicalOr: return "||";
        case CmdModulus: return "%";
        case CmdMultiply: return "*";
        case CmdNegate: return "negate";
        case CmdPositive: return "positive";
        case CmdPostDecrement: return "x--";
        case CmdPostIncrement: return "x++";
        case CmdPreDecrement: return "--x";
        case CmdPreIncrement: return "++x";
        case CmdReturn: return "return";
        case CmdShiftLeft: return "<<";
        case CmdShiftRight: return ">>";
        case CmdSizeof: return "sizeof";
        case CmdSizeofType: return "sizeof type";
        case CmdStructElement: return ".";
        case CmdSubtract: return "-";
        case CmdSwitch: return "switch";
        case CmdTernaryOperator: return "?:";
        case CmdWhile: return "while";
        
        /* nodes used only when creating parse trees */
        case CmdDeclarator: return "declarator";
        case CmdElementDesignator: return "element designator";
        case CmdTypedInitialiser: return "typed initialiser";
    }
    
    return "error in parse tree";
}  


/*
 * NAME:        ParserInit
 * ACTION:      Initialise the parser to parse a file
 * PARAMETERS:  ParseState *Parser - the parser
 *              const char *FileName - the file to start parsing, NULL for console input
 */

void ParserInit(ParseState *Parser, const char *FileName)
{
    Parser->InputStream = NULL;
    Parser->IsInteractive = FALSE;
    Parser->SourceFileName = NULL;
    Parser->Location = NULL;
    
    PreProcInit(Parser, FileName);
    
    LexerInit(Parser, &Parser->Lexer);
}


/*
 * NAME:        ParserClose
 * ACTION:      Initialise the parser to parse a file
 * PARAMETERS:  ParseState *Parser - 
 */

void ParserClose(ParseState *Parser)
{
    LexerClose(Parser->Lexer);
    PreProcClose(Parser);
}


/*
 * NAME:        ParserParseC
 * ACTION:      Parse and compile/run a C file given only its filename
 * PARAMETERS:  const char *FileName - the file to compile/run or NULL 
 *                  for console input
 */
 
void ParserParseC(const char *FileName)
{
    ParseState Parser;
    const char *ModuleName = FileName;
    
    /* set things up */
    if (ModuleName == NULL)
        ModuleName = "minic";
    
    ParserInit(&Parser, FileName);
    
    /* parse the source into an abstract syntax tree */
    if (DCParser_parse(&Parser))
    	ProgramFail(&Parser, "parse error");
    
    /* show the parse tree */
    ParseTreeShow(Parser.Tree, 0, 0);
    
    /* compile and run the tree */
    CodeGenModule(&Parser, Parser.Tree, ModuleName, NULL, DEFAULT_OPTIMISATION_LEVEL, TRUE);

    /* clean up */
    ParserClose(&Parser);
}


/*
 * NAME:        ParseTreeNode
 * ACTION:      Create a parse tree node with no arguments
 * PARAMETERS:  enum Command Cmd - the parse tree command
 * RETURNS:     a newly allocated parse tree node
 */
 
static ParseTree *ParseTreeNode(enum Cmd Command, int NumArgs, int NumPtrs, ...)
{
    int PtrCount;
    va_list Args;

    /* only allocate as much space as we're going to use */
    int AllocSize = sizeof(ParseTree) - sizeof(union ParseTreeArgs) + NumPtrs * sizeof(ParseTree *);
    ParseTree *NewNode = (ParseTree *)CheckedAlloc(AllocSize);
    NewNode->Command = Command;
    NewNode->NumArgs = NumArgs;
    NewNode->ListNext = NULL;
    NewNode->ListLast = NewNode;
    
    /* copy the arguments across */
    va_start(Args, NumPtrs);
    for (PtrCount = 0; PtrCount < NumPtrs; PtrCount++)
        NewNode->a.Arg[PtrCount] = va_arg(Args, ParseTree *);
        
    va_end(Args);
    
    return NewNode;
}


/*
 * NAME:        ParseTreeXArgs
 * ACTION:      Create a parse tree node with no arguments
 * PARAMETERS:  enum Command Cmd - the parse tree command
 *              ParseTree *Arg1... - the parse tree arguments to this node
 * RETURNS:     a newly allocated parse tree node
 */
 
ParseTree *ParseTreeNoArgs(enum Cmd Command)
{
    return ParseTreeNode(Command, 0, 0);
}

ParseTree *ParseTree1Arg(enum Cmd Command, ParseTree *Arg1)
{
    return ParseTreeNode(Command, 1, 1, Arg1);
}
 
ParseTree *ParseTree2Args(enum Cmd Command, ParseTree *Arg1, ParseTree *Arg2)
{
    return ParseTreeNode(Command, 2, 2, Arg1, Arg2);
}

ParseTree *ParseTree3Args(enum Cmd Command, ParseTree *Arg1, ParseTree *Arg2, ParseTree *Arg3)
{
    return ParseTreeNode(Command, 3, 3, Arg1, Arg2, Arg3);
}

ParseTree *ParseTree4Args(enum Cmd Command, ParseTree *Arg1, ParseTree *Arg2, ParseTree *Arg3, ParseTree *Arg4)
{
    return ParseTreeNode(Command, 4, 4, Arg1, Arg2, Arg3, Arg4);
}

ParseTree *ParseTreeIdentifier(const char *Ident)
{
    return ParseTreeNode(CmdIdentifier, 0, 1, Ident);
}

ParseTree *ParseTreeConstant(Value *Val)
{
    return ParseTreeNode(CmdConstant, 0, 1, Val);
}

ParseTree *ParseTreeDataType(DataType *Typ)
{
    return ParseTreeNode(CmdDataType, 0, 1, Typ);
}

ParseTree *ParseTreeDeclaration(DataType *DeclType, ParseTree *Ident, ParseTree *InitVal)
{
    return ParseTreeNode(CmdDeclaration, 0, 3, DeclType, Ident, InitVal);
}


ParseTree *ParseTreeFunctionDeclaration(ParseState *Parser, DataType *DeclarationSpecifiers, ParseTree *Declarator, ParseTree *ParameterList, ParseTree *Actions)
{
    /* args - name, return type, args, actions */
    DataType *FullType = DataTypeCombine(Parser, DeclarationSpecifiers, Declarator->a.Arg[1]->a.DType);
    return ParseTree4Args(CmdFunctionDef, Declarator->a.Arg[0], ParseTreeDataType(FullType), ParameterList, Actions);
}


/*
 * NAME:        ParseTreeFree
 * ACTION:      Free a parse tree including all sub-nodes
 * PARAMETERS:  ParseTree *OldNode - the top of the parse tree to free
 */
 
void ParseTreeFree(ParseTree *OldNode)
{
    int ArgCount;
    ParseTree *ThisNode;
    ParseTree *NextNode;
    
    for (ThisNode = OldNode; ThisNode != NULL; ThisNode = NextNode)
    {
        /* free the arguments */
        for (ArgCount = 0; ArgCount < ThisNode->NumArgs; ArgCount++)
            ParseTreeFree(ThisNode->a.Arg[ArgCount]);
        
        /* when we loop around we'll free the next node in the list */
        NextNode = ThisNode->ListNext;
        
        /* free the node */        
        free(ThisNode);
    }
}


/*
 * NAME:        ParseTreeAppend
 * ACTION:      Append a parse tree node to a list - usually a block of statements
 * PARAMETERS:  ParseTree *List - the top of the parse tree to free
 *              ParseTree *AppendNode - the node to add to the end of the list
 */
 
ParseTree *ParseTreeAppend(ParseTree *List, ParseTree *AppendNode)
{
    List->ListLast->ListNext = AppendNode;
    List->ListLast = AppendNode->ListLast;
    
    return List;
}


/*
 * NAME:        ParseTreeShowIndent
 * ACTION:      Do some indent so we can see the shape of the parse tree
 * PARAMETERS:  int Indent - how much to indent by, usually 0
 *              int ArgNo - what argument number to report, 0 for none
 */
 
void ParseTreeShowIndent(int Indent, int ArgNo)
{
    int CCount;

    for (CCount = 0; CCount < Indent; CCount++)
        putchar(' ');
    
    if (ArgNo > 0)
        printf("arg %d: ", ArgNo);
}


/*
 * NAME:        ParseTreeShowString
 * ACTION:      Show the contents of string
 * PARAMETERS:  char *Str - the string to show
 */
 
void ParseTreeShowString(char *Str)
{
    int CCount;
    char *Pos = Str;
    
    for (CCount = 0; CCount < MAX_STRING_SHOW && *Pos != '\0'; CCount++, Pos++)
    {
        switch (*Pos)
        {
            case '\a': printf("\\a"); break;
            case '\b': printf("\\b"); break;
            case '\f': printf("\\f"); break;
            case '\n': printf("\\n"); break;
            case '\r': printf("\\r"); break;
            case '\t': printf("\\t"); break;
            case '\v': printf("\\v"); break;
            default:   putchar(isprint(*Pos) ? *Pos : '.'); break;
        }
    }
    
    if (*Pos != '\0')
        printf("...");
}


/*
 * NAME:        ParseTreeShowType
 * ACTION:      Show the contents of a single parse tree node
 * PARAMETERS:  DataType *DType - the data type to show
 */
 
void ParseTreeShowType(DataType *DType)
{
    if (DType->FromType)
        ParseTreeShowType(DType->FromType);
    
    if (DType->TypeOpt & TypeOptConst)      printf("const"); 
    if (DType->TypeOpt & TypeOptRestrict)   printf("restrict"); 
    if (DType->TypeOpt & TypeOptVolatile)   printf("volatile"); 
    if (DType->TypeOpt & TypeOptInline)     printf("inline"); 
    if (DType->TypeOpt & TypeOptExtern)     printf("extern"); 
    if (DType->TypeOpt & TypeOptStatic)     printf("static"); 
    if (DType->TypeOpt & TypeOptAuto)       printf("auto"); 
    if (DType->TypeOpt & TypeOptRegister)   printf("register"); 
    if (DType->TypeOpt & TypeOptSigned)     printf("signed"); 
    if (DType->TypeOpt & TypeOptUnsigned)   printf("unsigned"); 
    if (DType->TypeOpt & TypeOptShort)      printf("short"); 
    if (DType->TypeOpt & TypeOptLong)       printf("long"); 
        
    switch (DType->Basic)
    {
        case BasicTypeUnknown:  printf("unknown"); break;
        case BasicTypeVoid:     printf("void"); break;
        case BasicTypeBool:     printf("bool"); break;
        case BasicTypeChar:     printf("char"); break;
        case BasicTypeInt:      printf("int"); break;
        case BasicTypeDouble:   printf("double"); break;
        case BasicTypeComplex:  printf("complex"); break;
        case BasicTypeImaginary:printf("imaginary"); break;
        case BasicTypeFunction: printf("()"); break;
        case BasicTypePointer:  printf("*"); break;
        case BasicTypeArray:    printf("[%d]", DType->ArraySize); break;
        case BasicTypeStruct:   printf("struct %s", DType->Identifier); break;
        case BasicTypeUnion:    printf("union %s", DType->Identifier); break;
        case BasicTypeEnum:     printf("enum %s", DType->Identifier); break;
    }
}


/*
 * NAME:        ParseTreeShowValue
 * ACTION:      Show the contents of a Value
 * PARAMETERS:  Value *Val - the Value to show
 *              int Indent - how much to indent by, usually 0
 */
 
void ParseTreeShowValue(Value *Val)
{
    putchar('(');
    ParseTreeShowType(Val->Type);
    putchar(')');

    switch (Val->Type->Basic)
    {
        case BasicTypeUnknown: break;
        case BasicTypeVoid: break;
        case BasicTypeBool: printf("%s", Val->v.BoolVal ? "true" : "false"); break;
        case BasicTypeChar: printf("'%c':0x%02x", isprint(Val->v.UChar) ? Val->v.UChar : '.', Val->v.UChar); break;
        case BasicTypeInt:
        {
            if (Val->Type->TypeOpt & TypeOptShort)
                printf("%d", Val->v.SShort);
            else if (Val->Type->TypeOpt & TypeOptLong)
                printf("%ld", Val->v.SLong);
            else
                printf("%d", Val->v.SInt);
            
            break;
        }
        case BasicTypeDouble:
        {
            if (Val->Type->TypeOpt & TypeOptShort)
                printf("%f", Val->v.FPFloat); 
            else
                printf("%f", Val->v.FPDouble);
            break;
        }
        case BasicTypeComplex:   printf("%f,%f", Val->v.ComplexVal.i, Val->v.ComplexVal.j); break;
        case BasicTypeImaginary: printf("%f", Val->v.FPDouble); break;
        case BasicTypeFunction:  putchar('-'); break;
        case BasicTypePointer: 
        {
            printf("0x%lx", (unsigned long)Val->v.Pointer); 
            if (Val->Type->FromType->Basic == BasicTypeChar)
            {
                printf(" \"");
                ParseTreeShowString((char *)Val->v.Pointer);
                putchar('\"');
            }
            break;
        }
        case BasicTypeArray:  putchar('-'); break;
        case BasicTypeStruct: putchar('-'); break;
        case BasicTypeUnion:  putchar('-'); break;
        case BasicTypeEnum:   printf("%d", Val->v.SInt); break;
    }
}


/*
 * NAME:        ParseTreeShowNode
 * ACTION:      Show the contents of a single parse tree node
 * PARAMETERS:  ParseTree *Tree - the parse tree to show
 *              int Indent - how much to indent by, usually 0
 *              int ArgNo - what argument number to report, 0 for none
 */
 
void ParseTreeShowNode(ParseTree *Tree, int Indent, int ArgNo)
{
    int ArgCount;
    
    ParseTreeShowIndent(Indent, ArgNo);
    printf("%s", CommandName(Tree->Command));
    switch (Tree->Command)
    {
        case CmdConstant:   putchar(' '); ParseTreeShowValue(Tree->a.Val); putchar('\n'); break;
        case CmdIdentifier: printf(" '%s'\n", Tree->a.Identifier); break;
        case CmdDataType:   putchar(' '); ParseTreeShowType(Tree->a.DType); putchar('\n'); break;
        default:
            /* show all the arguments to this command */
            printf(":\n");
            for (ArgCount = 0; ArgCount < Tree->NumArgs; ArgCount++)
                ParseTreeShow(Tree->a.Arg[ArgCount], Indent+2, ArgCount+1);
            break;
    }
}


/*
 * NAME:        ParseTreeShow
 * ACTION:      Show the contents of a parse tree, including blocks of code etc.
 * PARAMETERS:  ParseTree *Tree - the parse tree to show
 *              int Indent - how much to indent by, usually 0
 *              int ArgNo - what argument number to report, 0 for none
 */
 
void ParseTreeShow(ParseTree *Tree, int Indent, int ArgNo)
{
    if (Tree == NULL)
    {
        ParseTreeShowIndent(Indent, ArgNo);
        printf("NULL\n");
    }
    else
    {
        if (Tree->ListNext != NULL)
        {
            /* it's a list of items, show it surrounded by brackets */
            ParseTree *TreeNode;
            int NewIndent = Indent;
            
            if (ArgNo > 0)
                NewIndent += 7;
    
            ParseTreeShowIndent(Indent, ArgNo);
            printf("(\n");
            
            for (TreeNode = Tree; TreeNode != NULL; TreeNode = TreeNode->ListNext)
                ParseTreeShowNode(TreeNode, NewIndent+2, 0);
    
            ParseTreeShowIndent(NewIndent, 0);
            printf(")\n");
        }
        else
            ParseTreeShowNode(Tree, Indent, ArgNo); /* just a single node */
    }
}
