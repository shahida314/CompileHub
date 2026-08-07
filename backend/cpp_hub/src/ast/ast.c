#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* create_node(NodeType type) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error for ASTNode\n");
        exit(1);
    }
    node->node_type = type;
    node->data_type = TYPE_UNKNOWN;
    node->int_val = 0;
    node->float_val = 0.0;
    node->bool_val = 0;
    node->op_val = NULL;
    node->line = 0;
    node->left = NULL;
    node->right = NULL;
    node->third = NULL;
    node->next = NULL;
    return node;
}

ASTNode* create_int_node(int val) {
    ASTNode *node = create_node(NODE_INT);
    node->int_val = val;
    node->data_type = TYPE_INT;
    return node;
}

ASTNode* create_float_node(float val) {
    ASTNode *node = create_node(NODE_FLOAT);
    node->float_val = val;
    node->data_type = TYPE_FLOAT;
    return node;
}

ASTNode* create_bool_node(int val) {
    ASTNode *node = create_node(NODE_BOOL);
    node->bool_val = val;
    node->data_type = TYPE_BOOL;
    return node;
}

ASTNode* create_var_node(char *name, int line) {
    ASTNode *node = create_node(NODE_VAR);
    node->op_val = strdup(name);
    node->line = line;
    return node;
}

ASTNode* create_op_node(char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = create_node(NODE_BIN_OP);
    node->op_val = strdup(op);
    node->left = left;
    node->right = right;
    return node;
}

void free_ast(ASTNode *node) {
    if (!node) return;
    if (node->op_val) free(node->op_val);
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->third);
    free_ast(node->next);
    free(node);
}