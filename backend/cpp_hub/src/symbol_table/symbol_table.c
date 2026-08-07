#include "symbol_table.h"

static Scope *current_scope = NULL;
static int current_level = 0;

void init_symbol_table() {
    current_scope = (Scope*)malloc(sizeof(Scope));
    current_scope->level = 0;
    current_scope->symbols = NULL;
    current_scope->parent = NULL;
    current_level = 0;
}

void enter_scope() {
    Scope *new_scope = (Scope*)malloc(sizeof(Scope));
    new_scope->level = ++current_level;
    new_scope->symbols = NULL;
    new_scope->parent = current_scope;
    current_scope = new_scope;
}

void exit_scope() {
    if (current_scope->parent) {
        Scope *temp = current_scope;
        current_scope = current_scope->parent;
        free(temp);
    }
}

int insert_symbol(const char *name, DataType type, int line) {
    Symbol *curr = current_scope->symbols;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return 0; // Already declared in this scope
        }
        curr = curr->next;
    }

    Symbol *sym = (Symbol*)malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->line_declared = line;
    sym->scope_level = current_scope->level;
    sym->next = current_scope->symbols;
    current_scope->symbols = sym;
    return 1;
}

Symbol* lookup_symbol(const char *name) {
    Scope *sc = current_scope;
    while (sc) {
        Symbol *sym = sc->symbols;
        while (sym) {
            if (strcmp(sym->name, name) == 0) return sym;
            sym = sym->next;
        }
        sc = sc->parent;
    }
    return NULL;
}

DataType get_type_from_string(const char *type_str) {
    if (strcmp(type_str, "int") == 0) return TYPE_INT;
    if (strcmp(type_str, "float") == 0) return TYPE_FLOAT;
    if (strcmp(type_str, "bool") == 0) return TYPE_BOOL;
    return TYPE_UNKNOWN;
}

const char* type_to_string(DataType type) {
    switch(type) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_BOOL: return "bool";
        default: return "unknown";
    }
}