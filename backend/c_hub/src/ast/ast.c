#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// AST Node Types
typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_WHILE,
    NODE_PRINT,
    NODE_BINARY_OP,
    NODE_IDENTIFIER,
    NODE_LITERAL
} NodeType;

// AST Node Structure
typedef struct ASTNode {
    NodeType type;
    char *value;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;
} ASTNode;

// Function to Create AST Node
ASTNode* create_node(NodeType type, char *value, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error for AST Node\n");
        exit(1);
    }
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = left;
    node->right = right;
    node->next = NULL;
    return node;
}

// Function to Print AST
void print_ast(ASTNode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->type) {
        case NODE_PROGRAM: printf("Program\n"); break;
        case NODE_DECLARATION: printf("Declaration (%s)\n", node->value); break;
        case NODE_ASSIGNMENT: printf("Assignment (=)\n"); break;
        case NODE_IF: printf("If-Else Statement\n"); break;
        case NODE_WHILE: printf("While Loop\n"); break;
        case NODE_PRINT: printf("Print Statement\n"); break;
        case NODE_BINARY_OP: printf("Binary Op (%s)\n", node->value); break;
        case NODE_IDENTIFIER: printf("Identifier (%s)\n", node->value); break;
        case NODE_LITERAL: printf("Literal (%s)\n", node->value); break;
    }

    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
    print_ast(node->next, depth);
}

// Function to Free AST Memory
void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->next);
    if (node->value) free(node->value);
    free(node);
}