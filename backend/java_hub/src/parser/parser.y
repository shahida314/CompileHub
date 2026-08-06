%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum { NODE_DECL, NODE_ASSIGN, NODE_BINOP, NODE_UNOP, NODE_VAR, NODE_NUM,
               NODE_IF, NODE_WHILE, NODE_PRINT, NODE_BLOCK,
               NODE_STR, NODE_CALL, NODE_RETURN, NODE_INC } NodeType;

typedef struct ASTNode {
    NodeType type;
    char value[50];
    char data_type[10];
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *extra;
    struct ASTNode *next;
} ASTNode;

ASTNode* create_node(NodeType type, const char* val, ASTNode* left, ASTNode* right);
/* ------------------------------------------------------------------ */

int yylex();
void yyerror(const char *s);
extern int yylineno;

ASTNode *root = NULL;
%}

%union {
    char *str_val;
    struct ASTNode *node;
}

%token <str_val> INT FLOAT BOOL IDENTIFIER INT_LITERAL FLOAT_LITERAL STRING_LITERAL
%token IF ELSE WHILE FOR PRINT TRUE FALSE
%token CLASS PUBLIC STATIC VOID STRING_TYPE SYSTEM_KW RETURN NEW SCANNER
%token PLUS MINUS MULT DIV MOD ASSIGN
%token EQ NEQ LT GT LE GE AND OR NOT
%token SEMICOLON COMMA DOT LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET
%token INC

%type <node> program class_decl member_list member main_method func_decl
%type <node> param_list param_list_opt
%type <node> stmt_list stmt block decl_stmt assign_stmt if_stmt while_stmt func_stmt for_stmt
%type <node> for_init for_update scanner_decl
%type <node> print_stmt println_stmt return_stmt call_stmt
%type <node> expr expr_list expr_list_opt
%type <str_val> type_spec

%right ASSIGN
%left OR
%left AND
%nonassoc EQ NEQ
%nonassoc LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT UMINUS INC

%%

program:
      class_decl { root = $1; }
    | stmt_list  { root = $1; }
    ;
class_decl:
      CLASS IDENTIFIER LBRACE member_list RBRACE { $$ = $4; }
    | PUBLIC CLASS IDENTIFIER LBRACE member_list RBRACE { $$ = $5; }
    ;

member_list:
      member                 { $$ = $1; }
    | member_list member     {
            ASTNode *n = $1;
            while (n->next) n = n->next;
            n->next = $2;
            $$ = $1;
        }
    ;

member:
      main_method   { $$ = $1; }
    | func_decl     { $$ = $1; }
    ;

main_method:
      PUBLIC STATIC VOID IDENTIFIER LPAREN STRING_TYPE LBRACKET RBRACKET IDENTIFIER RPAREN block
        { $$ = create_node(NODE_BLOCK, "main", $11, NULL); }
    ;

func_decl:
      PUBLIC STATIC type_spec IDENTIFIER LPAREN param_list_opt RPAREN block
        {
            ASTNode *fn = create_node(NODE_CALL, $4, $6, $8);
            strcpy(fn->data_type, $3);
            $$ = fn;
        }
    ;

type_spec:
      INT          { $$ = "int"; }
    | FLOAT        { $$ = "float"; }
    | BOOL         { $$ = "bool"; }
    | VOID         { $$ = "void"; }
    | STRING_TYPE  { $$ = "String"; }
    ;

param_list_opt:
      /* empty */   { $$ = NULL; }
    | param_list    { $$ = $1; }
    ;

param_list:
      type_spec IDENTIFIER {
            ASTNode *n = create_node(NODE_DECL, $2, NULL, NULL);
            strcpy(n->data_type, $1);
            $$ = n;
        }
    | param_list COMMA type_spec IDENTIFIER {
            ASTNode *n = create_node(NODE_DECL, $4, NULL, NULL);
            strcpy(n->data_type, $3);
            ASTNode *p = $1;
            while (p->next) p = p->next;
            p->next = n;
            $$ = $1;
        }
    ;

block:
      LBRACE stmt_list RBRACE { $$ = create_node(NODE_BLOCK, "block", $2, NULL); }
    | LBRACE RBRACE           { $$ = create_node(NODE_BLOCK, "block", NULL, NULL); }
    ;

stmt_list:
      stmt               { $$ = $1; }
    | stmt_list stmt      {
            ASTNode *n = $1;
            while (n->next) n = n->next;
            n->next = $2;
            $$ = $1;
        }
    ;

stmt:
      decl_stmt      { $$ = $1; }
    | assign_stmt    { $$ = $1; }
    | if_stmt        { $$ = $1; }
    | while_stmt     { $$ = $1; }
    | for_stmt       { $$ = $1; }
    | print_stmt     { $$ = $1; }
    | println_stmt   { $$ = $1; }
    | return_stmt    { $$ = $1; }
    | call_stmt      { $$ = $1; }
    | func_stmt      { $$ = $1; }
    | scanner_decl   { $$ = $1; }
    | block          { $$ = $1; }
    | IDENTIFIER INC SEMICOLON {
            ASTNode *var = create_node(NODE_VAR, $1, NULL, NULL);
            $$ = create_node(NODE_INC, $1, var, NULL);
        }
    ;

decl_stmt:
      type_spec IDENTIFIER SEMICOLON {
            ASTNode *n = create_node(NODE_DECL, $2, NULL, NULL);
            strcpy(n->data_type, $1);
            $$ = n;
        }
    | type_spec IDENTIFIER ASSIGN expr SEMICOLON {
            ASTNode *decl = create_node(NODE_DECL, $2, NULL, NULL);
            strcpy(decl->data_type, $1);
            ASTNode *var = create_node(NODE_VAR, $2, NULL, NULL);
            ASTNode *asg = create_node(NODE_ASSIGN, $2, var, $4);
            decl->next = asg;
            $$ = decl;
        }
    ;

assign_stmt:
      IDENTIFIER ASSIGN expr SEMICOLON {
            ASTNode *var = create_node(NODE_VAR, $1, NULL, NULL);
            $$ = create_node(NODE_ASSIGN, $1, var, $3);
        }
    ;

if_stmt:
      IF LPAREN expr RPAREN block
        { $$ = create_node(NODE_IF, "if", $3, $5); }
    | IF LPAREN expr RPAREN block ELSE block
        {
            ASTNode *n = create_node(NODE_IF, "if", $3, $5);
            n->extra = $7;
            $$ = n;
        }
    ;

while_stmt:
      WHILE LPAREN expr RPAREN block
        { $$ = create_node(NODE_WHILE, "while", $3, $5); }
    ;

for_init:
      type_spec IDENTIFIER ASSIGN expr SEMICOLON {
            ASTNode *decl = create_node(NODE_DECL, $2, NULL, NULL);
            strcpy(decl->data_type, $1);
            ASTNode *var = create_node(NODE_VAR, $2, NULL, NULL);
            ASTNode *asg = create_node(NODE_ASSIGN, $2, var, $4);
            decl->next = asg;
            $$ = decl;
        }
    | IDENTIFIER ASSIGN expr SEMICOLON {
            ASTNode *var = create_node(NODE_VAR, $1, NULL, NULL);
            $$ = create_node(NODE_ASSIGN, $1, var, $3);
        }
    ;

for_update:
      IDENTIFIER ASSIGN expr {
            ASTNode *var = create_node(NODE_VAR, $1, NULL, NULL);
            $$ = create_node(NODE_ASSIGN, $1, var, $3);
        }
    | IDENTIFIER INC {
            ASTNode *var = create_node(NODE_VAR, $1, NULL, NULL);
            $$ = create_node(NODE_INC, $1, var, NULL);
        }
    ;

for_stmt:
      FOR LPAREN for_init expr SEMICOLON for_update RPAREN block
        {
            ASTNode *while_node = create_node(NODE_WHILE, "while", $4, $8);

            ASTNode *blk_stmts = $8->left;
            if (blk_stmts) {
                ASTNode *n = blk_stmts;
                while (n->next) n = n->next;
                n->next = $6;
            } else {
                $8->left = $6;
            }

            ASTNode *n2 = $3;
            while (n2->next) n2 = n2->next;
            n2->next = while_node;

            $$ = $3;
        }
    ;

func_stmt:
      type_spec IDENTIFIER LPAREN param_list_opt RPAREN block
        {
            ASTNode *fn = create_node(NODE_CALL, $2, $4, $6);
            strcpy(fn->data_type, $1);
            $$ = fn;
        }
    ;

print_stmt:
      PRINT expr SEMICOLON
        { $$ = create_node(NODE_PRINT, "print", $2, NULL); }
    ;
println_stmt:
      SYSTEM_KW DOT IDENTIFIER DOT IDENTIFIER LPAREN expr RPAREN SEMICOLON
        { $$ = create_node(NODE_PRINT, "println", $7, NULL); }
    ;

return_stmt:
      RETURN expr SEMICOLON
        { $$ = create_node(NODE_RETURN, "return", $2, NULL); }
    | RETURN SEMICOLON
        { $$ = create_node(NODE_RETURN, "return", NULL, NULL); }
    ;
call_stmt:
      IDENTIFIER LPAREN expr_list_opt RPAREN SEMICOLON
        { $$ = create_node(NODE_CALL, $1, $3, NULL); }
    ;

scanner_decl:
      SCANNER IDENTIFIER ASSIGN NEW SCANNER LPAREN SYSTEM_KW DOT IDENTIFIER RPAREN SEMICOLON
        { $$ = create_node(NODE_BLOCK, "noop", NULL, NULL); }
    ;

expr_list_opt:
      /* empty */   { $$ = NULL; }
    | expr_list    { $$ = $1; }
    ;

expr_list:
      expr {
            $$ = create_node(NODE_BLOCK, "arg", $1, NULL);
        }
    | expr_list COMMA expr {
            ASTNode *n = $1;
            while (n->next) n = n->next;
            n->next = create_node(NODE_BLOCK, "arg", $3, NULL);
            $$ = $1;
        }
    ;

expr:
      expr PLUS expr   { $$ = create_node(NODE_BINOP, "+", $1, $3); }
    | expr MINUS expr  { $$ = create_node(NODE_BINOP, "-", $1, $3); }
    | expr MULT expr   { $$ = create_node(NODE_BINOP, "*", $1, $3); }
    | expr DIV expr    { $$ = create_node(NODE_BINOP, "/", $1, $3); }
    | expr MOD expr    { $$ = create_node(NODE_BINOP, "%", $1, $3); }
    | expr EQ expr     { $$ = create_node(NODE_BINOP, "==", $1, $3); }
    | expr NEQ expr    { $$ = create_node(NODE_BINOP, "!=", $1, $3); }
    | expr LT expr     { $$ = create_node(NODE_BINOP, "<", $1, $3); }
    | expr GT expr     { $$ = create_node(NODE_BINOP, ">", $1, $3); }
    | expr LE expr     { $$ = create_node(NODE_BINOP, "<=", $1, $3); }
    | expr GE expr     { $$ = create_node(NODE_BINOP, ">=", $1, $3); }
    | expr AND expr    { $$ = create_node(NODE_BINOP, "&&", $1, $3); }
    | expr OR expr     { $$ = create_node(NODE_BINOP, "||", $1, $3); }
    | MINUS expr %prec UMINUS { $$ = create_node(NODE_UNOP, "-", $2, NULL); }
    | NOT expr          { $$ = create_node(NODE_UNOP, "!", $2, NULL); }
    | LPAREN expr RPAREN { $$ = $2; }
    | IDENTIFIER INC    {
            ASTNode *var = create_node(NODE_VAR, $1, NULL, NULL);
            $$ = create_node(NODE_INC, $1, var, NULL);
        }
    | IDENTIFIER LPAREN expr_list_opt RPAREN
        { $$ = create_node(NODE_CALL, $1, $3, NULL); }
    | IDENTIFIER DOT IDENTIFIER LPAREN RPAREN
        { $$ = create_node(NODE_CALL, $3, NULL, NULL); }
    | IDENTIFIER        { $$ = create_node(NODE_VAR, $1, NULL, NULL); }
    | INT_LITERAL        { $$ = create_node(NODE_NUM, $1, NULL, NULL); strcpy($$->data_type, "int"); }
    | FLOAT_LITERAL       { $$ = create_node(NODE_NUM, $1, NULL, NULL); strcpy($$->data_type, "float"); }
    | STRING_LITERAL      { $$ = create_node(NODE_STR, $1, NULL, NULL); strcpy($$->data_type, "String"); }
    | TRUE               { $$ = create_node(NODE_NUM, "true", NULL, NULL); strcpy($$->data_type, "bool"); }
    | FALSE              { $$ = create_node(NODE_NUM, "false", NULL, NULL); strcpy($$->data_type, "bool"); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parser Error at line %d: %s\n", yylineno, s);
}