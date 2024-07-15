/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include "treeUtils.h"
#include "scanType.h"
#include "semantics.h"
#include "symbolTable.h"
#include "emitcode.h"
#include "codegen.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

int numErrors = 0;
int numWarnings = 0;
extern int line;
extern int yylex();

void yyerror(const char *msg);

SymbolTable *symtab;
//symtab = new SymbolTable();
//symtab->debug(false);

TreeNode *addSibling(TreeNode *t, TreeNode *s)
{
   // make sure s is not null. If it is this, then there is a major error. Exit the program!
   if (s == NULL)
   {
      printf("TreeNode addSibling: TreeNode *s is null! Exiting the program.\n");
      exit(-1);
   }

   // Make sure t is not null. If it is, just return s
   if (t == NULL)
   {
      return s;
   }

   TreeNode *temp = t;
   // Look down t's sibling list until you fin with sibling = null (end of the list) and add s there
   while (temp->sibling != NULL)
   {
      temp = temp->sibling;
   }

   temp->sibling = s;

   return t;
}

void setType(TreeNode *t, ExpType type, bool isStatic)
{
   while (t)
   {
      // Set t->type and t->isStatic
      t->type = type;
      t->isStatic = isStatic;

      // Move to next sibling
      t = t->sibling;
   }
}

void printToken(TokenData myData, string tokenName, int type = 0) {
   cout << "Line: " << myData.linenum << " Type: " << tokenName;
   if(type==0)
     cout << " Token: " << myData.tokenstr;
   if(type==1)
     cout << " Token: " << myData.nvalue;
   if(type==2)
     cout << " Token: " << myData.cvalue;
   cout << endl;
}

TreeNode * syntaxTree;


#line 149 "parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FIRSTOP = 258,
    LASTOP = 259,
    ADDASS = 260,
    AND = 261,
    BOOL = 262,
    BOOLCONST = 263,
    BREAK = 264,
    BY = 265,
    CHAR = 266,
    CHARCONST = 267,
    CHSIGN = 268,
    DEC = 269,
    DIVASS = 270,
    DO = 271,
    ELSE = 272,
    EQ = 273,
    ERROR = 274,
    FOR = 275,
    GEQ = 276,
    ID = 277,
    IF = 278,
    INC = 279,
    INT = 280,
    LEQ = 281,
    MAX = 282,
    MIN = 283,
    MULASS = 284,
    NEQ = 285,
    NOT = 286,
    NUMCONST = 287,
    OR = 288,
    OP = 289,
    PRECOMPILER = 290,
    RETURN = 291,
    SIZEOF = 292,
    STATIC = 293,
    STRINGCONST = 294,
    SUBASS = 295,
    THEN = 296,
    TO = 297,
    WHILE = 298,
    LASTTERM = 299
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 85 "parser.y" /* yacc.c:355  */

   TokenData *tinfo;
   TreeNode *tree;
   ExpType type;  // for passing type spec up the tree

#line 240 "parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 257 "parser.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   267

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  115
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  176

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    19,     2,     2,
      17,    18,     5,     6,    16,     7,     2,     8,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    15,    11,
      13,    14,    12,    20,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     9,     2,    10,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     2,     4,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   189,   189,   192,   193,   194,   197,   198,   201,   202,
     206,   209,   210,   213,   214,   217,   218,   221,   222,   225,
     226,   227,   230,   231,   234,   235,   238,   239,   242,   246,
     247,   250,   251,   254,   255,   258,   259,   260,   261,   262,
     263,   264,   267,   268,   271,   272,   273,   274,   277,   278,
     282,   285,   286,   289,   290,   293,   294,   297,   300,   301,
     302,   303,   304,   307,   308,   309,   310,   311,   314,   315,
     318,   319,   322,   323,   326,   327,   330,   331,   332,   333,
     334,   335,   338,   339,   342,   343,   346,   347,   350,   351,
     354,   355,   358,   359,   360,   363,   364,   367,   368,   369,
     372,   373,   376,   377,   380,   381,   382,   385,   388,   389,
     392,   393,   396,   397,   398,   399
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'{'", "'}'", "'*'", "'+'", "'-'", "'/'",
  "'['", "']'", "';'", "'>'", "'<'", "'='", "':'", "','", "'('", "')'",
  "'%'", "'?'", "FIRSTOP", "LASTOP", "ADDASS", "AND", "BOOL", "BOOLCONST",
  "BREAK", "BY", "CHAR", "CHARCONST", "CHSIGN", "DEC", "DIVASS", "DO",
  "ELSE", "EQ", "ERROR", "FOR", "GEQ", "ID", "IF", "INC", "INT", "LEQ",
  "MAX", "MIN", "MULASS", "NEQ", "NOT", "NUMCONST", "OR", "OP",
  "PRECOMPILER", "RETURN", "SIZEOF", "STATIC", "STRINGCONST", "SUBASS",
  "THEN", "TO", "WHILE", "LASTTERM", "$accept", "program", "precomList",
  "declList", "decl", "varDecl", "scopedVarDecl", "varDeclList",
  "varDeclInit", "varDeclId", "typeSpec", "funDecl", "parms", "parmList",
  "parmTypeList", "parmIdList", "parmId", "stmt", "matched", "iterRange",
  "unmatched", "expstmt", "compoundstmt", "localDecls", "stmtList",
  "returnstmt", "breakstmt", "exp", "assignop", "simpleExp", "andExp",
  "unaryRelExp", "relExp", "relop", "minmaxExp", "minmaxop", "sumExp",
  "sumop", "mulExp", "mulop", "unaryExp", "unaryop", "factor", "mutable",
  "immutable", "call", "args", "argList", "constant", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   123,   125,    42,    43,    45,    47,    91,
      93,    59,    62,    60,    61,    58,    44,    40,    41,    37,
      63,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299
};
# endif

#define YYPACT_NINF -134

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-134)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -43,  -134,    22,    41,  -134,  -134,  -134,     9,  -134,  -134,
      25,  -134,  -134,   -12,  -134,     2,  -134,     3,    72,  -134,
      26,    33,    59,    69,  -134,    39,     2,  -134,    55,   192,
      88,    83,  -134,   116,     2,   102,    97,   108,  -134,  -134,
    -134,   192,  -134,  -134,  -134,    34,   192,  -134,  -134,    71,
     100,  -134,  -134,   207,    98,    66,  -134,   210,  -134,  -134,
    -134,  -134,  -134,   118,    33,  -134,  -134,   115,    89,   192,
     176,   192,  -134,  -134,  -134,  -134,  -134,  -134,  -134,   119,
      71,    -9,  -134,  -134,   116,   113,   192,   192,  -134,   192,
     192,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,   210,
     210,  -134,  -134,   210,  -134,  -134,  -134,   210,  -134,  -134,
    -134,    -8,  -134,   120,    16,  -134,   121,   -15,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,   154,  -134,  -134,   125,
    -134,   122,   128,   100,  -134,    62,    98,    66,  -134,     2,
    -134,    55,    52,   192,   116,  -134,   116,  -134,  -134,  -134,
    -134,   192,    55,    80,  -134,  -134,   114,   -35,  -134,   112,
    -134,  -134,  -134,    87,  -134,   116,   192,   116,  -134,  -134,
    -134,   -22,  -134,  -134,   192,    71
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     4,     0,     0,     1,    20,    21,     0,    19,     3,
       2,     7,     8,     0,     9,    25,     6,    17,     0,    14,
      15,     0,     0,    24,    27,     0,    25,    10,     0,     0,
      31,    28,    30,     0,     0,     0,     0,    17,    13,    98,
      97,     0,    99,   115,   113,   102,     0,   112,   114,    16,
      69,    71,    73,    75,    83,    87,    91,     0,    96,   101,
     100,   105,   106,     0,     0,    52,    49,     0,     0,     0,
       0,     0,    23,    33,    34,    38,    39,    40,    41,     0,
      61,   101,    26,    18,     0,     0,     0,   109,    72,     0,
       0,    78,    77,    80,    79,    76,    84,    85,    81,     0,
       0,    88,    89,     0,    92,    93,    94,     0,    95,    32,
      29,    54,    57,     0,     0,    55,     0,     0,    48,    63,
      64,    60,    67,    59,    66,    65,     0,    22,   104,     0,
     111,     0,   108,    68,    70,    74,    82,    86,    90,     0,
      51,     0,     0,     0,     0,    56,     0,    62,    58,   103,
     107,     0,     0,     0,    50,    53,     0,     0,    44,    33,
      36,    46,   110,     0,    12,     0,     0,     0,    11,    37,
      47,    42,    35,    45,     0,    43
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -134,  -134,  -134,  -134,   139,  -134,  -134,   -65,   123,  -134,
      -1,  -134,   124,  -134,   126,  -134,    94,   -80,  -133,  -134,
    -128,  -134,  -134,  -134,  -134,  -134,  -134,   -40,  -134,   -29,
      63,   -38,  -134,  -134,    54,  -134,    64,  -134,    60,  -134,
     -54,  -134,  -134,   -26,  -134,  -134,  -134,  -134,  -134
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    10,    11,    12,   140,    18,    19,    20,
      21,    14,    22,    23,    24,    31,    32,    72,    73,   156,
      74,    75,    76,   111,   142,    77,    78,    79,   126,    80,
      50,    51,    52,    99,    53,   100,    54,   103,    55,   107,
      56,    57,    58,    59,    60,    61,   131,   132,    62
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      49,    85,    13,   108,   127,   119,   174,    81,    88,    13,
       1,   159,    25,   160,   120,    81,    89,     5,   161,   146,
      26,     6,     4,   121,   122,   166,    15,     5,    17,    89,
     116,     6,   169,   123,   172,     8,    89,   170,   124,   173,
     114,    29,   117,    86,    81,     8,   129,   130,   139,   125,
       5,    87,   134,   138,     6,    65,   154,    39,    81,    40,
      81,    81,   155,    66,   158,     7,     5,    89,     8,    41,
       6,   104,    42,    30,   105,   144,   153,    33,    43,    67,
      34,     7,    44,    27,     8,   106,   148,   163,    28,    35,
      68,   164,    45,    69,     9,    37,    28,    63,   168,    64,
      81,    46,    47,    28,   101,   102,    70,    96,    97,    48,
     141,   162,    83,    71,   157,    84,    81,    25,    81,    65,
      81,    39,    89,    40,    90,    81,   112,    66,   109,   113,
     118,   128,   145,    41,   143,   149,    42,   171,   152,    81,
     150,    81,    43,    67,   151,   175,    44,   167,   165,    16,
      36,    38,   133,   135,    68,   147,    45,    69,   110,    39,
      82,    40,     0,   137,   136,    46,    47,     0,     0,     0,
      70,    41,     0,    48,    42,     0,     0,    71,     0,     0,
      43,    39,     0,    40,    44,     0,     0,   115,     0,     0,
       0,     0,     0,    41,    45,     0,    42,    39,     0,    40,
       0,     0,    43,    46,    47,     0,    44,     0,     0,    41,
       0,    48,    42,     0,     0,    39,    45,    40,    43,    91,
      92,     0,    44,     0,     0,    46,    47,    41,     0,     0,
      42,     0,    45,    48,     0,     0,    43,     0,     0,     0,
      44,    46,    47,    93,     0,     0,    94,     0,     0,    48,
      45,    95,    96,    97,     0,    98,     0,     0,     0,     0,
      47,     0,     0,     0,     0,     0,     0,    48
};

static const yytype_int16 yycheck[] =
{
      29,    41,     3,    57,    84,    14,    28,    33,    46,    10,
      53,   144,     9,   146,    23,    41,    51,    25,   146,    34,
      17,    29,     0,    32,    33,    60,    17,    25,    40,    51,
      70,    29,   165,    42,   167,    43,    51,   165,    47,   167,
      69,    15,    71,     9,    70,    43,    86,    87,    56,    58,
      25,    17,    90,   107,    29,     3,     4,     5,    84,     7,
      86,    87,   142,    11,   144,    40,    25,    51,    43,    17,
      29,     5,    20,    40,     8,    59,   141,    18,    26,    27,
      11,    40,    30,    11,    43,    19,   126,   152,    16,    50,
      38,    11,    40,    41,    53,    40,    16,     9,    11,    16,
     126,    49,    50,    16,     6,     7,    54,    45,    46,    57,
     111,   151,    10,    61,   143,    18,   142,     9,   144,     3,
     146,     5,    51,     7,    24,   151,    11,    11,    10,    40,
      11,    18,    11,    17,    14,    10,    20,   166,   139,   165,
      18,   167,    26,    27,    16,   174,    30,    35,    34,    10,
      26,    28,    89,    99,    38,     1,    40,    41,    64,     5,
      34,     7,    -1,   103,   100,    49,    50,    -1,    -1,    -1,
      54,    17,    -1,    57,    20,    -1,    -1,    61,    -1,    -1,
      26,     5,    -1,     7,    30,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    17,    40,    -1,    20,     5,    -1,     7,
      -1,    -1,    26,    49,    50,    -1,    30,    -1,    -1,    17,
      -1,    57,    20,    -1,    -1,     5,    40,     7,    26,    12,
      13,    -1,    30,    -1,    -1,    49,    50,    17,    -1,    -1,
      20,    -1,    40,    57,    -1,    -1,    26,    -1,    -1,    -1,
      30,    49,    50,    36,    -1,    -1,    39,    -1,    -1,    57,
      40,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    57
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    53,    64,    65,     0,    25,    29,    40,    43,    53,
      66,    67,    68,    73,    74,    17,    67,    40,    70,    71,
      72,    73,    75,    76,    77,     9,    17,    11,    16,    15,
      40,    78,    79,    18,    11,    50,    75,    40,    71,     5,
       7,    17,    20,    26,    30,    40,    49,    50,    57,    92,
      93,    94,    95,    97,    99,   101,   103,   104,   105,   106,
     107,   108,   111,     9,    16,     3,    11,    27,    38,    41,
      54,    61,    80,    81,    83,    84,    85,    88,    89,    90,
      92,   106,    77,    10,    18,    90,     9,    17,    94,    51,
      24,    12,    13,    36,    39,    44,    45,    46,    48,    96,
      98,     6,     7,   100,     5,     8,    19,   102,   103,    10,
      79,    86,    11,    40,    92,    11,    90,    92,    11,    14,
      23,    32,    33,    42,    47,    58,    91,    80,    18,    90,
      90,   109,   110,    93,    94,    97,    99,   101,   103,    56,
      69,    73,    87,    14,    59,    11,    34,     1,    90,    10,
      18,    16,    73,    70,     4,    80,    82,    92,    80,    81,
      81,    83,    90,    70,    11,    34,    60,    35,    11,    81,
      83,    92,    81,    83,    28,    92
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    63,    64,    65,    65,    65,    66,    66,    67,    67,
      68,    69,    69,    70,    70,    71,    71,    72,    72,    73,
      73,    73,    74,    74,    75,    75,    76,    76,    77,    78,
      78,    79,    79,    80,    80,    81,    81,    81,    81,    81,
      81,    81,    82,    82,    83,    83,    83,    83,    84,    84,
      85,    86,    86,    87,    87,    88,    88,    89,    90,    90,
      90,    90,    90,    91,    91,    91,    91,    91,    92,    92,
      93,    93,    94,    94,    95,    95,    96,    96,    96,    96,
      96,    96,    97,    97,    98,    98,    99,    99,   100,   100,
     101,   101,   102,   102,   102,   103,   103,   104,   104,   104,
     105,   105,   106,   106,   107,   107,   107,   108,   109,   109,
     110,   110,   111,   111,   111,   111
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     0,     2,     1,     1,     1,
       3,     4,     3,     3,     1,     1,     3,     1,     4,     1,
       1,     1,     6,     5,     1,     0,     3,     1,     2,     3,
       1,     1,     3,     1,     1,     6,     4,     6,     1,     1,
       1,     1,     3,     5,     4,     6,     4,     6,     2,     1,
       4,     2,     0,     2,     0,     2,     3,     2,     3,     2,
       2,     1,     3,     1,     1,     1,     1,     1,     3,     1,
       3,     1,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     1,     1,     4,     1,     0,
       3,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 189 "parser.y" /* yacc.c:1646  */
    {syntaxTree=(yyvsp[0].tree);}
#line 1490 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 192 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL; printf("%s\n", yylval.tinfo->tokenstr);}
#line 1496 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 193 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL; printf("%s\n", yylval.tinfo->tokenstr);/*printf("%s\n");*/}
#line 1502 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 194 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL;}
#line 1508 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 197 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = addSibling((yyvsp[-1].tree), (yyvsp[0].tree));}
#line 1514 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 198 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1520 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 201 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1526 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 202 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1532 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 206 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[-1].tree); setType((yyvsp[-1].tree), (yyvsp[-2].type), false); yyerrok;}
#line 1538 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 209 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[-1].tree); setType((yyvsp[-1].tree), (yyvsp[-2].type), true); yyerrok;}
#line 1544 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 210 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[-1].tree); setType((yyvsp[-1].tree), (yyvsp[-2].type), false); yyerrok;}
#line 1550 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 213 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = addSibling((yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1556 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 214 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1562 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 217 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1568 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 218 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[-2].tree); (yyval.tree)->child[0] = (yyvsp[0].tree); setType((yyval.tree), (yyvsp[0].tree)->type, false);}
#line 1574 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 221 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newDeclNode(DeclKind::VarK, ExpType::UndefinedType, (yyvsp[0].tinfo));}
#line 1580 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 222 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newDeclNode(DeclKind::VarK, ExpType::UndefinedType, (yyvsp[-3].tinfo)); (yyval.tree)->isArray = true; (yyval.tree)->size = (yyvsp[-1].tinfo)->nvalue + 1;}
#line 1586 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 225 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = ExpType::Integer;}
#line 1592 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 226 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = ExpType::Boolean;}
#line 1598 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 227 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = ExpType::Char;}
#line 1604 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 230 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newDeclNode(DeclKind::FuncK, (yyvsp[-5].type), (yyvsp[-4].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree)); setType((yyvsp[0].tree), (yyvsp[-5].type), false);}
#line 1610 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 231 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newDeclNode(DeclKind::FuncK, ExpType::Void, (yyvsp[-4].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1616 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 234 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1622 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 235 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL;}
#line 1628 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 238 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = addSibling((yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1634 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 239 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1640 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 242 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree); setType((yyvsp[0].tree), (yyvsp[-1].type), false); yyerrok;}
#line 1646 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 246 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = addSibling((yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1652 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 247 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1658 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 250 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newDeclNode(DeclKind::ParamK, ExpType::UndefinedType, (yyvsp[0].tinfo));}
#line 1664 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 251 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newDeclNode(DeclKind::ParamK, ExpType::UndefinedType, (yyvsp[-2].tinfo)); (yyval.tree)->isArray = true;}
#line 1670 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 254 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1676 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 255 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1682 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 258 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(IfK, (yyvsp[-5].tinfo), (yyvsp[-4].tree), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1688 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 259 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(WhileK, (yyvsp[-3].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1694 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 260 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(StmtKind::ForK, (yyvsp[-5].tinfo), newDeclNode(DeclKind::VarK, ExpType::Integer, (yyvsp[-4].tinfo)), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1700 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 261 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1706 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 262 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1712 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 263 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1718 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 264 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1724 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 267 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(StmtKind::RangeK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1730 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 268 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(StmtKind::RangeK, (yyvsp[-3].tinfo), (yyvsp[-4].tree), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1736 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 271 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(IfK, (yyvsp[-3].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1742 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 272 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(IfK, (yyvsp[-5].tinfo), (yyvsp[-4].tree), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1748 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 273 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(WhileK, (yyvsp[-3].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1754 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 274 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(StmtKind::ForK, (yyvsp[-5].tinfo), newDeclNode(DeclKind::VarK, ExpType::Integer, (yyvsp[-4].tinfo)), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1760 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 277 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[-1].tree);}
#line 1766 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 278 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL;}
#line 1772 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 282 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(CompoundK, (yyvsp[-3].tinfo), (yyvsp[-2].tree), (yyvsp[-1].tree)); yyerrok;}
#line 1778 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 285 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = addSibling((yyvsp[-1].tree), (yyvsp[0].tree));}
#line 1784 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 286 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL;}
#line 1790 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 289 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = ((yyvsp[0].tree) == NULL ? (yyvsp[-1].tree) : addSibling((yyvsp[-1].tree), (yyvsp[0].tree)));}
#line 1796 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 290 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL;}
#line 1802 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 293 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(StmtKind::ReturnK, (yyvsp[-1].tinfo));}
#line 1808 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 294 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(StmtKind::ReturnK, (yyvsp[-2].tinfo), (yyvsp[-1].tree));}
#line 1814 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 297 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newStmtNode(StmtKind::BreakK, (yyvsp[-1].tinfo));}
#line 1820 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 300 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::AssignK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1826 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 301 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::AssignK, (yyvsp[0].tinfo), (yyvsp[-1].tree));}
#line 1832 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 302 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::AssignK, (yyvsp[0].tinfo), (yyvsp[-1].tree));}
#line 1838 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 303 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1844 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 304 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL;}
#line 1850 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 307 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1856 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 308 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1862 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 309 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1868 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 310 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1874 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 311 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1880 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 314 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::OpK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1886 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 315 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1892 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 318 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::OpK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1898 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 319 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1904 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 322 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::OpK, (yyvsp[-1].tinfo), (yyvsp[0].tree));}
#line 1910 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 323 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1916 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 326 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::OpK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1922 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 327 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1928 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 330 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1934 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 331 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1940 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 332 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1946 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 333 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1952 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 334 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1958 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 335 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1964 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 338 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::OpK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1970 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 339 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1976 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 342 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1982 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 343 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 1988 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 346 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::OpK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1994 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 347 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 2000 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 350 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 2006 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 351 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 2012 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 354 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::OpK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 2018 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 355 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 2024 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 358 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 2030 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 359 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 2036 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 360 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 2042 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 363 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::OpK, (yyvsp[-1].tinfo), (yyvsp[0].tree));}
#line 2048 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 364 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 2054 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 367 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo); (yyval.tinfo)->tokenclass = CHSIGN;}
#line 2060 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 368 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo); (yyval.tinfo)->tokenclass = SIZEOF;}
#line 2066 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 369 "parser.y" /* yacc.c:1646  */
    {(yyval.tinfo) = (yyvsp[0].tinfo);}
#line 2072 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 372 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 2078 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 373 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 2084 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 376 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::IdK, (yyvsp[0].tinfo)); (yyval.tree)->attr.name = (yyvsp[0].tinfo)->svalue;}
#line 2090 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 377 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::OpK, (yyvsp[-2].tinfo), newExpNode(ExpKind::IdK, (yyvsp[-3].tinfo)), (yyvsp[-1].tree)); (yyval.tree)->child[0]->attr.name = (yyvsp[-3].tinfo)->svalue; (yyval.tree)->size = (yyvsp[-1].tree)->size;}
#line 2096 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 380 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[-1].tree);}
#line 2102 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 381 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 2108 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 382 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 2114 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 385 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::CallK, (yyvsp[-3].tinfo), (yyvsp[-1].tree));}
#line 2120 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 388 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 2126 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 389 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL;}
#line 2132 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 392 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = addSibling((yyvsp[-2].tree), (yyvsp[0].tree));}
#line 2138 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 393 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 2144 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 396 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::ConstantK, (yyvsp[0].tinfo)); (yyval.tree)->type = ExpType::Integer; (yyval.tree)->attr.value = (yyvsp[0].tinfo)->nvalue;}
#line 2150 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 397 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::ConstantK, (yyvsp[0].tinfo)); (yyval.tree)->type = ExpType::Char; (yyval.tree)->attr.cvalue = (yyvsp[0].tinfo)->cvalue; (yyval.tree)->size = 1; (yyval.tree)->attr.string = (yyvsp[0].tinfo)->svalue;}
#line 2156 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 398 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::ConstantK, (yyvsp[0].tinfo)); (yyval.tree)->type = ExpType::Char; (yyval.tree)->attr.string = (yyvsp[0].tinfo)->svalue; (yyval.tree)->isArray = true; (yyval.tree)->size = (yyvsp[0].tinfo)->nvalue + 1;}
#line 2162 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 399 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(ExpKind::ConstantK, (yyvsp[0].tinfo)); (yyval.tree)->type = ExpType::Boolean; (yyval.tree)->size = 1; (yyval.tree)->attr.value = (yyvsp[0].tinfo)->nvalue;}
#line 2168 "parser.tab.c" /* yacc.c:1646  */
    break;


#line 2172 "parser.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 402 "parser.y" /* yacc.c:1906  */


void yyerror (const char *msg)
{ 
   cout << "Error: " <<  msg << endl;
}

char *largerTokens[LASTTERM+1];

void initTokenStrings()
{
   largerTokens[OP] = (char *)"op";
   largerTokens[EQ] = (char *)"==";
   largerTokens[NEQ] = (char *)"!=";
   largerTokens[LEQ] = (char *)"<=";
   largerTokens[GEQ] = (char *)">=";
   largerTokens[MULASS] = (char *)"*=";
   largerTokens[ADDASS] = (char *)"+=";
   largerTokens[SUBASS] = (char *)"-=";
   largerTokens[DIVASS] = (char *)"/=";
   largerTokens[INC] = (char *)"++";
   largerTokens[DEC] = (char *)"--";
   largerTokens[MAX] = (char *)":>:";
   largerTokens[MIN] = (char *)":<:";
   largerTokens[AND] = (char *)"and";
   largerTokens[CHAR] = (char *)"char";
   largerTokens[BOOL] = (char *)"bool";
   largerTokens[BREAK] = (char *)"break";
   largerTokens[BY] = (char *)"by";
   largerTokens[DO] = (char *)"do";
   largerTokens[ELSE] = (char *)"else";
   largerTokens[FOR] = (char *)"for";
   largerTokens[IF] = (char *)"if";
   largerTokens[INT] = (char *)"int";
   largerTokens[NOT] = (char *)"not";
   largerTokens[OR] = (char *)"or";
   largerTokens[RETURN] = (char *)"return";
   largerTokens[STATIC] = (char *)"static";
   largerTokens[THEN] = (char *)"then";
   largerTokens[TO] = (char *)"to";
   largerTokens[WHILE] = (char *)"while";
   largerTokens[PRECOMPILER] = (char *)"precompiler";
   largerTokens[NUMCONST] = (char *)"numconst";
   largerTokens[STRINGCONST] = (char *)"stringconst";
   largerTokens[ERROR] = (char *)"error";
   largerTokens[ID] = (char *)"id";
}

char *tokenToStr(int type)
{
   switch(type)
   {
      case '=':
         return (char *)"=";
         break;
      case '-':
         return (char *)"-";
         break;
      case '*':
         return (char *)"*";
         break;
      case ADDASS:
         return (char *)"+=";
         break;
      case SUBASS:
         return (char *)"-=";
         break;
      case MULASS:
         return (char *)"*=";
         break;
      case DIVASS:
         return (char *)"/=";
         break;
      case OR:
         return (char *)"or";
         break;
      case GEQ:
         return (char *)">=";
         break;
      case LEQ:
         return (char *)"<=";
         break;
      case EQ:
         return (char *)"==";
         break;
      case INC:
         return (char *)"++";
         break;
      case DEC:
         return (char *)"--";
         break;
      case MIN:
         return (char *)":<:";
         break;
      case MAX:
         return (char *)":>:";
         break;
      case NEQ:
         return (char *)"!=";
         break;
      case NOT:
         return (char *)"not";
         break;
      case AND:
         return (char *)"and";
         break;
      case CHSIGN:
         return (char *)"chsign";
         break;
      case SIZEOF:
         return (char *)"sizeof";
         break;
      default:
         if(int(type) < 256)
         {
            char tmp[2];
            tmp[0] = int(type);
            tmp[1] = 0;
            return strdup(tmp);
         }
         return largerTokens[type];
   }
}

int main(int argc, char **argv) {
   yylval.tinfo = (TokenData*)malloc(sizeof(TokenData));
   yylval.tree = (TreeNode*)malloc(sizeof(TreeNode));

   yylval.tinfo->linenum = 1;

   int option, index;
   int globalOffset = 0;
   char *file = NULL;
   bool dotAST = false;
   extern FILE *yyin;

   //symtab = new SymbolTable();
   //symtab->debug(false);
   //syntaxTree = semanticAnalysis(syntaxTree, shareCompoundSpace, noDuplicateUndefs, symtab, globalOffset);

   while ((option = getopt (argc, argv, "d")) != -1)
   {
      switch (option)
      {
      case 'd':
         dotAST = true;
         break;
      case '?':
      default:
         ;
      }
   }

   if ( optind == argc ) yyparse();
   for (index = optind; index < argc; index++) 
   {
      yyin = fopen (argv[index], "r");
      yyparse();
      fclose (yyin);
   }

   //syntaxTree = semanticAnalysis(syntaxTree, true, true, symtab, globalOffset);
   //codegen(stdout, argv[1], syntaxTree, symtab, globalOffset, false);
   // SymbolTable *symtab;
   // symtab = new SymbolTable();
   // symtab->debug(debugSymTab);

   if (numErrors == 0)
   {
      SymbolTable *symtab;
      symtab = new SymbolTable();
      symtab->debug(false);
      syntaxTree = semanticAnalysis(syntaxTree, true, false, symtab, globalOffset);
      codegen(stdout, argv[1], syntaxTree, symtab, globalOffset, false);

      //printTree(stdout, syntaxTree, true, true);
   }
   if (numErrors == 0) // &&
   {
      //Print semantic tree
   }
   else
   {
      printf("-----------\n");
      printf("Errors: %d\n", numErrors);
      printf("-----------\n");
   }

   printf("Number of warnings: %d\n", numWarnings);
   printf("Number of errors: %d\n", numErrors);

   return 0;
}
