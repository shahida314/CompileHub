%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int line_num;
extern FILE *yyin;
void yyerror(const char *s);

extern void init_symbol_table();
extern void check_declaration(const char *name, const char *type, int scope);
extern void check_variable_usage(const char *name);
extern void set_variable_value(const char *name, int val);
extern int get_variable_value(const char *name);

typedef enum {
    NODE_STMT_LIST, 
    NODE_DECL, 
    NODE_ASSIGN, 
    NODE_IF, 
    NODE_WHILE, 
    NODE_PRINTF, 
    NODE_BINOP, 
    NODE_VAR, 
    NODE_NUM,
    NODE_FUNC_CALL,
    NODE_RETURN
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int int_val;
    char *str_val;
    int op;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;
} ASTNode;

extern ASTNode* create_node(NodeType type);
extern ASTNode* create_binop(int op, ASTNode *left, ASTNode *right);

int eval_ast(ASTNode *node);
void execute_ast(ASTNode *node);

typedef struct Function {
    char *name;
    char *param_name; 
    ASTNode *body;
    struct Function *next;
} Function;

Function *function_list = NULL;

void define_function(char *name, char *param_name, ASTNode *body) {
    Function *f = (Function*)malloc(sizeof(Function));
    f->name = strdup(name);
    f->param_name = param_name ? strdup(param_name) : NULL;
    f->body = body;
    f->next = function_list;
    function_list = f;
}

Function* get_function_obj(char *name) {
    Function *f = function_list;
    while (f != NULL) {
        if (strcmp(f->name, name) == 0) {
            return f;
        }
        f = f->next;
    }
    return NULL;
}

%}

%union {
    int int_val;
    float float_val;
    char* str_val;
    struct ASTNode* node;
}

%token INCLUDE_STDIO MAIN RETURN PRINTF
%token INT FLOAT CHAR BOOL IF ELSE WHILE PRINT TRUE FALSE
%token <str_val> IDENTIFIER STRING_LITERAL
%token <int_val> INT_LITERAL
%token <float_val> FLOAT_LITERAL
%token ASSIGN PLUS MINUS MULT DIV MOD
%token EQ NEQ LT GT LE GE AND OR NOT
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN COMMA

%type <node> statement statement_list declaration_stmt assignment_stmt if_stmt while_stmt printf_stmt block_stmt expression expression_list function_def function_call return_stmt function_list_all program includes identifier_list type_spec

%left OR
%left AND
%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT

%%

program:
    includes function_list_all {
        Function *main_func = get_function_obj("main");
        if (main_func != NULL && main_func->body != NULL) {
            execute_ast(main_func->body);
        }
    }
    | function_list_all {
        Function *main_func = get_function_obj("main");
        if (main_func != NULL && main_func->body != NULL) {
            execute_ast(main_func->body);
        }
    }
    ;

includes:
    INCLUDE_STDIO { $$ = NULL; }
    ;

function_list_all:
    function_list_all function_def { $$ = NULL; }
    | function_def { $$ = NULL; }
    ;

function_def:
    INT IDENTIFIER LPAREN INT IDENTIFIER RPAREN LBRACE statement_list return_stmt RBRACE {
        init_symbol_table(); 
        check_declaration($5, "int", 0);
        
        ASTNode *body = $8;
        ASTNode *ret = $9;
        if (ret != NULL) {
            if (body == NULL) body = ret;
            else {
                ASTNode *curr = body;
                while (curr->next != NULL) curr = curr->next;
                curr->next = ret;
            }
        }
        define_function($2, $5, body);
        $$ = NULL;
    }
    | INT IDENTIFIER LPAREN RPAREN LBRACE statement_list return_stmt RBRACE {
        init_symbol_table();
        
        ASTNode *body = $6;
        ASTNode *ret = $7;
        if (ret != NULL) {
            if (body == NULL) body = ret;
            else {
                ASTNode *curr = body;
                while (curr->next != NULL) curr = curr->next;
                curr->next = ret;
            }
        }
        define_function($2, NULL, body);
        $$ = NULL;
    }
    ;

return_stmt:
    RETURN expression SEMICOLON {
        ASTNode *n = create_node(NODE_RETURN);
        n->left = $2;
        $$ = n;
    }
    | /* empty */ { $$ = NULL; }
    ;

statement_list:
    statement_list statement {
        if ($1 == NULL) {
            $$ = $2;
        } else {
            ASTNode *curr = $1;
            while (curr->next != NULL) curr = curr->next;
            curr->next = $2;
            $$ = $1;
        }
    }
    | /* empty */ { $$ = NULL; }
    ;

statement:
    declaration_stmt
    | assignment_stmt
    | if_stmt
    | while_stmt
    | printf_stmt
    | block_stmt
    | function_call SEMICOLON { $$ = $1; }
    ;

type_spec:
    INT | FLOAT | CHAR | BOOL
    ;

declaration_stmt:
    type_spec identifier_list SEMICOLON { $$ = $2; }
    ;

identifier_list:
    IDENTIFIER { 
        check_declaration($1, "int", 0);
        ASTNode *n = create_node(NODE_DECL);
        n->str_val = $1;
        $$ = n;
    }
    | IDENTIFIER ASSIGN expression { 
        check_declaration($1, "int", 0);
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = $1;
        n->left = $3;
        $$ = n;
    }
    | identifier_list COMMA IDENTIFIER { 
        check_declaration($3, "int", 0);
        ASTNode *n = create_node(NODE_DECL);
        n->str_val = $3;
        
        ASTNode *curr = $1;
        while (curr->next != NULL) curr = curr->next;
        curr->next = n;
        $$ = $1;
    }
    | identifier_list COMMA IDENTIFIER ASSIGN expression { 
        check_declaration($3, "int", 0);
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = $3;
        n->left = $5;
        
        ASTNode *curr = $1;
        while (curr->next != NULL) curr = curr->next;
        curr->next = n;
        $$ = $1;
    }
    ;

assignment_stmt:
    IDENTIFIER ASSIGN expression SEMICOLON { 
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = $1;
        n->left = $3;
        $$ = n;
    }
    ;

if_stmt:
    IF LPAREN expression RPAREN statement ELSE statement {
        ASTNode *n = create_node(NODE_IF);
        n->left = $3;
        
        ASTNode *branches = create_node(NODE_IF); 
        branches->left = $5;
        branches->right = $7;
        
        n->right = branches; 
        n->next = NULL; 
        $$ = n;
    }
    | IF LPAREN expression RPAREN statement {
        ASTNode *n = create_node(NODE_IF);
        n->left = $3;
        
        ASTNode *branches = create_node(NODE_IF);
        branches->left = $5;
        branches->right = NULL; 
        
        n->right = branches;
        n->next = NULL;
        $$ = n;
    }
    ;

while_stmt:
    WHILE LPAREN expression RPAREN statement {
        ASTNode *n = create_node(NODE_WHILE);
        n->left = $3;
        n->right = $5;
        $$ = n;
    }
    ;

printf_stmt:
    PRINTF LPAREN STRING_LITERAL RPAREN SEMICOLON {
        ASTNode *n = create_node(NODE_PRINTF);
        n->str_val = $3;
        n->left = NULL;
        $$ = n;
    }
    | PRINTF LPAREN STRING_LITERAL COMMA expression_list RPAREN SEMICOLON {
        ASTNode *n = create_node(NODE_PRINTF);
        n->str_val = $3;
        n->left = $5;
        $$ = n;
    }
    | PRINTF LPAREN expression RPAREN SEMICOLON {
        ASTNode *n = create_node(NODE_PRINTF);
        n->str_val = NULL; 
        n->left = $3;
        $$ = n;
    }
    ;

function_call:
    IDENTIFIER LPAREN expression RPAREN {
        ASTNode *n = create_node(NODE_FUNC_CALL);
        n->str_val = $1;
        n->left = $3;
        $$ = n;
    }
    | IDENTIFIER LPAREN RPAREN {
        ASTNode *n = create_node(NODE_FUNC_CALL);
        n->str_val = $1;
        n->left = NULL;
        $$ = n;
    }
    ;

expression_list:
    expression { $$ = $1; }
    ;

block_stmt:
    LBRACE statement_list RBRACE { $$ = $2; }
    ;

expression:
    expression PLUS expression       { $$ = create_binop(PLUS, $1, $3); }
    | expression MINUS expression    { $$ = create_binop(MINUS, $1, $3); }
    | expression MULT expression     { $$ = create_binop(MULT, $1, $3); }
    | expression DIV expression      { $$ = create_binop(DIV, $1, $3); }
    | expression MOD expression      { $$ = create_binop(MOD, $1, $3); }
    | expression EQ expression       { $$ = create_binop(EQ, $1, $3); }
    | expression NEQ expression      { $$ = create_binop(NEQ, $1, $3); }
    | expression GT expression       { $$ = create_binop(GT, $1, $3); }
    | expression LT expression       { $$ = create_binop(LT, $1, $3); }
    | expression GE expression       { $$ = create_binop(GE, $1, $3); }
    | expression LE expression       { $$ = create_binop(LE, $1, $3); }
    | function_call                  { $$ = $1; }
    | IDENTIFIER                     { ASTNode *n = create_node(NODE_VAR); n->str_val = $1; $$ = n; }
    | INT_LITERAL                    { ASTNode *n = create_node(NODE_NUM); n->int_val = $1; $$ = n; }
    | FLOAT_LITERAL                  { ASTNode *n = create_node(NODE_NUM); n->int_val = (int)$1; $$ = n; }
    ;

%%

int eval_ast(ASTNode *node) {
    if (!node) return 0;
    if (node->type == NODE_NUM) return node->int_val;
    if (node->type == NODE_VAR) return get_variable_value(node->str_val);
    if (node->type == NODE_FUNC_CALL) {
        Function *f = get_function_obj(node->str_val);
        if (f) {
            if (f->param_name && node->left) {
                int arg_val = eval_ast(node->left);
                set_variable_value(f->param_name, arg_val);
            }
            if (f->body) {
                execute_ast(f->body);
            }
        }
        return 0;
    }
    if (node->type == NODE_BINOP) {
        int l = eval_ast(node->left);
        int r = eval_ast(node->right);
        switch(node->op) {
            case PLUS: return l + r;
            case MINUS: return l - r;
            case MULT: return l * r;
            case DIV: return r != 0 ? l / r : 0;
            case MOD: return r != 0 ? l % r : 0;
            case EQ: return l == r;
            case NEQ: return l != r;
            case GT: return l > r;
            case LT: return l < r;
            case GE: return l >= r;
            case LE: return l <= r;
        }
    }
    return 0;
}

void execute_ast(ASTNode *node) {
    ASTNode *curr = node;
    while (curr != NULL) {
        switch (curr->type) {
            case NODE_DECL:
                set_variable_value(curr->str_val, 0);
                break;
            case NODE_ASSIGN:
                set_variable_value(curr->str_val, eval_ast(curr->left));
                break;
            case NODE_PRINTF: {
                if (curr->str_val == NULL && curr->left != NULL) {
                    printf("%d\n", eval_ast(curr->left));
                } else if (curr->str_val != NULL) {
                    char *fmt = curr->str_val;
                    int len = strlen(fmt);
                    char temp[256];
                    int j = 0;
                    for (int i = 0; i < len; i++) {
                        if (fmt[i] != '"') {
                            temp[j++] = fmt[i];
                        }
                    }
                    temp[j] = '\0';
                    
                    if (curr->left) {
                        int val = eval_ast(curr->left);
                        char buffer[256];
                        snprintf(buffer, sizeof(buffer), temp, val);
                        printf("%s\n", buffer);
                    } else {
                        printf("%s\n", temp);
                    }
                }
                break;
            }
            case NODE_IF: {
                int cond = eval_ast(curr->left);
                ASTNode *branches = curr->right;
                
                if (cond) {
                    if (branches->left != NULL) {
                        execute_ast(branches->left);
                    }
                } else {
                    if (branches->right != NULL) {
                        execute_ast(branches->right);
                    }
                }
                break;
            }
            case NODE_WHILE: {
                while (eval_ast(curr->left)) {
                    execute_ast(curr->right);
                }
                break;
            }
            case NODE_FUNC_CALL: {
                Function *f = get_function_obj(curr->str_val);
                if (f) {
                    if (f->param_name && curr->left) {
                        int arg_val = eval_ast(curr->left);
                        set_variable_value(f->param_name, arg_val);
                    }
                    if (f->body) {
                        execute_ast(f->body);
                    }
                }
                break;
            }
            case NODE_RETURN:
                return;
            default:
                break;
        }
        curr = curr->next;
    }
}

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