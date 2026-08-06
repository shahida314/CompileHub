#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    NODE_STMT_LIST,
    NODE_DECL,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_PRINTF,
    NODE_SCANF,
    NODE_BINOP,
    NODE_VAR,
    NODE_NUM
} NodeType;

typedef struct ASTNode
{
    NodeType type;
    int int_val;
    char *str_val;
    int op;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *third;
    struct ASTNode *next;
} ASTNode;

ASTNode *create_node(NodeType type)
{
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (!node)
    {
        fprintf(stderr, "Memory allocation error for AST Node\n");
        exit(1);
    }
    node->type = type;
    return node;
}

ASTNode *create_binop(int op, ASTNode *left, ASTNode *right)
{
    ASTNode *node = create_node(NODE_BINOP);
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

void free_ast(ASTNode *node)
{
    if (!node)
        return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->third);
    free_ast(node->next);
    if (node->str_val)
        free(node->str_val);
    free(node);
}