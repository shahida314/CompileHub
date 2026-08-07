%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"
#include "../semantic/semantic.h"
#include "../codegen/codegen.h"

extern int yylex();
extern int yylineno_custom;
void yyerror(const char *s);

ASTNode *root_node = NULL;
%}

%union {
    int intval;
    float floatval;
    char *strval;
    struct ASTNode *node;
}

/* Tokens Definition */
%token INT_TYPE FLOAT_TYPE BOOL_TYPE IF ELSE FOR CIN COUT ENDL
%token <intval> INT_LITERAL BOOL_LITERAL
%token <floatval> FLOAT_LITERAL
%token <strval> IDENTIFIER
%token AND OR EQ NE LSHIFT RSHIFT

/* Operator Precedence and Associativity */
%right '='
%left OR
%left AND
%left EQ NE
%left '<' '>'
%left '+' '-'
%left '*' '/' '%'
%right '!'

/* Non-Terminals Type Binding */
%type <node> program stmt_list stmt decl_stmt assign_stmt io_stmt if_stmt for_stmt block_stmt expr
%type <strval> type_spec

%%

program
    : stmt_list { root_node = $1; }
    ;

stmt_list
    : stmt { $$ = $1; }
    | stmt stmt_list { $1->next = $2; $$ = $1; }
    ;

stmt
    : decl_stmt { $$ = $1; }
    | assign_stmt { $$ = $1; }
    | io_stmt { $$ = $1; }
    | if_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | block_stmt { $$ = $1; }
    ;

type_spec
    : INT_TYPE { $$ = "int"; }
    | FLOAT_TYPE { $$ = "float"; }
    | BOOL_TYPE { $$ = "bool"; }
    ;

decl_stmt
    : type_spec IDENTIFIER ';' {
        $$ = create_node(NODE_DECL);
        $$->op_val = $1;
        $$->left = create_var_node($2, yylineno_custom);
        $$->line = yylineno_custom;
    }
    ;

assign_stmt
    : IDENTIFIER '=' expr ';' {
        $$ = create_node(NODE_ASSIGN);
        $$->left = create_var_node($1, yylineno_custom);
        $$->right = $3;
        $$->line = yylineno_custom;
    }
    ;

io_stmt
    : CIN RSHIFT IDENTIFIER ';' {
        $$ = create_node(NODE_READ);
        $$->left = create_var_node($3, yylineno_custom);
        $$->line = yylineno_custom;
    }
    | COUT LSHIFT expr ';' {
        $$ = create_node(NODE_PRINT);
        $$->left = $3;
        $$->line = yylineno_custom;
    }
    | COUT LSHIFT ENDL ';' {
        $$ = create_node(NODE_PRINT);
        $$->left = create_int_node(0);
        $$->line = yylineno_custom;
    }
    ;

if_stmt
    : IF '(' expr ')' stmt {
        $$ = create_node(NODE_IF);
        $$->left = $3;
        $$->right = $5;
        $$->line = yylineno_custom;
    }
    | IF '(' expr ')' stmt ELSE stmt {
        $$ = create_node(NODE_IF);
        $$->left = $3;
        $$->right = $5;
        $$->third = $7;
        $$->line = yylineno_custom;
    }
    ;

for_stmt
    : FOR '(' decl_stmt expr ';' assign_stmt ')' stmt {
        $$ = create_node(NODE_FOR);
        $$->left = $3;
        $$->right = $4;
        $$->third = $6;
        $$->next = $8;
        $$->line = yylineno_custom;
    }
    | FOR '(' assign_stmt expr ';' assign_stmt ')' stmt {
        $$ = create_node(NODE_FOR);
        $$->left = $3;
        $$->right = $4;
        $$->third = $6;
        $$->next = $8;
        $$->line = yylineno_custom;
    }
    ;

block_stmt
    : '{' stmt_list '}' {
        $$ = create_node(NODE_BLOCK);
        $$->left = $2;
        $$->line = yylineno_custom;
    }
    ;

expr
    : expr '+' expr { $$ = create_op_node("+", $1, $3); }
    | expr '-' expr { $$ = create_op_node("-", $1, $3); }
    | expr '*' expr { $$ = create_op_node("*", $1, $3); }
    | expr '/' expr { $$ = create_op_node("/", $1, $3); }
    | expr '%' expr { $$ = create_op_node("%", $1, $3); }
    | expr '<' expr { $$ = create_op_node("<", $1, $3); }
    | expr '>' expr { $$ = create_op_node(">", $1, $3); }
    | expr EQ expr  { $$ = create_op_node("==", $1, $3); }
    | expr NE expr  { $$ = create_op_node("!=", $1, $3); }
    | expr AND expr { $$ = create_op_node("&&", $1, $3); }
    | expr OR expr  { $$ = create_op_node("||", $1, $3); }
    | '!' expr      { 
        $$ = create_node(NODE_UNARY_OP); 
        $$->op_val = "!"; 
        $$->left = $2; 
    }
    | '(' expr ')'  { $$ = $2; }
    | IDENTIFIER    { $$ = create_var_node($1, yylineno_custom); }
    | INT_LITERAL   { $$ = create_int_node($1); }
    | FLOAT_LITERAL { $$ = create_float_node($1); }
    | BOOL_LITERAL  { $$ = create_bool_node($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error (Line %d): %s\n", yylineno_custom, s);
    exit(1);
}

int main() {
    init_symbol_table();
    
    // Step 1: Lexical and Syntax Analysis (AST Construction)
    if (yyparse() == 0 && root_node) {
        
        // Step 2: Semantic Analysis Phase (Type Checking & Scope Validation)
        analyze_semantics(root_node);
        
        // Step 3: Intermediate Code Generation Phase (TAC Output Generation)
        printf("--- TAC Generated Code ---\n");
        generate_tac(root_node);
        printf("--- Execution Completed Successfully ---\n");
    }
    return 0;
}