%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_num = 1;
%}

%option noyywrap

%%

"#include"[ \t]*"<stdio.h>" { return INCLUDE_STDIO; }
"main"                      { return MAIN; }
"return"                    { return RETURN; }
"printf"                    { return PRINTF; }
"int"                       { return INT; }
"float"                     { return FLOAT; }
"bool"                      { return BOOL; }
"if"                        { return IF; }
"else"                      { return ELSE; }
"while"                     { return WHILE; }
"print"                     { return PRINT; }
"true"                      { return TRUE; }
"false"                     { return FALSE; }

\"[^\"]*\"                  { yylval.str_val = strdup(yytext); return STRING_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]*      { yylval.str_val = strdup(yytext); return IDENTIFIER; }
[0-9]+\.[0-9]+             { yylval.float_val = atof(yytext); return FLOAT_LITERAL; }
[0-9]+                     { yylval.int_val = atoi(yytext); return INT_LITERAL; }

"="                         { return ASSIGN; }
"+"                         { return PLUS; }
"-"                         { return MINUS; }
"*"                         { return MULT; }
"/"                         { return DIV; }
"%"                         { return MOD; }

"=="                        { return EQ; }
"!="                        { return NEQ; }
"<"                         { return LT; }
">"                         { return GT; }
"<="                        { return LE; }
">="                        { return GE; }

"&&"                        { return AND; }
"||"                        { return OR; }
"!"                         { return NOT; }

";"                         { return SEMICOLON; }
"{"                         { return LBRACE; }
"}"                         { return RBRACE; }
"("                         { return LPAREN; }
")"                         { return RPAREN; }

\n                          { line_num++; }
[ \t\r]+                    { /* Ignore whitespace */ }
"//".*                      { /* Ignore comments */ }

.                           { printf("Lexical Error at line %d: Unexpected character '%s'\n", line_num, yytext); }

%%