%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int yylex();
%}

%union {
    int ival;
    float fval;
    char* sval;
}

%token PRINT INPUT STRIP INT_CAST FLOAT_CAST BOOL_TYPE CHAR_TYPE ASSIGN
%token <sval> IDENTIFIER STRING_LITERAL CHAR_LITERAL
%token <ival> INT_LITERAL BOOLEAN_LITERAL
%token <fval> FLOAT_LITERAL
%token EQ NE LE GE

%left '+' '-'
%left '*' '/'

%%

program:
    statement_list
    ;

statement_list:
    statement
    | statement_list statement
    ;

statement:
    PRINT '(' expression ')' { printf("Parsed print statement\n"); }
    | IDENTIFIER ASSIGN expression { printf("Assigned value to %s\n", $1); }
    ;

expression:
    INT_LITERAL
    | FLOAT_LITERAL
    | STRING_LITERAL
    | CHAR_LITERAL
    | BOOLEAN_LITERAL
    | IDENTIFIER
    | input_expression
    | cast_expression
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression EQ expression
    ;

input_expression:
    INPUT '(' ')' { printf("Parsed input()\n"); }
    | INPUT '(' STRING_LITERAL ')' { printf("Parsed input(prompt)\n"); }
    | INPUT '(' ')' '.' STRIP '(' ')' { printf("Parsed input().strip()\n"); }
    | INPUT '(' STRING_LITERAL ')' '.' STRIP '(' ')' { printf("Parsed input(prompt).strip()\n"); }
    ;

cast_expression:
    INT_CAST '(' input_expression ')' { printf("Parsed int(input(...))\n"); }
    | FLOAT_CAST '(' input_expression ')' { printf("Parsed float(input(...))\n"); }
    | INT_CAST '(' IDENTIFIER ')' { printf("Parsed int(variable)\n"); }
    | FLOAT_CAST '(' IDENTIFIER ')' { printf("Parsed float(variable)\n"); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}