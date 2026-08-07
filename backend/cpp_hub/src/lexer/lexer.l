%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ast/ast.h"
#include "../parser/parser.tab.h"

int yylineno_custom = 1;
%}

%option noyywrap
%option nounput
%option noinput

%%

"int"           { return INT_TYPE; }
"float"         { return FLOAT_TYPE; }
"bool"          { return BOOL_TYPE; }
"if"            { return IF; }
"else"          { return ELSE; }
"for"           { return FOR; }
"cin"           { return CIN; }
"cout"          { return COUT; }
"endl"          { return ENDL; }
"true"          { yylval.intval = 1; return BOOL_LITERAL; }
"false"         { yylval.intval = 0; return BOOL_LITERAL; }

"&&"            { return AND; }
"||"            { return OR; }
"=="            { return EQ; }
"!="            { return NE; }
"<<"            { return LSHIFT; }
">>"            { return RSHIFT; }

"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"%"             { return '%'; }
"="             { return '='; }
"<"             { return '<'; }
">"             { return '>'; }
"!"             { return '!'; }

"{"             { return '{'; }
"}"             { return '}'; }
"("             { return '('; }
")"             { return ')'; }
";"             { return ';'; }

[0-9]+\.[0-9]+  { yylval.floatval = atof(yytext); return FLOAT_LITERAL; }
[0-9]+          { yylval.intval = atoi(yytext); return INT_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.strval = strdup(yytext); return IDENTIFIER; }

"//".*          ; /* Skip comments */
[ \t\r]+        ; /* Skip whitespace */
\n              { yylineno_custom++; }

.               { fprintf(stderr, "Lexical Error (Line %d): Unrecognized character '%s'\n", yylineno_custom, yytext); exit(1); }

%%