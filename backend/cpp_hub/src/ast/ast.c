#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_STMT_LIST,
    NODE_DECL,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_PRINTF,
    NODE_BINOP,
    NODE_UNOP,
    NODE_VAR,
    NODE_NUM_INT,
    NODE_NUM_FLOAT,
    NODE_BOOL,
    NODE_FUNC_CALL,
    NODE_RETURN,
    NODE_STRING,
    NODE_ENDL
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
    struct ASTNode *stream_next;
} ASTNode;

ASTNode *create_node(NodeType type) {
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error for AST Node\n");
        exit(1);
    }
    node->type = type;
    return node;
}

ASTNode *create_binop(int op, ASTNode *left, ASTNode *right) {
    ASTNode *node = create_node(NODE_BINOP);
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *create_unop(int op, ASTNode *operand) {
    ASTNode *node = create_node(NODE_UNOP);
    node->op = op;
    node->left = operand;
    return node;
}

void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->cond);
    free_ast(node->then_branch);
    free_ast(node->else_branch);
    free_ast(node->body);
    free_ast(node->init_stmt);
    free_ast(node->post_stmt);
    free_ast(node->stream_next);
    free_ast(node->next);
    if (node->str_val) free(node->str_val);
    free(node);
}

static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf(" ");
}

void print_ast(ASTNode *node, int indent) {
    while (node) {
        print_indent(indent);
        switch (node->type) {
            case NODE_DECL:
                printf("Decl: %s\n", node->str_val);
                if (node->left) {
                    print_indent(indent + 1); printf("Init:\n");
                    print_ast(node->left, indent + 2);
                }
                break;
            case NODE_ASSIGN:
                printf("Assign: %s\n", node->str_val);
                print_ast(node->left, indent + 1);
                break;
            case NODE_IF:
                printf("If\n");
                print_indent(indent + 1); printf("Cond:\n");
                print_ast(node->cond, indent + 2);
                print_indent(indent + 1); printf("Then:\n");
                print_ast(node->then_branch, indent + 2);
                if (node->else_branch) {
                    print_indent(indent + 1); printf("Else:\n");
                    print_ast(node->else_branch, indent + 2);
                }
                break;
            case NODE_WHILE:
                printf("While\n");
                print_indent(indent + 1); printf("Cond:\n");
                print_ast(node->cond, indent + 2);
                print_indent(indent + 1); printf("Body:\n");
                print_ast(node->body, indent + 2);
                break;
            case NODE_FOR:
                printf("For\n");
                print_indent(indent + 1); printf("Init:\n");
                print_ast(node->init_stmt, indent + 2);
                print_indent(indent + 1); printf("Cond:\n");
                print_ast(node->cond, indent + 2);
                print_indent(indent + 1); printf("Post:\n");
                print_ast(node->post_stmt, indent + 2);
                print_indent(indent + 1); printf("Body:\n");
                print_ast(node->body, indent + 2);
                break;
            case NODE_PRINTF:
                printf("Printf: %s\n", node->str_val ? node->str_val : "(stream expr)");
                if (node->left) print_ast(node->left, indent + 1);
                break;
            case NODE_BINOP:
                printf("BinOp(token=%d)\n", node->op);
                print_ast(node->left, indent + 1);
                print_ast(node->right, indent + 1);
                break;
            case NODE_UNOP:
                printf("UnOp(token=%d)\n", node->op);
                print_ast(node->left, indent + 1);
                break;
            case NODE_VAR:
                printf("Var: %s\n", node->str_val);
                break;
            case NODE_NUM_INT:
                printf("IntLiteral: %d\n", node->int_val);
                break;
            case NODE_NUM_FLOAT:
                printf("FloatLiteral: %f\n", node->float_val);
                break;
            case NODE_BOOL:
                printf("BoolLiteral: %s\n", node->bool_val ? "true" : "false");
                break;
            case NODE_STRING:
                printf("StringLiteral: %s\n", node->str_val);
                break;
            case NODE_ENDL:
                printf("Endl\n");
                break;
            case NODE_FUNC_CALL:
                printf("FuncCall: %s\n", node->str_val);
                if (node->left) print_ast(node->left, indent + 1);
                break;
            case NODE_RETURN:
                printf("Return\n");
                if (node->left) print_ast(node->left, indent + 1);
                break;
            default:
                printf("Unknown Node\n");
        }
        if (node->stream_next) {
            print_ast(node->stream_next, indent);
        }
        node = node->next;
    }
}