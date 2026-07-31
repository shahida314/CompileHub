#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Symbol {
    char *name;
    char *type;
    int scope;
    int int_val;
    float float_val;
    int bool_val;
    struct Symbol *next;
} Symbol;

extern Symbol *lookup_symbol(const char *name);
extern int insert_symbol(const char *name, const char *type, int scope);

int semantic_error_count = 0;

// Variable Declaration Semantic Check (redeclaration in same scope)
void check_declaration(const char *name, const char *type, int scope) {
    if (!insert_symbol(name, type, scope)) {
        fprintf(stderr, "Semantic Error: Variable '%s' is already declared in scope %d.\n", name, scope);
        semantic_error_count++;
    }
}

// Variable Usage Semantic Check (undeclared / used before declaration)
void check_variable_usage(const char *name) {
    Symbol *sym = lookup_symbol(name);
    if (sym == NULL) {
        fprintf(stderr, "Semantic Error: Variable '%s' used before declaration.\n", name);
        semantic_error_count++;
    }
}

// Type Compatibility Check for assignment, e.g. bool b = 5 + 3.2;
void check_assignment_type(const char *name, const char *expr_type) {
    Symbol *sym = lookup_symbol(name);
    if (sym == NULL) {
        fprintf(stderr, "Semantic Error: Assignment to undeclared variable '%s'.\n", name);
        semantic_error_count++;
        return;
    }

    // Allow int -> float widening; everything else must match exactly
    int compatible = (strcmp(sym->type, expr_type) == 0) ||
                      (strcmp(sym->type, "float") == 0 && strcmp(expr_type, "int") == 0);

    if (!compatible) {
        fprintf(stderr, "Semantic Error: Type mismatch in assignment for '%s'. Expected '%s', got '%s'.\n",
                name, sym->type, expr_type);
        semantic_error_count++;
    }
}

// Invalid expression check, e.g. logical op on numeric operands, or vice versa
void check_operator_types(const char *op_category, const char *left_type, const char *right_type) {
    if (strcmp(op_category, "logical") == 0) {
        if (strcmp(left_type, "bool") != 0 || strcmp(right_type, "bool") != 0) {
            fprintf(stderr, "Semantic Error: Logical operator requires bool operands, got '%s' and '%s'.\n",
                    left_type, right_type);
            semantic_error_count++;
        }
    } else if (strcmp(op_category, "arithmetic") == 0 || strcmp(op_category, "relational") == 0) {
        int left_numeric = (strcmp(left_type, "int") == 0 || strcmp(left_type, "float") == 0);
        int right_numeric = (strcmp(right_type, "int") == 0 || strcmp(right_type, "float") == 0);
        if (!left_numeric || !right_numeric) {
            fprintf(stderr, "Semantic Error: %s operator requires numeric operands, got '%s' and '%s'.\n",
                    op_category, left_type, right_type);
            semantic_error_count++;
        }
    }
}