#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.tab.h"

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

static int temp_count = 0;
static int label_count = 0;

static char *new_temp() {
    char *buf = (char *)malloc(16);
    snprintf(buf, 16, "t%d", ++temp_count);
    return buf;
}
static char *new_label() {
    char *buf = (char *)malloc(16);
    snprintf(buf, 16, "L%d", ++label_count);
    return buf;
}

static const char *op_symbol(int op) {
    switch (op) {
        case PLUS: return "+";
        case MINUS: return "-";
        case MULT: return "*";
        case DIV: return "/";
        case MOD: return "%";
        case LT: return "<";
        case GT: return ">";
        case LE: return "<=";
        case GE: return ">=";
        case EQ: return "==";
        case NEQ: return "!=";
        case AND: return "&&";
        case OR: return "||";
        case NOT: return "!";
        default: return "?";
    }
}

static char *gen_expr(ASTNode *node) {
    if (!node) return NULL;
    switch (node->type) {
        case NODE_NUM_INT: {
            char *buf = (char *)malloc(16);
            snprintf(buf, 16, "%d", node->int_val);
            return buf;
        }
        case NODE_NUM_FLOAT: {
            char *buf = (char *)malloc(32);
            snprintf(buf, 32, "%f", node->float_val);
            return buf;
        }
        case NODE_BOOL:
            return strdup(node->bool_val ? "true" : "false");
        case NODE_VAR:
            return strdup(node->str_val);
        case NODE_FUNC_CALL: {
            char *arg = node->left ? gen_expr(node->left) : NULL;
            char *t = new_temp();
            if (arg) { printf("%s = call %s, %s\n", t, node->str_val, arg); free(arg); }
            else printf("%s = call %s\n", t, node->str_val);
            return t;
        }
        case NODE_UNOP: {
            char *operand = gen_expr(node->left);
            char *t = new_temp();
            printf("%s = %s%s\n", t, op_symbol(node->op), operand);
            free(operand);
            return t;
        }
        case NODE_BINOP: {
            char *l = gen_expr(node->left);
            char *r = gen_expr(node->right);
            char *t = new_temp();
            printf("%s = %s %s %s\n", t, l, op_symbol(node->op), r);
            free(l); free(r);
            return t;
        }
        default: return NULL;
    }
}

static void gen_stmt(ASTNode *node);

static void gen_stmt_list(ASTNode *node) {
    while (node) { gen_stmt(node); node = node->next; }
}

static void gen_stmt(ASTNode *node) {
    if (!node) return;
    switch (node->type) {
        case NODE_DECL:
            if (node->left) {
                char *val = gen_expr(node->left);
                printf("%s = %s\n", node->str_val, val);
                free(val);
            }
            break;
        case NODE_ASSIGN: {
            char *val = gen_expr(node->left);
            printf("%s = %s\n", node->str_val, val);
            free(val);
            break;
        }
        case NODE_PRINTF: {
            if (node->left) {
                char *val = gen_expr(node->left);
                printf("print %s\n", val);
                free(val);
            } else if (node->str_val) {
                printf("print %s\n", node->str_val);
            }
            break;
        }
        case NODE_IF: {
            char *cond = gen_expr(node->cond);
            char *else_label = new_label();
            char *end_label = new_label();
            printf("ifFalse %s goto %s\n", cond, else_label);
            gen_stmt(node->then_branch);
            printf("goto %s\n", end_label);
            printf("%s:\n", else_label);
            if (node->else_branch) gen_stmt(node->else_branch);
            printf("%s:\n", end_label);
            free(cond); free(else_label); free(end_label);
            break;
        }
        case NODE_WHILE: {
            char *start_label = new_label();
            char *end_label = new_label();
            printf("%s:\n", start_label);
            char *cond = gen_expr(node->cond);
            printf("ifFalse %s goto %s\n", cond, end_label);
            gen_stmt(node->body);
            printf("goto %s\n", start_label);
            printf("%s:\n", end_label);
            free(cond); free(start_label); free(end_label);
            break;
        }
        case NODE_FOR: {
            gen_stmt(node->init_stmt);
            char *start_label = new_label();
            char *end_label = new_label();
            printf("%s:\n", start_label);
            char *cond = gen_expr(node->cond);
            printf("ifFalse %s goto %s\n", cond, end_label);
            gen_stmt(node->body);
            gen_stmt(node->post_stmt);
            printf("goto %s\n", start_label);
            printf("%s:\n", end_label);
            free(cond); free(start_label); free(end_label);
            break;
        }
        case NODE_RETURN: {
            char *val = gen_expr(node->left);
            printf("return %s\n", val ? val : "");
            if (val) free(val);
            break;
        }
        case NODE_FUNC_CALL:
            gen_expr(node); // discard the returned temp, just print the call
            break;
        default:
            break;
    }
}

// Entry point — called from parser.y's main after a successful parse
void generate_tac(ASTNode *node) {
    temp_count = 0;
    label_count = 0;
    gen_stmt_list(node);
}