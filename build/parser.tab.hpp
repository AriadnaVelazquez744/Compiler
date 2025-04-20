/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_BUILD_PARSER_TAB_HPP_INCLUDED
# define YY_YY_BUILD_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 21 "src/parser.y"

    #include <string>
    #include <iostream>
    #include <cmath>

#line 55 "build/parser.tab.hpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    STRING = 259,                  /* STRING  */
    BOOL = 260,                    /* BOOL  */
    NULL_VAL = 261,                /* NULL_VAL  */
    ID = 262,                      /* ID  */
    ADD = 263,                     /* ADD  */
    SUB = 264,                     /* SUB  */
    MUL = 265,                     /* MUL  */
    DIV = 266,                     /* DIV  */
    MOD = 267,                     /* MOD  */
    POW = 268,                     /* POW  */
    LT = 269,                      /* LT  */
    GT = 270,                      /* GT  */
    LE = 271,                      /* LE  */
    GE = 272,                      /* GE  */
    EQ = 273,                      /* EQ  */
    NE = 274,                      /* NE  */
    AND = 275,                     /* AND  */
    OR = 276,                      /* OR  */
    NOT = 277,                     /* NOT  */
    CONCAT = 278,                  /* CONCAT  */
    CONCAT_SPACE = 279,            /* CONCAT_SPACE  */
    SIN = 280,                     /* SIN  */
    COS = 281,                     /* COS  */
    MAX = 282,                     /* MAX  */
    MIN = 283,                     /* MIN  */
    SQRT = 284,                    /* SQRT  */
    EXP = 285,                     /* EXP  */
    LOG = 286,                     /* LOG  */
    RANDOM = 287,                  /* RANDOM  */
    PRINT = 288,                   /* PRINT  */
    READ = 289,                    /* READ  */
    PARSE = 290,                   /* PARSE  */
    PI = 291,                      /* PI  */
    E = 292                        /* E  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 31 "src/parser.y"

    double num;  // Tipo para números (enteros y decimales)
    std::string* str; 
    bool boolean;
    std::string* stmt;

#line 116 "build/parser.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_BUILD_PARSER_TAB_HPP_INCLUDED  */
