%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int line_num;
extern FILE *yyin;
void yyerror(const char *s);
%}

%token INCLUDE_STDIO RETURN PRINT PRINTF
%token INT FLOAT BOOL IF ELSE WHILE TRUE FALSE
%token IDENTIFIER STRING_LITERAL INT_LITERAL FLOAT_LITERAL
%token ASSIGN PLUS MINUS MULT DIV MOD
%token EQ NEQ LT GT LE GE AND OR NOT
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN COMMA

%nonassoc IFX
%nonassoc ELSE

%left OR
%left AND
%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT

%%

program:
    includes statement_list
    | statement_list
    ;

includes:
    INCLUDE_STDIO
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
    | block_stmt
    ;

type_spec:
    INT | FLOAT | BOOL
    ;

declaration_stmt:
    type_spec IDENTIFIER SEMICOLON
    | type_spec IDENTIFIER ASSIGN expression SEMICOLON
    ;

assignment_stmt:
    IDENTIFIER ASSIGN expression SEMICOLON
    ;

if_stmt:
    IF LPAREN expression RPAREN statement ELSE statement
    | IF LPAREN expression RPAREN statement %prec IFX
    ;

while_stmt:
    WHILE LPAREN expression RPAREN statement
    ;

print_stmt:
    PRINT IDENTIFIER SEMICOLON
    | PRINT expression SEMICOLON
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
    | IDENTIFIER
    | INT_LITERAL
    | FLOAT_LITERAL
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
    
    if (yyparse() == 0) {
        printf("Parsing Successful! No Syntax Errors.\n");
    } else {
        printf("Parsing Failed.\n");
    }
    return 0;
}