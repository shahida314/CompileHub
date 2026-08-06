%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
%}

DIGIT [0-9]
ID    [a-zA-Z_][a-zA-Z0-9_]*

%%

"print"       { return PRINT; }
"input"       { return INPUT; }
"strip"       { return STRIP; }
"int"         { return INT_CAST; }
"float"       { return FLOAT_CAST; }
"bool"        { return BOOL_TYPE; }
"char"        { return CHAR_TYPE; }
"True"        { yylval.ival = 1; return BOOLEAN_LITERAL; }
"False"       { yylval.ival = 0; return BOOLEAN_LITERAL; }

{DIGIT}+\.{DIGIT}+ { yylval.fval = atof(yytext); return FLOAT_LITERAL; }
{DIGIT}+           { yylval.ival = atoi(yytext); return INT_LITERAL; }
\'([^'\\]|\\.)\'   { yylval.sval = strdup(yytext); return CHAR_LITERAL; }
\"([^"\\]|\\.)*\"  { yylval.sval = strdup(yytext); return STRING_LITERAL; }
{ID}               { yylval.sval = strdup(yytext); return IDENTIFIER; }

"="           { return ASSIGN; }
"=="          { return EQ; }
"!="          { return NE; }
"<="          { return LE; }
">="          { return GE; }
"+"           { return '+'; }
"-"           { return '-'; }
"*"           { return '*'; }
"/"           { return '/'; }
"("           { return '('; }
")"           { return ')'; }
"."           { return '.'; }

[ \t\n]+      { /* ignore whitespace */ }
.             { printf("Unknown token: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}