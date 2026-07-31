#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Symbol {
    char *name;
    char *type;     // "int" | "float" | "bool"
    int scope;
    int int_val;
    float float_val;
    int bool_val;
    struct Symbol *next;
} Symbol;

Symbol *symbolTable[TABLE_SIZE];

unsigned int hash(const char *name) {
    unsigned int h = 0;
    while (*name) {
        h = (h << 5) + *name++;
    }
    return h % TABLE_SIZE;
}

void init_symbol_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        symbolTable[i] = NULL;
    }
}

// Variable Declaration
int insert_symbol(const char *name, const char *type, int scope) {
    unsigned int index = hash(name);
    Symbol *current = symbolTable[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->scope == scope) {
            return 0; // redeclaration in same scope
        }
        current = current->next;
    }

    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    new_symbol->name = strdup(name);
    new_symbol->type = strdup(type);
    new_symbol->scope = scope;
    new_symbol->int_val = 0;
    new_symbol->float_val = 0.0f;
    new_symbol->bool_val = 0;
    new_symbol->next = symbolTable[index];
    symbolTable[index] = new_symbol;

    return 1;
}

Symbol *lookup_symbol(const char *name) {
    unsigned int index = hash(name);
    Symbol *current = symbolTable[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// ---- int ----
void set_variable_value(const char *name, int val) {
    Symbol *s = lookup_symbol(name);
    if (!s) { insert_symbol(name, "int", 0); s = lookup_symbol(name); }
    if (s) s->int_val = val;
}
int get_variable_value(const char *name) {
    Symbol *s = lookup_symbol(name);
    return s ? s->int_val : 0;
}

// ---- float ----
void set_variable_value_float(const char *name, float val) {
    Symbol *s = lookup_symbol(name);
    if (!s) { insert_symbol(name, "float", 0); s = lookup_symbol(name); }
    if (s) s->float_val = val;
}
float get_variable_value_float(const char *name) {
    Symbol *s = lookup_symbol(name);
    return s ? s->float_val : 0.0f;
}

// ---- bool ----
void set_variable_value_bool(const char *name, int val) {
    Symbol *s = lookup_symbol(name);
    if (!s) { insert_symbol(name, "bool", 0); s = lookup_symbol(name); }
    if (s) s->bool_val = val;
}
int get_variable_value_bool(const char *name) {
    Symbol *s = lookup_symbol(name);
    return s ? s->bool_val : 0;
}

void print_symbol_table() {
    printf("\n=== SYMBOL TABLE ===\n");
    printf("%-15s %-10s %-10s\n", "Name", "Type", "Scope");
    printf("-----------------------------------------\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol *current = symbolTable[i];
        while (current != NULL) {
            printf("%-15s %-10s %-10d\n", current->name, current->type, current->scope);
            current = current->next;
        }
    }
    printf("===========================================\n\n");
}