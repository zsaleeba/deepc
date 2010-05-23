Letter                  [[:alpha:]_]
LetNum                  [{Letter}[:digit:]]

/* flex options to work nicely with bison */
%option reentrant
%option prefix="DCParser_"
%option bison-bridge bison-locations
%option yylineno
%option noyywrap

%{
#include "deepc.h"
#include "dcparser.h"

//#define YY_USER_ACTION yylloc->first_line = yylineno;

#define YY_INPUT(buf,result,max_size) { result = PreProcRead(yyextra, buf, max_size); }

void DCLexerComment(ParseState *Parser);
%}

%%
"/*"			{ DCLexerComment(yyextra); }
"//"[^\n]*              { /* consume c++-style comment */ }


"auto"			{ return AUTO; }
"break"			{ return BREAK; }
"case"			{ return CASE; }
"char"			{ return CHAR; }
"const"			{ return CONST; }
"continue"		{ return CONTINUE; }
"default"		{ return DEFAULT; }
"do"			{ return DO; }
"double"		{ return DOUBLE; }
"else"			{ return ELSE; }
"enum"			{ return ENUM; }
"extern"		{ return EXTERN; }
"float"			{ return FLOAT; }
"for"			{ return FOR; }
"goto"			{ return GOTO; }
"if"			{ return IF; }
"int"			{ return INT; }
"long"			{ return LONG; }
"return"		{ return RETURN; }
"short"			{ return SHORT; }
"signed"		{ return SIGNED; }
"sizeof"		{ return SIZEOF; }
"static"		{ return STATIC; }
"struct"		{ return STRUCT; }
"switch"		{ return SWITCH; }
"typedef"		{ return TYPEDEF; }
"union"			{ return UNION; }
"unsigned"		{ return UNSIGNED; }
"void"			{ return VOID; }
"volatile"		{ return VOLATILE; }
"while"			{ return WHILE; }

{Letter}{LetNum}*	{ return LexerIdentifier(yyextra, &yylval->Identifier, yytext); }

0[xX][:xdigit:]+	{ return LexerIntegerConstant(yyextra, &yylval->Tree, yytext+2, 16); }
0[:digit:]+		{ return LexerIntegerConstant(yyextra, &yylval->Tree, yytext+1, 8); }
[:digit:]+		{ return LexerIntegerConstant(yyextra, &yylval->Tree, yytext, 10); }
'[^']*'			{ return LexerCharConstant(yyextra, &yylval->Tree, yytext); }

\"[^\"]*\"		{ return LexerStringConstant(yyextra, &yylval->Tree, yytext); }

".."			{ return RANGE; }
"..."			{ return ELLIPSIS; }
">>="			{ return RIGHT_ASSIGN; }
"<<="			{ return LEFT_ASSIGN; }
"+="			{ return ADD_ASSIGN; }
"-="			{ return SUBTRACT_ASSIGN; }
"*="			{ return MULTIPLY_ASSIGN; }
"/="			{ return DIVIDE_ASSIGN; }
"%="			{ return MOD_ASSIGN; }
"&="			{ return ARITHMETIC_AND_ASSIGN; }
"|="			{ return ARITHMETIC_OR_ASSIGN; }
"^="			{ return ARITHMETIC_XOR_ASSIGN; }
"&&="			{ return LOGICAL_AND_ASSIGN; }
"||="			{ return LOGICAL_OR_ASSIGN; }
"^^="			{ return LOGICAL_XOR_ASSIGN; }
">>"			{ return RIGHT_SHIFT; }
"<<"			{ return LEFT_SHIFT; }
"++"			{ return INCREMENT; }
"--"			{ return DECREMENT; }
"->"			{ return POINTER_ARROW; }
"&&"			{ return LOGICAL_AND; }
"||"			{ return LOGICAL_OR; }
"^^"			{ return LOGICAL_XOR; }
"<="			{ return LESS_EQUAL; }
">="			{ return GREATER_EQUAL; }
"=="			{ return EQUAL; }
"!="			{ return NOT_EQUAL; }
";"			{ return ';'; }
"{"			{ return '{'; }
"}"			{ return '}'; }
","			{ return ','; }
":"			{ return ':'; }
"="			{ return '='; }
"("			{ return '('; }
")"			{ return ')'; }
"["			{ return '['; }
"]"			{ return ']'; }
"."			{ return '.'; }
"&"			{ return '&'; }
"!"			{ return '!'; }
"~"			{ return '~'; }
"-"			{ return '-'; }
"+"			{ return '+'; }
"*"			{ return '*'; }
"/"			{ return '/'; }
"%"			{ return '%'; }
"<"			{ return '<'; }
">"			{ return '>'; }
"^"			{ return '^'; }
"|"			{ return '|'; }
"?"			{ return '?'; }

[ \t\v\n\f]		{ }
.			{ /* Add code to complain about unmatched characters */ }

%%

#ifdef __cplusplus
#define lexinput(x) yyinput(x)
#else
#define lexinput(x) input(x)
#endif

/*
 * NAME:        DCLexerComment
 * ACTION:      Absorb a comment
 * PARAMETERS:  ParseState *Parser - the parser
 */
 
void DCLexerComment(ParseState *Parser)
{
}
