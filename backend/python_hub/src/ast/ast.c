#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ASTNode
{
    char *name;
    struct ASTNode *children[10];
    int children_count;
} ASTNode;

ASTNode *python_create_node(char *name)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->name = strdup(name);
    node->children_count = 0;
    return node;
}

void python_add_child(ASTNode *parent, ASTNode *child)
{
    if (parent->children_count < 10)
    {
        parent->children[parent->children_count++] = child;
    }
}