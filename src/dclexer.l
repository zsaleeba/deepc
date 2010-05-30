Letter                  [[:alpha:]_]
LetNum                  [{Letter}[:digit:]]

/* flex options to work nicely with bison */
%option c++
%option yyclass="DCLexer"
%option yylineno
%option noyywrap
%option nodefault

%{
class ParseTree;
class DataType;

#include "compiler.h"
#include "preproc.h"
#include "dcparser.hpp"
#include "dclexerclass.h"

//#define YY_USER_ACTION yylloc->first_line = yylineno;

%}

%%
"/*"			{ Comment(); }
"//"[^\n]*              { /* consume c++-style comment */ }


"auto"			{ return yy::DCParser::token::AUTO; }
"break"			{ return yy::DCParser::token::BREAK; }
"case"			{ return yy::DCParser::token::CASE; }
"char"			{ return yy::DCParser::token::CHAR; }
"const"			{ return yy::DCParser::token::CONST; }
"continue"		{ return yy::DCParser::token::CONTINUE; }
"default"		{ return yy::DCParser::token::DEFAULT; }
"do"			{ return yy::DCParser::token::DO; }
"double"		{ return yy::DCParser::token::DOUBLE; }
"else"			{ return yy::DCParser::token::ELSE; }
"enum"			{ return yy::DCParser::token::ENUM; }
"extern"		{ return yy::DCParser::token::EXTERN; }
"float"			{ return yy::DCParser::token::FLOAT; }
"for"			{ return yy::DCParser::token::FOR; }
"goto"			{ return yy::DCParser::token::GOTO; }
"if"			{ return yy::DCParser::token::IF; }
"int"			{ return yy::DCParser::token::INT; }
"long"			{ return yy::DCParser::token::LONG; }
"return"		{ return yy::DCParser::token::RETURN; }
"short"			{ return yy::DCParser::token::SHORT; }
"signed"		{ return yy::DCParser::token::SIGNED; }
"sizeof"		{ return yy::DCParser::token::SIZEOF; }
"static"		{ return yy::DCParser::token::STATIC; }
"struct"		{ return yy::DCParser::token::STRUCT; }
"switch"		{ return yy::DCParser::token::SWITCH; }
"typedef"		{ return yy::DCParser::token::TYPEDEF; }
"union"			{ return yy::DCParser::token::UNION; }
"unsigned"		{ return yy::DCParser::token::UNSIGNED; }
"void"			{ return yy::DCParser::token::VOID; }
"volatile"		{ return yy::DCParser::token::VOLATILE; }
"while"			{ return yy::DCParser::token::WHILE; }

{Letter}{LetNum}*	{ return Identifier(); }

0[xX][:xdigit:]+	{ return IntegerConstant(yytext+2, 16); }
0[:digit:]+		{ return IntegerConstant(yytext+1, 8); }
[:digit:]+		{ return IntegerConstant(yytext, 10); }
'[^']*'			{ return CharConstant(); }

\"[^\"]*\"		{ return StringConstant(); }

".."			{ return yy::DCParser::token::RANGE; }
"..."			{ return yy::DCParser::token::ELLIPSIS; }
">>="			{ return yy::DCParser::token::RIGHT_ASSIGN; }
"<<="			{ return yy::DCParser::token::LEFT_ASSIGN; }
"+="			{ return yy::DCParser::token::ADD_ASSIGN; }
"-="			{ return yy::DCParser::token::SUBTRACT_ASSIGN; }
"*="			{ return yy::DCParser::token::MULTIPLY_ASSIGN; }
"/="			{ return yy::DCParser::token::DIVIDE_ASSIGN; }
"%="			{ return yy::DCParser::token::MOD_ASSIGN; }
"&="			{ return yy::DCParser::token::ARITHMETIC_AND_ASSIGN; }
"|="			{ return yy::DCParser::token::ARITHMETIC_OR_ASSIGN; }
"^="			{ return yy::DCParser::token::ARITHMETIC_XOR_ASSIGN; }
"&&="			{ return yy::DCParser::token::LOGICAL_AND_ASSIGN; }
"||="			{ return yy::DCParser::token::LOGICAL_OR_ASSIGN; }
"^^="			{ return yy::DCParser::token::LOGICAL_XOR_ASSIGN; }
">>"			{ return yy::DCParser::token::RIGHT_SHIFT; }
"<<"			{ return yy::DCParser::token::LEFT_SHIFT; }
"++"			{ return yy::DCParser::token::INCREMENT; }
"--"			{ return yy::DCParser::token::DECREMENT; }
"->"			{ return yy::DCParser::token::POINTER_ARROW; }
"&&"			{ return yy::DCParser::token::LOGICAL_AND; }
"||"			{ return yy::DCParser::token::LOGICAL_OR; }
"^^"			{ return yy::DCParser::token::LOGICAL_XOR; }
"<="			{ return yy::DCParser::token::LESS_EQUAL; }
">="			{ return yy::DCParser::token::GREATER_EQUAL; }
"=="			{ return yy::DCParser::token::EQUAL; }
"!="			{ return yy::DCParser::token::NOT_EQUAL; }
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
