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

#define PI_VAL 3.14159265358979323846
#define TRACE(EXPR) std::cout << "elem_expr: " << *EXPR << std::endl;


#line 91 "build/parser.tab.cpp"

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
  YYSYMBOL_10_ = 10,                       /* '('  */
  YYSYMBOL_11_ = 11,                       /* ')'  */
  YYSYMBOL_12_ = 12,                       /* '{'  */
  YYSYMBOL_13_ = 13,                       /* '}'  */
  YYSYMBOL_LAMBDA = 14,                    /* LAMBDA  */
  YYSYMBOL_15_ = 15,                       /* '='  */
  YYSYMBOL_REASSIGN = 16,                  /* REASSIGN  */
  YYSYMBOL_ADD = 17,                       /* ADD  */
  YYSYMBOL_SUB = 18,                       /* SUB  */
  YYSYMBOL_MUL = 19,                       /* MUL  */
  YYSYMBOL_DIV = 20,                       /* DIV  */
  YYSYMBOL_MOD = 21,                       /* MOD  */
  YYSYMBOL_POW = 22,                       /* POW  */
  YYSYMBOL_LT = 23,                        /* LT  */
  YYSYMBOL_GT = 24,                        /* GT  */
  YYSYMBOL_LE = 25,                        /* LE  */
  YYSYMBOL_GE = 26,                        /* GE  */
  YYSYMBOL_EQ = 27,                        /* EQ  */
  YYSYMBOL_NE = 28,                        /* NE  */
  YYSYMBOL_AND = 29,                       /* AND  */
  YYSYMBOL_OR = 30,                        /* OR  */
  YYSYMBOL_NOT = 31,                       /* NOT  */
  YYSYMBOL_CONCAT = 32,                    /* CONCAT  */
  YYSYMBOL_CONCAT_SPACE = 33,              /* CONCAT_SPACE  */
  YYSYMBOL_SIN = 34,                       /* SIN  */
  YYSYMBOL_COS = 35,                       /* COS  */
  YYSYMBOL_MAX = 36,                       /* MAX  */
  YYSYMBOL_MIN = 37,                       /* MIN  */
  YYSYMBOL_SQRT = 38,                      /* SQRT  */
  YYSYMBOL_EXP = 39,                       /* EXP  */
  YYSYMBOL_LOG = 40,                       /* LOG  */
  YYSYMBOL_RANDOM = 41,                    /* RANDOM  */
  YYSYMBOL_PRINT = 42,                     /* PRINT  */
  YYSYMBOL_READ = 43,                      /* READ  */
  YYSYMBOL_PARSE = 44,                     /* PARSE  */
  YYSYMBOL_PI = 45,                        /* PI  */
  YYSYMBOL_E = 46,                         /* E  */
  YYSYMBOL_FUNC = 47,                      /* FUNC  */
  YYSYMBOL_LET = 48,                       /* LET  */
  YYSYMBOL_IN = 49,                        /* IN  */
  YYSYMBOL_IF = 50,                        /* IF  */
  YYSYMBOL_ELIF = 51,                      /* ELIF  */
  YYSYMBOL_ELSE = 52,                      /* ELSE  */
  YYSYMBOL_FOR = 53,                       /* FOR  */
  YYSYMBOL_WHILE = 54,                     /* WHILE  */
  YYSYMBOL_RANGE = 55,                     /* RANGE  */
  YYSYMBOL_YYACCEPT = 56,                  /* $accept  */
  YYSYMBOL_program = 57,                   /* program  */
  YYSYMBOL_statement = 58,                 /* statement  */
  YYSYMBOL_expression = 59,                /* expression  */
  YYSYMBOL_elem_expr = 60,                 /* elem_expr  */
  YYSYMBOL_block_expr = 61,                /* block_expr  */
  YYSYMBOL_block_body = 62,                /* block_body  */
  YYSYMBOL_params = 63,                    /* params  */
  YYSYMBOL_func_call_expr = 64,            /* func_call_expr  */
  YYSYMBOL_args = 65,                      /* args  */
  YYSYMBOL_assign_expr = 66,               /* assign_expr  */
  YYSYMBOL_let_expr = 67,                  /* let_expr  */
  YYSYMBOL_decls = 68,                     /* decls  */
  YYSYMBOL_decl = 69,                      /* decl  */
  YYSYMBOL_body = 70,                      /* body  */
  YYSYMBOL_if_expr = 71,                   /* if_expr  */
  YYSYMBOL_if_head = 72,                   /* if_head  */
  YYSYMBOL_while_expr = 73,                /* while_expr  */
  YYSYMBOL_for_expr = 74                   /* for_expr  */
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
#define YYLAST   1012

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  196

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


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
      10,    11,     2,     2,     8,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     9,
       2,    15,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    12,     2,    13,     2,     2,     2,     2,
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
       5,     6,     7,    14,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   140,   140,   142,   143,   147,   148,   149,   156,   173,
     174,   182,   190,   191,   192,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   212,   217,
     222,   227,   232,   237,   243,   248,   252,   257,   262,   267,
     272,   277,   282,   288,   292,   296,   301,   306,   312,   317,
     322,   327,   333,   338,   344,   349,   354,   362,   369,   370,
     371,   375,   376,   377,   381,   385,   386,   387,   391,   395,
     396,   397,   398,   403,   404,   408,   414,   423,   424,   425,
     429,   430,   434,   435,   439,   443
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
  "BOOL", "NULL_VAL", "ID", "','", "';'", "'('", "')'", "'{'", "'}'",
  "LAMBDA", "'='", "REASSIGN", "ADD", "SUB", "MUL", "DIV", "MOD", "POW",
  "LT", "GT", "LE", "GE", "EQ", "NE", "AND", "OR", "NOT", "CONCAT",
  "CONCAT_SPACE", "SIN", "COS", "MAX", "MIN", "SQRT", "EXP", "LOG",
  "RANDOM", "PRINT", "READ", "PARSE", "PI", "E", "FUNC", "LET", "IN", "IF",
  "ELIF", "ELSE", "FOR", "WHILE", "RANGE", "$accept", "program",
  "statement", "expression", "elem_expr", "block_expr", "block_body",
  "params", "func_call_expr", "args", "assign_expr", "let_expr", "decls",
  "decl", "body", "if_expr", "if_head", "while_expr", "for_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-79)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -79,   320,   -79,    20,   -79,   -79,   -79,   -79,    -4,    29,
     424,    12,    29,    27,    40,    47,    63,    70,    71,    74,
      75,    76,    78,    84,   -79,   -79,    81,    90,    91,    92,
      93,   -79,   662,   -79,   -79,   -79,   -79,   -79,    -5,   -79,
      10,   -79,   -79,   -79,    29,    29,   682,   -79,   -79,   -79,
     -79,   -79,   372,   -79,    16,    29,    29,    29,    29,    29,
      29,    29,    89,    29,   -79,    29,    95,   104,    98,    29,
     113,    29,   -79,    29,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29,    29,    29,    29,    73,
     476,   112,   528,   979,    -3,   979,   -79,   -79,   -79,   702,
     722,   256,   569,   742,   762,   592,   -79,   782,   802,   116,
      29,   117,   822,    77,   842,    -2,    -2,    85,    85,    85,
      85,    13,    13,    13,    13,    13,    13,    16,    16,    39,
      39,    90,   528,   115,   -79,   662,   118,    29,   -79,    29,
     -79,   -79,   -79,    29,    29,   -79,   -79,    29,   119,   120,
     -79,    48,   979,   121,   528,    79,   528,    98,   640,   122,
      29,   -79,   862,   979,   882,   902,   922,   -79,   -79,   123,
      26,    29,   -79,   127,   -79,   130,   942,   528,   -79,   -79,
     -79,   -79,   528,   -79,   979,    29,   -79,   119,   -79,   -79,
     615,    29,   962,   129,   528,   -79
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,    15,    16,    17,    18,    19,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,    44,     0,     0,     0,     0,
       0,     3,     0,    20,     9,    22,    23,    12,     0,    25,
      80,    13,    14,     4,    65,     0,     0,    21,    24,    26,
      27,    59,     0,    34,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     0,     0,     0,    73,     0,
       0,     0,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    68,    35,    57,    60,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,    61,
       0,     0,     0,     0,     0,    28,    29,    30,    31,    32,
      33,    48,    49,    50,    51,    52,    53,    54,    55,    46,
      47,     0,     0,     0,    77,    78,    69,     0,    81,     0,
      64,    36,    37,     0,     0,    40,    42,     0,     0,     0,
      62,     0,    75,     0,     0,     0,     0,    74,     0,     0,
       0,    71,     0,    67,     0,     0,     0,     6,     8,     0,
       0,     0,    82,     0,    84,    70,     0,     0,    39,    38,
      41,    63,     0,    11,    76,     0,    72,    79,    83,    10,
       0,     0,     0,     0,     0,    85
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -79,   -79,     1,   162,   -79,    -1,   -79,   -79,   -79,   -79,
     -79,     0,   -79,    11,   -78,   -79,   -79,     3,     6
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,   134,   135,    33,    47,    52,   151,    35,    94,
      36,    48,    38,    68,   136,    39,    40,    49,    50
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      34,    37,    31,    89,    41,   139,    44,    42,   140,    34,
      37,    51,    45,    41,   138,    53,    42,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    43,
      87,    88,     4,     5,     6,     7,     8,    55,    10,     9,
     182,    10,    85,    86,    90,    87,    88,    11,    87,    88,
      56,    34,    37,    98,   159,    41,   169,    57,    42,   170,
      12,    91,    92,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    58,    24,    25,   172,    27,   174,    28,
      59,    60,    29,    30,    61,    62,    63,    64,    66,    34,
      37,    34,    37,    41,    65,    41,    42,    67,    42,   188,
     106,    69,    70,    71,   189,   109,   111,    78,    79,    80,
      81,    82,    83,    84,    85,    86,   195,    87,    88,   110,
     113,   131,   137,   150,   153,   160,   155,   161,   167,   168,
     181,    34,    37,   175,   173,    41,   171,   185,    42,   186,
     194,     0,   157,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,    37,    34,    37,    41,     0,    41,
      42,     0,    42,    32,     0,     0,     0,     0,     0,   183,
       0,    46,    32,     0,    54,     0,    34,    37,     0,     0,
      41,    34,    37,    42,     0,    41,     0,     0,    42,     0,
       0,     0,     0,    34,    37,     0,     0,    41,     0,     0,
      42,     0,     0,     0,     0,     0,    93,    95,     0,     0,
       0,     0,     0,     0,    32,     0,     0,    99,   100,   101,
     102,   103,   104,   105,     0,   107,     0,   108,     0,     0,
       0,   112,     0,   114,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   143,     0,     0,     0,     0,     0,
       0,     0,   152,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,     0,    87,    88,
       0,     0,     0,     0,   158,     0,     0,     0,     0,   162,
       0,   163,     0,     0,     0,   164,   165,     0,     0,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     3,   176,     4,     5,     6,     7,     8,     0,     0,
       9,     0,    10,   184,     0,     0,     0,     0,    11,     0,
       0,     0,     0,     0,     0,     0,     0,   190,     0,     0,
       0,    12,     0,   192,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,     0,     0,    29,    30,     4,     5,     6,     7,     8,
       0,     0,     9,     0,    10,    97,     0,     0,     0,     0,
      11,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    12,     0,     0,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,     0,     0,    29,    30,     4,     5,     6,
       7,     8,     0,     0,     9,     0,    10,     0,     0,     0,
       0,     0,    11,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    12,     0,     0,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,     0,     0,    29,    30,     4,
       5,     6,     7,     8,     0,     0,   132,     0,    10,     0,
       0,     0,     0,     0,    11,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    12,     0,     0,
      13,    14,    15,    16,    17,    18,    19,    20,   133,    22,
      23,    24,    25,    26,    27,     0,    28,     0,     0,    29,
      30,     4,     5,     6,     7,     8,     0,     0,     9,     0,
      10,     0,     0,     0,     0,     0,    11,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    12,
       0,     0,    13,    14,    15,    16,    17,    18,    19,    20,
     133,    22,    23,    24,    25,    26,    27,   144,    28,     0,
       0,    29,    30,     0,     0,     0,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
     147,    87,    88,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   191,    87,    88,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,     0,    87,    88,    72,
       0,    96,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    72,    87,    88,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    96,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   141,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   142,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   145,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   146,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   148,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   149,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   154,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   156,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   177,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   178,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   179,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   180,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   187,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   193,    87,    88,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,     0,    87,    88,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
       0,    87,    88
};

static const yytype_int16 yycheck[] =
{
       1,     1,     1,     8,     1,     8,    10,     1,    11,    10,
      10,    10,    16,    10,    92,     3,    10,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,     9,
      32,    33,     3,     4,     5,     6,     7,    10,    12,    10,
      14,    12,    29,    30,    49,    32,    33,    18,    32,    33,
      10,    52,    52,    52,   132,    52,     8,    10,    52,    11,
      31,    51,    52,    34,    35,    36,    37,    38,    39,    40,
      41,    32,    33,    10,    45,    46,   154,    48,   156,    50,
      10,    10,    53,    54,    10,    10,    10,     9,     7,    90,
      90,    92,    92,    90,    10,    92,    90,     7,    92,   177,
      11,    10,    10,    10,   182,    10,     8,    22,    23,    24,
      25,    26,    27,    28,    29,    30,   194,    32,    33,    15,
       7,    48,    10,     7,     7,    10,    49,     9,     9,     9,
       7,   132,   132,    11,    55,   132,    15,    10,   132,     9,
      11,    -1,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   154,   156,   156,   154,    -1,   156,
     154,    -1,   156,     1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,     9,    10,    -1,    12,    -1,   177,   177,    -1,    -1,
     177,   182,   182,   177,    -1,   182,    -1,    -1,   182,    -1,
      -1,    -1,    -1,   194,   194,    -1,    -1,   194,    -1,    -1,
     194,    -1,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    -1,    55,    56,    57,
      58,    59,    60,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    69,    -1,    71,    -1,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,   137,
      -1,   139,    -1,    -1,    -1,   143,   144,    -1,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       0,     1,   160,     3,     4,     5,     6,     7,    -1,    -1,
      10,    -1,    12,   171,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,
      -1,    31,    -1,   191,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      50,    -1,    -1,    53,    54,     3,     4,     5,     6,     7,
      -1,    -1,    10,    -1,    12,    13,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    -1,    50,    -1,    -1,    53,    54,     3,     4,     5,
       6,     7,    -1,    -1,    10,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    -1,    50,    -1,    -1,    53,    54,     3,
       4,     5,     6,     7,    -1,    -1,    10,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    50,    -1,    -1,    53,
      54,     3,     4,     5,     6,     7,    -1,    -1,    10,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,     8,    50,    -1,
      -1,    53,    54,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
       8,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,     8,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,     9,    32,    33,    -1,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    11,    32,    33,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    57,     0,     1,     3,     4,     5,     6,     7,    10,
      12,    18,    31,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    50,    53,
      54,    58,    59,    60,    61,    64,    66,    67,    68,    71,
      72,    73,    74,     9,    10,    16,    59,    61,    67,    73,
      74,    58,    62,     3,    59,    10,    10,    10,    10,    10,
      10,    10,    10,    10,     9,    10,     7,     7,    69,    10,
      10,    10,     9,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,     8,
      49,    51,    52,    59,    65,    59,    11,    13,    58,    59,
      59,    59,    59,    59,    59,    59,    11,    59,    59,    10,
      15,     8,    59,     7,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    48,    10,    42,    58,    59,    70,    10,    70,     8,
      11,    11,    11,     8,     8,    11,    11,     8,    11,    11,
       7,    63,    59,     7,    11,    49,    11,    69,    59,    70,
      10,     9,    59,    59,    59,    59,    59,     9,     9,     8,
      11,    15,    70,    55,    70,    11,    59,    11,    11,    11,
      11,     7,    14,    61,    59,    10,     9,    11,    70,    70,
      59,     8,    59,    11,    11,    70
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    57,    57,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    61,    62,    62,
      62,    63,    63,    63,    64,    65,    65,    65,    66,    67,
      67,    67,    67,    68,    68,    69,    69,    70,    70,    70,
      71,    71,    72,    72,    73,    74
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     3,     2,     5,     2,     5,     1,
       7,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     2,     3,     4,     4,     6,     6,
       4,     6,     4,     3,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     0,     1,
       2,     0,     1,     3,     4,     0,     1,     3,     3,     3,
       5,     4,     6,     2,     4,     3,     5,     1,     1,     4,
       1,     3,     5,     6,     5,    12
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
  case 4: /* program: program error ';'  */
#line 143 "src/parser.y"
                        { yyerrok; }
#line 1552 "build/parser.tab.cpp"
    break;

  case 5: /* statement: expression ';'  */
#line 147 "src/parser.y"
                                    { (yyval.stmt) = (yyvsp[-1].stmt); delete (yyvsp[-1].stmt); }
#line 1558 "build/parser.tab.cpp"
    break;

  case 6: /* statement: PRINT '(' expression ')' ';'  */
#line 148 "src/parser.y"
                                    { std::cout << "Salida: " << *(yyvsp[-2].stmt) << std::endl; }
#line 1564 "build/parser.tab.cpp"
    break;

  case 7: /* statement: READ ';'  */
#line 149 "src/parser.y"
                                    { 
                                        std::string input; 
                                        //std::cin >> input;
                                        std::getline(std::cin, input); 
                                        //$$ = new std::string(input); 
                                        std::cout << "Entrada: " << input << std::endl;
                                    }
#line 1576 "build/parser.tab.cpp"
    break;

  case 8: /* statement: PARSE '(' expression ')' ';'  */
#line 156 "src/parser.y"
                                    {
                                        const std::string& raw = *(yyvsp[-2].stmt);
                                        // Try parse as number
                                        try {
                                            double num = std::stod(raw);
                                            std::cout << "Parseado como número: " << num << std::endl;
                                        } catch (...) {
                                            // Try parse as boolean
                                            if (raw == "true" || raw == "True")
                                                std::cout << "Parseado como booleano: true" << std::endl;
                                            else if (raw == "false" || raw == "False")
                                                std::cout << "Parseado como booleano: false" << std::endl;
                                            else
                                                std::cout << "Parse fallido: " << raw << std::endl;
                                        }
                                        delete (yyvsp[-2].stmt);
                                    }
#line 1598 "build/parser.tab.cpp"
    break;

  case 9: /* statement: block_expr  */
#line 173 "src/parser.y"
                                    { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1604 "build/parser.tab.cpp"
    break;

  case 10: /* statement: FUNC ID '(' params ')' LAMBDA body  */
#line 175 "src/parser.y"
                                    {
                                        std::cout << "Definición función inline: " << *(yyvsp[-5].str) << std::endl;
                                        delete (yyvsp[-5].str);
                                        for (auto s : *(yyvsp[-3].list)) delete s;
                                        delete (yyvsp[-3].list);
                                        (yyval.stmt) = nullptr;
                                    }
#line 1616 "build/parser.tab.cpp"
    break;

  case 11: /* statement: FUNC ID '(' params ')' block_expr  */
#line 183 "src/parser.y"
                                    {
                                        std::cout << "Definición función bloque: " << *(yyvsp[-4].str) << std::endl;
                                        delete (yyvsp[-4].str);
                                        for (auto s : *(yyvsp[-2].list)) delete s;
                                        delete (yyvsp[-2].list); delete (yyvsp[0].stmt);
                                        (yyval.stmt) = nullptr;
                                    }
#line 1628 "build/parser.tab.cpp"
    break;

  case 12: /* statement: let_expr  */
#line 190 "src/parser.y"
                                    { (yyval.stmt) = (yyvsp[0].stmt); std::cout << "let_expr: " << *(yyval.stmt) << std::endl; }
#line 1634 "build/parser.tab.cpp"
    break;

  case 13: /* statement: while_expr  */
#line 191 "src/parser.y"
                                    { (yyval.stmt) = (yyvsp[0].stmt); std::cout << "while_expr: " << *(yyval.stmt) << std::endl; }
#line 1640 "build/parser.tab.cpp"
    break;

  case 14: /* statement: for_expr  */
#line 192 "src/parser.y"
                                    { (yyval.stmt) = (yyvsp[0].stmt); std::cout << "for_expr: " << *(yyval.stmt) << std::endl; }
#line 1646 "build/parser.tab.cpp"
    break;

  case 15: /* expression: NUMBER  */
#line 196 "src/parser.y"
                                { (yyval.stmt) = new std::string(std::to_string((yyvsp[0].num))); }
#line 1652 "build/parser.tab.cpp"
    break;

  case 16: /* expression: STRING  */
#line 197 "src/parser.y"
                                { (yyval.stmt) = new std::string(*(yyvsp[0].str)); delete (yyvsp[0].str); }
#line 1658 "build/parser.tab.cpp"
    break;

  case 17: /* expression: BOOL  */
#line 198 "src/parser.y"
                                { (yyval.stmt) = new std::string((yyvsp[0].boolean) ? "true" : "false"); }
#line 1664 "build/parser.tab.cpp"
    break;

  case 18: /* expression: NULL_VAL  */
#line 199 "src/parser.y"
                                { (yyval.stmt) = new std::string("null"); }
#line 1670 "build/parser.tab.cpp"
    break;

  case 19: /* expression: ID  */
#line 200 "src/parser.y"
                                { (yyval.stmt) = (yyvsp[0].str); }
#line 1676 "build/parser.tab.cpp"
    break;

  case 20: /* expression: elem_expr  */
#line 201 "src/parser.y"
                                { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1682 "build/parser.tab.cpp"
    break;

  case 21: /* expression: block_expr  */
#line 202 "src/parser.y"
                                { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1688 "build/parser.tab.cpp"
    break;

  case 22: /* expression: func_call_expr  */
#line 203 "src/parser.y"
                                { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1694 "build/parser.tab.cpp"
    break;

  case 23: /* expression: assign_expr  */
#line 204 "src/parser.y"
                                { (yyval.stmt) = (yyvsp[0].stmt); }
#line 1700 "build/parser.tab.cpp"
    break;

  case 24: /* expression: let_expr  */
#line 205 "src/parser.y"
                                { (yyval.stmt) = (yyvsp[0].stmt); std::cout << "let_expr: " << *(yyval.stmt) << std::endl; }
#line 1706 "build/parser.tab.cpp"
    break;

  case 25: /* expression: if_expr  */
#line 206 "src/parser.y"
                                { (yyval.stmt) = (yyvsp[0].stmt); std::cout << "if_expr: " << *(yyval.stmt) << std::endl; }
#line 1712 "build/parser.tab.cpp"
    break;

  case 26: /* expression: while_expr  */
#line 207 "src/parser.y"
                                { (yyval.stmt) = (yyvsp[0].stmt); std::cout << "while_expr: " << *(yyval.stmt) << std::endl; }
#line 1718 "build/parser.tab.cpp"
    break;

  case 27: /* expression: for_expr  */
#line 208 "src/parser.y"
                                { (yyval.stmt) = (yyvsp[0].stmt); std::cout << "for_expr: " << *(yyval.stmt) << std::endl; }
#line 1724 "build/parser.tab.cpp"
    break;

  case 28: /* elem_expr: expression ADD expression  */
#line 212 "src/parser.y"
                                        {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " + " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1734 "build/parser.tab.cpp"
    break;

  case 29: /* elem_expr: expression SUB expression  */
#line 217 "src/parser.y"
                                        {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " - " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1744 "build/parser.tab.cpp"
    break;

  case 30: /* elem_expr: expression MUL expression  */
#line 222 "src/parser.y"
                                        {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " * " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1754 "build/parser.tab.cpp"
    break;

  case 31: /* elem_expr: expression DIV expression  */
#line 227 "src/parser.y"
                                        {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " / " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1764 "build/parser.tab.cpp"
    break;

  case 32: /* elem_expr: expression MOD expression  */
#line 232 "src/parser.y"
                                        {
                (yyval.stmt) = new std::string("mod(" + *(yyvsp[-2].stmt) + ", " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1774 "build/parser.tab.cpp"
    break;

  case 33: /* elem_expr: expression POW expression  */
#line 237 "src/parser.y"
                                        {
                (yyval.stmt) = new std::string("pow(" + *(yyvsp[-2].stmt) + ", " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1784 "build/parser.tab.cpp"
    break;

  case 34: /* elem_expr: SUB NUMBER  */
#line 243 "src/parser.y"
                         {
                (yyval.stmt) = new std::string("-" + std::to_string((yyvsp[0].num)));
                TRACE((yyval.stmt));
            }
#line 1793 "build/parser.tab.cpp"
    break;

  case 35: /* elem_expr: '(' expression ')'  */
#line 248 "src/parser.y"
                                 {
                (yyval.stmt) = (yyvsp[-1].stmt);    TRACE((yyval.stmt));
            }
#line 1801 "build/parser.tab.cpp"
    break;

  case 36: /* elem_expr: SIN '(' expression ')'  */
#line 252 "src/parser.y"
                                     {
                (yyval.stmt) = new std::string("sin(" + *(yyvsp[-1].stmt) + ")");
                delete (yyvsp[-1].stmt);
                TRACE((yyval.stmt));
            }
#line 1811 "build/parser.tab.cpp"
    break;

  case 37: /* elem_expr: COS '(' expression ')'  */
#line 257 "src/parser.y"
                                     {
                (yyval.stmt) = new std::string("cos(" + *(yyvsp[-1].stmt) + ")");
                delete (yyvsp[-1].stmt);
                TRACE((yyval.stmt));
            }
#line 1821 "build/parser.tab.cpp"
    break;

  case 38: /* elem_expr: MIN '(' expression ',' expression ')'  */
#line 262 "src/parser.y"
                                                    {
                (yyval.stmt) = new std::string("min(" + *(yyvsp[-3].stmt) + ", " + *(yyvsp[-1].stmt) + ")");
                delete (yyvsp[-3].stmt); delete (yyvsp[-1].stmt);
                TRACE((yyval.stmt));
            }
#line 1831 "build/parser.tab.cpp"
    break;

  case 39: /* elem_expr: MAX '(' expression ',' expression ')'  */
#line 267 "src/parser.y"
                                                    {
                (yyval.stmt) = new std::string("max(" + *(yyvsp[-3].stmt) + ", " + *(yyvsp[-1].stmt) + ")");
                delete (yyvsp[-3].stmt); delete (yyvsp[-1].stmt);
                TRACE((yyval.stmt));
            }
#line 1841 "build/parser.tab.cpp"
    break;

  case 40: /* elem_expr: SQRT '(' expression ')'  */
#line 272 "src/parser.y"
                                      {
                (yyval.stmt) = new std::string("sqrt(" + *(yyvsp[-1].stmt) + ")");
                delete (yyvsp[-1].stmt);
                TRACE((yyval.stmt));
            }
#line 1851 "build/parser.tab.cpp"
    break;

  case 41: /* elem_expr: LOG '(' expression ',' expression ')'  */
#line 277 "src/parser.y"
                                                    {
                (yyval.stmt) = new std::string("log(" + *(yyvsp[-3].stmt) + ", " + *(yyvsp[-1].stmt) + ")");
                delete (yyvsp[-3].stmt); delete (yyvsp[-1].stmt);
                TRACE((yyval.stmt));
            }
#line 1861 "build/parser.tab.cpp"
    break;

  case 42: /* elem_expr: EXP '(' expression ')'  */
#line 282 "src/parser.y"
                                     {
                (yyval.stmt) = new std::string("exp(" + *(yyvsp[-1].stmt) + ")");
                delete (yyvsp[-1].stmt);
                TRACE((yyval.stmt));
            }
#line 1871 "build/parser.tab.cpp"
    break;

  case 43: /* elem_expr: RANDOM '(' ')'  */
#line 288 "src/parser.y"
                             {
                (yyval.stmt) = new std::string("random()");
                TRACE((yyval.stmt));
            }
#line 1880 "build/parser.tab.cpp"
    break;

  case 44: /* elem_expr: E  */
#line 292 "src/parser.y"
                {
                (yyval.stmt) = new std::string("e");
                TRACE((yyval.stmt));
            }
#line 1889 "build/parser.tab.cpp"
    break;

  case 45: /* elem_expr: PI  */
#line 296 "src/parser.y"
                 {
                (yyval.stmt) = new std::string("pi");
                TRACE((yyval.stmt));
            }
#line 1898 "build/parser.tab.cpp"
    break;

  case 46: /* elem_expr: expression CONCAT expression  */
#line 301 "src/parser.y"
                                           {
                (yyval.stmt) = new std::string(*(yyvsp[-2].stmt) + *(yyvsp[0].stmt)); // string concatenation
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1908 "build/parser.tab.cpp"
    break;

  case 47: /* elem_expr: expression CONCAT_SPACE expression  */
#line 306 "src/parser.y"
                                                 {
                (yyval.stmt) = new std::string(*(yyvsp[-2].stmt) + " " + *(yyvsp[0].stmt));
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1918 "build/parser.tab.cpp"
    break;

  case 48: /* elem_expr: expression LT expression  */
#line 312 "src/parser.y"
                                       {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " < " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1928 "build/parser.tab.cpp"
    break;

  case 49: /* elem_expr: expression GT expression  */
#line 317 "src/parser.y"
                                       {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " > " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1938 "build/parser.tab.cpp"
    break;

  case 50: /* elem_expr: expression LE expression  */
#line 322 "src/parser.y"
                                       {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " <= " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1948 "build/parser.tab.cpp"
    break;

  case 51: /* elem_expr: expression GE expression  */
#line 327 "src/parser.y"
                                       {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " >= " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1958 "build/parser.tab.cpp"
    break;

  case 52: /* elem_expr: expression EQ expression  */
#line 333 "src/parser.y"
                                       {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " == " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1968 "build/parser.tab.cpp"
    break;

  case 53: /* elem_expr: expression NE expression  */
#line 338 "src/parser.y"
                                       {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " != " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1978 "build/parser.tab.cpp"
    break;

  case 54: /* elem_expr: expression AND expression  */
#line 344 "src/parser.y"
                                        {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " && " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1988 "build/parser.tab.cpp"
    break;

  case 55: /* elem_expr: expression OR expression  */
#line 349 "src/parser.y"
                                       {
                (yyval.stmt) = new std::string("(" + *(yyvsp[-2].stmt) + " || " + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[-2].stmt); delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 1998 "build/parser.tab.cpp"
    break;

  case 56: /* elem_expr: NOT expression  */
#line 354 "src/parser.y"
                             {
                (yyval.stmt) = new std::string("(!" + *(yyvsp[0].stmt) + ")");
                delete (yyvsp[0].stmt);
                TRACE((yyval.stmt));
            }
#line 2008 "build/parser.tab.cpp"
    break;

  case 57: /* block_expr: '{' block_body '}'  */
#line 362 "src/parser.y"
                                {
                                    (yyval.stmt) = new std::string("block"); // Placeholder
                                    delete (yyvsp[-1].list);
                                }
#line 2017 "build/parser.tab.cpp"
    break;

  case 58: /* block_body: %empty  */
#line 369 "src/parser.y"
                                            { (yyval.list) = new std::vector<std::string*>(); }
#line 2023 "build/parser.tab.cpp"
    break;

  case 59: /* block_body: statement  */
#line 370 "src/parser.y"
                                            { (yyval.list) = new std::vector<std::string*>(); (yyval.list)->push_back((yyvsp[0].stmt)); }
#line 2029 "build/parser.tab.cpp"
    break;

  case 60: /* block_body: block_body statement  */
#line 371 "src/parser.y"
                                            { (yyvsp[-1].list)->push_back((yyvsp[0].stmt)); (yyval.list) = (yyvsp[-1].list); }
#line 2035 "build/parser.tab.cpp"
    break;

  case 61: /* params: %empty  */
#line 375 "src/parser.y"
                                { (yyval.list) = new std::vector<std::string*>(); }
#line 2041 "build/parser.tab.cpp"
    break;

  case 62: /* params: ID  */
#line 376 "src/parser.y"
                                { (yyval.list) = new std::vector<std::string*>(); (yyval.list)->push_back((yyvsp[0].str)); }
#line 2047 "build/parser.tab.cpp"
    break;

  case 63: /* params: params ',' ID  */
#line 377 "src/parser.y"
                                { (yyvsp[-2].list)->push_back((yyvsp[0].str)); (yyval.list) = (yyvsp[-2].list); }
#line 2053 "build/parser.tab.cpp"
    break;

  case 64: /* func_call_expr: ID '(' args ')'  */
#line 381 "src/parser.y"
                                { (yyval.stmt) = new std::string("calling function " + *(yyvsp[-3].str)); }
#line 2059 "build/parser.tab.cpp"
    break;

  case 65: /* args: %empty  */
#line 385 "src/parser.y"
                                        { (yyval.list) = new std::vector<std::string*>(); }
#line 2065 "build/parser.tab.cpp"
    break;

  case 66: /* args: expression  */
#line 386 "src/parser.y"
                                        { (yyval.list) = new std::vector<std::string*>(); (yyval.list)->push_back((yyvsp[0].stmt)); }
#line 2071 "build/parser.tab.cpp"
    break;

  case 67: /* args: args ',' expression  */
#line 387 "src/parser.y"
                                        { (yyvsp[-2].list)->push_back((yyvsp[0].stmt)); (yyval.list) = (yyvsp[-2].list); }
#line 2077 "build/parser.tab.cpp"
    break;

  case 68: /* assign_expr: ID REASSIGN expression  */
#line 391 "src/parser.y"
                                      { (yyval.stmt) = new std::string("cambio de valor de la variable " + *(yyvsp[-2].str)); }
#line 2083 "build/parser.tab.cpp"
    break;

  case 69: /* let_expr: decls IN body  */
#line 395 "src/parser.y"
                                           { (yyval.stmt) = new std::string("let_expr"); delete (yyvsp[-2].list); }
#line 2089 "build/parser.tab.cpp"
    break;

  case 70: /* let_expr: decls IN '(' body ')'  */
#line 396 "src/parser.y"
                                           { (yyval.stmt) = new std::string("let_expr"); delete (yyvsp[-4].list); }
#line 2095 "build/parser.tab.cpp"
    break;

  case 71: /* let_expr: decls IN body ';'  */
#line 397 "src/parser.y"
                                           { (yyval.stmt) = new std::string("let_expr"); delete (yyvsp[-3].list); }
#line 2101 "build/parser.tab.cpp"
    break;

  case 72: /* let_expr: decls IN '(' body ')' ';'  */
#line 398 "src/parser.y"
                                           { (yyval.stmt) = new std::string("let_expr"); delete (yyvsp[-5].list); }
#line 2107 "build/parser.tab.cpp"
    break;

  case 73: /* decls: LET decl  */
#line 403 "src/parser.y"
                                        { (yyval.list) = (yyvsp[0].list); }
#line 2113 "build/parser.tab.cpp"
    break;

  case 74: /* decls: decls ',' LET decl  */
#line 404 "src/parser.y"
                                        { (yyvsp[-3].list)->insert((yyvsp[-3].list)->end(), (yyvsp[0].list)->begin(), (yyvsp[0].list)->end()); delete (yyvsp[0].list); (yyval.list) = (yyvsp[-3].list); }
#line 2119 "build/parser.tab.cpp"
    break;

  case 75: /* decl: ID '=' expression  */
#line 408 "src/parser.y"
                                            {
                                                std::vector<std::string*>* list = new std::vector<std::string*>();
                                                list->push_back(new std::string(*(yyvsp[-2].str) + " = " + *(yyvsp[0].stmt)));
                                                delete (yyvsp[-2].str); delete (yyvsp[0].stmt);
                                                (yyval.list) = list;
                                            }
#line 2130 "build/parser.tab.cpp"
    break;

  case 76: /* decl: decl ',' ID '=' expression  */
#line 414 "src/parser.y"
                                            {
                                                std::string* new_decl = new std::string(*(yyvsp[-2].str) + " = " + *(yyvsp[0].stmt));
                                                (yyvsp[-4].list)->insert((yyvsp[-4].list)->end(), new_decl);
                                                delete (yyvsp[-2].str); delete (yyvsp[0].stmt);
                                                (yyval.list) = (yyvsp[-4].list);
                                            }
#line 2141 "build/parser.tab.cpp"
    break;

  case 79: /* body: PRINT '(' expression ')'  */
#line 425 "src/parser.y"
                                            { std::cout << "Salida: " << *(yyvsp[-1].stmt) << std::endl; }
#line 2147 "build/parser.tab.cpp"
    break;

  case 80: /* if_expr: if_head  */
#line 429 "src/parser.y"
                                        { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2153 "build/parser.tab.cpp"
    break;

  case 81: /* if_expr: if_head ELSE body  */
#line 430 "src/parser.y"
                                        { (yyval.stmt) = new std::string("if-else"); delete (yyvsp[-2].stmt); }
#line 2159 "build/parser.tab.cpp"
    break;

  case 82: /* if_head: IF '(' expression ')' body  */
#line 434 "src/parser.y"
                                                            { (yyval.stmt) = new std::string("if"); }
#line 2165 "build/parser.tab.cpp"
    break;

  case 83: /* if_head: if_head ELIF '(' expression ')' body  */
#line 435 "src/parser.y"
                                                            { (yyval.stmt) = new std::string("if-elif"); delete (yyvsp[-5].stmt); }
#line 2171 "build/parser.tab.cpp"
    break;

  case 84: /* while_expr: WHILE '(' expression ')' body  */
#line 439 "src/parser.y"
                                                            { (yyval.stmt) = new std::string("while"); }
#line 2177 "build/parser.tab.cpp"
    break;

  case 85: /* for_expr: FOR '(' ID IN RANGE '(' expression ',' expression ')' ')' body  */
#line 443 "src/parser.y"
                                                                                { (yyval.stmt) = new std::string("for"); }
#line 2183 "build/parser.tab.cpp"
    break;


#line 2187 "build/parser.tab.cpp"

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

#line 446 "src/parser.y"


void yyerror(const char *msg) {
    fprintf(stderr, "Error en línea %d, columna %d: %s\n",
            yylloc.first_line, yylloc.first_column, msg);
}


