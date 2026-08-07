#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void analyze_semantics(ASTNode *node) {
    while (node) {
        switch(node->node_type) {
            case NODE_DECL: {
                DataType dt = get_type_from_string(node->op_val);
                if (!insert_symbol(node->left->op_val, dt, node->line)) {
                    fprintf(stderr, "Semantic Error (Line %d): Variable '%s' is already declared in this scope.\n", 
                            node->line, node->left->op_val);
                    exit(1);
                }
                break;
            }
            case NODE_ASSIGN: {
                Symbol *sym = lookup_symbol(node->left->op_val);
                if (!sym) {
                    fprintf(stderr, "Semantic Error (Line %d): Variable '%s' used without declaration.\n", 
                            node->line, node->left->op_val);
                    exit(1);
                }
                // Right hand side evaluation
                analyze_semantics(node->right);
                
                // Type mismatch warning/error check
                if (node->right && node->right->data_type != TYPE_UNKNOWN && sym->type != node->right->data_type) {
                    fprintf(stderr, "Semantic Warning (Line %d): Type mismatch. Assigning expression to variable '%s'.\n", 
                            node->line, sym->name);
                }
                break;
            }
            case NODE_READ: {
                Symbol *sym = lookup_symbol(node->left->op_val);
                if (!sym) {
                    fprintf(stderr, "Semantic Error (Line %d): Variable '%s' in cin is not declared.\n", 
                            node->line, node->left->op_val);
                    exit(1);
                }
                break;
            }
            case NODE_PRINT: {
                if (node->left) {
                    analyze_semantics(node->left);
                }
                break;
            }
            case NODE_IF: {
                analyze_semantics(node->left); // Condition check
                enter_scope();
                analyze_semantics(node->right); // Then block
                exit_scope();

                if (node->third) {
                    enter_scope();
                    analyze_semantics(node->third); // Else block
                    exit_scope();
                }
                break;
            }
            case NODE_FOR: {
                enter_scope();
                if (node->left) analyze_semantics(node->left);   // Init
                if (node->right) analyze_semantics(node->right); // Condition
                
                analyze_semantics(node->next); // Body
                
                if (node->third) analyze_semantics(node->third); // Increment/Step
                exit_scope();
                break;
            }
            case NODE_BLOCK: {
                enter_scope();
                analyze_semantics(node->left);
                exit_scope();
                break;
            }
            case NODE_VAR: {
                Symbol *sym = lookup_symbol(node->op_val);
                if (!sym) {
                    fprintf(stderr, "Semantic Error (Line %d): Identifier '%s' is undeclared.\n", 
                            node->line, node->op_val);
                    exit(1);
                }
                node->data_type = sym->type;
                break;
            }
            case NODE_BIN_OP: {
                analyze_semantics(node->left);
                analyze_semantics(node->right);

                if (strcmp(node->op_val, "&&") == 0 || strcmp(node->op_val, "||") == 0 ||
                    strcmp(node->op_val, "==") == 0 || strcmp(node->op_val, "!=") == 0 ||
                    strcmp(node->op_val, "<") == 0  || strcmp(node->op_val, ">") == 0) {
                    node->data_type = TYPE_BOOL;
                } else {
                    node->data_type = node->left->data_type;
                }
                break;
            }
            default: break;
        }

        // For-loop statement body is handled internally, others traverse using ->next
        if (node->node_type != NODE_FOR) {
            node = node->next;
        } else {
            node = NULL;
        }
    }
}