%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int yylex();
%}>

%token PRINT NUMBER IDENTIFIER ASSIGN

%%

program:
    statement_list
    ;

statement_list:
    statement
    | statement_list statement
    ;

statement:
    PRINT IDENTIFIER
    | IDENTIFIER ASSIGN NUMBER
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}