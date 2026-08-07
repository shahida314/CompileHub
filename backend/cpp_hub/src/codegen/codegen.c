#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variable Storage for Runtime Execution
typedef struct {
    char name[32];
    double value;
} RuntimeVar;

static RuntimeVar var_table[100];
static int var_count = 0;

void set_var_value(const char* name, double val) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var_table[i].name, name) == 0) {
            var_table[i].value = val;
            return;
        }
    }
    strcpy(var_table[var_count].name, name);
    var_table[var_count].value = val;
    var_count++;
}

double get_var_value(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var_table[i].name, name) == 0) {
            return var_table[i].value;
        }
    }
    return 0.0;
}

double eval_ast_expr(ASTNode *node) {
    if (!node) return 0.0;

    if (node->node_type == NODE_INT) return (double)node->int_val;
    if (node->node_type == NODE_FLOAT) return (double)node->float_val;
    if (node->node_type == NODE_BOOL) return (double)node->bool_val;
    if (node->node_type == NODE_VAR) return get_var_value(node->op_val);

    if (node->node_type == NODE_BIN_OP) {
        double left_val = eval_ast_expr(node->left);
        double right_val = eval_ast_expr(node->right);

        if (strcmp(node->op_val, "+") == 0) return left_val + right_val;
        if (strcmp(node->op_val, "-") == 0) return left_val - right_val;
        if (strcmp(node->op_val, "*") == 0) return left_val * right_val;
        if (strcmp(node->op_val, "/") == 0) return right_val != 0 ? left_val / right_val : 0;
        if (strcmp(node->op_val, "%") == 0) return (int)left_val % (int)right_val;
        if (strcmp(node->op_val, "<") == 0) return left_val < right_val;
        if (strcmp(node->op_val, ">") == 0) return left_val > right_val;
        if (strcmp(node->op_val, "<=") == 0) return left_val <= right_val;
        if (strcmp(node->op_val, ">=") == 0) return left_val >= right_val;
        if (strcmp(node->op_val, "==") == 0) return left_val == right_val;
        if (strcmp(node->op_val, "!=") == 0) return left_val != right_val;
        if (strcmp(node->op_val, "&&") == 0) return left_val && right_val;
        if (strcmp(node->op_val, "||") == 0) return left_val || right_val;
    }
    return 0.0;
}

void execute_ast(ASTNode *node) {
    while (node) {
        switch(node->node_type) {
            case NODE_DECL: {
                set_var_value(node->left->op_val, 0.0);
                break;
            }
            case NODE_ASSIGN: {
                double val = eval_ast_expr(node->right);
                set_var_value(node->left->op_val, val);
                break;
            }
            case NODE_READ: { // cin >> x
                double in_val = 0.0;
                if (scanf("%lf", &in_val) == 1) {
                    set_var_value(node->left->op_val, in_val);
                }
                break;
            }
            case NODE_PRINT: { // cout << expr
                if (node->left && node->left->node_type == NODE_INT && node->left->int_val == 0) {
                    printf("\n");
                } else {
                    double val = eval_ast_expr(node->left);
                    if (val == (int)val) {
                        printf("%d", (int)val);
                    } else {
                        printf("%.2f", val);
                    }
                }
                fflush(stdout);
                break;
            }
            case NODE_IF: {
                double cond = eval_ast_expr(node->left);
                if (cond != 0.0) {
                    execute_ast(node->right);
                } else if (node->third) {
                    execute_ast(node->third);
                }
                break;
            }
            case NODE_FOR: {
                if (node->left) execute_ast(node->left); // Initialization

                while (eval_ast_expr(node->right)) { // Condition check
                    execute_ast(node->next); // Loop body execution
                    if (node->third) execute_ast(node->third); // Step/Increment
                }
                break;
            }
            case NODE_BLOCK: {
                execute_ast(node->left);
                break;
            }
            default: break;
        }

        if (node->node_type != NODE_FOR) {
            node = node->next;
        } else {
            node = NULL;
        }
    }
}

void generate_tac(ASTNode *node) {
    execute_ast(node);
}