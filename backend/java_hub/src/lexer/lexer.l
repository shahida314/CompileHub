%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

int line_num = 1;
void yyerror(const char *s);
%}

%option noyywrap

%%

"//".*                  { }
"/*"([^*]|\*+[^/*])*\*+"/" { 
    for (int i = 0; i < yyleng; i++) {
        if (yytext[i] == '\n') line_num++;
    }
}

"#include"[ \t]*"<stdio.h>"[ \t]*\n { line_num++; return INCLUDE_STDIO; }
"#include"[ \t]*"<stdio.h>"        { return INCLUDE_STDIO; }

"int"       { return INT; }
"float"     { return FLOAT; }
"bool"      { return BOOL; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"return"    { return RETURN; }
"print"     { return PRINT; }
"printf"    { return PRINTF; }
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
","         { return COMMA; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"("         { return LPAREN; }
")"         { return RPAREN; }

[0-9]+\.[0-9]+ { return FLOAT_LITERAL; }
[0-9]+         { return INT_LITERAL; }
\"([^\"\\\n]|\\.)*\" { return STRING_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]* { return IDENTIFIER; }

\n          { line_num++; }
[ \t\r]+    { }

. {
    fprintf(stderr, "Lexical Error at line %d: Unexpected character '%s'\n", line_num, yytext);
}

%%