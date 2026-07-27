#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Symbol
{
    char *name;
    char *type;
    int scope;
    struct Symbol *next;
} Symbol;

extern Symbol *lookup_symbol(const char *name);
extern int insert_symbol(const char *name, const char *type, int scope);

// Variable Declaration Semantic Check
void check_declaration(const char *name, const char *type, int scope)
{
    if (!insert_symbol(name, type, scope))
    {
        fprintf(stderr, "Semantic Error: Variable '%s' is already declared in scope %d.\n", name, scope);
    }
}

// Variable Usage Semantic Check
void check_variable_usage(const char *name)
{
    Symbol *sym = lookup_symbol(name);
    if (sym == NULL)
    {
        fprintf(stderr, "Semantic Error: Variable '%s' used before declaration.\n", name);
    }
}

// Type Compatibility Check
void check_assignment_type(const char *name, const char *expr_type)
{
    Symbol *sym = lookup_symbol(name);
    if (sym != NULL)
    {
        if (strcmp(sym->type, expr_type) != 0)
        {
            fprintf(stderr, "Semantic Error: Type mismatch in assignment for '%s'. Expected '%s', got '%s'.\n",
                    name, sym->type, expr_type);
        }
    }
}