#ifndef CODEGEN_H
#define CODEGEN_H

#include "../ast/ast.h"

char* new_temp();
char* new_label();
char* gen_tac_expr(ASTNode *node);
void generate_tac(ASTNode *node);

#endif