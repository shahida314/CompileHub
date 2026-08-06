%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

int line_num = 1;
void yyerror(const char *s);
%}

%%

"#include"[ \t]*"<stdio.h>"[ \t]*\n { 
    line_num++; 
    return INCLUDE_STDIO; 
}

"#include"[ \t]*"<stdio.h>" { 
    return INCLUDE_STDIO; 
}

"int"       { return INT; }
"float"     { return FLOAT; }
"bool"      { return BOOL; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"for"       { return FOR; }
"return"    { return RETURN; }
"printf"    { return PRINTF; }
"scanf"     { return SCANF; }
"true"      { return TRUE; }
"false"     { return FALSE; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"%"         { return MOD; }
"="         { return ASSIGN; }
"=="        { return EQ; }
"!="        { return NEQ; }
"<"         { return LT; }
">"         { return GT; }
"<="        { return LE; }
">="        { return GE; }
"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }
"++"        { return INC; }
"--"        { return DEC; }

";"         { return SEMICOLON; }
","         { return COMMA; }
"&"         { return AMPERSAND; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"("         { return LPAREN; }
")"         { return RPAREN; }

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_val = strdup(yytext);
    return IDENTIFIER;
}

[0-9]+\.[0-9]+ {
    yylval.float_val = atof(yytext);
    return FLOAT_LITERAL;
}

[0-9]+ {
    yylval.int_val = atoi(yytext);
    return INT_LITERAL;
}

\"([^\"\n])*\" {
    yylval.str_val = strdup(yytext);
    return STRING_LITERAL;
}

\n { line_num++; }
[ \t\r]+ { /* skip whitespace */ }

. {
    fprintf(stderr, "Lexical Error at line %d: Unknown character '%s'\n", line_num, yytext);
}

%%

int yywrap() {
    return 1;
}