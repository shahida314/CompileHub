#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_UNKNOWN } DataType;

typedef struct Symbol {
    char *name;
    DataType type;
    int line_declared;
    int scope_level;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    int level;
    Symbol *symbols;
    struct Scope *parent;
} Scope;

void init_symbol_table();
void enter_scope();
void exit_scope();
int insert_symbol(const char *name, DataType type, int line);
Symbol* lookup_symbol(const char *name);
DataType get_type_from_string(const char *type_str);
const char* type_to_string(DataType type);

#endif