%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yylineno;
extern char *yytext;
extern FILE *yyin;

void yyerror(const char *s);

#include "src/symbol_table/symbol_table.c"
#include "src/ast/ast.c"
#include "src/semantic/semantic.c"
#include "src/codegen/codegen.c"
#include "src/interpreter/interpreter.c"


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
%type <node> if_stmt while_stmt print_stmt block
%type <str_val> type

%right NOT UMINUS
%left OR
%left AND
%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD

%%

program:
    stmt_list { root = $1; }
    ;

stmt_list:
    stmt { $$ = $1; }
    | stmt stmt_list { $1->next = $2; $$ = $1; }
    ;

block:
    LBRACE { enter_scope(); } stmt_list RBRACE { exit_scope(); $$ = $3; }
    ;

stmt:
    decl_stmt { $$ = $1; }
    | assign_stmt { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | print_stmt { $$ = $1; }
    | block { $$ = create_node(NODE_BLOCK, "block", $1, NULL); }
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

if_stmt:
    IF LPAREN expr RPAREN block {
        $$ = create_node(NODE_IF, "if", $3, $5);
    }
    | IF LPAREN expr RPAREN block ELSE block {
        $$ = create_node(NODE_IF, "if", $3, $5);
        $$->extra = $7;
    }
    ;

while_stmt:
    WHILE LPAREN expr RPAREN block {
        $$ = create_node(NODE_WHILE, "while", $3, $5);
    }
    ;

print_stmt:
    PRINT expr SEMICOLON {
        $$ = create_node(NODE_PRINT, "print", $2, NULL);
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
    | LPAREN expr RPAREN { $$ = $2; }
    | MINUS expr %prec UMINUS {
        $$ = create_node(NODE_UNOP, "-", $2, NULL);
        strcpy($$->data_type, $2->data_type);
    }
    | NOT expr {
        $$ = create_node(NODE_UNOP, "!", $2, NULL);
        strcpy($$->data_type, "bool");
    }
    | expr PLUS expr {
        $$ = create_node(NODE_BINOP, "+", $1, $3);
        strcpy($$->data_type, $1->data_type);
    }
    | expr MINUS expr {
        $$ = create_node(NODE_BINOP, "-", $1, $3);
        strcpy($$->data_type, $1->data_type);
    }
    | expr MULT expr {
        $$ = create_node(NODE_BINOP, "*", $1, $3);
        strcpy($$->data_type, $1->data_type);
    }
    | expr DIV expr {
        $$ = create_node(NODE_BINOP, "/", $1, $3);
        strcpy($$->data_type, $1->data_type);
    }
    | expr MOD expr {
        $$ = create_node(NODE_BINOP, "%", $1, $3);
        strcpy($$->data_type, $1->data_type);
    }
    | expr EQ expr {
        $$ = create_node(NODE_BINOP, "==", $1, $3);
        strcpy($$->data_type, "bool");
    }
    | expr NEQ expr {
        $$ = create_node(NODE_BINOP, "!=", $1, $3);
        strcpy($$->data_type, "bool");
    }
    | expr LT expr {
        $$ = create_node(NODE_BINOP, "<", $1, $3);
        strcpy($$->data_type, "bool");
    }
    | expr GT expr {
        $$ = create_node(NODE_BINOP, ">", $1, $3);
        strcpy($$->data_type, "bool");
    }
    | expr LE expr {
        $$ = create_node(NODE_BINOP, "<=", $1, $3);
        strcpy($$->data_type, "bool");
    }
    | expr GE expr {
        $$ = create_node(NODE_BINOP, ">=", $1, $3);
        strcpy($$->data_type, "bool");
    }
    | expr AND expr {
        $$ = create_node(NODE_BINOP, "&&", $1, $3);
        strcpy($$->data_type, "bool");
    }
    | expr OR expr {
        $$ = create_node(NODE_BINOP, "||", $1, $3);
        strcpy($$->data_type, "bool");
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

   int debug_mode = 0;
    if (argc > 2 && strcmp(argv[2], "--debug") == 0) {
        debug_mode = 1;
    }

    if (yyparse() == 0 && semantic_errors == 0) {
        if (debug_mode) {
            printf("Parsing & Semantic Analysis Successful!\n");
            printf("\nGenerated AST Structure:\n");
            print_ast(root, 0);
            generate_tac(root);
            printf("\n");
        }
        run_program(root);
    } else {
        printf("Compilation Failed due to errors.\n");
    }
    return 0;
}