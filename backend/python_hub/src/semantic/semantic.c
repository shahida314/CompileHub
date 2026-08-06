#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void python_semantic_analyze(char *var_type, char *expr_type)
{
    if (strcmp(var_type, expr_type) != 0)
    {
        printf("Semantic Warning: Type mismatch between %s and %s\n", var_type, expr_type);
    }
    else
    {
        printf("Python Semantic Analysis: Checked successfully.\n");
    }
}