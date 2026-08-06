#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_processed_string(const char *str);

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

void print_ast(ASTNode *node, int level);

extern void register_function(const char *name, const char *return_type, struct ASTNode *params, struct ASTNode *body);
extern struct ASTNode* get_function_params(const char *name);
extern struct ASTNode* get_function_body(const char *name);

#define MAX_VARS 200
#define MAX_ARGS 20

typedef struct {
    char name[50];
    double value;
    char type[10];
    char str_value[200];
} RuntimeVar;

static RuntimeVar runtime_vars[MAX_VARS];
static int runtime_var_count = 0;

static int returning = 0;
static double return_value = 0;

void set_runtime_var(const char *name, double value) {
    for (int i = 0; i < runtime_var_count; i++) {
        if (strcmp(runtime_vars[i].name, name) == 0) {
            runtime_vars[i].value = value;
            return;
        }
    }
    strcpy(runtime_vars[runtime_var_count].name, name);
    runtime_vars[runtime_var_count].value = value;
    strcpy(runtime_vars[runtime_var_count].type, "int");
    runtime_vars[runtime_var_count].str_value[0] = '\0';
    runtime_var_count++;
}

double get_runtime_var(const char *name) {
    for (int i = 0; i < runtime_var_count; i++) {
        if (strcmp(runtime_vars[i].name, name) == 0) {
            return runtime_vars[i].value;
        }
    }
    return 0;
}

void declare_runtime_var(const char *name, const char *type) {
    for (int i = 0; i < runtime_var_count; i++) {
        if (strcmp(runtime_vars[i].name, name) == 0) {
            strcpy(runtime_vars[i].type, type);
            return;
        }
    }
    strcpy(runtime_vars[runtime_var_count].name, name);
    runtime_vars[runtime_var_count].value = 0;
    runtime_vars[runtime_var_count].str_value[0] = '\0';
    strcpy(runtime_vars[runtime_var_count].type, type);
    runtime_var_count++;
}

const char* get_runtime_var_type(const char *name) {
    for (int i = 0; i < runtime_var_count; i++) {
        if (strcmp(runtime_vars[i].name, name) == 0) {
            return runtime_vars[i].type;
        }
    }
    return "int";
}

void set_runtime_var_str(const char *name, const char *value) {
    for (int i = 0; i < runtime_var_count; i++) {
        if (strcmp(runtime_vars[i].name, name) == 0) {
            strcpy(runtime_vars[i].str_value, value);
            strcpy(runtime_vars[i].type, "String");
            return;
        }
    }
    strcpy(runtime_vars[runtime_var_count].name, name);
    strcpy(runtime_vars[runtime_var_count].str_value, value);
    strcpy(runtime_vars[runtime_var_count].type, "String");
    runtime_vars[runtime_var_count].value = 0;
    runtime_var_count++;
}

const char* get_runtime_var_str(const char *name) {
    for (int i = 0; i < runtime_var_count; i++) {
        if (strcmp(runtime_vars[i].name, name) == 0) {
            return runtime_vars[i].str_value;
        }
    }
    return "";
}

double scanner_read_number(void) {
    double v = 0;
    if (scanf(" %lf", &v) != 1) v = 0;
    return v;
}

int scanner_read_bool(void) {
    char buf[16] = {0};
    if (scanf(" %15s", buf) != 1) return 0;
    return strcmp(buf, "true") == 0;
}

const char* scanner_read_line(void) {
    static char buf[256];
    buf[0] = '\0';
    int c = getchar();
    while (c == '\n' || c == '\r') c = getchar();
    int i = 0;
    while (c != EOF && c != '\n' && i < 255) {
        buf[i++] = (char)c;
        c = getchar();
    }
    buf[i] = '\0';
    return buf;
}

const char* scanner_read_token(void) {
    static char buf[256];
    if (scanf(" %255s", buf) != 1) buf[0] = '\0';
    return buf;
}

int is_string_node(ASTNode *node) {
    if (!node) return 0;
    if (node->type == NODE_STR) return 1;
    if (node->type == NODE_VAR) {
        return strcmp(get_runtime_var_type(node->value), "String") == 0;
    }
    if (node->type == NODE_CALL &&
        (strcmp(node->value, "nextLine") == 0 || strcmp(node->value, "next") == 0)) {
        return 1;
    }
    return 0;
}

const char* get_expr_string(ASTNode *node) {
    if (!node) return "";
    if (node->type == NODE_STR) return node->value;
    if (node->type == NODE_VAR) return get_runtime_var_str(node->value);
    if (node->type == NODE_CALL) {
        if (strcmp(node->value, "nextLine") == 0) return scanner_read_line();
        if (strcmp(node->value, "next") == 0) return scanner_read_token();
    }
    return "";
}

double call_function(const char *name, ASTNode *args);

double eval_expr_rt(ASTNode *node) {
    if (!node) return 0;
    double l, r;

    switch (node->type) {
        case NODE_NUM:
            if (strcmp(node->data_type, "bool") == 0)
                return strcmp(node->value, "true") == 0 ? 1 : 0;
            return atof(node->value);

        case NODE_VAR:
            return get_runtime_var(node->value);

        case NODE_CALL:
            if (strcmp(node->value, "nextInt") == 0 ||
                strcmp(node->value, "nextDouble") == 0 ||
                strcmp(node->value, "nextFloat") == 0) {
                return scanner_read_number();
            }
            if (strcmp(node->value, "nextBoolean") == 0) {
                return scanner_read_bool();
            }
            return call_function(node->value, node->left);

        case NODE_INC: {
            double val = get_runtime_var(node->value);
            set_runtime_var(node->value, val + 1);
            return val + 1;
        }

        case NODE_UNOP:
            l = eval_expr_rt(node->left);
            if (strcmp(node->value, "-") == 0) return -l;
            if (strcmp(node->value, "!") == 0) return (l == 0) ? 1 : 0;
            return 0;

        case NODE_BINOP:
            l = eval_expr_rt(node->left);
            r = eval_expr_rt(node->right);
            if (strcmp(node->value, "+") == 0) return l + r;
            if (strcmp(node->value, "-") == 0) return l - r;
            if (strcmp(node->value, "*") == 0) return l * r;
            if (strcmp(node->value, "/") == 0) return r != 0 ? l / r : 0;
            if (strcmp(node->value, "%") == 0) return r != 0 ? (double)((long)l % (long)r) : 0;
            if (strcmp(node->value, "==") == 0) return l == r;
            if (strcmp(node->value, "!=") == 0) return l != r;
            if (strcmp(node->value, "<") == 0) return l < r;
            if (strcmp(node->value, ">") == 0) return l > r;
            if (strcmp(node->value, "<=") == 0) return l <= r;
            if (strcmp(node->value, ">=") == 0) return l >= r;
            if (strcmp(node->value, "&&") == 0) return (l != 0) && (r != 0);
            if (strcmp(node->value, "||") == 0) return (l != 0) || (r != 0);
            return 0;

        default:
            return 0;
    }
}

void print_runtime_value(ASTNode *expr_node, double val) {
    const char *type = expr_node->data_type;
    if (expr_node->type == NODE_VAR) {
        type = get_runtime_var_type(expr_node->value);
    }
    if (strcmp(type, "bool") == 0) {
        printf("%s\n", val != 0 ? "true" : "false");
    } else if (strcmp(type, "float") == 0) {
        printf("%g\n", val);
    } else {
        printf("%ld\n", (long)val);
    }
    fflush(stdout); // বাফার ক্লিয়ার করে দ্রুত আউটপুট প্রেরণের জন্য
}

void exec_stmt_list_rt(ASTNode *node);
void exec_stmt_rt(ASTNode *node);

double call_function(const char *name, ASTNode *args) {
    ASTNode *params = get_function_params(name);
    ASTNode *body = get_function_body(name);

    if (!body) {
        fprintf(stderr, "Runtime Error: function '%s' not found\n", name);
        fflush(stderr);
        return 0;
    }

    double arg_vals[MAX_ARGS];
    char arg_strs[MAX_ARGS][200];
    int arg_is_str[MAX_ARGS];
    int argc = 0;

    ASTNode *a = args;
    while (a && argc < MAX_ARGS) {
        ASTNode *ex = a->left;
        if (is_string_node(ex)) {
            arg_is_str[argc] = 1;
            strcpy(arg_strs[argc], get_expr_string(ex));
        } else {
            arg_is_str[argc] = 0;
            arg_vals[argc] = eval_expr_rt(ex);
        }
        argc++;
        a = a->next;
    }

    int saved_count = runtime_var_count;
    RuntimeVar saved_vars[MAX_VARS];
    memcpy(saved_vars, runtime_vars, sizeof(RuntimeVar) * saved_count);
    runtime_var_count = 0;

    ASTNode *p = params;
    int i = 0;
    while (p && i < argc) {
        declare_runtime_var(p->value, p->data_type);
        if (arg_is_str[i]) set_runtime_var_str(p->value, arg_strs[i]);
        else set_runtime_var(p->value, arg_vals[i]);
        p = p->next;
        i++;
    }

    int prev_returning = returning;
    double prev_return_value = return_value;
    returning = 0;
    return_value = 0;

    exec_stmt_list_rt(body->left);

    double result = return_value;

    runtime_var_count = saved_count;
    memcpy(runtime_vars, saved_vars, sizeof(RuntimeVar) * saved_count);

    returning = prev_returning;
    return_value = prev_return_value;

    return result;
}

void exec_stmt_rt(ASTNode *node) {
    double val;
    switch (node->type) {
        case NODE_DECL:
            declare_runtime_var(node->value, node->data_type);
            break;

        case NODE_ASSIGN:
            if (is_string_node(node->right)) {
                set_runtime_var_str(node->left->value, get_expr_string(node->right));
            } else {
                val = eval_expr_rt(node->right);
                set_runtime_var(node->left->value, val);
            }
            break;

        case NODE_INC:
            eval_expr_rt(node);
            break;

   case NODE_PRINT:
    if (is_string_node(node->left)) {
        print_processed_string(get_expr_string(node->left));
    } else {
        val = eval_expr_rt(node->left);
        print_runtime_value(node->left, val);
        printf("\n"); 
    }
    fflush(stdout);
    break;

        case NODE_BLOCK:
            exec_stmt_list_rt(node->left);
            break;

        case NODE_IF:
            val = eval_expr_rt(node->left);
            if (val != 0) {
                exec_stmt_list_rt(node->right);
            } else if (node->extra) {
                exec_stmt_list_rt(node->extra);
            }
            break;

        case NODE_WHILE:
            while (!returning && eval_expr_rt(node->left) != 0) {
                exec_stmt_list_rt(node->right);
            }
            break;

        case NODE_RETURN:
            return_value = node->left ? eval_expr_rt(node->left) : 0;
            returning = 1;
            break;

        case NODE_CALL:
            call_function(node->value, node->left);
            break;

        default:
            break;
    }
}
void print_processed_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\\' && str[i+1] == 'n') {
            printf("\n");
            i++; // skip 'n'
        } else if (str[i] == '\\' && str[i+1] == 't') {
            printf("\t");
            i++; // skip 't'
        } else {
            putchar(str[i]);
        }
    }
    fflush(stdout);
}
void exec_stmt_list_rt(ASTNode *node) {
    while (node && !returning) {
        exec_stmt_rt(node);
        node = node->next;
    }
}

void run_program(ASTNode *root) {
    ASTNode *n = root;
    ASTNode *main_body = NULL;

    while (n) {
        if (n->type == NODE_BLOCK && strcmp(n->value, "main") == 0) {
            main_body = n->left;
        } else if (n->type == NODE_CALL && n->right != NULL) {
            register_function(n->value, n->data_type, n->left, n->right);
        }
        n = n->next;
    }

    if (main_body) {
        exec_stmt_list_rt(main_body->left);
    } else {
        exec_stmt_list_rt(root);
    }
}

extern int yyparse(void);
extern FILE *yyin;
extern ASTNode *root;

int main(int argc, char *argv[]) {
    // Standard Output এবং Error Buffering নিষ্ক্রিয় করা হলো
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]);
        return 1;
    }

    int parse_result = yyparse();
    fclose(yyin);

    if (parse_result != 0 || root == NULL) {
        fprintf(stderr, "Parsing failed.\n");
        return 1;
    }

    run_program(root);

    return 0;
}