/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 85 "parser.y" /* yacc.c:1909  */

   TokenData *tinfo;
   TreeNode *tree;
   ExpType type;  // for passing type spec up the tree

#line 105 "parser.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
