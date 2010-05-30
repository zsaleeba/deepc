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

using namespace std;
%}

/* %expect 1 */
%skeleton "lalr1.cc"            /* this will be a C++ parser */
%locations
%defines
%define parser_class_name "DCParser"
%error-verbose

%parse-param {Compiler &dcc}
%lex-param   {DCLexer &dcl}

%union 
{
    ParseTree *Tree;            /* a parse tree */
    DataType *DType;            /* a data type */
//    string Identifier;          /* an identifier */
}

%code {
	// Prototype for the yylex function
//	static int yylex(semantic_type *yylval,
//	                 location_type *yylloc,
//	                 DCLexer &dcl);
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
void Parser::error(Parser::location_type &Location, const std::string &Message)
{
    ProgramFail(Message);
}


//static int yylex(Parser::semantic_type *yylval,
//                 Parser::location_type *yylloc,
//                 DCLexer &l) 
//{
//	return scanner.yylex(yylval, yylloc);
//}

