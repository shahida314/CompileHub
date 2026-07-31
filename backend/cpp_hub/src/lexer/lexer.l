%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

int line_num = 1;
void yyerror(const char *s);
%}

%%

"//".*                        { /* single-line comment: discard */ }
"/\*"([^*]|\*+[^*/])*\*+"/"   { /* block comment: discard */ }

"int"     { return INT_TYPE; }
"float"   { return FLOAT_TYPE; }
"bool"    { return BOOL_TYPE; }
"if"      { return IF; }
"else"    { return ELSE; }
"while"   { return WHILE; }
"print"   { return PRINT; }
"true"    { return TRUE_TOK; }
"false"   { return FALSE_TOK; }

"+"   { return PLUS; }
"-"   { return MINUS; }
"*"   { return MULT; }
"/"   { return DIV; }
"%"   { return MOD; }
"=="  { return EQ; }
"!="  { return NEQ; }
"<="  { return LE; }
">="  { return GE; }
"="   { return ASSIGN; }
"<"   { return LT; }
">"   { return GT; }
"&&"  { return AND; }
"||"  { return OR; }
"!"   { return NOT; }

";"   { return SEMICOLON; }
","   { return COMMA; }
"{"   { return LBRACE; }
"}"   { return RBRACE; }
"("   { return LPAREN; }
")"   { return RPAREN; }

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.sval = strdup(yytext);
    return IDENTIFIER;
}

[0-9]+"."[0-9]+ {
    yylval.fval = atof(yytext);
    return FLOAT_LITERAL;
}

[0-9]+ {
    yylval.ival = atoi(yytext);
    return INT_LITERAL;
}

\n       { line_num++; }
[ \t\r]+ { /* skip whitespace */ }

. {
    fprintf(stderr, "Lexical Error at line %d: unrecognized character '%s'\n", line_num, yytext);
}

%%

int yywrap() {
    return 1;
}