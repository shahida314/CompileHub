#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Symbol* lookup_symbol(const char *name);

int semantic_errors = 0;

void check_declaration(const char* name, int line) {
    Symbol *sym = lookup_symbol(name);
    if (!sym) {
        fprintf(stderr, "Semantic Error at line %d: Variable '%s' undeclared.\n", line, name);
        semantic_errors++;
    }
}

void check_type_match(const char* target_type, const char* expr_type, int line) {
    if (strcmp(target_type, expr_type) != 0 && strcmp(expr_type, "unknown") != 0) {
        fprintf(stderr, "Semantic Error at line %d: Type mismatch. Cannot assign %s to %s.\n", line, expr_type, target_type);
        semantic_errors++;
    }
}