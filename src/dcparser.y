/*
 * bison grammar for Deep C
 *
 * by Zik Saleeba 2010-05-16
 */

%{
#include <string>

class ParseTree;
class DataType;
class Compiler;
class DCLexer;

#include <FlexLexer.h>

#include "dclexerclass.h"

using namespace std;

// tell the parser to call our lexer object instead of a global yylex function
#define yylex(yylval, yylloc, dcl) dcl->yylex(yylval, yylloc)

%}

/* %expect 1 */
%skeleton "lalr1.cc"            /* this will be a C++ parser */
%locations
%defines
%define parser_class_name "DCParser"
%error-verbose

%parse-param {Compiler *dcc}
%parse-param {DCLexer *dcl}
%lex-param   {DCLexer *dcl}

%union 
{
    ParseTree *Tree;            /* a parse tree */
    DataType *DType;            /* a data type */
    string *Identifier;         /* an identifier */
}

%token TYPEDEF EXTERN STATIC AUTO REGISTER INLINE RESTRICT CONST VOLATILE

%token SIZEOF
%token POINTER_ARROW INCREMENT DECREMENT LEFT_SHIFT RIGHT_SHIFT LESS_EQUAL
%token GREATER_EQUAL EQUAL NOT_EQUAL LOGICAL_AND LOGICAL_OR LOGICAL_XOR
%token MULTIPLY_ASSIGN DIVIDE_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUBTRACT_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN ARITHMETIC_AND_ASSIGN ARITHMETIC_OR_ASSIGN
%token ARITHMETIC_XOR_ASSIGN LOGICAL_AND_ASSIGN LOGICAL_OR_ASSIGN
%token LOGICAL_XOR_ASSIGN 
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token BOOL COMPLEX IMAGINARY 
%token STRUCT UNION ENUM RANGE ELLIPSIS

%token <Tree> CONSTANT STRING_LITERAL
%token <Identifier> IDENTIFIER TYPE_NAME

%start module
%%

module
    : CONSTANT

%%

namespace yy {

    void DCParser::error(const location_type &Loc, const std::string &Message)
    {
        cout << Message;
    }

};
