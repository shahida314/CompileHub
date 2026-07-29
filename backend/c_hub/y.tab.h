/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INCLUDE_STDIO = 258,
     MAIN = 259,
     RETURN = 260,
     PRINTF = 261,
     INT = 262,
     FLOAT = 263,
     CHAR = 264,
     BOOL = 265,
     IF = 266,
     ELSE = 267,
     WHILE = 268,
     PRINT = 269,
     TRUE = 270,
     FALSE = 271,
     IDENTIFIER = 272,
     STRING_LITERAL = 273,
     INT_LITERAL = 274,
     FLOAT_LITERAL = 275,
     ASSIGN = 276,
     PLUS = 277,
     MINUS = 278,
     MULT = 279,
     DIV = 280,
     MOD = 281,
     EQ = 282,
     NEQ = 283,
     LT = 284,
     GT = 285,
     LE = 286,
     GE = 287,
     AND = 288,
     OR = 289,
     NOT = 290,
     SEMICOLON = 291,
     LBRACE = 292,
     RBRACE = 293,
     LPAREN = 294,
     RPAREN = 295,
     COMMA = 296
   };
#endif
/* Tokens.  */
#define INCLUDE_STDIO 258
#define MAIN 259
#define RETURN 260
#define PRINTF 261
#define INT 262
#define FLOAT 263
#define CHAR 264
#define BOOL 265
#define IF 266
#define ELSE 267
#define WHILE 268
#define PRINT 269
#define TRUE 270
#define FALSE 271
#define IDENTIFIER 272
#define STRING_LITERAL 273
#define INT_LITERAL 274
#define FLOAT_LITERAL 275
#define ASSIGN 276
#define PLUS 277
#define MINUS 278
#define MULT 279
#define DIV 280
#define MOD 281
#define EQ 282
#define NEQ 283
#define LT 284
#define GT 285
#define LE 286
#define GE 287
#define AND 288
#define OR 289
#define NOT 290
#define SEMICOLON 291
#define LBRACE 292
#define RBRACE 293
#define LPAREN 294
#define RPAREN 295
#define COMMA 296




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 78 "src/parser/parser.y"

    int int_val;
    float float_val;
    char* str_val;
    struct ASTNode* node;



/* Line 1685 of yacc.c  */
#line 142 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


