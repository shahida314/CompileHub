#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { NODE_DECL, NODE_ASSIGN, NODE_BINOP, NODE_UNOP, NODE_VAR, NODE_NUM,
               NODE_IF, NODE_WHILE, NODE_PRINT, NODE_BLOCK } NodeType;

typedef struct ASTNode {
    NodeType type;
    char value[50];
    char data_type[10]; // int, float, bool
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *extra; // used for if-else's else-branch
    struct ASTNode *next;  // next statement in a statement list
} ASTNode;

ASTNode* create_node(NodeType type, const char* val, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    if (val) strcpy(node->value, val);
    else node->value[0] = '\0';
    strcpy(node->data_type, "unknown");
    node->left = left;
    node->right = right;
    node->extra = NULL;
    node->next = NULL;
    return node;
}

void print_ast(ASTNode *node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("- %s (%s)\n", node->value, node->data_type);

    if (node->type == NODE_BLOCK) {
        print_ast(node->left, level + 1);
    } else {
        print_ast(node->left, level + 1);
        print_ast(node->right, level + 1);
        if (node->extra) {
            for (int i = 0; i < level + 1; i++) printf("  ");
            printf("- else:\n");
            print_ast(node->extra, level + 2);
        }
    }
    print_ast(node->next, level);
}