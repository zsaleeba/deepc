Letter                  [[:alpha:]_]
LetNum                  [{Letter}_]

/* flex options to work nicely with bison */
%option reentrant
%option prefix="DCParser_"
%option bison-bridge bison-locations
%option yylineno
%option noyywrap

%{
#include "deepc.h"
#include "dcparser.h"

#define YY_USER_ACTION yylloc->first_line = yylineno;

#define YY_INPUT(buf,result,max_size) { result = PreProcRead(yyextra, buf, max_size); }

void DCLexerComment(ParseState *Parser);
%}

%%
"/*"			{ DCLexerComment(yyextra); }
"//"[^\n]*              { /* consume c++-style comment */ }


"auto"			{ return AUTO; }
"bool"			{ return BOOL; }
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
"inline"		{ return INLINE; }
"int"			{ return INT; }
"long"			{ return LONG; }
"register"		{ return REGISTER; }
"restrict"		{ return RESTRICT; }
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
'(\\.|[^\\'\n])+'	{ return LexerCharConstant(yyextra, &yylval->Tree, yytext); }

\"(\\.|[^\\"\n])*\"	{ return LexerStringConstant(yyextra, &yylval->Tree, yytext); }

".."			{ return RANGE; }
"..."			{ return ELLIPSIS; }
">>="			{ return RIGHT_ASSIGN; }
"<<="			{ return LEFT_ASSIGN; }
"+="			{ return ADD_ASSIGN; }
"-="			{ return SUB_ASSIGN; }
"*="			{ return MUL_ASSIGN; }
"/="			{ return DIV_ASSIGN; }
"%="			{ return MOD_ASSIGN; }
"&="			{ return AND_ASSIGN; }
"^="			{ return XOR_ASSIGN; }
"|="			{ return OR_ASSIGN; }
">>"			{ return RIGHT_OP; }
"<<"			{ return LEFT_OP; }
"++"			{ return INC_OP; }
"--"			{ return DEC_OP; }
"->"			{ return PTR_OP; }
"&&"			{ return AND_OP; }
"||"			{ return OR_OP; }
"<="			{ return LE_OP; }
">="			{ return GE_OP; }
"=="			{ return EQ_OP; }
"!="			{ return NE_OP; }
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
 * NAME:        CLexerComment
 * ACTION:      Absorb a comment
 * PARAMETERS:  ParseState *Parser - the parser
 */
 
void CLexerComment(ParseState *Parser)
{
    char c, prev = 0;
  
    while ((c = lexinput(Parser)) != 0)      /* (EOF maps to 0) */
    {
        if (c == '/' && prev == '*')
            return;
            
        prev = c;
    }
    
    ProgramFail(Parser, "unterminated comment");
}
