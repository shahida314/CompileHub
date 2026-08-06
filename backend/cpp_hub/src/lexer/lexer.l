%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

int line_num = 1;
void yyerror(const char *s);
%}

%%

"#include"[ \t]*"<iostream>"[ \t]*\n   { line_num++; return INCLUDE_STDIO; }
"#include"[ \t]*"<iostream>"           { return INCLUDE_STDIO; }

"//".*                                  { /* single-line comment: discard */ }
"/\*"([^*]|\*+[^*/])*\*+"/"             { /* block comment: discard */ }

"std"        { return STD; }
"using"      { return USING; }
"namespace"  { return NAMESPACE; }
"endl"       { return ENDL; }
"cin"        { return CIN; }
"::"         { return SCOPE; }

"int"        { return INT; }
"float"      { return FLOAT; }
"bool"       { return BOOL; }
"if"         { return IF; }
"else"       { return ELSE; }
"while"      { return WHILE; }
"for"        { return FOR; }
"return"     { return RETURN; }
"cout"       { return PRINTF; }
"true"       { return TRUE; }
"false"      { return FALSE; }

"+"  { return PLUS; }
"-"  { return MINUS; }
"*"  { return MULT; }
"/"  { return DIV; }
"%"  { return MOD; }

"<<" { return SHL; }
">>" { return SHR; }

"="  { return ASSIGN; }
"==" { return EQ; }
"!=" { return NEQ; }
"<=" { return LE; }
">=" { return GE; }
"<"  { return LT; }
">"  { return GT; }
"&&" { return AND; }
"||" { return OR; }
"!"  { return NOT; }

";" { return SEMICOLON; }
"," { return COMMA; }
"{" { return LBRACE; }
"}" { return RBRACE; }
"(" { return LPAREN; }
")" { return RPAREN; }

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

\n            { line_num++; }
[ \t\r]+      { /* skip whitespace */ }

. {
    fprintf(stderr, "Lexical Error at line %d: Unknown character '%s'\n", line_num, yytext);
}

%%

int yywrap() {
    return 1;
}