#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Symbol {
    char name[50];
    char type[10]; // int, float, bool
    int scope;
    int line;
    struct Symbol *next;
} Symbol;

Symbol *symbol_table = NULL;
int current_scope = 0;

void enter_scope() {
    current_scope++;
}

void exit_scope() {
    // Current scope এর ভ্যারিয়েবল রিমুভ করা
    Symbol **curr = &symbol_table;
    while (*curr) {
        if ((*curr)->scope == current_scope) {
            Symbol *temp = *curr;
            *curr = (*curr)->next;
            free(temp);
        } else {
            curr = &((*curr)->next);
        }
    }
    if (current_scope > 0) current_scope--;
}

int insert_symbol(const char *name, const char *type, int line) {
    // বর্তমান স্কোপে একই নামের ভ্যারিয়েবল আছে কিনা চেক
    Symbol *curr = symbol_table;
    while (curr) {
        if (strcmp(curr->name, name) == 0 && curr->scope == current_scope) {
            return 0; // Redeclaration error
        }
        curr = curr->next;
    }

    Symbol *new_sym = (Symbol *)malloc(sizeof(Symbol));
    strcpy(new_sym->name, name);
    strcpy(new_sym->type, type);
    new_sym->scope = current_scope;
    new_sym->line = line;
    new_sym->next = symbol_table;
    symbol_table = new_sym;
    return 1;
}

Symbol* lookup_symbol(const char *name) {
    Symbol *curr = symbol_table;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL; // Not found
}