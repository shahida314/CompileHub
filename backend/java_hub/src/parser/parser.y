%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yylineno;
extern char *yytext;
extern FILE *yyin;

void yyerror(const char *s);


#include "../symbol_table/symbol_table.c"
#include "../ast/ast.c"
#include "../semantic/semantic.c"

ASTNode *root = NULL;
%}
%union {
    char *str_val;
    struct ASTNode *node;
}

%token <str_val> INT FLOAT BOOL
%token <str_val> INT_LITERAL FLOAT_LITERAL IDENTIFIER
%token IF ELSE WHILE PRINT TRUE FALSE
%token PLUS MINUS MULT DIV MOD ASSIGN
%token EQ NEQ LT GT LE GE AND OR NOT
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN

%type <node> program stmt_list stmt decl_stmt assign_stmt expr
%type <str_val> type

%left OR
%left AND
%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT

%%

program:
    stmt_list { root = $1; }
    ;

stmt_list:
    stmt { $$ = $1; }
    | stmt stmt_list { $1->next = $2; $$ = $1; }
    ;

stmt:
    decl_stmt { $$ = $1; }
    | assign_stmt { $$ = $1; }
    | LBRACE { enter_scope(); } stmt_list RBRACE { exit_scope(); $$ = $3; }
    ;

type:
    INT { $$ = "int"; }
    | FLOAT { $$ = "float"; }
    | BOOL { $$ = "bool"; }
    ;

decl_stmt:
    type IDENTIFIER SEMICOLON {
        if (!insert_symbol($2, $1, yylineno)) {
            fprintf(stderr, "Semantic Error at line %d: Variable '%s' redeclared.\n", yylineno, $2);
            semantic_errors++;
        }
        $$ = create_node(NODE_DECL, $2, NULL, NULL);
        strcpy($$->data_type, $1);
    }
    ;

assign_stmt:
    IDENTIFIER ASSIGN expr SEMICOLON {
        check_declaration($1, yylineno);
        Symbol *s = lookup_symbol($1);
        if (s) {
            check_type_match(s->type, $3->data_type, yylineno);
        }
        $$ = create_node(NODE_ASSIGN, "=", create_node(NODE_VAR, $1, NULL, NULL), $3);
    }
    ;

expr:
    INT_LITERAL { $$ = create_node(NODE_NUM, $1, NULL, NULL); strcpy($$->data_type, "int"); }
    | FLOAT_LITERAL { $$ = create_node(NODE_NUM, $1, NULL, NULL); strcpy($$->data_type, "float"); }
    | TRUE { $$ = create_node(NODE_NUM, "true", NULL, NULL); strcpy($$->data_type, "bool"); }
    | FALSE { $$ = create_node(NODE_NUM, "false", NULL, NULL); strcpy($$->data_type, "bool"); }
    | IDENTIFIER { 
        check_declaration($1, yylineno);
        $$ = create_node(NODE_VAR, $1, NULL, NULL);
        Symbol *s = lookup_symbol($1);
        if (s) strcpy($$->data_type, s->type);
    }
    | expr PLUS expr { 
        $$ = create_node(NODE_BINOP, "+", $1, $3); 
        strcpy($$->data_type, $1->data_type);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d near token '%s': %s\n", yylineno, yytext, s);
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

    if (yyparse() == 0 && semantic_errors == 0) {
        printf("Parsing & Semantic Analysis Successful!\n");
        printf("\nGenerated AST Structure:\n");
        print_ast(root, 0);
    } else {
        printf("Compilation Failed due to errors.\n");
    }
    return 0;
}