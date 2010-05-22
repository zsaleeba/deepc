/*
 * bison grammar for Deep C
 *
 * by Zik Saleeba 2010-05-16
 */

%{
#include "deepc.h"

#define Lexer Parser->Lexer
%}

/* %expect 1 */
%pure-parser
%name-prefix="DCParser_"
%locations
%defines
%error-verbose

%lex-param   {void *Lexer}
%parse-param {ParseState *Parser}

%union 
{
    ParseTree *Tree;            /* a parse tree */
    DataType *DType;            /* a data type */
    const char *Identifier;     /* an identifier */
}

%token TYPEDEF EXTERN STATIC AUTO REGISTER INLINE RESTRICT CONST VOLATILE

%token SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token BOOL COMPLEX IMAGINARY 
%token STRUCT UNION ENUM ELLIPSIS

%token <Tree> CONSTANT STRING_LITERAL
%token <Identifier> IDENTIFIER TYPE_NAME

%start module
%%

module
    : CONSTANT

%%
