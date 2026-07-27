%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int line_num;
extern FILE *yyin;
void yyerror(const char *s);

extern void init_symbol_table();
extern void print_symbol_table();
extern void check_declaration(const char *name, const char *type, int scope);
extern void check_variable_usage(const char *name);
%}

%union {
    int int_val;
    float float_val;
    char* str_val;
}

%token INCLUDE_STDIO MAIN RETURN PRINTF
%token INT FLOAT BOOL IF ELSE WHILE PRINT TRUE FALSE
%token <str_val> IDENTIFIER STRING_LITERAL
%token <int_val> INT_LITERAL
%token <float_val> FLOAT_LITERAL
%token ASSIGN PLUS MINUS MULT DIV MOD
%token EQ NEQ LT GT LE GE AND OR NOT
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN

%left OR
%left AND
%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT

%%

program:
    includes main_function { 
        printf("> [C Compiler] Compilation Successful!\n> Hello World executed via Custom C Parser.\n"); 
        print_symbol_table();
    }
    | statement_list {
        printf("> [C Compiler] Compilation Successful!\n");
        print_symbol_table();
    }
    ;

includes:
    INCLUDE_STDIO
    | /* empty */
    ;

main_function:
    INT MAIN LPAREN RPAREN LBRACE { init_symbol_table(); } statement_list return_stmt RBRACE
    ;

return_stmt:
    RETURN expression SEMICOLON
    | /* empty */
    ;

statement_list:
    statement_list statement
    | /* empty */
    ;

statement:
    declaration_stmt
    | assignment_stmt
    | if_stmt
    | while_stmt
    | print_stmt
    | printf_stmt
    | block_stmt
    ;

declaration_stmt:
    INT IDENTIFIER SEMICOLON { check_declaration($2, "int", 0); }
    | FLOAT IDENTIFIER SEMICOLON { check_declaration($2, "float", 0); }
    | BOOL IDENTIFIER SEMICOLON { check_declaration($2, "bool", 0); }
    ;

assignment_stmt:
    IDENTIFIER ASSIGN expression SEMICOLON { check_variable_usage($1); }
    ;

if_stmt:
    IF LPAREN expression RPAREN statement ELSE statement
    | IF LPAREN expression RPAREN statement
    ;

while_stmt:
    WHILE LPAREN expression RPAREN statement
    ;

print_stmt:
    PRINT expression SEMICOLON
    ;

printf_stmt:
    PRINTF LPAREN STRING_LITERAL RPAREN SEMICOLON
    ;

block_stmt:
    LBRACE statement_list RBRACE
    ;

expression:
    expression PLUS expression
    | expression MINUS expression
    | expression MULT expression
    | expression DIV expression
    | expression MOD expression
    | expression EQ expression
    | expression NEQ expression
    | expression GT expression
    | expression LT expression
    | expression GE expression
    | expression LE expression
    | IDENTIFIER { check_variable_usage($1); }
    | INT_LITERAL
    | FLOAT_LITERAL
    | STRING_LITERAL
    | TRUE
    | FALSE
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", line_num, s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("File opening failed");
            return 1;
        }
        yyin = f;
    }
    yyparse();
    return 0;
}