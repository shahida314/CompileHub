#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Symbol
{
    char *name;
    char *type;
    int scope;
    int value;
    struct Symbol *next;
} Symbol;

Symbol *symbolTable[TABLE_SIZE];

unsigned int hash(const char *name)
{
    unsigned int hash = 0;
    while (*name)
    {
        hash = (hash << 5) + *name++;
    }
    return hash % TABLE_SIZE;
}

void init_symbol_table()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        symbolTable[i] = NULL;
    }
}

int insert_symbol(const char *name, const char *type, int scope)
{
    unsigned int index = hash(name);
    Symbol *current = symbolTable[index];

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0 && current->scope == scope)
        {
            return 0;
        }
        current = current->next;
    }

    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    new_symbol->name = strdup(name);
    new_symbol->type = strdup(type);
    new_symbol->scope = scope;
    new_symbol->value = 0;
    new_symbol->next = symbolTable[index];
    symbolTable[index] = new_symbol;

    return 1;
}

Symbol *lookup_symbol(const char *name)
{
    unsigned int index = hash(name);
    Symbol *current = symbolTable[index];

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void set_variable_value(const char *name, int val)
{
    Symbol *s = lookup_symbol(name);
    if (s)
    {
        s->value = val;
    }
    else
    {
        insert_symbol(name, "int", 0);
        s = lookup_symbol(name);
        if (s)
            s->value = val;
    }
}

int get_variable_value(const char *name)
{
    Symbol *s = lookup_symbol(name);
    if (s)
    {
        return s->value;
    }
    return 0;
}

void print_symbol_table()
{
    printf("\n=== SYMBOL TABLE ===\n");
    printf("%-15s %-10s %-10s %-10s\n", "Name", "Type", "Scope", "Value");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Symbol *current = symbolTable[i];
        while (current != NULL)
        {
            printf("%-15s %-10s %-10d %-10d\n", current->name, current->type, current->scope, current->value);
            current = current->next;
        }
    }
    printf("===================================================\n\n");
}