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
    #include <vector>

#line 56 "build/parser.tab.hpp"

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
    LAMBDA = 263,                  /* LAMBDA  */
    REASSIGN = 264,                /* REASSIGN  */
    ADD = 265,                     /* ADD  */
    SUB = 266,                     /* SUB  */
    MUL = 267,                     /* MUL  */
    DIV = 268,                     /* DIV  */
    MOD = 269,                     /* MOD  */
    POW = 270,                     /* POW  */
    LT = 271,                      /* LT  */
    GT = 272,                      /* GT  */
    LE = 273,                      /* LE  */
    GE = 274,                      /* GE  */
    EQ = 275,                      /* EQ  */
    NE = 276,                      /* NE  */
    AND = 277,                     /* AND  */
    OR = 278,                      /* OR  */
    NOT = 279,                     /* NOT  */
    CONCAT = 280,                  /* CONCAT  */
    CONCAT_SPACE = 281,            /* CONCAT_SPACE  */
    SIN = 282,                     /* SIN  */
    COS = 283,                     /* COS  */
    MAX = 284,                     /* MAX  */
    MIN = 285,                     /* MIN  */
    SQRT = 286,                    /* SQRT  */
    EXP = 287,                     /* EXP  */
    LOG = 288,                     /* LOG  */
    RANDOM = 289,                  /* RANDOM  */
    PRINT = 290,                   /* PRINT  */
    READ = 291,                    /* READ  */
    PARSE = 292,                   /* PARSE  */
    PI = 293,                      /* PI  */
    E = 294,                       /* E  */
    FUNC = 295,                    /* FUNC  */
    LET = 296,                     /* LET  */
    IN = 297,                      /* IN  */
    IF = 298,                      /* IF  */
    ELIF = 299,                    /* ELIF  */
    ELSE = 300,                    /* ELSE  */
    FOR = 301,                     /* FOR  */
    WHILE = 302,                   /* WHILE  */
    RANGE = 303,                   /* RANGE  */
    TYPE = 304,                    /* TYPE  */
    NEW = 305,                     /* NEW  */
    SELF = 306,                    /* SELF  */
    INHERITS = 307                 /* INHERITS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 32 "src/parser.y"

    double num;  // Tipo para números (enteros y decimales)
    std::string* str; 
    bool boolean;
    std::string* stmt;
    std::vector<std::string*>* list;

#line 133 "build/parser.tab.hpp"

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
