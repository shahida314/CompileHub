%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"
#include "../semantic/semantic.h"
#include "../codegen/codegen.h"

extern int yylex();
extern int line_num;
extern FILE *yyin;
void yyerror(const char *s);

ASTNode *ast_root = NULL;
%}

%union {
    int ival;
    float fval;
    char *sval;
    struct ASTNode *node;
    int dtype;
}

%token <sval> IDENTIFIER STRING_LITERAL
%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token INT_TYPE FLOAT_TYPE BOOL_TYPE
%token IF ELSE WHILE PRINT TRUE_TOK FALSE_TOK
%token ASSIGN PLUS MINUS MULT DIV MOD
%token EQ NEQ LT GT LE GE AND OR NOT
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN COMMA

%type <node> program stmt_list stmt decl_stmt assign_stmt if_stmt while_stmt print_stmt block_stmt expr
%type <dtype> type_spec

%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT UMINUS

%%

program:
    stmt_list { ast_root = $1; }
    ;

stmt_list:
    stmt_list stmt {
        if ($1 == NULL) { $$ = $2; }
        else {
            ASTNode *cur = $1;
            while (cur->next) cur = cur->next;
            cur->next = $2;
            $$ = $1;
        }
    }
    | /* empty */ { $$ = NULL; }
    ;

stmt:
    decl_stmt
    | assign_stmt
    | if_stmt
    | while_stmt
    | print_stmt
    | block_stmt
    ;

type_spec:
    INT_TYPE    { $$ = TYPE_INT; }
    | FLOAT_TYPE  { $$ = TYPE_FLOAT; }
    | BOOL_TYPE   { $$ = TYPE_BOOL; }
    ;

decl_stmt:
    type_spec IDENTIFIER SEMICOLON {
        check_declaration($2, $1, line_num);
        ASTNode *n = create_node(NODE_DECL);
        n->str_val = $2;
        n->data_type = $1;
        $$ = n;
    }
    | type_spec IDENTIFIER ASSIGN expr SEMICOLON {
        check_declaration($2, $1, line_num);
        ASTNode *n = create_node(NODE_DECL);
        n->str_val = $2;
        n->data_type = $1;
        n->left = $4;
        check_assignment($2, $4, line_num);
        $$ = n;
    }
    ;

assign_stmt:
    IDENTIFIER ASSIGN expr SEMICOLON {
        check_assignment($1, $3, line_num);
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = $1;
        n->left = $3;
        $$ = n;
    }
    ;

if_stmt:
    IF LPAREN expr RPAREN stmt ELSE stmt {
        ASTNode *n = create_node(NODE_IF);
        n->cond = $3;
        n->then_branch = $5;
        n->else_branch = $7;
        $$ = n;
    }
    | IF LPAREN expr RPAREN stmt {
        ASTNode *n = create_node(NODE_IF);
        n->cond = $3;
        n->then_branch = $5;
        n->else_branch = NULL;
        $$ = n;
    }
    ;

while_stmt:
    WHILE LPAREN expr RPAREN stmt {
        ASTNode *n = create_node(NODE_WHILE);
        n->cond = $3;
        n->body = $5;
        $$ = n;
    }
    ;

print_stmt:
    PRINT expr SEMICOLON {
        ASTNode *n = create_node(NODE_PRINT);
        n->left = $2;
        $$ = n;
    }
    ;

block_stmt:
    LBRACE { enter_scope(); } stmt_list RBRACE {
        exit_scope();
        ASTNode *n = create_node(NODE_BLOCK);
        n->body = $3;
        $$ = n;
    }
    ;

expr:
    expr PLUS expr        { $$ = create_binop(PLUS, $1, $3); }
    | expr MINUS expr       { $$ = create_binop(MINUS, $1, $3); }
    | expr MULT expr        { $$ = create_binop(MULT, $1, $3); }
    | expr DIV expr         { $$ = create_binop(DIV, $1, $3); }
    | expr MOD expr         { $$ = create_binop(MOD, $1, $3); }
    | expr LT expr          { $$ = create_binop(LT, $1, $3); }
    | expr GT expr          { $$ = create_binop(GT, $1, $3); }
    | expr LE expr          { $$ = create_binop(LE, $1, $3); }
    | expr GE expr          { $$ = create_binop(GE, $1, $3); }
    | expr EQ expr          { $$ = create_binop(EQ, $1, $3); }
    | expr NEQ expr         { $$ = create_binop(NEQ, $1, $3); }
    | expr AND expr         { $$ = create_binop(AND, $1, $3); }
    | expr OR expr          { $$ = create_binop(OR, $1, $3); }
    | NOT expr              { $$ = create_unop(NOT, $2); }
    | MINUS expr %prec UMINUS { $$ = create_unop(MINUS, $2); }
    | LPAREN expr RPAREN    { $$ = $2; }
    | IDENTIFIER {
        check_variable_usage($1, line_num);
        ASTNode *n = create_node(NODE_VAR);
        n->str_val = $1;
        $$ = n;
    }
    | INT_LITERAL {
        ASTNode *n = create_node(NODE_NUM_INT);
        n->int_val = $1;
        $$ = n;
    }
    | FLOAT_LITERAL {
        ASTNode *n = create_node(NODE_NUM_FLOAT);
        n->float_val = $1;
        $$ = n;
    }
    | TRUE_TOK {
        ASTNode *n = create_node(NODE_BOOL);
        n->bool_val = 1;
        $$ = n;
    }
    | FALSE_TOK {
        ASTNode *n = create_node(NODE_BOOL);
        n->bool_val = 0;
        $$ = n;
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", line_num, s);
}

int main(int argc, char **argv) {
    init_symbol_table();

    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) { perror("File opening failed"); return 1; }
        yyin = f;
    }

    int parse_result = yyparse();

    if (parse_result == 0 && semantic_error_count == 0) {
        printf("\n=== ABSTRACT SYNTAX TREE ===\n");
        print_ast(ast_root, 0);

        printf("\n=== THREE ADDRESS CODE (TAC) ===\n");
        generate_tac(ast_root);
    } else {
        fprintf(stderr, "\nCompilation failed with %d semantic error(s). TAC not generated.\n", semantic_error_count);
    }

    return parse_result == 0 && semantic_error_count == 0 ? 0 : 1;
}