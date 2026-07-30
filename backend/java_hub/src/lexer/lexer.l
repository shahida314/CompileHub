%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

void yyerror(const char *s);
%}

%option noyywrap
%option yylineno

%%

"//".*                  { }
"/*"([^*]|\*+[^/*])*\*+"/" { }

"int"       { yylval.str_val = strdup(yytext); return INT; }
"float"     { yylval.str_val = strdup(yytext); return FLOAT; }
"bool"      { yylval.str_val = strdup(yytext); return BOOL; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"print"     { return PRINT; }
"true"      { return TRUE; }
"false"     { return FALSE; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"%"         { return MOD; }
"="         { return ASSIGN; }

"=="        { return EQ; }
"!="        { return NEQ; }
"<"         { return LT; }
">"         { return GT; }
"<="        { return LE; }
">="        { return GE; }
"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }

";"         { return SEMICOLON; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"("         { return LPAREN; }
")"         { return RPAREN; }

[0-9]+\.[0-9]+ { yylval.str_val = strdup(yytext); return FLOAT_LITERAL; }
[0-9]+         { yylval.str_val = strdup(yytext); return INT_LITERAL; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str_val = strdup(yytext); return IDENTIFIER; }

\n          { }
[ \t\r]+    { }

. { fprintf(stderr, "Lexical Error at line %d: Unexpected character '%s'\n", yylineno, yytext); }

%%