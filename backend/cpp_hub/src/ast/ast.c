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
    NODE_RETURN
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int int_val;
    float float_val;
    int bool_val;
    char *str_val;         // identifier name / format string
    int op;                // token for binop/unop

    struct ASTNode *left;
    struct ASTNode *right;

    struct ASTNode *cond;          // if/while/for condition
    struct ASTNode *then_branch;   // if - true branch
    struct ASTNode *else_branch;   // if - false branch
    struct ASTNode *body;          // while/for body
    struct ASTNode *init_stmt;     // for - init
    struct ASTNode *post_stmt;     // for - update

    struct ASTNode *next;
} ASTNode;

// Create a plain AST node (used by parser.y)
ASTNode *create_node(NodeType type) {
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error for AST Node\n");
        exit(1);
    }
    node->type = type;
    return node;
}

// Create a binary operation node (used by parser.y)
ASTNode *create_binop(int op, ASTNode *left, ASTNode *right) {
    ASTNode *node = create_node(NODE_BINOP);
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

// Create a unary operation node, e.g. !flag or -x (used by parser.y)
ASTNode *create_unop(int op, ASTNode *operand) {
    ASTNode *node = create_node(NODE_UNOP);
    node->op = op;
    node->left = operand;
    return node;
}

// Free AST memory
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
    free_ast(node->next);
    if (node->str_val) free(node->str_val);
    free(node);
}

static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}

// Text-based AST printer (required by manual, Section 4.3)
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
                printf("Printf: %s\n", node->str_val ? node->str_val : "(expr)");
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
        node = node->next;
    }
}