/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "src/parser/parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int line_num;
extern FILE *yyin;
void yyerror(const char *s);

extern void init_symbol_table();
extern void check_declaration(const char *name, const char *type, int scope);
extern void check_variable_usage(const char *name);
extern void set_variable_value(const char *name, int val);
extern int get_variable_value(const char *name);

typedef enum {
    NODE_STMT_LIST, 
    NODE_DECL, 
    NODE_ASSIGN, 
    NODE_IF, 
    NODE_WHILE, 
    NODE_PRINTF, 
    NODE_BINOP, 
    NODE_VAR, 
    NODE_NUM,
    NODE_FUNC_CALL,
    NODE_RETURN
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int int_val;
    char *str_val;
    int op;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;
} ASTNode;

extern ASTNode* create_node(NodeType type);
extern ASTNode* create_binop(int op, ASTNode *left, ASTNode *right);

int eval_ast(ASTNode *node);
void execute_ast(ASTNode *node);

typedef struct Function {
    char *name;
    char *param_name; 
    ASTNode *body;
    struct Function *next;
} Function;

Function *function_list = NULL;

void define_function(char *name, char *param_name, ASTNode *body) {
    Function *f = (Function*)malloc(sizeof(Function));
    f->name = strdup(name);
    f->param_name = param_name ? strdup(param_name) : NULL;
    f->body = body;
    f->next = function_list;
    function_list = f;
}

Function* get_function_obj(char *name) {
    Function *f = function_list;
    while (f != NULL) {
        if (strcmp(f->name, name) == 0) {
            return f;
        }
        f = f->next;
    }
    return NULL;
}



/* Line 189 of yacc.c  */
#line 150 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
#line 78 "src/parser/parser.y"

    int int_val;
    float float_val;
    char* str_val;
    struct ASTNode* node;



/* Line 214 of yacc.c  */
#line 277 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 289 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   234

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  54
/* YYNRULES -- Number of states.  */
#define YYNSTATES  112

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     8,    10,    13,    15,    26,    35,
      39,    40,    43,    44,    46,    48,    50,    52,    54,    56,
      59,    61,    63,    65,    67,    71,    73,    77,    81,    87,
      92,   100,   106,   112,   118,   126,   132,   137,   141,   143,
     147,   151,   155,   159,   163,   167,   171,   175,   179,   183,
     187,   191,   193,   195,   197
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      43,     0,    -1,    44,    45,    -1,    45,    -1,     3,    -1,
      45,    46,    -1,    46,    -1,     7,    17,    39,     7,    17,
      40,    37,    48,    47,    38,    -1,     7,    17,    39,    40,
      37,    48,    47,    38,    -1,     5,    60,    36,    -1,    -1,
      48,    49,    -1,    -1,    51,    -1,    53,    -1,    54,    -1,
      55,    -1,    56,    -1,    59,    -1,    57,    36,    -1,     7,
      -1,     8,    -1,     9,    -1,    10,    -1,    50,    52,    36,
      -1,    17,    -1,    17,    21,    60,    -1,    52,    41,    17,
      -1,    52,    41,    17,    21,    60,    -1,    17,    21,    60,
      36,    -1,    11,    39,    60,    40,    49,    12,    49,    -1,
      11,    39,    60,    40,    49,    -1,    13,    39,    60,    40,
      49,    -1,     6,    39,    18,    40,    36,    -1,     6,    39,
      18,    41,    58,    40,    36,    -1,     6,    39,    60,    40,
      36,    -1,    17,    39,    60,    40,    -1,    17,    39,    40,
      -1,    60,    -1,    37,    48,    38,    -1,    60,    22,    60,
      -1,    60,    23,    60,    -1,    60,    24,    60,    -1,    60,
      25,    60,    -1,    60,    26,    60,    -1,    60,    27,    60,
      -1,    60,    28,    60,    -1,    60,    30,    60,    -1,    60,
      29,    60,    -1,    60,    32,    60,    -1,    60,    31,    60,
      -1,    57,    -1,    17,    -1,    19,    -1,    20,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   107,   107,   113,   122,   126,   127,   131,   148,   167,
     172,   176,   186,   190,   191,   192,   193,   194,   195,   196,
     200,   200,   200,   200,   204,   208,   214,   221,   231,   245,
     254,   266,   281,   290,   296,   302,   311,   317,   326,   330,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INCLUDE_STDIO", "MAIN", "RETURN",
  "PRINTF", "INT", "FLOAT", "CHAR", "BOOL", "IF", "ELSE", "WHILE", "PRINT",
  "TRUE", "FALSE", "IDENTIFIER", "STRING_LITERAL", "INT_LITERAL",
  "FLOAT_LITERAL", "ASSIGN", "PLUS", "MINUS", "MULT", "DIV", "MOD", "EQ",
  "NEQ", "LT", "GT", "LE", "GE", "AND", "OR", "NOT", "SEMICOLON", "LBRACE",
  "RBRACE", "LPAREN", "RPAREN", "COMMA", "$accept", "program", "includes",
  "function_list_all", "function_def", "return_stmt", "statement_list",
  "statement", "type_spec", "declaration_stmt", "identifier_list",
  "assignment_stmt", "if_stmt", "while_stmt", "printf_stmt",
  "function_call", "expression_list", "block_stmt", "expression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    43,    44,    45,    45,    46,    46,    47,
      47,    48,    48,    49,    49,    49,    49,    49,    49,    49,
      50,    50,    50,    50,    51,    52,    52,    52,    52,    53,
      54,    54,    55,    56,    56,    56,    57,    57,    58,    59,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     1,    10,     8,     3,
       0,     2,     0,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     3,     1,     3,     3,     5,     4,
       7,     5,     5,     5,     7,     5,     4,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     0,     0,     0,     3,     6,     0,     1,     2,
       5,     0,     0,     0,     0,    12,     0,    10,    12,     0,
       0,    20,    21,    22,    23,     0,     0,     0,    12,     0,
      11,     0,    13,    14,    15,    16,    17,     0,    18,    10,
      52,    53,    54,    51,     0,     0,     0,     0,     0,     0,
       0,     8,    25,     0,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     9,     0,     0,
       0,     0,     0,    37,     0,    39,     0,    24,     0,     7,
      40,    41,    42,    43,    44,    45,    46,    48,    47,    50,
      49,     0,     0,     0,     0,     0,    29,    36,    26,    27,
      33,     0,    38,    35,    31,    32,     0,     0,     0,    28,
      34,    30
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    29,    17,    30,    31,    32,
      53,    33,    34,    35,    36,    43,   101,    38,    44
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -93
static const yytype_int16 yypact[] =
{
       6,   -93,    -7,    15,    11,    11,   -93,   -19,   -93,    11,
     -93,     5,    14,    -5,    20,   -93,     9,    42,   -93,    83,
      28,   -93,   -93,   -93,   -93,    30,    41,   -20,   -93,    33,
     -93,    64,   -93,   -93,   -93,   -93,   -93,    46,   -93,    42,
      44,   -93,   -93,   -93,   161,    56,    83,    83,    83,   -12,
      55,   -93,    69,   -30,   -93,    66,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,   -93,   -11,    84,
     104,   123,   176,   -93,   142,   -93,    83,   -93,   100,   -93,
      32,    32,   -93,   -93,   -93,   202,   202,    63,    63,    63,
      63,    82,    83,    85,    88,    88,   -93,   -93,   191,    98,
     -93,    80,   191,   -93,   110,   -93,    83,    87,    88,   191,
     -93,   -93
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -93,   -93,   -93,   133,    12,    99,   -14,   -92,   -93,   -93,
     -93,   -93,   -93,   -93,   -93,   -17,   -93,   -93,   -22
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      37,    48,   104,   105,    39,    40,    77,    41,    42,     1,
       7,    78,    12,     2,    50,     8,   111,    10,     2,    49,
      11,    10,    37,    69,    70,    71,    72,    74,    73,    91,
      92,    14,    15,    37,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    13,    18,    19,    20,    21,
      22,    23,    24,    25,    98,    26,    58,    59,    60,    27,
      16,    20,    21,    22,    23,    24,    25,    45,    26,    46,
     102,    51,    27,    40,    68,    41,    42,    37,    37,    28,
      47,    52,    54,    49,   109,    56,    57,    58,    59,    60,
      76,    37,    28,    75,    20,    21,    22,    23,    24,    25,
      40,    26,    41,    42,    79,    27,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    99,   100,   106,
     107,   103,   108,   110,    93,    28,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,     9,    55,     0,
       0,     0,     0,     0,    94,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,     0,     0,     0,     0,
       0,     0,     0,    95,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,     0,     0,     0,     0,     0,
       0,     0,    97,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,     0,     0,     0,    67,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,     0,
       0,     0,    96,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    56,    57,    58,    59,    60,     0,
       0,    63,    64,    65,    66
};

static const yytype_int8 yycheck[] =
{
      17,    21,    94,    95,    18,    17,    36,    19,    20,     3,
      17,    41,     7,     7,    28,     0,   108,     5,     7,    39,
      39,     9,    39,    45,    46,    47,    48,    49,    40,    40,
      41,    17,    37,    50,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    40,    37,     5,     6,     7,
       8,     9,    10,    11,    76,    13,    24,    25,    26,    17,
      40,     6,     7,     8,     9,    10,    11,    39,    13,    39,
      92,    38,    17,    17,    18,    19,    20,    94,    95,    37,
      39,    17,    36,    39,   106,    22,    23,    24,    25,    26,
      21,   108,    37,    38,     6,     7,     8,     9,    10,    11,
      17,    13,    19,    20,    38,    17,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    17,    36,    21,
      40,    36,    12,    36,    40,    37,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,     4,    39,    -1,
      -1,    -1,    -1,    -1,    40,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    36,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    36,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     7,    43,    44,    45,    46,    17,     0,    45,
      46,    39,     7,    40,    17,    37,    40,    48,    37,     5,
       6,     7,     8,     9,    10,    11,    13,    17,    37,    47,
      49,    50,    51,    53,    54,    55,    56,    57,    59,    48,
      17,    19,    20,    57,    60,    39,    39,    39,    21,    39,
      48,    38,    17,    52,    36,    47,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    36,    18,    60,
      60,    60,    60,    40,    60,    38,    21,    36,    41,    38,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    40,    41,    40,    40,    40,    36,    40,    60,    17,
      36,    58,    60,    36,    49,    49,    21,    40,    12,    60,
      36,    49
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1464 of yacc.c  */
#line 107 "src/parser/parser.y"
    {
        Function *main_func = get_function_obj("main");
        if (main_func != NULL && main_func->body != NULL) {
            execute_ast(main_func->body);
        }
    }
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 113 "src/parser/parser.y"
    {
        Function *main_func = get_function_obj("main");
        if (main_func != NULL && main_func->body != NULL) {
            execute_ast(main_func->body);
        }
    }
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 122 "src/parser/parser.y"
    { (yyval.node) = NULL; }
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 126 "src/parser/parser.y"
    { (yyval.node) = NULL; }
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 127 "src/parser/parser.y"
    { (yyval.node) = NULL; }
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 131 "src/parser/parser.y"
    {
        init_symbol_table(); 
        check_declaration((yyvsp[(5) - (10)].str_val), "int", 0);
        
        ASTNode *body = (yyvsp[(8) - (10)].node);
        ASTNode *ret = (yyvsp[(9) - (10)].node);
        if (ret != NULL) {
            if (body == NULL) body = ret;
            else {
                ASTNode *curr = body;
                while (curr->next != NULL) curr = curr->next;
                curr->next = ret;
            }
        }
        define_function((yyvsp[(2) - (10)].str_val), (yyvsp[(5) - (10)].str_val), body);
        (yyval.node) = NULL;
    }
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 148 "src/parser/parser.y"
    {
        init_symbol_table();
        
        ASTNode *body = (yyvsp[(6) - (8)].node);
        ASTNode *ret = (yyvsp[(7) - (8)].node);
        if (ret != NULL) {
            if (body == NULL) body = ret;
            else {
                ASTNode *curr = body;
                while (curr->next != NULL) curr = curr->next;
                curr->next = ret;
            }
        }
        define_function((yyvsp[(2) - (8)].str_val), NULL, body);
        (yyval.node) = NULL;
    }
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 167 "src/parser/parser.y"
    {
        ASTNode *n = create_node(NODE_RETURN);
        n->left = (yyvsp[(2) - (3)].node);
        (yyval.node) = n;
    }
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 172 "src/parser/parser.y"
    { (yyval.node) = NULL; }
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 176 "src/parser/parser.y"
    {
        if ((yyvsp[(1) - (2)].node) == NULL) {
            (yyval.node) = (yyvsp[(2) - (2)].node);
        } else {
            ASTNode *curr = (yyvsp[(1) - (2)].node);
            while (curr->next != NULL) curr = curr->next;
            curr->next = (yyvsp[(2) - (2)].node);
            (yyval.node) = (yyvsp[(1) - (2)].node);
        }
    }
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 186 "src/parser/parser.y"
    { (yyval.node) = NULL; }
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 196 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); }
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 204 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); }
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 208 "src/parser/parser.y"
    { 
        check_declaration((yyvsp[(1) - (1)].str_val), "int", 0);
        ASTNode *n = create_node(NODE_DECL);
        n->str_val = (yyvsp[(1) - (1)].str_val);
        (yyval.node) = n;
    }
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 214 "src/parser/parser.y"
    { 
        check_declaration((yyvsp[(1) - (3)].str_val), "int", 0);
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = (yyvsp[(1) - (3)].str_val);
        n->left = (yyvsp[(3) - (3)].node);
        (yyval.node) = n;
    }
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 221 "src/parser/parser.y"
    { 
        check_declaration((yyvsp[(3) - (3)].str_val), "int", 0);
        ASTNode *n = create_node(NODE_DECL);
        n->str_val = (yyvsp[(3) - (3)].str_val);
        
        ASTNode *curr = (yyvsp[(1) - (3)].node);
        while (curr->next != NULL) curr = curr->next;
        curr->next = n;
        (yyval.node) = (yyvsp[(1) - (3)].node);
    }
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 231 "src/parser/parser.y"
    { 
        check_declaration((yyvsp[(3) - (5)].str_val), "int", 0);
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = (yyvsp[(3) - (5)].str_val);
        n->left = (yyvsp[(5) - (5)].node);
        
        ASTNode *curr = (yyvsp[(1) - (5)].node);
        while (curr->next != NULL) curr = curr->next;
        curr->next = n;
        (yyval.node) = (yyvsp[(1) - (5)].node);
    }
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 245 "src/parser/parser.y"
    { 
        ASTNode *n = create_node(NODE_ASSIGN);
        n->str_val = (yyvsp[(1) - (4)].str_val);
        n->left = (yyvsp[(3) - (4)].node);
        (yyval.node) = n;
    }
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 254 "src/parser/parser.y"
    {
        ASTNode *n = create_node(NODE_IF);
        n->left = (yyvsp[(3) - (7)].node);
        
        ASTNode *branches = create_node(NODE_IF); 
        branches->left = (yyvsp[(5) - (7)].node);
        branches->right = (yyvsp[(7) - (7)].node);
        
        n->right = branches; 
        n->next = NULL; 
        (yyval.node) = n;
    }
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 266 "src/parser/parser.y"
    {
        ASTNode *n = create_node(NODE_IF);
        n->left = (yyvsp[(3) - (5)].node);
        
        ASTNode *branches = create_node(NODE_IF);
        branches->left = (yyvsp[(5) - (5)].node);
        branches->right = NULL; 
        
        n->right = branches;
        n->next = NULL;
        (yyval.node) = n;
    }
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 281 "src/parser/parser.y"
    {
        ASTNode *n = create_node(NODE_WHILE);
        n->left = (yyvsp[(3) - (5)].node);
        n->right = (yyvsp[(5) - (5)].node);
        (yyval.node) = n;
    }
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 290 "src/parser/parser.y"
    {
        ASTNode *n = create_node(NODE_PRINTF);
        n->str_val = (yyvsp[(3) - (5)].str_val);
        n->left = NULL;
        (yyval.node) = n;
    }
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 296 "src/parser/parser.y"
    {
        ASTNode *n = create_node(NODE_PRINTF);
        n->str_val = (yyvsp[(3) - (7)].str_val);
        n->left = (yyvsp[(5) - (7)].node);
        (yyval.node) = n;
    }
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 302 "src/parser/parser.y"
    {
        ASTNode *n = create_node(NODE_PRINTF);
        n->str_val = NULL; 
        n->left = (yyvsp[(3) - (5)].node);
        (yyval.node) = n;
    }
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 311 "src/parser/parser.y"
    {
        ASTNode *n = create_node(NODE_FUNC_CALL);
        n->str_val = (yyvsp[(1) - (4)].str_val);
        n->left = (yyvsp[(3) - (4)].node);
        (yyval.node) = n;
    }
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 317 "src/parser/parser.y"
    {
        ASTNode *n = create_node(NODE_FUNC_CALL);
        n->str_val = (yyvsp[(1) - (3)].str_val);
        n->left = NULL;
        (yyval.node) = n;
    }
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 326 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 330 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); }
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 334 "src/parser/parser.y"
    { (yyval.node) = create_binop(PLUS, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 335 "src/parser/parser.y"
    { (yyval.node) = create_binop(MINUS, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 336 "src/parser/parser.y"
    { (yyval.node) = create_binop(MULT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 337 "src/parser/parser.y"
    { (yyval.node) = create_binop(DIV, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 338 "src/parser/parser.y"
    { (yyval.node) = create_binop(MOD, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 339 "src/parser/parser.y"
    { (yyval.node) = create_binop(EQ, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 340 "src/parser/parser.y"
    { (yyval.node) = create_binop(NEQ, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 341 "src/parser/parser.y"
    { (yyval.node) = create_binop(GT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 342 "src/parser/parser.y"
    { (yyval.node) = create_binop(LT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 343 "src/parser/parser.y"
    { (yyval.node) = create_binop(GE, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 344 "src/parser/parser.y"
    { (yyval.node) = create_binop(LE, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 345 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 346 "src/parser/parser.y"
    { ASTNode *n = create_node(NODE_VAR); n->str_val = (yyvsp[(1) - (1)].str_val); (yyval.node) = n; }
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 347 "src/parser/parser.y"
    { ASTNode *n = create_node(NODE_NUM); n->int_val = (yyvsp[(1) - (1)].int_val); (yyval.node) = n; }
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 348 "src/parser/parser.y"
    { ASTNode *n = create_node(NODE_NUM); n->int_val = (int)(yyvsp[(1) - (1)].float_val); (yyval.node) = n; }
    break;



/* Line 1464 of yacc.c  */
#line 2053 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1684 of yacc.c  */
#line 351 "src/parser/parser.y"


int eval_ast(ASTNode *node) {
    if (!node) return 0;
    if (node->type == NODE_NUM) return node->int_val;
    if (node->type == NODE_VAR) return get_variable_value(node->str_val);
    if (node->type == NODE_FUNC_CALL) {
        Function *f = get_function_obj(node->str_val);
        if (f) {
            if (f->param_name && node->left) {
                int arg_val = eval_ast(node->left);
                set_variable_value(f->param_name, arg_val);
            }
            if (f->body) {
                execute_ast(f->body);
            }
        }
        return 0;
    }
    if (node->type == NODE_BINOP) {
        int l = eval_ast(node->left);
        int r = eval_ast(node->right);
        switch(node->op) {
            case PLUS: return l + r;
            case MINUS: return l - r;
            case MULT: return l * r;
            case DIV: return r != 0 ? l / r : 0;
            case MOD: return r != 0 ? l % r : 0;
            case EQ: return l == r;
            case NEQ: return l != r;
            case GT: return l > r;
            case LT: return l < r;
            case GE: return l >= r;
            case LE: return l <= r;
        }
    }
    return 0;
}

void execute_ast(ASTNode *node) {
    ASTNode *curr = node;
    while (curr != NULL) {
        switch (curr->type) {
            case NODE_DECL:
                set_variable_value(curr->str_val, 0);
                break;
            case NODE_ASSIGN:
                set_variable_value(curr->str_val, eval_ast(curr->left));
                break;
            case NODE_PRINTF: {
                if (curr->str_val == NULL && curr->left != NULL) {
                    printf("%d\n", eval_ast(curr->left));
                } else if (curr->str_val != NULL) {
                    char *fmt = curr->str_val;
                    int len = strlen(fmt);
                    char temp[256];
                    int j = 0;
                    for (int i = 0; i < len; i++) {
                        if (fmt[i] != '"') {
                            temp[j++] = fmt[i];
                        }
                    }
                    temp[j] = '\0';
                    
                    if (curr->left) {
                        int val = eval_ast(curr->left);
                        char buffer[256];
                        snprintf(buffer, sizeof(buffer), temp, val);
                        printf("%s\n", buffer);
                    } else {
                        printf("%s\n", temp);
                    }
                }
                break;
            }
            case NODE_IF: {
                int cond = eval_ast(curr->left);
                ASTNode *branches = curr->right;
                
                if (cond) {
                    if (branches->left != NULL) {
                        execute_ast(branches->left);
                    }
                } else {
                    if (branches->right != NULL) {
                        execute_ast(branches->right);
                    }
                }
                break;
            }
            case NODE_WHILE: {
                while (eval_ast(curr->left)) {
                    execute_ast(curr->right);
                }
                break;
            }
            case NODE_FUNC_CALL: {
                Function *f = get_function_obj(curr->str_val);
                if (f) {
                    if (f->param_name && curr->left) {
                        int arg_val = eval_ast(curr->left);
                        set_variable_value(f->param_name, arg_val);
                    }
                    if (f->body) {
                        execute_ast(f->body);
                    }
                }
                break;
            }
            case NODE_RETURN:
                return;
            default:
                break;
        }
        curr = curr->next;
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", line_num, s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("File opening failed");
            return 1;
        }
        yyin = f;
    }
    yyparse();
    return 0;
}
