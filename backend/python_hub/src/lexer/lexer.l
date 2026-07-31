%{
#include <stdio.h>
#include <stdlib.h>
#2
%}

DIGIT [0-9]
ID    [a-zA-Z_][a-zA-Z0-9_]*

%%

"print"       { return PRINT; }
{DIGIT}+      { return NUMBER; }
{ID}          { return IDENTIFIER; }
"="           { return ASSIGN; }
[ \t\n]+      { /* ignore whitespace */ }
.             { printf("Unknown token: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}