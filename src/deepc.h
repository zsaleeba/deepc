#ifndef DEEPC_H
#define DEEPC_H

#if 0
/* some extra data so the lexer can track the parser state */
#define YY_EXTRA_TYPE ParseState *

/* forward declarations of types */
typedef struct ParseTreeStruct ParseTree;
typedef struct DataTypeStruct DataType;
typedef struct ParseStateStruct ParseState;
typedef struct ValueStruct Value;
typedef struct SymTableEntryStruct SymTableEntry;
typedef struct SymTableStruct SymTable;
typedef struct ComplexStruct Complex;


/* parser-related declarations */
#include "dcparser.h"


/* handy definitions */
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef min
#define min(x,y) (((x)<(y))?(x):(y))
#endif

/* constants */
#define MAX_PARSE_TREE_ARGS 4
#define INITIAL_STRING_TABLE_SIZE 1000

/* hashed symbol table data structure */
struct SymTableEntryStruct
{
    SymTableEntry *Next;                /* next item in this hash chain */
    unsigned long Key;                  /* a hash value for this entry */
};

struct SymTableStruct
{
    int Size;                           /* current size of the table */
    int SizeIndex;                      /* index to the current size of the table in the TableSize array */
    int Entries;                        /* the number of entries stored in the table */
    SymTableEntry **HashTable;          /* the hash table itself */
};

/* the state of a parse */
struct ParseStateStruct
{
    /* input */
    FILE *InputStream;
    int IsInteractive;
    const char *SourceFileName;
    
    /* lexer */
    YYLTYPE *Location;
    void *Lexer;
    
    /* parser result */
    ParseTree *Tree;
    
};

/* data types */
enum BasicType
{
    BasicTypeUnknown,               /* no type has been given */
    BasicTypeVoid,                  /* a type with no data */
    BasicTypeBool,                  /* a boolean value */
    BasicTypeChar,                  /* a character */
    BasicTypeInt,                   /* integer */
    BasicTypeDouble,                /* double precision floating point */
    BasicTypeComplex,               /* a complex double precision floating point */
    BasicTypeImaginary,             /* an imaginary double precision floating point */
    BasicTypeFunction,              /* a function */
    BasicTypePointer,               /* a pointer */
    BasicTypeArray,                 /* an array of a sub-type */
    BasicTypeStruct,                /* aggregate type */
    BasicTypeUnion,                 /* merged type */
    BasicTypeEnum                   /* enumerated integer type */
};

struct DataTypeStruct
{
    enum BasicType Basic;           /* what kind of type this is */
    int TypeOpt;                    /* options for this type from enum TypeOptEnum */
    int Sizeof;                     /* the memory size of this type */
    int ArraySize;                  /* the size of an array type */
    const char *Identifier;         /* the name of a struct or union */
    DataType *FromType;             /* the type we're derived from (or NULL) */
    DataType *DerivedTypeList;      /* first in a list of types derived from this one */
    DataType *Next;                 /* next item in the derived type list */
    SymTable *Members;              /* members of a struct or union */
};

/* values */
struct ComplexStruct
{
    double i;
    double j;
};

struct ArrayValue 
{
    int Size;                       /* -1 if the size is undefined */
    void *Data;                     /* the array data */
};

struct ValueStruct
{
    DataType *Type;                 /* the type of this value */
    union AnyValue                  /* the value will be one of these possibilities */
    {
        /* types used for C values */
        unsigned char BoolVal;
        unsigned char UChar;
        signed char SChar;
        unsigned short UShort;
        signed short SShort;
        unsigned int UInt;
        signed int SInt;
        unsigned long ULong;
        signed long SLong;
        float FPFloat;
        double FPDouble;
        Complex ComplexVal;
        void *Pointer;
        struct ArrayValue Array;
    } v;
};

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


/* parse trees */
union ParseTreeArgs
{
    Value *Val;                 /* value argument for CmdConstant constants */
    const char *Identifier;     /* identifier when doing CmdIdentifier */
    DataType *DType;            /* data type when doing CmdType */
    ParseTree *Arg[MAX_PARSE_TREE_ARGS]; /* sub-trees for command parameters */
};

struct ParseTreeStruct
{
    enum Cmd Command;               /* what type of command this node represents */
    ParseTree *ListNext;            /* the next tree node in a linked list of statements */
    ParseTree *ListLast;            /* the last tree node in a linked list of statements */

    /* arguments to this node */
    int NumArgs;                    /* the number of arguments */
    union ParseTreeArgs a;    
};

/* type specifiers used in parsing */
enum TypeOptEnum
{
    TypeOptNone =       0x0000,
    TypeOptConst =      0x0001,
    TypeOptRestrict =   0x0002,
    TypeOptVolatile =   0x0004,
    TypeOptInline =     0x0008,
    TypeOptTypedef =    0x0010,
    TypeOptExtern =     0x0020,
    TypeOptStatic =     0x0040,
    TypeOptAuto =       0x0080,
    TypeOptRegister =   0x0100,
    TypeOptShort =      0x0200,
    TypeOptLong =       0x0400,
    TypeOptSigned =     0x0800,
    TypeOptUnsigned =   0x1000
};


/* predefined types */
extern DataType *TypeUber;
extern DataType *TypeUnknown;
extern DataType *TypeVoid;
extern DataType *TypeBool;
extern DataType *TypeUChar;
extern DataType *TypeSChar;
extern DataType *TypeUShort;
extern DataType *TypeSShort;
extern DataType *TypeUInt;
extern DataType *TypeSInt;
extern DataType *TypeULong;
extern DataType *TypeSLong;
extern DataType *TypeFloat;
extern DataType *TypeDouble;
extern DataType *TypeComplex;
extern DataType *TypeImaginary;
extern DataType *TypeCharPtr;
extern DataType *TypeCharArray;
extern DataType *TypeVoidPtr;
extern const char *StrEmpty;

/* clexer.l */
int DCParser_lex(YYSTYPE *yylval_param, struct YYLTYPE *Location, void *Lexer);

/* lexer.c */
void LexerInit(ParseState *Parser, void **LexerPtr);
void LexerClose(void *Lexer);
int LexerCheckType(ParseState *Parser, const char **Dest, const char *SrcText);
int LexerIntegerConstant(ParseState *Parser, ParseTree **Dest, const char *SrcText, int Base);
int LexerCharConstant(ParseState *Parser, ParseTree **Dest, const char *SrcText);
int LexerStringConstant(ParseState *Parser, ParseTree **Dest, const char *SrcText);
int LexerIdentifier(ParseState *Parser, const char **Dest, const char *SrcText);

/* cparser.y */
void DCParser_error(struct YYLTYPE *Location, ParseState *Parser, char const *s);
int DCParser_parse(ParseState *Parser);

/* parser.c */
void ParserInit(ParseState *Parser, const char *FileName);
void ParserClose(ParseState *Parser);
void ParserParseC(const char *FileName);
ParseTree *ParseTreeNoArgs(enum Cmd Command);
ParseTree *ParseTree1Arg(enum Cmd Command, ParseTree *Arg1);
ParseTree *ParseTree2Args(enum Cmd Command, ParseTree *Arg1, ParseTree *Arg2);
ParseTree *ParseTree3Args(enum Cmd Command, ParseTree *Arg1, ParseTree *Arg2, ParseTree *Arg3);
ParseTree *ParseTree4Args(enum Cmd Command, ParseTree *Arg1, ParseTree *Arg2, ParseTree *Arg3, ParseTree *Arg4);
ParseTree *ParseTreeIdentifier(const char *Ident);
ParseTree *ParseTreeConstant(Value *Val);
ParseTree *ParseTreeDeclaration(DataType *DeclType, ParseTree *Ident, ParseTree *InitVal);
ParseTree *ParseTreeDataType(DataType *Typ);
ParseTree *ParseTreeFunctionDeclaration(ParseState *Parser, DataType *DeclarationSpecifiers, ParseTree *Declarator, ParseTree *ParameterList, ParseTree *Actions);
void ParseTreeFree(ParseTree *OldNode);
ParseTree *ParseTreeAppend(ParseTree *List, ParseTree *AppendNode);
void ParseTreeShow(ParseTree *Tree, int Indent, int ArgNo);

/* preproc.c */
void PreProcInit(ParseState *Parser, const char *FileName);
void PreProcClose(ParseState *Parser);
int PreProcRead(ParseState *Parser, char *ReadBuf, int MaxSize);

/* datatype.c */
void DataTypeInit();
void DataTypeCleanup();
DataType *DataTypeGetMatching(ParseState *Parser, DataType *ParentType, enum BasicType Basic, int TypeOptions, int ArraySize, const char *Identifier);
DataType *DataTypeGetTypedef(const char *Identifier);
Value *DataTypeAllocValue(DataType *Type);
DataType *DataTypeCombine(ParseState *Parser, DataType *LeftType, DataType *RightType);
DataType *DataTypeFromTypeSpec(int TypeSpec);

/* symtable.c */
void SymTableInit(SymTable *Tbl, int Size);
void SymTableClose(SymTable *Tbl);
int SymTableSet(SymTable *Tbl, const char *Key, Value *Val);
int SymTableGet(SymTable *Tbl, const char *Key, Value **Val);
Value *SymTableDelete(SymTable *Tbl, const char *Key);
void SymTableResize(SymTable *Tbl);

/* sharestr.c */
void ShareStrInit();
void ShareStrClose();
const char *ShareStr(const char *Str);
const char *ShareStr2(const char *Str, int Len);

/* codegen.cpp */
void CodeGenInit();
void CodeGenModule(ParseState *Parser, ParseTree *Tree, const char *ModuleName, const char *OutFileName, int OptimisationLevel, int RunIt);

/* deepc.c */
void Cleanup(ParseState *Parser);

/* lib.c */
void ProgramFail(ParseState *Parser, const char *Format, ...);
void ProgramErrorMessage(ParseState *Parser, const char *Format, ...);
void *CheckedAlloc(int Size);
#endif

#endif /* DEEPC_H */
