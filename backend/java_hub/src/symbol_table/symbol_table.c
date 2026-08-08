#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ASTNode;

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
    Symbol *curr = symbol_table;
    while (curr) {
        if (strcmp(curr->name, name) == 0 && curr->scope == current_scope) {
            return 0;
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
    return NULL;
}



typedef struct Function {
    char name[50];
    char return_type[10];
    struct ASTNode *params;
    struct ASTNode *body;
    struct Function *next;
} Function;

Function *function_table = NULL;

void register_function(const char *name, const char *return_type, struct ASTNode *params, struct ASTNode *body) {
    Function *f = (Function *)malloc(sizeof(Function));
    strcpy(f->name, name);
    strcpy(f->return_type, return_type);
    f->params = params;
    f->body = body;
    f->next = function_table;
    function_table = f;
}

Function* lookup_function(const char *name) {
    Function *curr = function_table;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
struct ASTNode* get_function_params(const char *name) {
    Function *f = lookup_function(name);
    return f ? (struct ASTNode*)f->params : NULL;
}

struct ASTNode* get_function_body(const char *name) {
    Function *f = lookup_function(name);
    return f ? (struct ASTNode*)f->body : NULL;
}