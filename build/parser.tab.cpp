/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/parser.y"


#include <stdio.h>
#include "../include/AST.hpp"

extern int yylex();
void yyerror(const char *msg);

// Definir estructura para ubicación
typedef struct YYLTYPE {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} YYLTYPE;
#define YYLTYPE_IS_DECLARED 1

ASTNode* root;

#define PI_VAL 3.14159265358979323846
#define TRACE(EXPR) std::cout << "elem_expr: " << *EXPR << std::endl;

std::vector<ASTNode*> vectorize(ASTNode* arg1, ASTNode* arg2, int n) {
    
    std::vector<ASTNode*> args = std::vector<ASTNode*>();
    
    if ((n == 1) || (n == 2)) { args.push_back(arg1); }
    if (n == 2) { args.push_back(arg2); }

    return args;
}


#line 105 "build/parser.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_STRING = 4,                     /* STRING  */
  YYSYMBOL_BOOL = 5,                       /* BOOL  */
  YYSYMBOL_NULL_VAL = 6,                   /* NULL_VAL  */
  YYSYMBOL_ID = 7,                         /* ID  */
  YYSYMBOL_8_ = 8,                         /* ','  */
  YYSYMBOL_9_ = 9,                         /* ';'  */
  YYSYMBOL_10_ = 10,                       /* '.'  */
  YYSYMBOL_11_ = 11,                       /* '('  */
  YYSYMBOL_12_ = 12,                       /* ')'  */
  YYSYMBOL_13_ = 13,                       /* '{'  */
  YYSYMBOL_14_ = 14,                       /* '}'  */
  YYSYMBOL_LAMBDA = 15,                    /* LAMBDA  */
  YYSYMBOL_16_ = 16,                       /* '='  */
  YYSYMBOL_REASSIGN = 17,                  /* REASSIGN  */
  YYSYMBOL_ADD = 18,                       /* ADD  */
  YYSYMBOL_SUB = 19,                       /* SUB  */
  YYSYMBOL_MUL = 20,                       /* MUL  */
  YYSYMBOL_DIV = 21,                       /* DIV  */
  YYSYMBOL_MOD = 22,                       /* MOD  */
  YYSYMBOL_POW = 23,                       /* POW  */
  YYSYMBOL_LT = 24,                        /* LT  */
  YYSYMBOL_GT = 25,                        /* GT  */
  YYSYMBOL_LE = 26,                        /* LE  */
  YYSYMBOL_GE = 27,                        /* GE  */
  YYSYMBOL_EQ = 28,                        /* EQ  */
  YYSYMBOL_NE = 29,                        /* NE  */
  YYSYMBOL_AND = 30,                       /* AND  */
  YYSYMBOL_OR = 31,                        /* OR  */
  YYSYMBOL_NOT = 32,                       /* NOT  */
  YYSYMBOL_CONCAT = 33,                    /* CONCAT  */
  YYSYMBOL_CONCAT_SPACE = 34,              /* CONCAT_SPACE  */
  YYSYMBOL_SIN = 35,                       /* SIN  */
  YYSYMBOL_COS = 36,                       /* COS  */
  YYSYMBOL_MAX = 37,                       /* MAX  */
  YYSYMBOL_MIN = 38,                       /* MIN  */
  YYSYMBOL_SQRT = 39,                      /* SQRT  */
  YYSYMBOL_EXP = 40,                       /* EXP  */
  YYSYMBOL_LOG = 41,                       /* LOG  */
  YYSYMBOL_RANDOM = 42,                    /* RANDOM  */
  YYSYMBOL_PRINT = 43,                     /* PRINT  */
  YYSYMBOL_READ = 44,                      /* READ  */
  YYSYMBOL_PARSE = 45,                     /* PARSE  */
  YYSYMBOL_PI = 46,                        /* PI  */
  YYSYMBOL_E = 47,                         /* E  */
  YYSYMBOL_FUNC = 48,                      /* FUNC  */
  YYSYMBOL_LET = 49,                       /* LET  */
  YYSYMBOL_IN = 50,                        /* IN  */
  YYSYMBOL_IF = 51,                        /* IF  */
  YYSYMBOL_ELIF = 52,                      /* ELIF  */
  YYSYMBOL_ELSE = 53,                      /* ELSE  */
  YYSYMBOL_FOR = 54,                       /* FOR  */
  YYSYMBOL_WHILE = 55,                     /* WHILE  */
  YYSYMBOL_RANGE = 56,                     /* RANGE  */
  YYSYMBOL_TYPE = 57,                      /* TYPE  */
  YYSYMBOL_NEW = 58,                       /* NEW  */
  YYSYMBOL_SELF = 59,                      /* SELF  */
  YYSYMBOL_INHERITS = 60,                  /* INHERITS  */
  YYSYMBOL_YYACCEPT = 61,                  /* $accept  */
  YYSYMBOL_program = 62,                   /* program  */
  YYSYMBOL_statement = 63,                 /* statement  */
  YYSYMBOL_expression = 64,                /* expression  */
  YYSYMBOL_elem_expr = 65,                 /* elem_expr  */
  YYSYMBOL_block_expr = 66,                /* block_expr  */
  YYSYMBOL_block_body = 67,                /* block_body  */
  YYSYMBOL_params = 68,                    /* params  */
  YYSYMBOL_func_call_expr = 69,            /* func_call_expr  */
  YYSYMBOL_args = 70,                      /* args  */
  YYSYMBOL_assign_expr = 71,               /* assign_expr  */
  YYSYMBOL_let_expr = 72,                  /* let_expr  */
  YYSYMBOL_decl = 73,                      /* decl  */
  YYSYMBOL_body = 74,                      /* body  */
  YYSYMBOL_if_expr = 75,                   /* if_expr  */
  YYSYMBOL_if_head = 76,                   /* if_head  */
  YYSYMBOL_while_expr = 77,                /* while_expr  */
  YYSYMBOL_for_expr = 78                   /* for_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   668

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  148

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   307


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      11,    12,     2,     2,     8,     2,    10,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     9,
       2,    16,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    13,     2,    14,     2,     2,     2,     2,
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
       5,     6,     7,    15,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   161,   161,   163,   164,   168,   169,   197,   198,   204,
     210,   211,   212,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   232,   236,   240,   244,
     248,   253,   258,   263,   307,   308,   310,   314,   319,   324,
     328,   332,   337,   341,   346,   350,   354,   361,   368,   369,
     370,   374,   375,   382,   391,   395,   396,   397,   401,   405,
     406,   407,   408,   412,   419,   429,   430,   431,   438,   439,
     443,   450,   460,   464
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "STRING",
  "BOOL", "NULL_VAL", "ID", "','", "';'", "'.'", "'('", "')'", "'{'",
  "'}'", "LAMBDA", "'='", "REASSIGN", "ADD", "SUB", "MUL", "DIV", "MOD",
  "POW", "LT", "GT", "LE", "GE", "EQ", "NE", "AND", "OR", "NOT", "CONCAT",
  "CONCAT_SPACE", "SIN", "COS", "MAX", "MIN", "SQRT", "EXP", "LOG",
  "RANDOM", "PRINT", "READ", "PARSE", "PI", "E", "FUNC", "LET", "IN", "IF",
  "ELIF", "ELSE", "FOR", "WHILE", "RANGE", "TYPE", "NEW", "SELF",
  "INHERITS", "$accept", "program", "statement", "expression", "elem_expr",
  "block_expr", "block_body", "params", "func_call_expr", "args",
  "assign_expr", "let_expr", "decl", "body", "if_expr", "if_head",
  "while_expr", "for_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-69)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -69,    53,   -69,     4,   -69,   -69,   -69,   -69,    -3,   376,
     287,   376,   376,    -6,   -69,   -69,    18,    21,    22,    23,
      24,   -69,   462,   -69,   -69,   -69,   -69,   -69,   -69,   -29,
     -69,   -69,   -69,   376,   376,   482,   -69,   -69,   -69,   -69,
     -69,   230,   634,    -4,   376,    27,    33,    -5,   376,    30,
     376,   -69,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,    28,   305,
     619,     7,   619,   -69,   -69,   -69,   502,    36,   376,    43,
     358,   522,     2,   542,   634,   634,   150,   150,   150,   150,
     -13,   -13,   -13,   -13,   -13,   -13,    -4,    -4,    -2,    -2,
     376,    40,   -69,   462,   -69,   376,   -69,    46,   -69,    14,
     619,    45,   305,    54,   305,    15,   305,   562,   376,   619,
     -69,    63,    -9,   376,   439,    61,   -69,   -69,    65,   -69,
     305,   582,   -69,   305,   -69,   619,    68,   376,   -69,    46,
     -69,   -69,   416,   376,   602,    62,   305,   -69
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,    13,    14,    15,    16,    17,     0,
      48,     0,     0,     0,    35,    34,     0,     0,     0,     0,
       0,     3,     0,    18,     7,    20,    21,    10,    23,    68,
      11,    12,     4,    55,     0,     0,    19,    22,    24,    25,
      49,     0,    32,    46,     0,     0,     0,     0,     0,     0,
       0,     5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,    58,    33,    47,    50,     0,    51,     0,     0,
       0,     0,     0,     0,    26,    27,    28,    29,    30,    31,
      38,    39,    40,    41,    42,    43,    44,    45,    36,    37,
       0,     0,    65,    66,    69,     0,    54,     0,    52,     0,
      63,     0,     0,    59,     0,     0,     0,     0,     0,    57,
       6,     0,     0,     0,     0,     0,    61,    70,     0,    72,
       0,     0,    53,     0,     9,    64,    60,     0,    71,    67,
       8,    62,     0,     0,     0,     0,     0,    73
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -69,   -69,     1,   152,   -69,    -1,   -69,   -69,   -69,   -69,
     -69,     0,   -69,   -68,   -69,   -69,     6,    26
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,   102,   103,    23,    36,    41,   109,    25,    71,
      26,    37,    47,   104,    28,    29,    38,    39
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      24,    27,    21,    79,    10,    44,   133,    30,    33,    24,
      27,    40,   113,    32,    34,   105,    30,    64,    65,   106,
      66,    67,   121,    68,    69,    45,   122,    31,    46,    66,
      67,    -1,    -1,    48,    49,    50,    31,    82,    77,   100,
      24,    27,    75,   108,   125,    80,   127,    30,   129,    78,
     111,   118,   115,     2,     3,   120,     4,     5,     6,     7,
       8,   123,   138,   126,     9,   140,    10,    31,    24,    27,
     132,   128,    11,   136,   146,    30,   137,   141,   147,    24,
      27,     0,     0,     0,     0,    12,    30,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    13,     0,     0,    14,
      15,    16,    17,     0,    18,     0,    31,    19,    20,     0,
       0,    24,    27,    24,    27,    24,    27,     0,    30,     0,
      30,   134,    30,     0,     0,     0,     0,     0,     0,    24,
      27,     0,    24,    27,     0,     0,    30,     0,    31,    30,
      31,     0,    31,     0,     0,    24,    27,     0,     0,     0,
       0,     0,    30,    22,     0,     0,    31,     0,     0,    31,
       0,    35,    22,    42,    43,     0,     0,     0,     0,     0,
       0,     0,    31,    57,    58,    59,    60,    61,    62,    63,
      64,    65,     0,    66,    67,    70,    72,     0,     0,     0,
       0,     0,     0,    22,     0,     0,    76,     0,     0,     0,
      81,     0,    83,     0,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,     9,     0,    10,    74,     0,     0,     0,     0,    11,
       0,     0,   117,     0,     0,     0,     0,   119,     0,     0,
       0,     0,    12,     0,   124,     0,     0,     0,     0,     0,
     131,     0,     0,    13,     0,   135,    14,    15,    16,    17,
       0,    18,     0,     0,    19,    20,     0,     0,     0,   142,
       4,     5,     6,     7,     8,   144,     0,     0,     9,     0,
      10,     0,     0,     0,     0,     0,    11,     0,     4,     5,
       6,     7,     8,     0,     0,     0,     9,     0,    10,    12,
       0,     0,     0,     0,    11,     0,     0,     0,     0,     0,
      13,     0,     0,    14,    15,    16,    17,    12,    18,     0,
       0,    19,    20,     0,     0,     0,     0,     0,   101,     0,
       0,    14,    15,    16,    17,     0,    18,     0,     0,    19,
      20,     4,     5,     6,     7,     8,     0,     0,     0,   112,
       0,    10,     0,     0,     0,     0,     0,    11,     0,     4,
       5,     6,     7,     8,     0,     0,     0,     9,     0,    10,
      12,     0,     0,     0,     0,    11,     0,     0,     0,     0,
       0,   101,     0,     0,    14,    15,    16,    17,    12,    18,
       0,     0,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    14,    15,   143,    17,     0,    18,     0,     0,
      19,    20,     0,     0,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    51,    66,
      67,    73,     0,     0,     0,     0,     0,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    51,    66,    67,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    73,    66,    67,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   107,    66,    67,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   114,    66,    67,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   116,    66,    67,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   130,    66,    67,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   139,    66,    67,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   145,    66,    67,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,     0,    66,    67,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,     0,    66,    67,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,     0,    66,    67
};

static const yytype_int16 yycheck[] =
{
       1,     1,     1,     8,    13,    11,    15,     1,    11,    10,
      10,    10,    80,     9,    17,     8,    10,    30,    31,    12,
      33,    34,     8,    52,    53,     7,    12,     1,     7,    33,
      34,    33,    34,    11,    11,    11,    10,     7,    11,    11,
      41,    41,    41,     7,   112,    50,   114,    41,   116,    16,
       7,    11,    50,     0,     1,     9,     3,     4,     5,     6,
       7,    16,   130,     9,    11,   133,    13,    41,    69,    69,
       7,    56,    19,    12,    12,    69,    11,     9,   146,    80,
      80,    -1,    -1,    -1,    -1,    32,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    43,    -1,    -1,    46,
      47,    48,    49,    -1,    51,    -1,    80,    54,    55,    -1,
      -1,   112,   112,   114,   114,   116,   116,    -1,   112,    -1,
     114,   122,   116,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     130,    -1,   133,   133,    -1,    -1,   130,    -1,   112,   133,
     114,    -1,   116,    -1,    -1,   146,   146,    -1,    -1,    -1,
      -1,    -1,   146,     1,    -1,    -1,   130,    -1,    -1,   133,
      -1,     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    33,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    50,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    -1,    13,    14,    -1,    -1,    -1,    -1,    19,
      -1,    -1,   100,    -1,    -1,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    32,    -1,   112,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    43,    -1,   123,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    54,    55,    -1,    -1,    -1,   137,
       3,     4,     5,     6,     7,   143,    -1,    -1,    11,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    -1,    13,    32,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    46,    47,    48,    49,    32,    51,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    47,    48,    49,    -1,    51,    -1,    -1,    54,
      55,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    -1,    13,
      32,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    47,    48,    49,    32,    51,
      -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,     8,    49,    -1,    51,    -1,    -1,
      54,    55,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     9,    33,
      34,    12,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     9,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    12,    33,    34,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    12,    33,    34,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    12,    33,    34,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    12,    33,    34,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    12,    33,    34,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    12,    33,    34,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    12,    33,    34,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    62,     0,     1,     3,     4,     5,     6,     7,    11,
      13,    19,    32,    43,    46,    47,    48,    49,    51,    54,
      55,    63,    64,    65,    66,    69,    71,    72,    75,    76,
      77,    78,     9,    11,    17,    64,    66,    72,    77,    78,
      63,    67,    64,    64,    11,     7,     7,    73,    11,    11,
      11,     9,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    33,    34,    52,    53,
      64,    70,    64,    12,    14,    63,    64,    11,    16,     8,
      50,    64,     7,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      11,    43,    63,    64,    74,     8,    12,    12,     7,    68,
      64,     7,    11,    74,    12,    50,    12,    64,    11,    64,
       9,     8,    12,    16,    64,    74,     9,    74,    56,    74,
      12,    64,     7,    15,    66,    64,    12,    11,    74,    12,
      74,     9,    64,     8,    64,    12,    12,    74
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    61,    62,    62,    62,    63,    63,    63,    63,    63,
      63,    63,    63,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    66,    67,    67,
      67,    68,    68,    68,    69,    70,    70,    70,    71,    72,
      72,    72,    72,    73,    73,    74,    74,    74,    75,    75,
      76,    76,    77,    78
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     3,     2,     5,     1,     7,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     2,     3,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     0,     1,
       2,     0,     1,     3,     4,     0,     1,     3,     3,     4,
       6,     5,     7,     3,     5,     1,     1,     4,     1,     3,
       5,     6,     5,    12
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* program: program statement  */
#line 163 "src/parser.y"
                                    { root = (yyvsp[0].node); }
#line 1483 "build/parser.tab.cpp"
    break;

  case 4: /* program: program error ';'  */
#line 164 "src/parser.y"
                                    { yyerrok; }
#line 1489 "build/parser.tab.cpp"
    break;

  case 5: /* statement: expression ';'  */
#line 168 "src/parser.y"
                                    { (yyval.node) = (yyvsp[-1].node); delete (yyvsp[-1].node); }
#line 1495 "build/parser.tab.cpp"
    break;

  case 6: /* statement: PRINT '(' expression ')' ';'  */
#line 169 "src/parser.y"
                                    { 
                                        std::vector<ASTNode*> args = vectorize((yyvsp[-2].node), nullptr, 1);
                                        (yyval.node) = new BuiltInFunctionNode("print", args, yylloc.first_line);
                                    }
#line 1504 "build/parser.tab.cpp"
    break;

  case 7: /* statement: block_expr  */
#line 197 "src/parser.y"
                                    { (yyval.node) = (yyvsp[0].node); }
#line 1510 "build/parser.tab.cpp"
    break;

  case 8: /* statement: FUNC ID '(' params ')' LAMBDA body  */
#line 199 "src/parser.y"
                                    {
                                        (yyval.node) = new FunctionDeclarationNode(*(yyvsp[-5].str), (yyvsp[-3].param), (yyvsp[0].node), true, yylloc.first_line);
                                        std::cout << "Definición función inline: " << *(yyvsp[-5].str) << std::endl;
                                        delete (yyvsp[-5].str); delete (yyvsp[-3].param);
                                    }
#line 1520 "build/parser.tab.cpp"
    break;

  case 9: /* statement: FUNC ID '(' params ')' block_expr  */
#line 205 "src/parser.y"
                                    {
                                        (yyval.node) = new FunctionDeclarationNode(*(yyvsp[-4].str), (yyvsp[-2].param), (yyvsp[0].node), false, yylloc.first_line);
                                        std::cout << "Definición función bloque: " << *(yyvsp[-4].str) << std::endl;
                                        delete (yyvsp[-4].str); delete (yyvsp[-2].param);
                                    }
#line 1530 "build/parser.tab.cpp"
    break;

  case 10: /* statement: let_expr  */
#line 210 "src/parser.y"
                                    { (yyval.node) = (yyvsp[0].node); std::cout << "let_expr " << std::endl; }
#line 1536 "build/parser.tab.cpp"
    break;

  case 11: /* statement: while_expr  */
#line 211 "src/parser.y"
                                    { (yyval.node) = (yyvsp[0].node); std::cout << "while_expr " << std::endl; }
#line 1542 "build/parser.tab.cpp"
    break;

  case 12: /* statement: for_expr  */
#line 212 "src/parser.y"
                                    { (yyval.node) = (yyvsp[0].node); std::cout << "for_expr " << std::endl; }
#line 1548 "build/parser.tab.cpp"
    break;

  case 13: /* expression: NUMBER  */
#line 216 "src/parser.y"
                                { (yyval.node) = new LiteralNode(std::to_string((yyvsp[0].num)), "Number", yylloc.first_line); }
#line 1554 "build/parser.tab.cpp"
    break;

  case 14: /* expression: STRING  */
#line 217 "src/parser.y"
                                { (yyval.node) = new LiteralNode(*(yyvsp[0].str), "String", yylloc.first_line); delete (yyvsp[0].str); }
#line 1560 "build/parser.tab.cpp"
    break;

  case 15: /* expression: BOOL  */
#line 218 "src/parser.y"
                                { (yyval.node) = new LiteralNode((yyvsp[0].boolean) ? "true" : "false", "Boolean", yylloc.first_line); }
#line 1566 "build/parser.tab.cpp"
    break;

  case 16: /* expression: NULL_VAL  */
#line 219 "src/parser.y"
                                { (yyval.node) = new LiteralNode("null", "Null", yylloc.first_line); }
#line 1572 "build/parser.tab.cpp"
    break;

  case 17: /* expression: ID  */
#line 220 "src/parser.y"
                                { (yyval.node) = new IdentifierNode(*(yyvsp[0].str), yylloc.first_line); }
#line 1578 "build/parser.tab.cpp"
    break;

  case 18: /* expression: elem_expr  */
#line 221 "src/parser.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1584 "build/parser.tab.cpp"
    break;

  case 19: /* expression: block_expr  */
#line 222 "src/parser.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1590 "build/parser.tab.cpp"
    break;

  case 20: /* expression: func_call_expr  */
#line 223 "src/parser.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1596 "build/parser.tab.cpp"
    break;

  case 21: /* expression: assign_expr  */
#line 224 "src/parser.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1602 "build/parser.tab.cpp"
    break;

  case 22: /* expression: let_expr  */
#line 225 "src/parser.y"
                                { (yyval.node) = (yyvsp[0].node); std::cout << "let_expr " << std::endl; }
#line 1608 "build/parser.tab.cpp"
    break;

  case 23: /* expression: if_expr  */
#line 226 "src/parser.y"
                                { (yyval.node) = (yyvsp[0].node); std::cout << "if_expr " << std::endl; }
#line 1614 "build/parser.tab.cpp"
    break;

  case 24: /* expression: while_expr  */
#line 227 "src/parser.y"
                                { (yyval.node) = (yyvsp[0].node); std::cout << "while_expr " << std::endl; }
#line 1620 "build/parser.tab.cpp"
    break;

  case 25: /* expression: for_expr  */
#line 228 "src/parser.y"
                                { (yyval.node) = (yyvsp[0].node); std::cout << "for_expr " << std::endl; }
#line 1626 "build/parser.tab.cpp"
    break;

  case 26: /* elem_expr: expression ADD expression  */
#line 232 "src/parser.y"
                                        {
                (yyval.node) = new BinaryOpNode("+", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1635 "build/parser.tab.cpp"
    break;

  case 27: /* elem_expr: expression SUB expression  */
#line 236 "src/parser.y"
                                        {
                (yyval.node) = new BinaryOpNode("-", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1644 "build/parser.tab.cpp"
    break;

  case 28: /* elem_expr: expression MUL expression  */
#line 240 "src/parser.y"
                                        {
                (yyval.node) = new BinaryOpNode("*", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1653 "build/parser.tab.cpp"
    break;

  case 29: /* elem_expr: expression DIV expression  */
#line 244 "src/parser.y"
                                        {
                (yyval.node) = new BinaryOpNode("/", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1662 "build/parser.tab.cpp"
    break;

  case 30: /* elem_expr: expression MOD expression  */
#line 248 "src/parser.y"
                                        {
                
                (yyval.node) = new BinaryOpNode("%", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1672 "build/parser.tab.cpp"
    break;

  case 31: /* elem_expr: expression POW expression  */
#line 253 "src/parser.y"
                                        {
                (yyval.node) = new BinaryOpNode("^", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1681 "build/parser.tab.cpp"
    break;

  case 32: /* elem_expr: SUB expression  */
#line 258 "src/parser.y"
                             {
                (yyval.node) = new UnaryOpNode("-", (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[0].node);
            }
#line 1690 "build/parser.tab.cpp"
    break;

  case 33: /* elem_expr: '(' expression ')'  */
#line 263 "src/parser.y"
                                 {
                (yyval.node) = (yyvsp[-1].node);
            }
#line 1698 "build/parser.tab.cpp"
    break;

  case 34: /* elem_expr: E  */
#line 307 "src/parser.y"
                    { (yyval.node) = new IdentifierNode("e", yylloc.first_line); }
#line 1704 "build/parser.tab.cpp"
    break;

  case 35: /* elem_expr: PI  */
#line 308 "src/parser.y"
                    { (yyval.node) = new IdentifierNode("pi", yylloc.first_line); }
#line 1710 "build/parser.tab.cpp"
    break;

  case 36: /* elem_expr: expression CONCAT expression  */
#line 310 "src/parser.y"
                                           {
                (yyval.node) = new BinaryOpNode("@", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1719 "build/parser.tab.cpp"
    break;

  case 37: /* elem_expr: expression CONCAT_SPACE expression  */
#line 314 "src/parser.y"
                                                 {
                (yyval.node) = new BinaryOpNode("@@", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1728 "build/parser.tab.cpp"
    break;

  case 38: /* elem_expr: expression LT expression  */
#line 319 "src/parser.y"
                                       {
                
                (yyval.node) = new BinaryOpNode("<", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1738 "build/parser.tab.cpp"
    break;

  case 39: /* elem_expr: expression GT expression  */
#line 324 "src/parser.y"
                                       {
                (yyval.node) = new BinaryOpNode(">", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1747 "build/parser.tab.cpp"
    break;

  case 40: /* elem_expr: expression LE expression  */
#line 328 "src/parser.y"
                                       {
                (yyval.node) = new BinaryOpNode("<=", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1756 "build/parser.tab.cpp"
    break;

  case 41: /* elem_expr: expression GE expression  */
#line 332 "src/parser.y"
                                       {
                (yyval.node) = new BinaryOpNode(">=", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1765 "build/parser.tab.cpp"
    break;

  case 42: /* elem_expr: expression EQ expression  */
#line 337 "src/parser.y"
                                       {
                (yyval.node) = new BinaryOpNode("==", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1774 "build/parser.tab.cpp"
    break;

  case 43: /* elem_expr: expression NE expression  */
#line 341 "src/parser.y"
                                       {
                (yyval.node) = new BinaryOpNode("!=", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1783 "build/parser.tab.cpp"
    break;

  case 44: /* elem_expr: expression AND expression  */
#line 346 "src/parser.y"
                                        {
                (yyval.node) = new BinaryOpNode("&", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1792 "build/parser.tab.cpp"
    break;

  case 45: /* elem_expr: expression OR expression  */
#line 350 "src/parser.y"
                                       {
                (yyval.node) = new BinaryOpNode("|", (yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[-2].node); delete (yyvsp[0].node);
            }
#line 1801 "build/parser.tab.cpp"
    break;

  case 46: /* elem_expr: NOT expression  */
#line 354 "src/parser.y"
                             {
                (yyval.node) = new UnaryOpNode("!", (yyvsp[0].node), yylloc.first_line);
                delete (yyvsp[0].node);
            }
#line 1810 "build/parser.tab.cpp"
    break;

  case 47: /* block_expr: '{' block_body '}'  */
#line 361 "src/parser.y"
                                {
                                    (yyval.node) = new BlockNode(*(yyvsp[-1].list), yylloc.first_line); // Placeholder
                                    delete (yyvsp[-1].list);
                                }
#line 1819 "build/parser.tab.cpp"
    break;

  case 48: /* block_body: %empty  */
#line 368 "src/parser.y"
                                            { (yyval.list) = new std::vector<ASTNode*>(); }
#line 1825 "build/parser.tab.cpp"
    break;

  case 49: /* block_body: statement  */
#line 369 "src/parser.y"
                                            { (yyval.list) = new std::vector<ASTNode*>(); (yyval.list)->push_back((yyvsp[0].node)); }
#line 1831 "build/parser.tab.cpp"
    break;

  case 50: /* block_body: block_body statement  */
#line 370 "src/parser.y"
                                            { (yyvsp[-1].list)->push_back((yyvsp[0].node)); (yyval.list) = (yyvsp[-1].list); }
#line 1837 "build/parser.tab.cpp"
    break;

  case 51: /* params: %empty  */
#line 374 "src/parser.y"
                                { (yyval.param) = new std::vector<Parameter>(); }
#line 1843 "build/parser.tab.cpp"
    break;

  case 52: /* params: ID  */
#line 375 "src/parser.y"
                                { 
                                    Parameter p;
                                    p.name = *(yyvsp[0].str);
                                    (yyval.param) = new std::vector<Parameter>(); 
                                    (yyval.param)->push_back(p); 
                                    delete (yyvsp[0].str);
                                }
#line 1855 "build/parser.tab.cpp"
    break;

  case 53: /* params: params ',' ID  */
#line 382 "src/parser.y"
                                { 
                                    Parameter p;
                                    p.name = *(yyvsp[0].str);
                                    (yyvsp[-2].param)->push_back(p); 
                                    (yyval.param) = (yyvsp[-2].param); 
                                }
#line 1866 "build/parser.tab.cpp"
    break;

  case 54: /* func_call_expr: ID '(' args ')'  */
#line 391 "src/parser.y"
                                { (yyval.node) = new FunctionCallNode(*(yyvsp[-3].str), *(yyvsp[-1].list), yylloc.first_line); delete (yyvsp[-3].str); delete (yyvsp[-1].list);}
#line 1872 "build/parser.tab.cpp"
    break;

  case 55: /* args: %empty  */
#line 395 "src/parser.y"
                                        { (yyval.list) = new std::vector<ASTNode*>(); }
#line 1878 "build/parser.tab.cpp"
    break;

  case 56: /* args: expression  */
#line 396 "src/parser.y"
                                        { (yyval.list) = new std::vector<ASTNode*>(); (yyval.list)->push_back((yyvsp[0].node)); }
#line 1884 "build/parser.tab.cpp"
    break;

  case 57: /* args: args ',' expression  */
#line 397 "src/parser.y"
                                        { (yyvsp[-2].list)->push_back((yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 1890 "build/parser.tab.cpp"
    break;

  case 58: /* assign_expr: ID REASSIGN expression  */
#line 401 "src/parser.y"
                                      { (yyval.node) = new AssignmentNode(*(yyvsp[-2].str), (yyvsp[0].node), yylloc.first_line); delete (yyvsp[-2].str); }
#line 1896 "build/parser.tab.cpp"
    break;

  case 59: /* let_expr: LET decl IN body  */
#line 405 "src/parser.y"
                                              { (yyval.node) = new LetNode((yyvsp[-2].let_decl), (yyvsp[0].node), yylloc.first_line); delete (yyvsp[-2].let_decl); }
#line 1902 "build/parser.tab.cpp"
    break;

  case 60: /* let_expr: LET decl IN '(' body ')'  */
#line 406 "src/parser.y"
                                              { (yyval.node) = new LetNode((yyvsp[-4].let_decl), (yyvsp[-1].node), yylloc.first_line); delete (yyvsp[-4].let_decl); }
#line 1908 "build/parser.tab.cpp"
    break;

  case 61: /* let_expr: LET decl IN body ';'  */
#line 407 "src/parser.y"
                                              { (yyval.node) = new LetNode((yyvsp[-3].let_decl), (yyvsp[-1].node), yylloc.first_line); delete (yyvsp[-3].let_decl); }
#line 1914 "build/parser.tab.cpp"
    break;

  case 62: /* let_expr: LET decl IN '(' body ')' ';'  */
#line 408 "src/parser.y"
                                              { (yyval.node) = new LetNode((yyvsp[-5].let_decl), (yyvsp[-2].node), yylloc.first_line); delete (yyvsp[-5].let_decl); }
#line 1920 "build/parser.tab.cpp"
    break;

  case 63: /* decl: ID '=' expression  */
#line 412 "src/parser.y"
                                            {
                                                LetDeclaration d;
                                                d.name = *(yyvsp[-2].str);
                                                d.initializer = (yyvsp[0].node);
                                                (yyval.let_decl) = new std::vector<LetDeclaration>();
                                                (yyval.let_decl)->push_back(d); delete (yyvsp[-2].str);
                                            }
#line 1932 "build/parser.tab.cpp"
    break;

  case 64: /* decl: decl ',' ID '=' expression  */
#line 419 "src/parser.y"
                                            {
                                                LetDeclaration d;
                                                d.name = *(yyvsp[-2].str);
                                                d.initializer = (yyvsp[0].node);
                                                (yyvsp[-4].let_decl)->push_back(d); (yyval.let_decl) = (yyvsp[-4].let_decl);
                                                delete (yyvsp[-2].str);
                                            }
#line 1944 "build/parser.tab.cpp"
    break;

  case 65: /* body: statement  */
#line 429 "src/parser.y"
                                            { (yyval.node) = (yyvsp[0].node); }
#line 1950 "build/parser.tab.cpp"
    break;

  case 66: /* body: expression  */
#line 430 "src/parser.y"
                                            { (yyval.node) = (yyvsp[0].node); }
#line 1956 "build/parser.tab.cpp"
    break;

  case 67: /* body: PRINT '(' expression ')'  */
#line 431 "src/parser.y"
                                            { 
                                                std::vector<ASTNode*> args = vectorize((yyvsp[-1].node), nullptr, 1);
                                                (yyval.node) = new BuiltInFunctionNode("print", args, yylloc.first_line);
                                            }
#line 1965 "build/parser.tab.cpp"
    break;

  case 68: /* if_expr: if_head  */
#line 438 "src/parser.y"
                                        { (yyval.node) = new IfNode((yyvsp[0].if_branch), nullptr, yylloc.first_line); }
#line 1971 "build/parser.tab.cpp"
    break;

  case 69: /* if_expr: if_head ELSE body  */
#line 439 "src/parser.y"
                                        { (yyval.node) = new IfNode((yyvsp[-2].if_branch), (yyvsp[0].node), yylloc.first_line); }
#line 1977 "build/parser.tab.cpp"
    break;

  case 70: /* if_head: IF '(' expression ')' body  */
#line 443 "src/parser.y"
                                                            { 
                                                                IfBranch b;
                                                                b.condition = (yyvsp[-2].node);
                                                                b.body = (yyvsp[0].node);
                                                                (yyval.if_branch) = new std::vector<IfBranch>(); 
                                                                (yyval.if_branch)->push_back(b);
                                                            }
#line 1989 "build/parser.tab.cpp"
    break;

  case 71: /* if_head: if_head ELIF '(' expression ')' body  */
#line 450 "src/parser.y"
                                                            { 
                                                                IfBranch b;
                                                                b.condition = (yyvsp[-2].node);
                                                                b.body = (yyvsp[0].node);
                                                                (yyvsp[-5].if_branch)->push_back(b); 
                                                                (yyval.if_branch) = (yyvsp[-5].if_branch);  
                                                            }
#line 2001 "build/parser.tab.cpp"
    break;

  case 72: /* while_expr: WHILE '(' expression ')' body  */
#line 460 "src/parser.y"
                                                            { (yyval.node) = new WhileNode((yyvsp[-2].node), (yyvsp[0].node), yylloc.first_line); }
#line 2007 "build/parser.tab.cpp"
    break;

  case 73: /* for_expr: FOR '(' ID IN RANGE '(' expression ',' expression ')' ')' body  */
#line 464 "src/parser.y"
                                                                                { (yyval.node) = new ForNode(*(yyvsp[-9].str), (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[0].node), yylloc.first_line); delete (yyvsp[-9].str); }
#line 2013 "build/parser.tab.cpp"
    break;


#line 2017 "build/parser.tab.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 467 "src/parser.y"


void yyerror(const char *msg) {
    fprintf(stderr, "Error en línea %d, columna %d: %s\n",
            yylloc.first_line, yylloc.first_column, msg);
}


