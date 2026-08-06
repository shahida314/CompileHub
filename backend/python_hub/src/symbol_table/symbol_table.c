#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Symbol
{
    char name[50];
    char type[20]; // int, float, bool, char, string
    struct Symbol *next;
} Symbol;

void python_init_symbol_table()
{
    // Initialize symbol table for Python variables and their types
    printf("Python Symbol Table Initialized.\n");
}