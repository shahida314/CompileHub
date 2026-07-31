#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 200

typedef struct {
    char name[50];
    double value;
} RuntimeVar;

static RuntimeVar runtime_vars[MAX_VARS];
static int runtime_var_count = 0;

void set_runtime_var(const char *name, double value) {
    for (int i = 0; i < runtime_var_count; i++) {
        if (strcmp(runtime_vars[i].name, name) == 0) {
            runtime_vars[i].value = value;
            return;
        }
    }
    strcpy(runtime_vars[runtime_var_count].name, name);
    runtime_vars[runtime_var_count].value = value;
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
    if (strcmp(type, "bool") == 0) {
        printf("%s\n", val != 0 ? "true" : "false");
    } else if (strcmp(type, "float") == 0) {
        printf("%g\n", val);
    } else {
        printf("%ld\n", (long)val);
    }
}

void exec_stmt_list_rt(ASTNode *node);

void exec_stmt_rt(ASTNode *node) {
    double val;
    switch (node->type) {
        case NODE_DECL:
            set_runtime_var(node->value, 0);
            break;

        case NODE_ASSIGN:
            val = eval_expr_rt(node->right);
            set_runtime_var(node->left->value, val);
            break;

        case NODE_PRINT:
            val = eval_expr_rt(node->left);
            print_runtime_value(node->left, val);
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
            while (eval_expr_rt(node->left) != 0) {
                exec_stmt_list_rt(node->right);
            }
            break;

        default:
            break;
    }
}

void exec_stmt_list_rt(ASTNode *node) {
    while (node) {
        exec_stmt_rt(node);
        node = node->next;
    }
}

void run_program(ASTNode *root) {
    exec_stmt_list_rt(root);
}