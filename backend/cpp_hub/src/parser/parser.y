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
extern void check_assignment_type(const char *name, const char *expr_type);
extern void check_operator_types(const char *op_category, const char *left_type, const char *right_type);
extern int semantic_error_count;

extern void set_variable_value(const char *name, int val);
extern int get_variable_value(const char *name);
extern void set_variable_value_float(const char *name, float val);
extern float get_variable_value_float(const char *name);
extern void set_variable_value_bool(const char *name, int val);
extern int get_variable_value_bool(const char *name);

typedef enum {
    NODE_STMT_LIST, NODE_DECL, NODE_ASSIGN, NODE_IF, NODE_WHILE, NODE_FOR,
    NODE_PRINTF, NODE_BINOP, NODE_UNOP, NODE_VAR, NODE_NUM_INT, NODE_NUM_FLOAT,
    NODE_BOOL, NODE_FUNC_CALL, NODE_RETURN
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int int_val;
    float float_val;
    int bool_val;
    char *str_val;
    int op;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *cond;
    struct ASTNode *then_branch;
    struct ASTNode *else_branch;
    struct ASTNode *body;
    struct ASTNode *init_stmt;
    struct ASTNode *post_stmt;
    struct ASTNode *next;
} ASTNode;

extern ASTNode *create_node(NodeType type);
extern ASTNode *create_binop(int op, ASTNode *left, ASTNode *right);
extern ASTNode *create_unop(int op, ASTNode *operand);
extern void print_ast(ASTNode *node, int indent);

extern void generate_tac(ASTNode *node);

int eval_ast(ASTNode *node);
const char *eval_type(ASTNode *node);
void execute_ast(ASTNode *node);

typedef struct Function {
    char *name;
    char *param_name;
    char *param_type;
    ASTNode *body;
    struct Function *next;
} Function;

Function *function_list = NULL;

void define_function(char *name, char *param_name, char *param_type, ASTNode *body) {
    Function *f = (Function *)malloc(sizeof(Function));
    f->name = strdup(name);
    f->param_name = param_name ? strdup(param_name) : NULL;
    f->param_type = param_type ? strdup(param_type) : NULL;
    f->body = body;
    f->next = function_list;
    function_list = f;
}

Function *get_function_obj(char *name) {
    Function *f = function_list;
    while (f != NULL) {
        if (strcmp(f->name, name) == 0) return f;
        f = f->next;
    }
    return NULL;
}

ASTNode *main_body = NULL;
char *current_decl_type = "int";
%}

%union {
    int int_val;
    float float_val;
    char *str_val;
    struct ASTNode *node;
}

%token INCLUDE_STDIO RETURN PRINTF
%token INT FLOAT BOOL IF ELSE WHILE FOR TRUE FALSE
%token <str_val> IDENTIFIER STRING_LITERAL
%token <int_val> INT_LITERAL
%token <float_val> FLOAT_LITERAL
%token ASSIGN PLUS MINUS MULT DIV MOD
%token EQ NEQ LT GT LE GE AND OR NOT
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN COMMA

%type <node> statement statement_list declaration_stmt assignment_stmt if_stmt while_stmt for_stmt
%type <node> printf_stmt block_stmt expression expression_list function_def function_call
%type <node> return_stmt function_list_all program includes identifier_list for_init for_update
%type <str_val> type_spec

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left OR
%left AND
%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT UMINUS

%%

program:
    includes function_list_all {
        Function *main_func = get_function_obj("main");
        if (main_func != NULL && main_func->body != NULL) {
            main_body = main_func->body;
        }
    }
    | function_list_all {
        Function *main_func = get_function_obj("main");
        if (main_func != NULL && main_func->body != NULL) {
            main_body = main_func->body;
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
    type_spec IDENTIFIER LPAREN type_spec IDENTIFIER RPAREN LBRACE statement_list return_stmt RBRACE {
        check_declaration($5, $4, 1);
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
        define_function($2, $5, $4, body);
        $$ = NULL;
    }
    | type_spec IDENTIFIER LPAREN RPAREN LBRACE statement_list return_stmt RBRACE {
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
        define_function($2, NULL, NULL, body);
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
        if ($1 == NULL) { $$ = $2; }
        else {
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
    | for_stmt
    | printf_stmt
    | block_stmt
    | function_call SEMICOLON { $$ = $1; }
    ;

type_spec:
    INT    { $$ = "int"; current_decl_type = "int"; }
    | FLOAT  { $$ = "float"; current_decl_type = "float"; }
    | BOOL   { $$ = "bool"; current_decl_type = "bool"; }
    ;

declaration_stmt:
    type_spec identifier_list SEMICOLON { $$ = $2; }
    ;

identifier_list:
    IDENTIFIER {
        check_declaration($1, current_decl_type, 0);
        ASTNode *n = create_node(NODE_DECL);
        n->str_val = $1;
        $$ = n;
    }
    | IDENTIFIER ASSIGN expression {
        check_declaration($1, current_decl_type, 0);
        check_assignment_type($1, eval_type($3));
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = $1;
        n->left = $3;
        $$ = n;
    }
    | identifier_list COMMA IDENTIFIER {
        check_declaration($3, current_decl_type, 0);
        ASTNode *n = create_node(NODE_DECL);
        n->str_val = $3;
        ASTNode *curr = $1;
        while (curr->next != NULL) curr = curr->next;
        curr->next = n;
        $$ = $1;
    }
    | identifier_list COMMA IDENTIFIER ASSIGN expression {
        check_declaration($3, current_decl_type, 0);
        check_assignment_type($3, eval_type($5));
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
        check_variable_usage($1);
        check_assignment_type($1, eval_type($3));
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = $1;
        n->left = $3;
        $$ = n;
    }
    ;

if_stmt:
    IF LPAREN expression RPAREN statement ELSE statement {
        ASTNode *n = create_node(NODE_IF);
        n->cond = $3;
        n->then_branch = $5;
        n->else_branch = $7;
        $$ = n;
    }
    | IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE {
        ASTNode *n = create_node(NODE_IF);
        n->cond = $3;
        n->then_branch = $5;
        n->else_branch = NULL;
        $$ = n;
    }
    ;

while_stmt:
    WHILE LPAREN expression RPAREN statement {
        ASTNode *n = create_node(NODE_WHILE);
        n->cond = $3;
        n->body = $5;
        $$ = n;
    }
    ;

for_init:
    type_spec IDENTIFIER ASSIGN expression {
        check_declaration($2, $1, 0);
        check_assignment_type($2, eval_type($4));
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = $2;
        n->left = $4;
        $$ = n;
    }
    | IDENTIFIER ASSIGN expression {
        check_variable_usage($1);
        check_assignment_type($1, eval_type($3));
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = $1;
        n->left = $3;
        $$ = n;
    }
    ;

for_update:
    IDENTIFIER ASSIGN expression {
        check_variable_usage($1);
        check_assignment_type($1, eval_type($3));
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = $1;
        n->left = $3;
        $$ = n;
    }
    ;

for_stmt:
    FOR LPAREN for_init SEMICOLON expression SEMICOLON for_update RPAREN statement {
        ASTNode *n = create_node(NODE_FOR);
        n->init_stmt = $3;
        n->cond = $5;
        n->post_stmt = $7;
        n->body = $9;
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
    expression PLUS expression        { $$ = create_binop(PLUS, $1, $3); check_operator_types("arithmetic", eval_type($1), eval_type($3)); }
    | expression MINUS expression       { $$ = create_binop(MINUS, $1, $3); check_operator_types("arithmetic", eval_type($1), eval_type($3)); }
    | expression MULT expression        { $$ = create_binop(MULT, $1, $3); check_operator_types("arithmetic", eval_type($1), eval_type($3)); }
    | expression DIV expression         { $$ = create_binop(DIV, $1, $3); check_operator_types("arithmetic", eval_type($1), eval_type($3)); }
    | expression MOD expression         { $$ = create_binop(MOD, $1, $3); check_operator_types("arithmetic", eval_type($1), eval_type($3)); }
    | expression EQ expression          { $$ = create_binop(EQ, $1, $3); }
    | expression NEQ expression         { $$ = create_binop(NEQ, $1, $3); }
    | expression GT expression          { $$ = create_binop(GT, $1, $3); check_operator_types("relational", eval_type($1), eval_type($3)); }
    | expression LT expression          { $$ = create_binop(LT, $1, $3); check_operator_types("relational", eval_type($1), eval_type($3)); }
    | expression GE expression          { $$ = create_binop(GE, $1, $3); check_operator_types("relational", eval_type($1), eval_type($3)); }
    | expression LE expression          { $$ = create_binop(LE, $1, $3); check_operator_types("relational", eval_type($1), eval_type($3)); }
    | expression AND expression         { $$ = create_binop(AND, $1, $3); check_operator_types("logical", eval_type($1), eval_type($3)); }
    | expression OR expression          { $$ = create_binop(OR, $1, $3); check_operator_types("logical", eval_type($1), eval_type($3)); }
    | NOT expression                    { $$ = create_unop(NOT, $2); }
    | MINUS expression %prec UMINUS     { $$ = create_unop(MINUS, $2); }
    | LPAREN expression RPAREN          { $$ = $2; }
    | function_call                     { $$ = $1; }
    | IDENTIFIER {
        check_variable_usage($1);
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
    | TRUE {
        ASTNode *n = create_node(NODE_BOOL);
        n->bool_val = 1;
        $$ = n;
    }
    | FALSE {
        ASTNode *n = create_node(NODE_BOOL);
        n->bool_val = 0;
        $$ = n;
    }
    ;

%%

// Infers the semantic "type name" of an expression, for type-checking
const char *eval_type(ASTNode *node) {
    if (!node) return "unknown";
    switch (node->type) {
        case NODE_NUM_INT: return "int";
        case NODE_NUM_FLOAT: return "float";
        case NODE_BOOL: return "bool";
        case NODE_VAR: {
            extern void *lookup_symbol(const char *name);
            return "int";
        }
        case NODE_UNOP:
            return node->op == NOT ? "bool" : eval_type(node->left);
        case NODE_BINOP: {
            if (node->op == AND || node->op == OR) return "bool";
            if (node->op == LT || node->op == GT || node->op == LE ||
                node->op == GE || node->op == EQ || node->op == NEQ) return "bool";
            const char *lt = eval_type(node->left);
            const char *rt = eval_type(node->right);
            if (strcmp(lt, "float") == 0 || strcmp(rt, "float") == 0) return "float";
            return "int";
        }
        default: return "int";
    }
}

int eval_ast(ASTNode *node) {
    if (!node) return 0;
    switch (node->type) {
        case NODE_NUM_INT: return node->int_val;
        case NODE_NUM_FLOAT: return (int)node->float_val;
        case NODE_BOOL: return node->bool_val;
        case NODE_VAR: return get_variable_value(node->str_val);
        case NODE_FUNC_CALL: {
            Function *f = get_function_obj(node->str_val);
            if (f) {
                if (f->param_name && node->left) {
                    int arg_val = eval_ast(node->left);
                    set_variable_value(f->param_name, arg_val);
                }
                if (f->body) execute_ast(f->body);
            }
            return 0;
        }
        case NODE_UNOP: {
            int v = eval_ast(node->left);
            if (node->op == NOT) return !v;
            if (node->op == MINUS) return -v;
            return v;
        }
        case NODE_BINOP: {
            int l = eval_ast(node->left);
            int r = eval_ast(node->right);
            switch (node->op) {
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
                case AND: return l && r;
                case OR: return l || r;
            }
        }
        default: return 0;
    }
    return 0;
}

void execute_ast(ASTNode *node) {
    ASTNode *curr = node;
    while (curr != NULL) {
        switch (curr->type) {
            case NODE_DECL:
                set_variable_value(curr->str_val, curr->left ? eval_ast(curr->left) : 0);
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
                        if (fmt[i] != '"') temp[j++] = fmt[i];
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
            case NODE_IF:
                if (eval_ast(curr->cond)) {
                    if (curr->then_branch) execute_ast(curr->then_branch);
                } else {
                    if (curr->else_branch) execute_ast(curr->else_branch);
                }
                break;
            case NODE_WHILE:
                while (eval_ast(curr->cond)) execute_ast(curr->body);
                break;
            case NODE_FOR:
                execute_ast(curr->init_stmt);
                while (eval_ast(curr->cond)) {
                    execute_ast(curr->body);
                    execute_ast(curr->post_stmt);
                }
                break;
            case NODE_FUNC_CALL: {
                Function *f = get_function_obj(curr->str_val);
                if (f) {
                    if (f->param_name && curr->left) {
                        int arg_val = eval_ast(curr->left);
                        set_variable_value(f->param_name, arg_val);
                    }
                    if (f->body) execute_ast(f->body);
                }
                break;
            }
            case NODE_RETURN:
                if (curr->left) eval_ast(curr->left);
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
    init_symbol_table();

    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) { perror("File opening failed"); return 1; }
        yyin = f;
    }

    int parse_result = yyparse();

    if (parse_result == 0 && semantic_error_count == 0 && main_body != NULL) {
        printf("\n=== ABSTRACT SYNTAX TREE ===\n");
        print_ast(main_body, 0);

        printf("\n=== THREE ADDRESS CODE (TAC) ===\n");
        generate_tac(main_body);

        printf("\n=== PROGRAM OUTPUT ===\n");
        execute_ast(main_body);
    } else if (semantic_error_count > 0) {
        fprintf(stderr, "\nCompilation failed with %d semantic error(s). TAC not generated.\n", semantic_error_count);
        return 1;
    }

    return parse_result;
}