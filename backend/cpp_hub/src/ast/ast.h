#ifndef AST_H
#define AST_H

#include "../symbol_table/symbol_table.h"

typedef enum {
    NODE_PROGRAM, NODE_DECL, NODE_ASSIGN, NODE_READ, NODE_PRINT,
    NODE_IF, NODE_FOR, NODE_BLOCK, NODE_BIN_OP, NODE_UNARY_OP,
    NODE_VAR, NODE_INT, NODE_FLOAT, NODE_BOOL
} NodeType;

typedef struct ASTNode {
    NodeType node_type;
    DataType data_type;
    char *op_val;
    int int_val;
    float float_val;
    int bool_val;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *third;
    struct ASTNode *next;
    int line;
} ASTNode;

ASTNode* create_node(NodeType type);
ASTNode* create_var_node(char *name, int line);
ASTNode* create_int_node(int val);
ASTNode* create_float_node(float val);
ASTNode* create_bool_node(int val);
ASTNode* create_op_node(char *op, ASTNode *left, ASTNode *right);

void check_semantic_and_gen_tac(ASTNode *node);
char* new_temp();
char* new_label();

#endif