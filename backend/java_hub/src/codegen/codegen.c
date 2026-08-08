#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int temp_count = 0;
static int label_count = 0;

char* new_temp() {
    char *buf = (char*)malloc(16);
    sprintf(buf, "t%d", ++temp_count);
    return buf;
}

char* new_label() {
    char *buf = (char*)malloc(16);
    sprintf(buf, "L%d", ++label_count);
    return buf;
}

char* gen_expr(ASTNode *node) {
    if (!node) return strdup("");
    char *left, *right, *temp;

    switch (node->type) {
        case NODE_NUM:
        case NODE_VAR:
            return strdup(node->value);

        case NODE_BINOP:
            left = gen_expr(node->left);
            right = gen_expr(node->right);
            temp = new_temp();
            printf("%s = %s %s %s\n", temp, left, node->value, right);
            free(left);
            free(right);
            return temp;

        case NODE_UNOP:
            left = gen_expr(node->left);
            temp = new_temp();
            printf("%s = %s%s\n", temp, node->value, left);
            free(left);
            return temp;

        default:
            return strdup("");
    }
}

void gen_stmt_list(ASTNode *node);

void gen_stmt(ASTNode *node) {
    char *place, *l1, *l2;

    switch (node->type) {
        case NODE_DECL:
            
            break;

        case NODE_ASSIGN:
            place = gen_expr(node->right);
            printf("%s = %s\n", node->left->value, place);
            free(place);
            break;

        case NODE_PRINT:
            place = gen_expr(node->left);
            printf("print %s\n", place);
            free(place);
            break;

        case NODE_BLOCK:
            gen_stmt_list(node->left);
            break;

        case NODE_IF:
            place = gen_expr(node->left); // condition
            if (node->extra) {
                l1 = new_label(); // else label
                l2 = new_label(); // end label
                printf("if %s == 0 goto %s\n", place, l1);
                gen_stmt_list(node->right); // then-branch
                printf("goto %s\n", l2);
                printf("%s:\n", l1);
                gen_stmt_list(node->extra); // else-branch
                printf("%s:\n", l2);
                free(l1);
                free(l2);
            } else {
                l1 = new_label();
                printf("if %s == 0 goto %s\n", place, l1);
                gen_stmt_list(node->right);
                printf("%s:\n", l1);
                free(l1);
            }
            free(place);
            break;

        case NODE_WHILE:
            l1 = new_label(); // start
            l2 = new_label(); // end
            printf("%s:\n", l1);
            place = gen_expr(node->left); // condition
            printf("if %s == 0 goto %s\n", place, l2);
            gen_stmt_list(node->right); // body
            printf("goto %s\n", l1);
            printf("%s:\n", l2);
            free(place);
            free(l1);
            free(l2);
            break;

        default:
            break;
    }
}

void gen_stmt_list(ASTNode *node) {
    while (node) {
        gen_stmt(node);
        node = node->next;
    }
}

void generate_tac(ASTNode *root) {
    printf("\nGenerated Three Address Code (TAC):\n");
    gen_stmt_list(root);
}