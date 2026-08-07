%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

void yyerror(const char *s);
%}

%option noyywrap
%option yylineno

%%
"import"[ \t]+[a-zA-Z0-9_\.]+\;   { /* import statement ignore */ }
"//".*                   { }
"/*"([^*]|\*+[^/*])*\*+"/" { }

"int"       { yylval.str_val = strdup(yytext); return INT; }
"float"     { yylval.str_val = strdup(yytext); return FLOAT; }
"bool"|"boolean" { yylval.str_val = strdup("bool"); return BOOL; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"for"       { return FOR; }
"print"     { return PRINT; }
"true"      { return TRUE; }
"false"     { return FALSE; }

"class"     { return CLASS; }
"public"    { return PUBLIC; }
"static"    { return STATIC; }
"void"      { return VOID; }
"String"    { return STRING_TYPE; }
"System"    { return SYSTEM_KW; }
"return"    { return RETURN; }
"new"       { return NEW; }
"Scanner"   { return SCANNER; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"%"         { return MOD; }
"="         { return ASSIGN; }
"++"        { return INC; }
"=="        { return EQ; }
"!="        { return NEQ; }
"<"         { return LT; }
">"         { return GT; }
"<="        { return LE; }
">="        { return GE; }
"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }

";"         { return SEMICOLON; }
","         { return COMMA; }
"."         { return DOT; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }

\"([^\"\n])*\" {
    yylval.str_val = strdup(yytext + 1);
    yylval.str_val[strlen(yylval.str_val) - 1] = '\0';
    return STRING_LITERAL;
}

[0-9]+\.[0-9]+([fF])? { yylval.str_val = strdup(yytext); return FLOAT_LITERAL; }
[0-9]+                 { yylval.str_val = strdup(yytext); return INT_LITERAL; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str_val = strdup(yytext); return IDENTIFIER; }

\n          { }
[ \t\r]+    { }

. { fprintf(stderr, "Lexical Error at line %d: Unexpected character '%s'\n", yylineno, yytext); }

%%