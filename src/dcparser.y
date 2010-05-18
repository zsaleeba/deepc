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

%type <Tree> primary_expression postfix_expression argument_expression_list
%type <Tree> unary_expression cast_expression multiplicative_expression
%type <Tree> additive_expression shift_expression relational_expression
%type <Tree> equality_expression and_expression exclusive_or_expression
%type <Tree> inclusive_or_expression logical_and_expression
%type <Tree> logical_or_expression conditional_expression 
%type <Tree> assignment_expression expression constant_expression
%type <Tree> declaration

%type <DType> declaration_specifiers

%type <Tree> designator designator_list
%type <Tree> initializer initializer_list
%type <Tree> init_declarator_list init_declarator
%type <Tree> declarator direct_declarator

%type <DType> storage_class_specifier type_qualifier
%type <DType> function_specifier type_qualifier_list
%type <DType> struct_or_union_specifier type_specifier
%type <DType> enum_specifier enumerator_list enumerator pointer
%type <DType> parameter_type_list parameter_list parameter_declaration
%type <DType> identifier_list type_name abstract_declarator 
%type <DType> direct_abstract_declarator struct_declaration_list
%type <DType> struct_declarator_list specifier_qualifier_list
%type <DType> struct_declaration struct_declarator

%type <Tree> statement labeled_statement compound_statement block_item_list
%type <Tree> block_item expression_statement selection_statement 
%type <Tree> iteration_statement jump_statement translation_unit 
%type <Tree> external_declaration function_definition declaration_list

%start module
%%

