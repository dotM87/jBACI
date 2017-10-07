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
#line 1 "bapascal.y" /* yacc.c:339  */

       /* yacc grammar for BACI Concurrent Pascal */
       /* based on the Pascal grammar from the      */
       /* primost.cs.wisc.edu compiler archive      */
#include <stdio.h>
#include <string.h>
#include <time.h>

extern   int   yylineno;
extern   int   yydebug;
extern   char  yytext[];
extern   char  numbertext[];
extern   int   numberbase;
extern   char  laststring[];
extern   char  lastident[];
extern   char  lasttoken[];

extern   void  global_init(int argc, char** argv);


#include "../include/globdata.h"
#include "../include/globtabs.h"
#include "../include/compdata.h"
#include "../include/genutil.h"
#include "../include/computil.h"
#include "../include/incfiles.h"
#include "../include/pcode.h"

extern   int fprintf(FILE*,const char*,...);
extern   int printf(const char*,...);
/* extern   int free(void*); */
extern   void yyerror(char*);
extern   int yylex(void); 
extern   int atoi(char*);

void process_var_inits(int level);
void add_var_init(int* curr_var, int tix, int level, int value);

#define YYDEBUG 1

char  comp_tail[] = {"Pascal to PCODE Compiler in C, "};
extern char  comp_date[];  /* set in date.c on compilation day */

char  source_suffix[] = {".pm"};
char  list_suffix[] = {".lst"};

extern FILE  *yyin;  /* source file */
extern FILE  *yyout; /* listing file */

BUFFER   pbuf;   /* string buffer for parsing */
/* constants for passing synthesized information back up the parse tree */
int dx;           /* to calc size of the stackframe of a proc or function */
int level=0;      /* current static level */
int in_mon_decl;  /* 1 if in a monitor decl, 0 otherwise */
int in_mon_init;  /* 1 if in monitor init code, 0 otherwise */

/* for passing type of constant back to constant productions */
TYPES consttype;
/* for passing back information to productions using 'type' prod */

extern FNAME_STRING test;
#define NO_EXPR -1
   /* to pass up the parse tree when no entry in the expr array */
   /* has been generated                                        */

#define MAXARNEST 50    /* max nesting in function calls */
struct {
   int   aref;    /* atab index */
   TYPES eltyp;   /* element type for this atab index */
} arstack[MAXARNEST];
int   topars = -1;   /* index of top item on arstack */
int   last_aref;  /* for calculating array references */
TYPES last_eltyp;

#define MAXPFNEST 50    /* max nesting in function calls */
struct {
   int   pct;     /* parm count for this call */
   int   tix;     /* tab index for this proc or function */
} pfstack[MAXPFNEST];
int   toppfs = -1;   /* index of top item on pfstack */
int   last_pfv;   /* tab index of last proc func or variable ref */
int   last_pf;    /* tab index of last proc or func ref */
int   parmct;     /* count of parms in a proc or func call */
int   prt;        /* tab index of current proc */

#define MAXVARINIT   100   /* max # of vbls to be initialized */
   /* at present, only monitors need this data structure,     */
   /* because the grammar allows variable declaration and     */
   /* function declaration to be intermixed in the outer      */
   /* level of the monitor.  The actual intialization has to  */
   /* be postponed until the monitor init code is generated.  */
   /* The var_init list is never shortened.  The lev field of */
   /* a variable is set to -1 after it's initialized.         */
struct {
   int   value;         /* value of the variable */
   int   lev;           /* level of the variable  */
   int   tix;           /* tab index of the variable */
}var_init[MAXVARINIT];  
int   curr_var = -1;

int   in_cobegin = 0;  /* 1 when in COBEGIN-COEND block */
int   atomic;     /* pass ATOMIC proc or func up the parse tree */

int   in_include = 0;  /* to signal yylex to change its tactics */

extern int last_predeclared;  /* tab index of last predeclared id */

int making_objfile = 0; /* 1 if -c option --> make an object file, else 0 */

int external = 0;    /* 1 if an external var decl is underway, else 0 */

int  sscanf_cnt = 0;    /* number of sscanf addresses pushed */

int  sprintf_cnt = 0;   /* number of sprintf addresses pushed */

int outer_call_lc;      /* lc of the SHORTCALL instr to the outerblock */
int ob_tix;             /* tab index of the outer block */
int main_declared = 0;  /* 1 if the PROGRAM keyword has been seen */

int in_proc_decl;       /* 1 if in a procedure or function decl, 0 otherwise */


#define emit_push_addr(x)  (\
emit2(((x).normal ? LOAD_ADDR : LOAD_VALUE), (x).lev, (x).adr))


#line 193 "y.tab.c" /* yacc.c:339  */

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
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    UNSIGNED_INT = 258,
    STRING = 259,
    RAWSTRING = 260,
    STRINGCONCAT = 261,
    STRINGCOMPARE = 262,
    STRINGCOPY = 263,
    STRINGLENGTH = 264,
    SSCANF = 265,
    SPRINTF = 266,
    IDENTIFIER = 267,
    CHAR = 268,
    INT = 269,
    NE = 270,
    LE = 271,
    GE = 272,
    BECOMES = 273,
    DIV = 274,
    MOD = 275,
    OR = 276,
    AND = 277,
    NOT = 278,
    DOTDOT = 279,
    IF = 280,
    THEN = 281,
    ELSE = 282,
    CASE = 283,
    OF = 284,
    REPEAT = 285,
    UNTIL = 286,
    WHILE = 287,
    DO = 288,
    FOR = 289,
    TO = 290,
    CBEGIN = 291,
    SBEGIN = 292,
    END = 293,
    CEND = 294,
    CONST = 295,
    VAR = 296,
    TYPE = 297,
    ARRAY = 298,
    FUNCTION = 299,
    MONITOR = 300,
    PROCEDURE = 301,
    PROGRAM = 302,
    INCLUDE = 303,
    ATOMIC = 304,
    FNSTRING = 305,
    SEND = 306,
    RECEIVE = 307,
    BROADCAST = 308,
    EXTERNAL = 309,
    LOWER_THAN_ELSE = 310
  };
#endif
/* Tokens.  */
#define UNSIGNED_INT 258
#define STRING 259
#define RAWSTRING 260
#define STRINGCONCAT 261
#define STRINGCOMPARE 262
#define STRINGCOPY 263
#define STRINGLENGTH 264
#define SSCANF 265
#define SPRINTF 266
#define IDENTIFIER 267
#define CHAR 268
#define INT 269
#define NE 270
#define LE 271
#define GE 272
#define BECOMES 273
#define DIV 274
#define MOD 275
#define OR 276
#define AND 277
#define NOT 278
#define DOTDOT 279
#define IF 280
#define THEN 281
#define ELSE 282
#define CASE 283
#define OF 284
#define REPEAT 285
#define UNTIL 286
#define WHILE 287
#define DO 288
#define FOR 289
#define TO 290
#define CBEGIN 291
#define SBEGIN 292
#define END 293
#define CEND 294
#define CONST 295
#define VAR 296
#define TYPE 297
#define ARRAY 298
#define FUNCTION 299
#define MONITOR 300
#define PROCEDURE 301
#define PROGRAM 302
#define INCLUDE 303
#define ATOMIC 304
#define FNSTRING 305
#define SEND 306
#define RECEIVE 307
#define BROADCAST 308
#define EXTERNAL 309
#define LOWER_THAN_ELSE 310

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 354 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   545

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  122
/* YYNRULES -- Number of rules.  */
#define YYNRULES  217
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  368

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    58,     2,     2,     2,     2,
      66,    67,    70,    60,    64,    61,    57,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    65,    56,
      68,    59,    69,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    62,     2,    63,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   145,   145,   158,   165,   175,   188,   189,   192,   193,
     194,   195,   196,   197,   198,   201,   202,   203,   206,   210,
     216,   220,   222,   226,   235,   236,   239,   260,   269,   282,
     283,   286,   287,   290,   291,   292,   293,   294,   295,   298,
     310,   316,   328,   331,   332,   335,   343,   345,   347,   349,
     351,   359,   367,   371,   375,   378,   379,   382,   401,   402,
     403,   406,   431,   475,   477,   480,   497,   508,   511,   526,
     529,   530,   533,   598,   601,   602,   606,   607,   615,   618,
     625,   634,   642,   650,   651,   656,   659,   676,   684,   691,
     703,   721,   722,   726,   727,   730,   731,   732,   735,   759,
     783,   803,   814,   833,   840,   846,   849,   852,   862,   865,
     866,   867,   869,   873,   874,   875,   876,   877,   878,   880,
     882,   884,   886,   888,   892,   896,   900,   904,   908,   912,
     916,   930,   940,   949,   961,   977,   995,  1020,  1056,  1057,
    1060,  1061,  1064,  1073,  1084,  1089,  1115,  1116,  1117,  1118,
    1119,  1122,  1149,  1151,  1165,  1179,  1192,  1204,  1208,  1221,
    1234,  1242,  1253,  1266,  1281,  1282,  1299,  1300,  1301,  1302,
    1303,  1304,  1307,  1308,  1317,  1328,  1351,  1355,  1356,  1357,
    1360,  1361,  1388,  1392,  1393,  1394,  1395,  1398,  1399,  1407,
    1415,  1416,  1429,  1479,  1480,  1481,  1485,  1505,  1506,  1507,
    1508,  1511,  1530,  1549,  1560,  1572,  1586,  1611,  1626,  1637,
    1646,  1657,  1670,  1701,  1711,  1723,  1730,  1734
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "UNSIGNED_INT", "STRING", "RAWSTRING",
  "STRINGCONCAT", "STRINGCOMPARE", "STRINGCOPY", "STRINGLENGTH", "SSCANF",
  "SPRINTF", "IDENTIFIER", "CHAR", "INT", "NE", "LE", "GE", "BECOMES",
  "DIV", "MOD", "OR", "AND", "NOT", "DOTDOT", "IF", "THEN", "ELSE", "CASE",
  "OF", "REPEAT", "UNTIL", "WHILE", "DO", "FOR", "TO", "CBEGIN", "SBEGIN",
  "END", "CEND", "CONST", "VAR", "TYPE", "ARRAY", "FUNCTION", "MONITOR",
  "PROCEDURE", "PROGRAM", "INCLUDE", "ATOMIC", "FNSTRING", "SEND",
  "RECEIVE", "BROADCAST", "EXTERNAL", "LOWER_THAN_ELSE", "';'", "'.'",
  "'#'", "'='", "'+'", "'-'", "'['", "']'", "','", "':'", "'('", "')'",
  "'<'", "'>'", "'*'", "$accept", "program", "init_outerscope",
  "outer_decls", "outer_decl", "extern_decl", "extern_var_decl",
  "extern_proc_decl", "the_external", "proc_or_func_proto",
  "extern_mon_decl", "proto_list", "pgm_decl", "pgm_hdg_decls",
  "pgm_heading", "opt_declarations", "declarations", "declaration",
  "include_dcl_part", "start_include", "const_dcl_part", "const_defs",
  "const_def", "constant", "unsigned_num", "type_dcl_part", "type_defs",
  "type_def", "type", "simple_type", "struct_type", "index_type",
  "dimension", "low", "last_atab", "string_type", "var_dcl_part",
  "variable_dcls", "variable_dcl", "newident_list", "new_id_list",
  "opt_initializer", "proc_dcl_part", "proc_or_func", "proc_hdg_decl",
  "proc_proto", "opt_atomic", "proc_heading", "proc_id", "func_hdg_decl",
  "func_proto", "func_heading", "func_id", "formal_params",
  "formal_p_sects", "formal_p_sect", "param_group", "var_param_group",
  "string_param_group", "mon_dcl_part", "mon_heading", "mon_decl",
  "opt_stmt_part", "compound_stmt", "the_begin", "statement_part",
  "statements", "statement", "if_expr", "then_stmt", "the_else",
  "the_while", "while_expr", "the_repeat", "for_id", "for_header",
  "for_id_becomes_expr", "cbegin", "assignment", "id_becomes",
  "procedure_call", "actual_params", "actuals_list", "actual_param",
  "pfv_id", "check_proc", "special_proc_call", "send_call",
  "left_exprparm", "broadcast_call", "stringconcat_call",
  "right_stringparm", "rawstring_parm", "stringcopy_call", "sprintf_call",
  "sprintf_buf_fmt", "sprintf_parmlist", "expr", "relational_op",
  "simple_expr", "left_simp_expr", "add_op", "term", "left_term",
  "mult_op", "factor", "variable", "check_func", "special_func_call",
  "stringcompare_call", "stringlength_call", "single_exprparm",
  "receive_call", "sscanf_call", "sscanf_buf_fmt", "sscanf_parmlist",
  "array_var", "array_id", "index_expr", "typecast", "ident", "newident", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,    59,    46,    35,    61,
      43,    45,    91,    93,    44,    58,    40,    41,    60,    62,
      42
};
# endif

#define YYPACT_NINF -238

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-238)))

#define YYTABLE_NINF -213

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -238,    21,   224,  -238,    24,    24,    24,    24,    24,  -238,
    -238,    97,    88,  -238,  -238,  -238,  -238,    98,  -238,  -238,
      69,   107,  -238,    11,  -238,  -238,  -238,  -238,  -238,    69,
     170,   147,    69,   170,  -238,   160,  -238,   126,  -238,   100,
     140,  -238,   138,   152,  -238,   165,  -238,   174,   178,  -238,
    -238,  -238,  -238,    24,  -238,  -238,  -238,   175,  -238,  -238,
     366,   182,   170,  -238,  -238,   179,  -238,   160,  -238,  -238,
    -238,  -238,  -238,  -238,  -238,   228,   235,   192,   183,   194,
     183,   197,  -238,  -238,   176,    24,    71,    24,    35,    24,
      24,    35,  -238,   199,  -238,    -1,   -16,   201,   201,   202,
    -238,   304,  -238,  -238,   258,  -238,   201,   208,  -238,    58,
    -238,   254,   304,   400,   257,   492,   252,   427,  -238,   304,
    -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,   227,   274,
     231,     0,  -238,  -238,  -238,  -238,  -238,  -238,  -238,    83,
    -238,  -238,   229,  -238,  -238,   239,  -238,  -238,  -238,  -238,
     105,   105,  -238,  -238,  -238,  -238,   234,   236,   279,  -238,
    -238,  -238,  -238,  -238,  -238,  -238,    24,   244,  -238,  -238,
    -238,   185,   238,   238,   304,   201,   208,   237,  -238,   242,
     331,   201,   331,   331,   304,  -238,  -238,  -238,   425,   428,
      32,    50,  -238,  -238,  -238,  -238,  -238,  -238,  -238,   240,
    -238,  -238,   243,  -238,   248,   249,  -238,  -238,   492,   492,
     292,   287,  -238,    12,   304,  -238,   304,    66,  -238,   255,
     304,   304,   304,  -238,    24,   259,    51,  -238,  -238,  -238,
    -238,   258,  -238,  -238,  -238,  -238,  -238,    71,    71,    71,
    -238,  -238,  -238,  -238,  -238,   185,  -238,  -238,   261,   238,
    -238,   304,   304,  -238,    74,    32,    32,   256,   255,  -238,
    -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,   304,   331,
    -238,  -238,  -238,  -238,   331,   304,   304,   262,   263,  -238,
    -238,  -238,   492,   492,   304,  -238,   289,  -238,   276,  -238,
      94,  -238,  -238,  -238,   166,   266,    38,    83,  -238,  -238,
     269,   302,   168,  -238,    71,  -238,   268,   272,   328,  -238,
     282,   280,   304,  -238,  -238,  -238,     4,    25,    32,  -238,
    -238,   103,   281,  -238,  -238,  -238,  -238,  -238,  -238,  -238,
     119,  -238,  -238,   304,  -238,  -238,   304,   258,  -238,  -238,
    -238,  -238,  -238,  -238,    71,  -238,  -238,  -238,  -238,   328,
    -238,   283,   304,  -238,  -238,   276,  -238,  -238,  -238,  -238,
     320,  -238,  -238,  -238,  -238,  -238,    35,  -238
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,    83,     1,     0,     0,     0,     0,     0,    84,
      20,     0,    83,     6,    14,    15,    16,    83,    17,     4,
       0,     0,     8,     0,    12,    11,    13,     9,    78,     0,
      83,     0,     0,    83,    10,    29,   217,     0,    44,     0,
       0,    71,     0,    73,    75,     0,    56,     0,     0,    28,
      40,     7,     3,     0,    19,    21,    22,    83,   107,   108,
       0,     0,    83,    39,    41,     0,    81,    30,    32,    33,
      34,    35,    36,    37,    38,     0,     0,     0,    91,     0,
      91,     0,    87,   103,     0,    42,     0,    69,     0,     0,
      54,     0,   102,     0,    24,    83,     0,     0,     0,     0,
     216,     0,   129,   127,     0,   133,     0,     0,   114,     0,
     110,     0,     0,     0,     0,   113,     0,     0,   115,     0,
     116,   145,   117,   146,   147,   148,   149,   150,     0,     0,
       0,   144,    26,    27,    79,    31,    90,    86,    82,     0,
      85,    88,     0,    80,   104,     0,   105,    43,    53,    52,
       0,     0,    45,    46,    49,    70,     0,     0,    76,    58,
      59,    60,    61,    74,    55,    57,    18,     0,    25,   112,
     111,     0,     0,     0,     0,     0,     0,     0,   189,     0,
       0,     0,     0,     0,     0,   188,   196,   124,   164,     0,
     172,     0,   180,   187,   193,   197,   198,   199,   200,     0,
     194,   195,   144,   130,     0,     0,   154,   106,   113,   113,
     118,     0,   128,     0,     0,   122,     0,     0,   134,   138,
       0,     0,     0,   136,     0,     0,     0,    94,    95,    96,
      97,     0,   101,    47,    50,    48,    51,     0,     0,     0,
      72,    23,   158,   152,   153,     0,   155,   159,     0,     0,
     202,     0,     0,   191,     0,   173,   174,     0,   138,   171,
     169,   170,   179,   166,   177,   178,   167,   168,     0,     0,
     184,   185,   186,   183,     0,     0,     0,     0,     0,   109,
     125,   126,   113,   113,     0,   132,     0,   123,     0,   137,
       0,   162,   135,   214,     0,     0,     0,     0,    92,    89,
       0,     0,     0,    64,     0,    77,     0,     0,     0,   201,
       0,     0,     0,   205,   190,   192,   165,     0,   175,   181,
     209,     0,     0,   204,   203,   119,   120,   121,   131,   143,
       0,   141,   142,     0,   160,   211,     0,     0,   100,    98,
      93,    68,    66,    67,     0,    65,   157,   156,   161,     0,
     215,     0,     0,   207,   151,     0,   139,   163,   213,    99,
       0,    63,   208,   206,   210,   140,     0,    62
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -238,  -238,  -238,  -238,   339,  -238,  -238,  -238,  -238,   -46,
    -238,  -238,   340,  -238,  -238,    47,  -238,   286,    33,  -238,
     111,  -238,   270,   -77,   -84,   132,  -238,   267,   -90,  -238,
    -238,  -238,    14,  -238,  -238,  -238,   162,   306,   -72,  -132,
    -238,  -238,   164,  -238,  -238,     2,  -238,  -238,  -238,  -238,
       6,  -238,  -238,   288,  -238,    63,  -238,  -238,  -238,   173,
     344,  -238,  -238,     9,  -238,   155,    23,  -110,  -238,  -238,
    -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,
    -238,   104,  -238,     8,   -57,  -238,  -238,  -238,   -94,  -238,
    -238,  -146,  -237,  -238,  -238,  -238,  -238,   -95,  -238,   101,
     108,  -238,  -169,  -238,  -238,  -170,  -238,  -238,  -238,  -238,
    -238,   190,  -238,  -238,  -238,  -238,   -40,  -238,  -238,  -238,
     -60,    26
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    12,    13,    14,    15,    16,    17,    54,
      18,    95,    19,    20,    21,    66,    67,    68,    69,    23,
      70,    37,    38,   301,   185,    71,    45,    46,   158,   159,
     160,   302,   303,   304,   360,   161,    72,    40,    41,    42,
      43,   240,    73,    28,    29,    30,    31,    77,    78,    32,
      33,    79,    80,   140,   226,   227,   228,   229,   230,    74,
      35,    84,   145,   108,    60,    61,   109,   110,   111,   210,
     282,   112,   211,   113,   114,   115,   116,   117,   118,   119,
     120,   289,   330,   331,   186,   219,   122,   123,   172,   124,
     125,   246,   243,   126,   127,   128,   290,   332,   268,   188,
     189,   269,   190,   191,   274,   192,   193,   258,   194,   195,
     196,   206,   197,   198,   199,   321,   200,   130,   294,   201,
     202,    44
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     131,   165,   153,   121,   173,   215,   187,   225,   306,   152,
     253,    94,   204,   255,   256,   155,    63,   212,   223,    55,
     129,     3,   169,    56,   218,  -176,   154,   247,   162,    59,
      39,   162,    47,    48,    49,    22,    36,   167,    59,   156,
     170,    59,   338,   284,   203,    22,   262,   100,     9,   168,
     100,  -182,  -182,   131,  -182,   131,   121,   131,   121,    55,
     121,    64,  -212,    56,  -176,  -176,   233,   235,   208,   270,
     271,   348,   272,   129,   148,   129,   244,   129,   157,   248,
      82,   249,    83,   100,   149,   264,   265,   254,    -2,   257,
     234,   236,   295,   146,   155,    36,   207,    55,   279,   280,
     318,    56,  -182,   309,   319,   287,    58,   297,   148,   133,
     278,    39,   362,    24,   208,   163,    47,   100,   298,   285,
     273,   286,   208,    24,   224,   291,   292,   293,     4,     5,
       6,   150,   151,     7,    25,     8,   213,     9,   312,    53,
     217,   313,    10,     7,    25,    50,    11,     9,   131,   131,
     307,   121,   121,   153,   153,   153,   310,   311,   333,    86,
     300,   334,   305,    62,    26,   225,    27,   352,   129,   129,
     353,   299,   325,   326,    26,    34,    27,   154,   154,   154,
     320,   322,    85,   355,    65,    34,   356,    81,   148,   327,
     242,    75,   175,    76,   176,   177,    87,   100,   178,   179,
       4,     5,     6,    88,   -83,     7,   -83,   -29,   180,     9,
       4,     5,     6,    58,   144,     7,    89,   351,    11,     9,
     153,    90,   131,   131,     9,   121,   121,   345,    11,   335,
     336,   343,   344,    91,    92,   134,   339,   181,   357,   132,
     136,   358,   129,   129,   154,   182,   183,   137,   138,   139,
     141,   184,   148,   143,   277,   166,   175,   364,   176,   177,
     153,   100,   178,   179,     4,     5,     6,   171,   174,     7,
     100,     8,   180,     9,   205,   214,   367,   359,    10,   148,
     209,   329,    11,   175,   154,   176,   177,   216,   100,   178,
     179,   220,   221,   222,   231,   232,   237,   239,   238,   180,
     241,   181,   245,   251,   275,  -212,   162,   148,   252,   182,
     183,   175,   276,   176,   177,   184,   100,   178,   179,   281,
     283,   288,   328,   314,   296,   308,   342,   180,   181,   323,
     324,   337,   341,   242,   148,   346,   182,   183,   175,   347,
     176,   177,   184,   100,   178,   179,   349,   350,   354,   366,
     363,    51,    52,   135,   180,   147,   181,   164,   361,    93,
     340,    57,   315,   365,   182,   183,   250,    96,   142,   316,
     184,     0,    97,     0,    98,     0,   317,    99,   100,     0,
       0,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,     0,     0,   102,   184,   103,     0,
     104,    96,   105,    58,  -113,     0,    97,     0,    98,     0,
       0,    99,   100,     0,     0,     0,     0,   106,     0,   107,
       0,     0,  -113,     0,     0,   101,     0,     0,    96,     0,
     102,  -113,   103,    97,   104,    98,   105,    58,    99,   100,
    -176,  -176,  -176,   259,   260,   261,  -176,     0,     0,   262,
       0,   106,   101,   107,     0,     0,  -113,   102,     0,   103,
       0,   104,     0,   105,    58,     0,  -113,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,     0,
     107,     0,     0,  -113,  -176,  -176,  -176,   263,   264,   265,
       0,     0,     0,  -176,  -176,     0,   266,   267,    97,     0,
      98,     0,     0,    99,   100,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
       0,     0,   102,     0,   103,     0,   104,     0,   105,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,     0,   107
};

static const yytype_int16 yycheck[] =
{
      60,    91,    86,    60,    98,   115,   101,   139,   245,    86,
     180,    57,   106,   182,   183,    87,     5,   112,    18,    17,
      60,     0,    38,    17,   119,    21,    86,   173,    88,    20,
       4,    91,     6,     7,     8,     2,    12,    38,    29,     4,
      56,    32,     4,    31,   104,    12,    21,    12,    49,    95,
      12,    19,    20,   113,    22,   115,   113,   117,   115,    57,
     117,    50,    62,    57,    60,    61,   150,   151,    56,    19,
      20,   308,    22,   113,     3,   115,   171,   117,    43,   174,
      33,   175,    35,    12,    13,    60,    61,   181,     0,   184,
     150,   151,   224,    84,   166,    12,    38,    95,   208,   209,
     269,    95,    70,   249,   274,    39,    37,    56,     3,    62,
     205,    85,   349,     2,    56,    89,    90,    12,    67,   214,
      70,   216,    56,    12,    41,   220,   221,   222,    40,    41,
      42,    60,    61,    45,     2,    47,   113,    49,    64,    41,
     117,    67,    54,    45,    12,    48,    58,    49,   208,   209,
     245,   208,   209,   237,   238,   239,   251,   252,    64,    59,
     237,    67,   239,    56,     2,   297,     2,    64,   208,   209,
      67,   231,   282,   283,    12,     2,    12,   237,   238,   239,
     275,   276,    56,    64,    29,    12,    67,    32,     3,   284,
       5,    44,     7,    46,     9,    10,    56,    12,    13,    14,
      40,    41,    42,    65,    44,    45,    46,    37,    23,    49,
      40,    41,    42,    37,    38,    45,    64,   312,    58,    49,
     304,    56,   282,   283,    49,   282,   283,   304,    58,    63,
      64,    63,    64,    59,    56,    56,   296,    52,   333,    57,
      12,   336,   282,   283,   304,    60,    61,    12,    56,    66,
      56,    66,     3,    56,     5,    56,     7,   352,     9,    10,
     344,    12,    13,    14,    40,    41,    42,    66,    66,    45,
      12,    47,    23,    49,    66,    18,   366,   337,    54,     3,
      26,     5,    58,     7,   344,     9,    10,    35,    12,    13,
      14,    64,    18,    62,    65,    56,    62,    18,    62,    23,
      56,    52,    64,    66,    64,    62,   366,     3,    66,    60,
      61,     7,    64,     9,    10,    66,    12,    13,    14,    27,
      33,    66,    33,    67,    65,    64,    24,    23,    52,    67,
      67,    65,    63,     5,     3,    67,    60,    61,     7,    67,
       9,    10,    66,    12,    13,    14,    64,    67,    67,    29,
      67,    12,    12,    67,    23,    85,    52,    90,   344,    53,
     297,    17,   258,   355,    60,    61,   176,     1,    80,   268,
      66,    -1,     6,    -1,     8,    -1,   268,    11,    12,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    30,    66,    32,    -1,
      34,     1,    36,    37,    38,    -1,     6,    -1,     8,    -1,
      -1,    11,    12,    -1,    -1,    -1,    -1,    51,    -1,    53,
      -1,    -1,    56,    -1,    -1,    25,    -1,    -1,     1,    -1,
      30,    31,    32,     6,    34,     8,    36,    37,    11,    12,
      15,    16,    17,    15,    16,    17,    21,    -1,    -1,    21,
      -1,    51,    25,    53,    -1,    -1,    56,    30,    -1,    32,
      -1,    34,    -1,    36,    37,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      53,    -1,    -1,    56,    59,    60,    61,    59,    60,    61,
      -1,    -1,    -1,    68,    69,    -1,    68,    69,     6,    -1,
       8,    -1,    -1,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    30,    -1,    32,    -1,    34,    -1,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    53
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    72,    73,     0,    40,    41,    42,    45,    47,    49,
      54,    58,    74,    75,    76,    77,    78,    79,    81,    83,
      84,    85,    89,    90,    91,    96,   107,   113,   114,   115,
     116,   117,   120,   121,   130,   131,    12,    92,    93,   192,
     108,   109,   110,   111,   192,    97,    98,   192,   192,   192,
      48,    75,    83,    41,    80,   116,   121,   131,    37,   134,
     135,   136,    56,     5,    50,   136,    86,    87,    88,    89,
      91,    96,   107,   113,   130,    44,    46,   118,   119,   122,
     123,   136,    86,    86,   132,    56,    59,    56,    65,    64,
      56,    59,    56,   108,    80,    82,     1,     6,     8,    11,
      12,    25,    30,    32,    34,    36,    51,    53,   134,   137,
     138,   139,   142,   144,   145,   146,   147,   148,   149,   150,
     151,   155,   157,   158,   160,   161,   164,   165,   166,   187,
     188,   191,    57,    86,    56,    88,    12,    12,    56,    66,
     124,    56,   124,    56,    38,   133,   134,    93,     3,    13,
      60,    61,    94,    95,   191,   109,     4,    43,    99,   100,
     101,   106,   191,   192,    98,    99,    56,    38,    80,    38,
      56,    66,   159,   159,    66,     7,     9,    10,    13,    14,
      23,    52,    60,    61,    66,    95,   155,   168,   170,   171,
     173,   174,   176,   177,   179,   180,   181,   183,   184,   185,
     187,   190,   191,   191,   159,    66,   182,    38,    56,    26,
     140,   143,   168,   137,    18,   138,    35,   137,   168,   156,
      64,    18,    62,    18,    41,   110,   125,   126,   127,   128,
     129,    65,    56,    95,   191,    95,   191,    62,    62,    18,
     112,    56,     5,   163,   168,    64,   162,   162,   168,   159,
     182,    66,    66,   176,   159,   173,   173,   168,   178,    15,
      16,    17,    21,    59,    60,    61,    68,    69,   169,   172,
      19,    20,    22,    70,   175,    64,    64,     5,   168,   138,
     138,    27,   141,    33,    31,   168,   168,    39,    66,   152,
     167,   168,   168,   168,   189,   110,    65,    56,    67,   191,
      94,    94,   102,   103,   104,    94,   163,   168,    64,   162,
     168,   168,    64,    67,    67,   152,   170,   171,   173,   176,
     168,   186,   168,    67,    67,   138,   138,   168,    33,     5,
     153,   154,   168,    64,    67,    63,    64,    65,     4,   191,
     126,    63,    24,    63,    64,    94,    67,    67,   163,    64,
      67,   168,    64,    67,    67,    64,    67,   168,   168,   191,
     105,   103,   163,    67,   168,   154,    29,    99
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    71,    72,    72,    72,    73,    74,    74,    75,    75,
      75,    75,    75,    75,    75,    76,    76,    76,    77,    78,
      79,    80,    80,    81,    82,    82,    83,    84,    85,    86,
      86,    87,    87,    88,    88,    88,    88,    88,    88,    89,
      90,    89,    91,    92,    92,    93,    94,    94,    94,    94,
      94,    94,    94,    95,    96,    97,    97,    98,    99,    99,
      99,   100,   101,   102,   102,   103,   104,   105,   106,   107,
     108,   108,   109,   110,   111,   111,   112,   112,   113,   114,
     114,   115,   116,   117,   117,   118,   119,   120,   121,   122,
     123,   124,   124,   125,   125,   126,   126,   126,   127,   128,
     129,   130,   131,   132,   133,   133,   134,   135,   136,   137,
     137,   137,   137,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   149,   150,   151,   152,   152,
     153,   153,   154,   154,   155,   156,   157,   157,   157,   157,
     157,   158,   159,   159,   160,   161,   162,   162,   163,   164,
     165,   166,   167,   167,   168,   168,   169,   169,   169,   169,
     169,   169,   170,   170,   170,   170,   171,   172,   172,   172,
     173,   173,   174,   175,   175,   175,   175,   176,   176,   176,
     176,   176,   177,   177,   177,   177,   178,   179,   179,   179,
     179,   180,   181,   182,   182,   183,   183,   184,   185,   186,
     186,   187,   188,   189,   189,   190,   191,   192
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     3,     2,     0,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     2,
       1,     1,     1,     5,     1,     2,     3,     3,     2,     0,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     3,     3,     1,     3,     1,     2,     2,     1,
       2,     2,     1,     1,     3,     3,     1,     3,     1,     1,
       1,     1,     7,     3,     1,     2,     2,     0,     4,     3,
       3,     1,     4,     1,     3,     1,     0,     2,     1,     3,
       3,     2,     3,     0,     1,     2,     2,     2,     3,     4,
       2,     0,     3,     3,     1,     1,     1,     1,     3,     4,
       3,     4,     3,     1,     1,     1,     3,     1,     1,     3,
       1,     2,     2,     0,     1,     1,     1,     1,     2,     4,
       4,     4,     2,     3,     2,     2,     1,     1,     1,     1,
       2,     4,     3,     1,     2,     3,     2,     3,     0,     3,
       3,     1,     1,     1,     1,     0,     1,     1,     1,     1,
       1,     5,     2,     2,     2,     3,     3,     3,     1,     3,
       4,     5,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     3,     1,     1,     1,     0,     1,     1,     1,
       1,     3,     2,     3,     3,     3,     5,     4,     5,     1,
       3,     4,     1,     3,     1,     4,     1,     1
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
#line 146 "bapascal.y" /* yacc.c:1646  */
    {
         int i;
         tab[ob_tix].adr = lc;  /* outerblock entry pt */
         for (i = 0; i <= mon; i++){
             /* don't emit CALL_MONINIT for ext_monitor objects */
            if (tab[mtab[i]].obj == monitor) {
               emit1(MARKSTACK,mtab[i]-last_predeclared);
               emit(CALL_MONINIT);
            }
         }
         process_var_inits(level);
      }
#line 1782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 159 "bapascal.y" /* yacc.c:1646  */
    { 
         if (!making_objfile&&!main_declared) 
            yyerror("No 'main' function declared");
         if (main_declared)
            emit(SHORTRET);
      }
#line 1793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 166 "bapascal.y" /* yacc.c:1646  */
    { 
         if (!making_objfile&&!main_declared) 
            yyerror("No 'main' function declared");
         if (main_declared)
            emit(SHORTRET);
      }
#line 1804 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 175 "bapascal.y" /* yacc.c:1646  */
    {   
         level = 0;
         enter(outerblock_name,outerblock,level,level);
         ob_tix = last_tab;
         /* initialize the block table index with the outer block*/
         tab[ob_tix].ref = last_btab = 0;
         btab[last_btab].lastpar = 0;
         btab[last_btab].psize = btab[last_btab].vsize = 0;
         /* initialize the static display */
         display[last_btab] = level;
      }
#line 1820 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 207 "bapascal.y" /* yacc.c:1646  */
    { external = 0; }
#line 1826 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 211 "bapascal.y" /* yacc.c:1646  */
    { 
         external = 0; 
      }
#line 1834 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 217 "bapascal.y" /* yacc.c:1646  */
    {  external = 1; }
#line 1840 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 221 "bapascal.y" /* yacc.c:1646  */
    { leave_block(&level); }
#line 1846 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 223 "bapascal.y" /* yacc.c:1646  */
    { leave_block(&level); }
#line 1852 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 227 "bapascal.y" /* yacc.c:1646  */
    {  
         tab[(yyvsp[-3])].adr = -1;
         in_mon_decl = 0; 
         external = 0;
         leave_block(&level);
      }
#line 1863 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 240 "bapascal.y" /* yacc.c:1646  */
    {
         int i;
         emit(HALT); 
         /* need to backpatch location of outer block code */
         /* in SHORTCALL instruction                       */
         code[outer_call_lc].y = lc; 
         tab[ob_tix].adr = lc;  /* outerblock entry pt */
         for (i = 0; i <= mon; i++){
            /* don't emit CALL_MONINIT for ext_monitor objects */
            if (tab[mtab[i]].obj == monitor) {
               emit1(MARKSTACK,mtab[i]-last_predeclared);
               emit(CALL_MONINIT);
            }
         }
         process_var_inits(level);
         process_var_inits(level-1);
      }
#line 1885 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 261 "bapascal.y" /* yacc.c:1646  */
    { 
         tab[(yyvsp[-2])].adr = lc;          /* main pgm entry point */ 
         process_var_inits(level);  /* process any var inits */
         outer_call_lc = lc;        /* loc of SHORTCALL instr */
         emit(SHORTCALL);           /* target addr is backpatched later */
      }
#line 1896 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 270 "bapascal.y" /* yacc.c:1646  */
    { /* pgm name is last symbol in outer scope */
         (yyval) = (yyvsp[0]);
         main_declared = 1;
         external = 0;
         tab[last_tab].obj = mainproc;
         prt = last_tab;
         btab[0].lastpar = prt;  /* save main's tabix for the interp */
         enter_block(&last_btab,&level,last_tab);
         btab[last_btab].lastpar = last_tab; /* no parms */
      }
#line 1911 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 299 "bapascal.y" /* yacc.c:1646  */
    {
         if (strcmp(laststring,infile[curr_infile].fname) == 0) 
            yyerror("Recursive file inclusion");
         else {
            curr_infile = open_infile(laststring,curr_infile);
            yyin = infile[curr_infile].f;
         }
         in_include = 0;
      }
#line 1925 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 311 "bapascal.y" /* yacc.c:1646  */
    { 
         in_include = 1;
      }
#line 1933 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 317 "bapascal.y" /* yacc.c:1646  */
    {
         if (strcmp(laststring,infile[curr_infile].fname) == 0) 
            yyerror("Recursive file inclusion");
         else {
            curr_infile = open_infile(laststring,curr_infile);
            yyin = infile[curr_infile].f;
         }
         in_include = 0;
      }
#line 1947 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 336 "bapascal.y" /* yacc.c:1646  */
    {
         tab[last_tab].typ = consttype;
         tab[last_tab].adr = (yyvsp[0]); 
         tab[last_tab].ref = 0;
      }
#line 1957 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 344 "bapascal.y" /* yacc.c:1646  */
    { consttype = ints; }
#line 1963 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 346 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); consttype = ints; }
#line 1969 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 348 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = - (yyvsp[0]); consttype = ints; }
#line 1975 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 350 "bapascal.y" /* yacc.c:1646  */
    {  (yyval) = get_constant(lastident,level,&consttype); }
#line 1981 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 352 "bapascal.y" /* yacc.c:1646  */
    {  (yyval) = get_constant(lastident,level,&consttype);
            if (consttype != ints) {
               sprintf(pbuf,"Identifier '%s' not INTEGER type",lastident);
               yyerror(pbuf);
               consttype = notyp;
            }
         }
#line 1993 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 360 "bapascal.y" /* yacc.c:1646  */
    {  (yyval) = - get_constant(lastident,level,&consttype);
            if (consttype != ints) {
               sprintf(pbuf,"Identifier '%s' not INTEGER type",lastident);
               yyerror(pbuf);
               consttype = notyp;
            }
         }
#line 2005 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 368 "bapascal.y" /* yacc.c:1646  */
    { consttype = chars; (yyval) = laststring[0]; }
#line 2011 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 372 "bapascal.y" /* yacc.c:1646  */
    {(yyval) = strtoi(numbertext,numberbase);}
#line 2017 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 383 "bapascal.y" /* yacc.c:1646  */
    {
         tab[(yyvsp[-2])].obj = type;
         if (expr[(yyvsp[0])].typ == arrays) {
            tab[(yyvsp[-2])].typ = arrays;
            tab[(yyvsp[-2])].ref = expr[(yyvsp[0])].ref;  /* its atab index */
            /* atab sizes are in stack words */
            /* adr field is in bytes */
            tab[(yyvsp[-2])].adr = sizeof(int)*atab[expr[(yyvsp[0])].ref].size;
         }
         else { /* for non-arrays, the expr info is handled identically */
            tab[(yyvsp[-2])].typ = expr[(yyvsp[0])].typ;
            tab[(yyvsp[-2])].ref = expr[(yyvsp[0])].ref; 
            tab[(yyvsp[-2])].adr = expr[(yyvsp[0])].adr;
         }
         free_expr((yyvsp[0]));
      }
#line 2038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 407 "bapascal.y" /* yacc.c:1646  */
    {  
         if ((yyvsp[0]) != 0) {
            if ((tab[(yyvsp[0])].obj != type)||(tab[(yyvsp[0])].typ == notyp)){
               sprintf(pbuf,"Identifier '%s' is not a valid type",lastident);
               yyerror(pbuf);
               (yyval) = NO_EXPR;
            }
            else  {
               (yyval) = new_expr();
               expr[(yyval)].tix = (yyvsp[0]);
               expr[(yyval)].adr = tab[(yyvsp[0])].adr;
               expr[(yyval)].typ = tab[(yyvsp[0])].typ;
               expr[(yyval)].ref = tab[(yyvsp[0])].ref;
               expr[(yyval)].lev = tab[(yyvsp[0])].lev;
               expr[(yyval)].normal = tab[(yyvsp[0])].normal;
               expr[(yyval)].arelt = 0;
            }
         }            
         else /* wasn't there, loc has already complained */
            (yyval) = NO_EXPR;
      }
#line 2064 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 432 "bapascal.y" /* yacc.c:1646  */
    {  
      int i;
      TYPES eltyp;
      int   elsize,elref,last;
         (yyval) = new_expr();
         expr[(yyval)].obj = type;
         expr[(yyval)].typ = arrays;
         expr[(yyval)].ref = (yyvsp[-4]); /* atab index is passed up through index_type */
         if (expr[(yyvsp[0])].typ == arrays) { /* type production has atab index */
            i = expr[(yyvsp[0])].ref; /* get the atab index */
            elsize = atab[i].size;  /* atab has size in stack words */
            eltyp = arrays;
            last = (yyvsp[-2]);     /* last dimension atab */
            elref = i;
         }
         else if (expr[(yyvsp[0])].typ == strings) {
               /* ref field of string is size in bytes */
            elsize = stacksize(expr[(yyvsp[0])].ref);
            eltyp = strings;
            elref = 0;  /* last non-array elref in atab must be 0 */
            last = last_atab;
         } 
         else { /* type production has tab index and isn't typ arrays */
            i = expr[(yyvsp[0])].tix;    /* get tab index */
            elsize = stacksize(tab[i].adr);/* adr is byte size of type object */
            eltyp = tab[i].typ;
            elref = 0;  /* last non-array elref in atab must be 0 */
            last = last_atab;
         }
         for (i= last; i >= (yyvsp[-4]); i--){ /* work from 'back to front' */
            atab[i].elref = elref;
            atab[i].elsize = elsize;
            atab[i].size = (1 + atab[i].high - atab[i].low)*elsize;
            elsize = atab[i].size;
            atab[i].eltyp = eltyp;
            eltyp = arrays;
            elref = i;
         }
         free_expr((yyvsp[0]));
      }
#line 2109 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 481 "bapascal.y" /* yacc.c:1646  */
    {
         if (consttype != atab[last_atab].inxtyp)
            yyerror("Array index type mismatch");
         else {
            if ((yyvsp[0]) < atab[last_atab].low){
               sprintf(pbuf,"Upper array index %d less than lower index %d",
                  (yyvsp[0]),atab[last_atab].low);
               yyerror(pbuf);
            }
            else
               atab[last_atab].high = (yyvsp[0]);
         }
         (yyval) = last_atab;
      }
#line 2128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 498 "bapascal.y" /* yacc.c:1646  */
    {
         if (last_atab == AMAX) 
            cfatal("Array table full with %d entries",AMAX);
         atab[++last_atab].inxtyp = consttype;
         atab[last_atab].high = atab[last_atab].low = (yyvsp[-1]);
         (yyval) = last_atab;  /* pass the array start up the parse tree */
      }
#line 2140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 508 "bapascal.y" /* yacc.c:1646  */
    {  (yyval) = last_atab; }
#line 2146 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 512 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = new_expr();
         if (consttype != ints)
            yyerror("Size of 'string' type must be an integer");
         else if ((yyvsp[-1]) <= 0)
            yyerror("Size of 'string' type must be positive");
         else {
            expr[(yyval)].ref = 1 + (yyvsp[-1]);  /* allow for null terminator */
            expr[(yyval)].typ = strings;
            expr[(yyval)].obj = type;
         }
      }
#line 2163 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 534 "bapascal.y" /* yacc.c:1646  */
    {  
      int i,vartyp,varref,varsize,dx = btab[display[level]].vsize;
      int val;
         if ((yyvsp[0]) != NO_EXPR) {
            val = expr[(yyvsp[0])].adr;
            if (expr[(yyvsp[-1])].typ == conds)
               yyerror("Cannot assign to a 'condition'");
            else if (expr[(yyvsp[-1])].typ == bsems) {
               if ((expr[(yyvsp[0])].typ != ints)||(val < 0)||(val > 1)) 
                  yyerror("A binary semaphore can only become 0 or 1");
            }
            else if (expr[(yyvsp[-1])].typ == sems) {
               if (expr[(yyvsp[0])].typ != ints)
                  yyerror("integer initialization value expected");
               else if (val < 0)
                  yyerror("A semaphore cannot be negative");
            }
            else if ((expr[(yyvsp[-1])].typ != ints) && (expr[(yyvsp[-1])].typ != chars) &&
               (expr[(yyvsp[-1])].typ != bools))  
               yyerror("'INTEGER', 'CHAR', or 'BOOLEAN' type expected");
            else if (expr[(yyvsp[-1])].typ != expr[(yyvsp[0])].typ)
               yyerror("Type of initializer does not match variable type");
            if (external)
               yyerror("External variables cannot be initialized");
         }
         else
            val = 0;
         if (expr[(yyvsp[-1])].typ == arrays) { /* type production has atab index */
            varsize = atab[expr[(yyvsp[-1])].ref].size;
            vartyp = arrays;
            varref = expr[(yyvsp[-1])].ref;
         }
         else if (expr[(yyvsp[-1])].typ == strings){
            varsize = stacksize(expr[(yyvsp[-1])].ref);
            vartyp = strings;
            varref = expr[(yyvsp[-1])].ref;
         }
         else {  /* type production has tab index */ 
            varsize = stacksize(expr[(yyvsp[-1])].adr);
            vartyp = expr[(yyvsp[-1])].typ;
            varref = expr[(yyvsp[-1])].ref;

         }
         for (i=(yyvsp[-3]); i<= last_tab; i++){
            tab[i].typ = vartyp;
            tab[i].ref = varref;
            tab[i].adr = dx;
            tab[i].obj = (external ? ext_variable : variable);
            if ((yyvsp[0]) != NO_EXPR){
               if (((in_mon_decl)&&(!in_proc_decl))||  /* global mon  vars */
                   ((level == 0)&&(!main_declared))||  /* outer global vars */
                   ((level == 1)&&(main_declared))||   /* main global vars */
                   (in_proc_decl)){ /* in a proc or func decl */
                  /* in these cases, must generate the init code later */
                  add_var_init(&curr_var,i,level,val); 
               }
            }
            dx += varsize;
         }
         btab[display[level]].vsize = dx;
         free_expr((yyvsp[-1]));  free_expr((yyvsp[0]));
      }
#line 2230 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 602 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2236 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 606 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = NO_EXPR; }
#line 2242 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 608 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = new_expr();
         expr[(yyval)].typ = consttype;
         expr[(yyval)].adr = (yyvsp[0]);
      }
#line 2252 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 619 "bapascal.y" /* yacc.c:1646  */
    { 
         if (tab[(yyvsp[-2])].mon) emit(EXITMON);
         emit(EXIT_PROC);
         leave_block(&level);
         in_proc_decl = 0;
      }
#line 2263 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 626 "bapascal.y" /* yacc.c:1646  */
    { 
         if (tab[(yyvsp[-2])].mon) emit(EXITMON);
         emit(EXIT_FCN);
         leave_block(&level);
         in_proc_decl = 0;
      }
#line 2274 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 635 "bapascal.y" /* yacc.c:1646  */
    {
         tab[(yyvsp[-1])].adr = (external? -1 : lc);
         if (tab[(yyvsp[-1])].mon) emit1(ENTERMON,tab[(yyvsp[-1])].mon); 
         process_var_inits(level);
      }
#line 2284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 643 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = (yyvsp[-1]);
         tab[(yyvsp[-1])].atomic = atomic;
      }
#line 2293 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 650 "bapascal.y" /* yacc.c:1646  */
    { atomic = FALSE; }
#line 2299 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 652 "bapascal.y" /* yacc.c:1646  */
    { atomic = TRUE; }
#line 2305 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 660 "bapascal.y" /* yacc.c:1646  */
    { int vis_level,real_level;
         in_proc_decl = 1;
         if (in_mon_decl) {
            real_level = level;
            vis_level = ((level > 0) ? level -1 : 0);
         }
         else {
            vis_level = real_level = level;
         }
         (yyval) = prt = enter(lastident, (external? ext_procedure : procedure)
                              ,real_level,vis_level);
         if (in_mon_decl) tab[prt].mon = mtab[mon] - last_predeclared;
         enter_block(&last_btab,&level,prt);
      }
#line 2324 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 677 "bapascal.y" /* yacc.c:1646  */
    {
         tab[(yyvsp[-1])].adr = (external? -1 : lc);
         if (tab[(yyvsp[-1])].mon) emit1(ENTERMON,tab[(yyvsp[-1])].mon); 
         process_var_inits(level);
      }
#line 2334 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 685 "bapascal.y" /* yacc.c:1646  */
    {
         tab[(yyvsp[-1])].atomic = atomic;
         (yyval) = (yyvsp[-1]);
      }
#line 2343 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 692 "bapascal.y" /* yacc.c:1646  */
    { TYPES ft = tab[(yyvsp[0])].typ; 
         if (tab[(yyvsp[0])].obj != type){
            sprintf(pbuf,"'%s' is not a valid type",tab[(yyvsp[0])].name);
            yyerror(pbuf);
         }
         if ((ft != ints)&&(ft != bools)&&(ft != chars)&&(ft != notyp)) 
            yyerror("Function return type must be INTEGER, BOOLEAN, or CHAR");
         tab[(yyvsp[-3])].typ = tab[(yyvsp[0])].typ;
      }
#line 2357 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 704 "bapascal.y" /* yacc.c:1646  */
    { int vis_level,real_level;
         in_proc_decl = 1;
         if (in_mon_decl) {
            real_level = level;
            vis_level = ((level > 0) ? level -1 : 0);
         }
         else {
            vis_level = real_level = level;
         }
         (yyval) = prt = enter(lastident, (external? ext_function: function)
                           ,real_level,vis_level);
         if (in_mon_decl) tab[prt].mon = mtab[mon] - last_predeclared;
         enter_block(&last_btab,&level,prt);
      }
#line 2376 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 721 "bapascal.y" /* yacc.c:1646  */
    { btab[display[level]].lastpar = last_tab;}
#line 2382 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 723 "bapascal.y" /* yacc.c:1646  */
    { btab[display[level]].lastpar = last_tab;}
#line 2388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 736 "bapascal.y" /* yacc.c:1646  */
    { int i,dx,dv,j,elem_size,stack_dx;
         if (tab[(yyvsp[0])].obj != type) {
            sprintf(pbuf,"'%s' is not a valid type",tab[(yyvsp[0])].name);
            yyerror(pbuf);
         }
         j = display[level];
         elem_size = tab[(yyvsp[0])].adr;
         stack_dx = stacksize(elem_size);
         dx = btab[j].vsize;
         dv = dx - btab[j].psize; /* in case vbls have been declared */
         for (i = (yyvsp[-2]); i <= last_tab; i++) {
            tab[i].typ = tab[(yyvsp[0])].typ;
            tab[i].ref = tab[(yyvsp[0])].ref;
            tab[i].lev = level;
            tab[i].obj = variable;
            tab[i].adr = dx;
            dx = stack_dx + dx;
         }
         btab[j].psize = dx;
         btab[j].vsize = dx + dv;
      }
#line 2414 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 760 "bapascal.y" /* yacc.c:1646  */
    { int i,dx,dv,j,elem_size;
         if (tab[(yyvsp[0])].obj != type) {
            sprintf(pbuf,"'%s' is not a valid type",tab[(yyvsp[0])].name);
            yyerror(pbuf);
         }
         j = display[level];
         elem_size = tab[(yyvsp[0])].adr; 
         dx = btab[j].vsize;
         dv = dx - btab[j].psize; /* in case vbls have been declared */
         for (i = (yyvsp[-2]); i <= last_tab; i++) {
            tab[i].typ = tab[(yyvsp[0])].typ;
            tab[i].ref = tab[(yyvsp[0])].ref;
            tab[i].lev = level;
            tab[i].obj = variable;
            tab[i].adr = dx;
            tab[i].normal = FALSE; /* VAR parms are not normal */
            dx++;                  /* pointers occupy 1 stack word */
         }
         btab[j].psize = dx;
         btab[j].vsize = dx + dv;
      }
#line 2440 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 784 "bapascal.y" /* yacc.c:1646  */
    { int i,dx,dv,j,stack_dx;
         j = display[level];
         stack_dx = 1;            /* strings always pass by reference */
         dx = btab[j].vsize;
         dv = dx - btab[j].psize; /* in case vbls have been declared */
         for (i = (yyvsp[-2]); i <= last_tab; i++) {
            tab[i].typ = strings;
            tab[i].ref = 0;
            tab[i].normal = 0;   /* pass by reference */
            tab[i].lev = level;
            tab[i].obj = variable;
            tab[i].adr = dx;
            dx = stack_dx + dx;
         }
         btab[j].psize = dx;
         btab[j].vsize = dx + dv;
      }
#line 2462 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 804 "bapascal.y" /* yacc.c:1646  */
    {
         if (tab[mtab[mon]].obj == monitor) {
            /* don't do the following for ext_monitor objects */
            /* main pgm will CALL_MONINIT, even if no init code */
            emit(RET_MONINIT); 
         }
         leave_block(&level);
      }
#line 2475 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 815 "bapascal.y" /* yacc.c:1646  */
    {
         if (mon == MAXMON) 
            cfatal("No more than %d monitors can be declared",MAXMON);
         if (level > 1) {
            sprintf(pbuf,
            "Monitors must be declared at level 1 or level 0, not level %d\n",
            level);
            yyerror(pbuf);
         }
         tab[(yyvsp[-1])].obj = (external ? ext_monitor : monitor);
         mtab[++mon] = (yyvsp[-1]);
         in_mon_decl = 1;
         (yyval) = (yyvsp[-1]);
         enter_block(&last_btab,&level,last_tab);
         btab[last_btab].lastpar = last_tab;  /* no parms */
      }
#line 2496 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 834 "bapascal.y" /* yacc.c:1646  */
    {
         in_mon_decl = 0;  /* optional declarations are over */
         in_mon_init = 1;  /* could have some init code     */
      }
#line 2505 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 841 "bapascal.y" /* yacc.c:1646  */
    {
         tab[mtab[mon]].adr = lc;
         process_var_inits(level);
         in_mon_init = 0; 
      }
#line 2515 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 853 "bapascal.y" /* yacc.c:1646  */
    {
         if (in_mon_init) {   /* beginning of the mon init code */
            tab[mtab[mon]].adr = lc;
            process_var_inits(level);
            in_mon_init = 0; 
         } 
      }
#line 2527 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 868 "bapascal.y" /* yacc.c:1646  */
    { yyerrok; }
#line 2533 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 870 "bapascal.y" /* yacc.c:1646  */
    { yyerrok; }
#line 2539 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 879 "bapascal.y" /* yacc.c:1646  */
    { code[(yyvsp[-1])].y = (yyvsp[0]); }
#line 2545 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 881 "bapascal.y" /* yacc.c:1646  */
    { code[(yyvsp[-3])].y = (yyvsp[-1]); code[(yyvsp[-2])].y = lc; }
#line 2551 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 883 "bapascal.y" /* yacc.c:1646  */
    { emit1(JUMP,(yyvsp[-3])); code[(yyvsp[-2])].y = lc; }
#line 2557 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 885 "bapascal.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); emit1(JZER,(yyvsp[-3])); free_expr((yyvsp[0])); }
#line 2563 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 887 "bapascal.y" /* yacc.c:1646  */
    { emit1(ENDFOR,1+(yyvsp[-1])); code[(yyvsp[-1])].y = lc; }
#line 2569 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 889 "bapascal.y" /* yacc.c:1646  */
    { if (level == 1) emit(COEND); in_cobegin = 0; }
#line 2575 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 893 "bapascal.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); (yyval) = lc; emit(JZER); free_expr((yyvsp[0])); }
#line 2581 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 897 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = lc; }
#line 2587 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 901 "bapascal.y" /* yacc.c:1646  */
    { emit(JUMP); (yyval) = lc; }
#line 2593 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 905 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = lc; }
#line 2599 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 909 "bapascal.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); free_expr((yyvsp[0])); (yyval) = lc; emit(JZER);}
#line 2605 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 913 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = lc; }
#line 2611 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 917 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = (yyvsp[0]);
         if (tab[(yyvsp[0])].obj != variable) 
            yyerror("FOR loop index must be a VARIABLE");
         if (!tab[(yyvsp[0])].normal) 
            yyerror("FOR loop index cannot be a VAR parameter");
         emit2(LOAD_ADDR,tab[(yyvsp[0])].lev,tab[(yyvsp[0])].adr);
         if ((tab[(yyvsp[0])].typ != ints)&&(tab[(yyvsp[0])].typ != bools)&&
             (tab[(yyvsp[0])].typ != chars)&&(tab[(yyvsp[0])].typ != notyp))
            yyerror("FOR loop index must be of type INTEGER, BOOLEAN, or CHAR");
      }
#line 2627 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 931 "bapascal.y" /* yacc.c:1646  */
    {
         if (expr[(yyvsp[-3])].typ != expr[(yyvsp[-1])].typ)
            yyerror("Type mismatch in FOR loop limits");
         gen_exprval((yyvsp[-1]));
         (yyval) = lc; emit(BEGINFOR);
         free_expr((yyvsp[-3])); free_expr((yyvsp[-1]));
      }
#line 2639 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 941 "bapascal.y" /* yacc.c:1646  */
    {
         if ((tab[(yyvsp[-2])].typ != expr[(yyvsp[0])].typ)&&(tab[(yyvsp[-2])].typ != notyp))
            yyerror("Type mismatch in FOR loop limits");
         gen_exprval((yyvsp[0]));
         (yyval) = (yyvsp[0]); /* pass expr index back to previous rule for type chk*/
      }
#line 2650 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 950 "bapascal.y" /* yacc.c:1646  */
    {
         if (level != 1)
            yyerror("COBEGIN-COEND block allowed only in main program");
         else if (in_cobegin)
            yyerror("COBEGIN-COEND blocks cannot be nested");
         else
            emit(COBEGIN);
         in_cobegin = 1;
      }
#line 2664 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 962 "bapascal.y" /* yacc.c:1646  */
    {
         gen_exprval((yyvsp[0]));
         if (tab[(yyvsp[-1])].typ == expr[(yyvsp[0])].typ) {
            if ((tab[(yyvsp[-1])].typ == sems)||(tab[(yyvsp[-1])].typ == bsems)|| 
                (tab[(yyvsp[-1])].typ == conds))
               yyerror("Cannot assign to SEMAPHORE or CONDITION");
            else if (tab[(yyvsp[-1])].typ == arrays)
               emit1(COPY_BLOCK,atab[tab[(yyvsp[-1])].ref].size);
            else  /* standard types */
               emit(STORE);
         }
         else
            yyerror("Type mismatch in assignment");
         free_expr((yyvsp[0]));
      }
#line 2684 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 978 "bapascal.y" /* yacc.c:1646  */
    {
         gen_exprval((yyvsp[0]));
         if (expr[(yyvsp[-2])].typ == expr[(yyvsp[0])].typ) {
            if ((expr[(yyvsp[-2])].typ == sems)||(expr[(yyvsp[-2])].typ == bsems)|| 
                (expr[(yyvsp[-2])].typ == conds))
               yyerror("Cannot assign to SEMAPHORE or CONDITION");
            else if (expr[(yyvsp[-2])].typ == arrays)
               emit1(COPY_BLOCK,atab[expr[(yyvsp[0])].ref].size);
            else
               emit(STORE);
         }
         else
            yyerror("Type mismatch in assignment");
         free_expr((yyvsp[-2])); free_expr((yyvsp[0]));
      }
#line 2704 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 996 "bapascal.y" /* yacc.c:1646  */
    {
         switch (tab[(yyvsp[-1])].obj) {
         case ext_variable:
         case variable:
            if ((tab[(yyvsp[-1])].typ == sems)||(tab[(yyvsp[-1])].typ == bsems))
               yyerror("Must use INITIALSEM to initialize a SEMAPHORE");
            else
               emit2((tab[(yyvsp[-1])].normal ? LOAD_ADDR : LOAD_VALUE),
                  tab[(yyvsp[-1])].lev,tab[(yyvsp[-1])].adr);
            break;
         case ext_function:
         case function:
            if ((tab[(yyvsp[-1])].lev+1) == level) /* fn name declared in outer scope */
               /* s[display[tab[$1].lev+1]+0] is loc of fn return value */
               emit2(LOAD_ADDR,tab[(yyvsp[-1])].lev+1,0);
            else
               yyerror("FUNCTION identifier inaccessible at this level");
            break;
         default:
            yyerror("VARIABLE or FUNCTION identifier expected");
         } /* switch */
      }
#line 2731 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1021 "bapascal.y" /* yacc.c:1646  */
    { int k;
         if ((tab[(yyvsp[-2])].obj == procedure)||
             (tab[(yyvsp[-2])].obj == ext_procedure)) {
            if (tab[(yyvsp[-2])].lev == -1) { /* std proc unfinished business */
               stdproc_pcode((yyvsp[-2]),parmct);
            } /* if std proc */
            else { /* regular proc call */
               /* first check on too few parms */
               k = btab[tab[(yyvsp[-2])].ref].lastpar - (yyvsp[-2]);
               if ((k > parmct)&&(first_parmcterr)) {
                  yyerror("Too few actual parameters in the call");
                  first_parmcterr = 0;
               }
               k = btab[tab[(yyvsp[-2])].ref].psize - 1;
               if (tab[(yyvsp[-2])].mon == 0)
                  emit2(CALL,0,k);
               else
                  emit2(CALL,-1,k); /* CALL pcode: x == -1 => mon call */
               if (tab[(yyvsp[-2])].lev < level) /* update display, ord nesting*/
                  emit2(UPDATE_DISPLAY,tab[(yyvsp[-2])].lev,level);
               else if ((tab[(yyvsp[-2])].mon)&&(level == tab[(yyvsp[-2])].lev))
               /* must fix display differently for monitor call */
                  emit2(UPDATE_DISPLAY,level-1,level);
            } /* else regular proc */
            toppfs--;
            if (toppfs < 0 )
               parmct = last_pf = 0;
            else {
               parmct = pfstack[toppfs].pct;
               last_pf = pfstack[toppfs].tix;
            }
         }
      }
#line 2769 "y.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 1065 "bapascal.y" /* yacc.c:1646  */
    {
         parmct = ++pfstack[toppfs].pct;
         if (tab[last_pf].lev == -1)
            stdproc_parm(last_pf,(yyvsp[0]),parmct);
         else
            proc_parm(last_pf,(yyvsp[0]),parmct);
         free_expr((yyvsp[0]));
      }
#line 2782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 1074 "bapascal.y" /* yacc.c:1646  */
    { int tmp = store_string(laststring,&stab_size);
         if ((tab[last_pf].lev == -1)&&
            ((tab[last_pf].adr == SP_WRITE)||
             (tab[last_pf].adr == SP_WRITELN))){
               emit1(WRITE_RAWSTRING,tmp);
         }
         parmct++;
      }
#line 2795 "y.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 1085 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = last_pfv = (yyvsp[0]); }
#line 2801 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1089 "bapascal.y" /* yacc.c:1646  */
    {
         if ((tab[last_pfv].obj == procedure)||
             (tab[last_pfv].obj == ext_procedure)){
            if ((++toppfs) > 0){
               yyerror("Procedure calls can't be nested");
               pfstack[toppfs].tix = last_pf = last_pfv;
               pfstack[toppfs].pct = parmct = 0;
            }
            else{
               pfstack[toppfs].tix = last_pf = last_pfv;
               pfstack[toppfs].pct = parmct = 0;
               first_parmcterr = 1;
               if (tab[last_pf].lev != -1){ /* regular proc call */
                  if ((tab[last_pf].mon)&&(tab[prt].mon)&&
                      (tab[last_pf].mon != tab[prt].mon))
                     yyerror("Cross monitor calls not allowed");
                  emit1(MARKSTACK,last_pf-last_predeclared);
               } /* if regular proc */
            } /* non-nested proc call */ 
         } /* if proc */
         else{
            yyerror("PROCEDURE call expected");
         }
      }
#line 2830 "y.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 1123 "bapascal.y" /* yacc.c:1646  */
    {
            (yyval) = new_expr();
            expr[(yyval)].obj = function;
            expr[(yyval)].typ = ints;
            expr[(yyval)].normal = 1; /* the value will be left on the stack */
            expr[(yyval)].ref = 0;
               /* make sure no one uses these */
            expr[(yyval)].adr = expr[(yyval)].lev = -1;
            if (expr[(yyvsp[-1])].typ != ints)
               yyerror("right parameter is not of type 'int'");
            else {
                  /* put the value of the id expr on the stack */
               gen_exprval((yyvsp[-1]));
               if (expr[(yyvsp[-3])].typ == rawstrings)     /* raw str message */
                  emit1(SEND_RAWSTRING,expr[(yyvsp[-3])].adr);
               else if (expr[(yyvsp[-3])].typ == ints)      /* int message */
                  emit(SEND_INT);
               else if (expr[(yyvsp[-3])].typ == strings)   /* msg is a string var */
                  emit(SEND_STRING);
               else
                  yyerror("parameter is not 'rawstring', 'string', or 'int'");
            }
            free_expr((yyvsp[-3])); free_expr((yyvsp[-1]));
         }
#line 2859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 1150 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2865 "y.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 1152 "bapascal.y" /* yacc.c:1646  */
    { 
            if (expr[(yyvsp[0])].typ == strings) {
               /* left parm is a string. If it's not an array elt, */
               /* put the address of the string n the stack        */
               if (!expr[(yyvsp[0])].arelt)
                  emit_push_addr(expr[(yyvsp[0])]);
            }
            else if (expr[(yyvsp[0])].typ == ints) 
               gen_exprval((yyvsp[0]));  /* put the int on the stack */
            (yyval) = (yyvsp[0]);
         }
#line 2881 "y.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 1166 "bapascal.y" /* yacc.c:1646  */
    {
            if (expr[(yyvsp[0])].typ == rawstrings)     /* raw str message */
               emit1(BROADCAST_RAWSTRING,expr[(yyvsp[0])].adr);
            else if (expr[(yyvsp[0])].typ == strings)   /* msg is a string var */
               emit(BROADCAST_STRING);
            else if (expr[(yyvsp[0])].typ == ints)      /* int msg */
               emit(BROADCAST_INT);
            else
               yyerror("parameter is not 'rawstring', 'string', or 'int'");
            free_expr((yyvsp[0])); 
         }
#line 2897 "y.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 1180 "bapascal.y" /* yacc.c:1646  */
    {
         if (expr[(yyvsp[-1])].typ != strings)
            yyerror("left parameter is not of type 'string'");
         (yyval) = NO_EXPR;     /* for free_expr() call up the parse tree */
         if (expr[(yyvsp[0])].typ == rawstrings) 
            emit1(CONCAT_RAWSTRING,expr[(yyvsp[0])].adr);
         else 
            emit(CONCAT_STRING);
         free_expr((yyvsp[-1])); free_expr((yyvsp[0]));
      }
#line 2912 "y.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 1193 "bapascal.y" /* yacc.c:1646  */
    {
         if (expr[(yyvsp[-1])].typ != strings) {
            yyerror("right parameter is not of type 'string'");
         }
         else {
            if (!expr[(yyvsp[-1])].arelt)
               /* push addr of source string */
               /* addr of array ref is already on the stack */
               emit_push_addr(expr[(yyvsp[-1])]);
         }
      }
#line 2928 "y.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 1205 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2934 "y.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 1209 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = new_expr();
         expr[(yyval)].obj = variable;
         expr[(yyval)].typ = rawstrings;
         expr[(yyval)].adr = store_string(laststring,&stab_size);
         expr[(yyval)].ref = 1 + strlen(laststring); /* includes null char */
         expr[(yyval)].normal = 1;
         expr[(yyval)].tix = -1;
      }
#line 2948 "y.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 1222 "bapascal.y" /* yacc.c:1646  */
    {
         if (expr[(yyvsp[-1])].typ != strings)
            yyerror("left parameter is not of type 'string'");
         (yyval) = NO_EXPR;     /* for free_expr() call up the parse tree */
         if (expr[(yyvsp[0])].typ == rawstrings) 
            emit1(COPY_RAWSTRING,expr[(yyvsp[0])].adr);
         else 
            emit(COPY_STRING);
         free_expr((yyvsp[-1])); free_expr((yyvsp[0]));
      }
#line 2963 "y.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 1235 "bapascal.y" /* yacc.c:1646  */
    {
            /* push the parm count for interp's SPRINTF_OP to use */
            emit1(PUSH_LIT,sprintf_cnt); 
            emit1(SPRINTF_OP,(yyvsp[-3]));
         }
#line 2973 "y.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 1243 "bapascal.y" /* yacc.c:1646  */
    { 
            if (expr[(yyvsp[-2])].typ != strings)
               yyerror("Leftmost sprintf parameter must be of type 'string'");
            (yyval) = expr[(yyvsp[0])].adr;   /* pass raw string index up the tree */
            emit_push_addr(expr[(yyvsp[-2])]);
            free_expr((yyvsp[-2])); free_expr((yyvsp[0]));
            sprintf_cnt = 0;
         }
#line 2986 "y.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 1254 "bapascal.y" /* yacc.c:1646  */
    {  
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != strings)&&
                (expr[(yyvsp[0])].typ != chars))
               yyerror(
            "sprintf parameter must be either of type 'char', 'int' or type 'string'");
            if (expr[(yyvsp[0])].typ == strings)
               emit_push_addr(expr[(yyvsp[0])]);
            else
               gen_exprval((yyvsp[0]));
            sprintf_cnt++;
            free_expr((yyvsp[0]));
         }
#line 3003 "y.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 1267 "bapascal.y" /* yacc.c:1646  */
    {  
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != strings)&&
                (expr[(yyvsp[0])].typ != chars))
               yyerror(
            "sprintf parameter must be either of type 'char', 'int' or type 'string'");
            if (expr[(yyvsp[0])].typ == strings)
               emit_push_addr(expr[(yyvsp[-2])]);
            else
               gen_exprval((yyvsp[0]));
            sprintf_cnt++;
            free_expr((yyvsp[0]));
         }
#line 3020 "y.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 1283 "bapascal.y" /* yacc.c:1646  */
    {
         gen_exprval((yyvsp[0]));
         if (expr[(yyvsp[-2])].typ == expr[(yyvsp[0])].typ){
            if ((expr[(yyvsp[-2])].typ == ints)||(expr[(yyvsp[-2])].typ == bools)||
                (expr[(yyvsp[-2])].typ == chars))
               emit((yyvsp[-1]));
            else
               yyerror("Only INTEGER, BOOLEAN, CHAR types can be compared");
         }
         else
            yyerror("Types in comparison are unequal");
         expr[(yyvsp[-2])].typ = bools;
         free_expr((yyvsp[0]));
      }
#line 3039 "y.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 1299 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = TEST_EQ; }
#line 3045 "y.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 1300 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = TEST_LT; }
#line 3051 "y.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 1301 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = TEST_GT; }
#line 3057 "y.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 1302 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = TEST_LE; }
#line 3063 "y.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 1303 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = TEST_GE; }
#line 3069 "y.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 1304 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = TEST_NE; }
#line 3075 "y.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 1309 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = (yyvsp[0]);
         gen_exprval((yyval));
         if ((expr[(yyval)].typ != ints)&&(expr[(yyval)].typ != notyp)){
            yyerror("INTEGER type expected");
            expr[(yyval)].typ = notyp;
         }
      }
#line 3088 "y.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 1318 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = (yyvsp[0]);
         gen_exprval((yyval));
         if ((expr[(yyval)].typ != ints)&&(expr[(yyval)].typ != notyp)){
            yyerror("INTEGER type expected");
            expr[(yyval)].typ = notyp;
         }
         else
            emit(NEGATE);
      }
#line 3103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 1329 "bapascal.y" /* yacc.c:1646  */
    {
         gen_exprval((yyvsp[0]));
         switch ((yyvsp[-1])) {
         case DO_OR:
            if ((expr[(yyvsp[-2])].typ == bools)&&(expr[(yyvsp[0])].typ == bools))
               emit(DO_OR);
            else if ((expr[(yyvsp[-2])].typ != notyp)&&(expr[(yyvsp[0])].typ != notyp)){
               yyerror("BOOLEAN type expected");
               expr[(yyvsp[-2])].typ = notyp;
            }
            break;
         case DO_ADD:
         case DO_SUB:
            expr[(yyvsp[-2])].typ = resulttype(expr[(yyvsp[-2])].typ,expr[(yyvsp[0])].typ);
            if (expr[(yyvsp[-2])].typ == ints)
               emit((yyvsp[-1]));
            break;
         } /* switch */
         free_expr((yyvsp[0]));
      }
#line 3128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 1352 "bapascal.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); }
#line 3134 "y.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 1355 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = DO_ADD; }
#line 3140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 1356 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = DO_SUB; }
#line 3146 "y.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 1357 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = DO_OR; }
#line 3152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 1362 "bapascal.y" /* yacc.c:1646  */
    {
         gen_exprval((yyvsp[0]));
         switch((yyvsp[-1])) {
         case DO_AND:
            if ((expr[(yyvsp[-2])].typ == bools)&&(expr[(yyvsp[0])].typ == bools))
               emit(DO_AND);
            else if ((expr[(yyvsp[-2])].typ != notyp)&&(expr[(yyvsp[0])].typ != notyp)){
               yyerror("BOOLEAN type expected");
               expr[(yyvsp[-2])].typ = notyp;
            }
            break;
         case DO_MUL:
         case DO_DIV:
         case DO_MOD:
            if ((expr[(yyvsp[-2])].typ == ints)&&(expr[(yyvsp[0])].typ == ints))
               emit((yyvsp[-1]));
            else if ((expr[(yyvsp[-2])].typ != notyp)&&(expr[(yyvsp[0])].typ != notyp)){
               yyerror("INTEGER type expected");
               expr[(yyvsp[-2])].typ = notyp;
            }
            break;
         } /* switch */
         free_expr((yyvsp[0]));
      }
#line 3181 "y.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 1389 "bapascal.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); }
#line 3187 "y.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 1392 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = DO_MUL; }
#line 3193 "y.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 1393 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = DO_DIV; }
#line 3199 "y.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 1394 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = DO_MOD; }
#line 3205 "y.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 1395 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = DO_AND; }
#line 3211 "y.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 1400 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = new_expr();
         expr[(yyval)].obj = constant;
         expr[(yyval)].adr = (yyvsp[0]);
         expr[(yyval)].typ = ints;
         expr[(yyval)].lev = level;
      }
#line 3223 "y.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 1408 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = new_expr();
         expr[(yyval)].obj = constant;
         expr[(yyval)].adr = laststring[0];
         expr[(yyval)].typ = chars;
         expr[(yyval)].lev = level;
      }
#line 3235 "y.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 1415 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]);}
#line 3241 "y.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 1417 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = (yyvsp[0]);
         gen_exprval((yyval));
         if (expr[(yyval)].typ == bools)
            emit(COMPLEMENT);
         else if (expr[(yyval)].typ != notyp){
            yyerror("BOOLEAN type expected");
            expr[(yyval)].typ = notyp;
         }
      }
#line 3256 "y.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 1430 "bapascal.y" /* yacc.c:1646  */
    { int k; 
         (yyval) = new_expr();
         expr[(yyval)].obj = tab[(yyvsp[-2])].obj;
         expr[(yyval)].typ = tab[(yyvsp[-2])].typ;
         expr[(yyval)].adr = tab[(yyvsp[-2])].adr;
         expr[(yyval)].ref = tab[(yyvsp[-2])].ref;
         expr[(yyval)].lev = tab[(yyvsp[-2])].lev;
         expr[(yyval)].normal = tab[(yyvsp[-2])].normal;
         if ((tab[(yyvsp[-2])].obj != variable)&&
             (tab[(yyvsp[-2])].obj != ext_variable)&&
             (tab[(yyvsp[-2])].obj != constant)&& 
             (tab[(yyvsp[-2])].obj != function)&&
             (tab[(yyvsp[-2])].obj != ext_function))
            yyerror("VARIABLE, CONSTANT, or FUNCTION CALL expected");
         if ((tab[(yyvsp[-2])].obj == function)||
             (tab[(yyvsp[-2])].obj == ext_function)) {
            expr[(yyval)].ref = 0;   /* in a func call ref field of func == 0 */
            if (tab[(yyvsp[-2])].lev == -1) { /* std function unfinished business */
               stdproc_pcode((yyvsp[-2]),parmct);
            } /* if std proc */
            else { /* regular func call */
               /* first check on too few parms */
               k = btab[tab[(yyvsp[-2])].ref].lastpar - (yyvsp[-2]);
               if ((k > parmct)&&(first_parmcterr)) {
                  yyerror("Too few actual parameters in the call");
                  first_parmcterr = 0;
               }
               if (in_cobegin)
                  yyerror("Function calls are not allowed in a COBEGIN block");
               k = btab[tab[(yyvsp[-2])].ref].psize - 1;
               if (tab[(yyvsp[-2])].mon == 0)
                  emit2(CALL,0,k);
               else
                  emit2(CALL,-1,k); /* CALL pcode: x == -1 => mon call */
               if (tab[(yyvsp[-2])].lev < level) /* update display, ord nesting*/
                  emit2(UPDATE_DISPLAY,tab[(yyvsp[-2])].lev,level);
               else if ((tab[(yyvsp[-2])].mon)&&(level == tab[(yyvsp[-2])].lev))
               /* must fix display differently for monitor call */
                  emit2(UPDATE_DISPLAY,level-1,level);
            } /* else regular func */
            toppfs--;
            if (toppfs < 0 )
               parmct = last_pf = 0;
            else {
               parmct = pfstack[toppfs].pct;
               last_pf = pfstack[toppfs].tix;
            }
         }
      }
#line 3310 "y.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 1485 "bapascal.y" /* yacc.c:1646  */
    {
         if ((tab[last_pfv].obj == function)||
             (tab[last_pfv].obj == ext_function)) {
            if ((++toppfs) == MAXPFNEST)
               yyerror("Maximum function call nesting exceeded");
            else{  /* legal func call */
               pfstack[toppfs].tix = last_pf = last_pfv;
               pfstack[toppfs].pct = parmct = 0;
               first_parmcterr = 1;
               if (tab[last_pf].lev != -1){ /* regular proc call */
                  if ((tab[last_pf].mon)&&(tab[prt].mon)&&
                      (tab[last_pf].mon != tab[prt].mon))
                     yyerror("Cross monitor calls not allowed");
                  emit1(MARKSTACK,last_pf-last_predeclared);
               } /* if regular proc */
            } /* legal func call */ 
         } /* if func */
      }
#line 3333 "y.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 1512 "bapascal.y" /* yacc.c:1646  */
    {
         if (expr[(yyvsp[-1])].typ != strings)
            yyerror("left parameter is not of type 'string'");
         (yyval) = new_expr();
         expr[(yyval)].obj = function;
         expr[(yyval)].typ = ints;
         expr[(yyval)].normal = 1; /* the value will be left on the stack */
         expr[(yyval)].ref = 0;
            /* make sure no one uses these */
         expr[(yyval)].adr = expr[(yyval)].lev = -1;
         if (expr[(yyvsp[0])].typ == rawstrings) 
            emit1(COMPARE_RAWSTRING,expr[(yyvsp[0])].adr);
         else 
            emit(COMPARE_STRING);
         free_expr((yyvsp[0]));
      }
#line 3354 "y.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 1531 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = new_expr();
         expr[(yyval)].obj = function;
         expr[(yyval)].typ = ints;
         expr[(yyval)].normal = 1; /* the value will be left on the stack */
         expr[(yyval)].ref = 0;
            /* make sure no one uses these */
         expr[(yyval)].adr = expr[(yyval)].lev = -1;
         if (expr[(yyvsp[0])].typ == rawstrings)
               /* user wants length of a rawtring !???!!! */
               /* let 'em have it */
            emit1(PUSH_LIT,expr[(yyvsp[0])].ref - 1);
         else  /* was a string variable */
            emit(LENGTH_STRING);
         free_expr((yyvsp[0]));
      }
#line 3375 "y.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 1550 "bapascal.y" /* yacc.c:1646  */
    {
         if (expr[(yyvsp[-1])].typ == strings) {
            if (!expr[(yyvsp[-1])].arelt)
               /* addr of array ref is already on the stack */
               emit_push_addr(expr[(yyvsp[-1])]);
         } else if (expr[(yyvsp[-1])].typ == ints)
            gen_exprval((yyvsp[-1]));
         (yyval) = (yyvsp[-1]);
         /* parent rule will free_expr($2) */
      }
#line 3390 "y.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 1561 "bapascal.y" /* yacc.c:1646  */
    {
         (yyval) = new_expr();
         expr[(yyval)].obj = variable;
         expr[(yyval)].typ = rawstrings;
         expr[(yyval)].adr = store_string(laststring,&stab_size);
         expr[(yyval)].ref = 1 + strlen(laststring); /* includes null char */
         expr[(yyval)].normal = 1;
         expr[(yyval)].tix = -1;
      }
#line 3404 "y.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 1573 "bapascal.y" /* yacc.c:1646  */
    {
         if (expr[(yyvsp[-1])].typ != strings)
            yyerror("Left parameter must be of type 'string'");
         emit(RECEIVE_OP);
         free_expr((yyvsp[-1]));
         (yyval) = new_expr();
         expr[(yyval)].obj = function;
         expr[(yyval)].typ = ints;
         expr[(yyval)].normal = 1; /* the value will be left on the stack */
         expr[(yyval)].ref = 0;
            /* make sure no one uses these */
         expr[(yyval)].adr = expr[(yyval)].lev = -1;
}
#line 3422 "y.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 1587 "bapascal.y" /* yacc.c:1646  */
    {
         if (expr[(yyvsp[-3])].typ != strings)
            yyerror("Left parameter must be of type 'string'");
         /* implements receive(string,int&) call */
         if (expr[(yyvsp[-1])].obj != variable) 
            yyerror("the right 'receive' parameter must be a variable");
         else if (expr[(yyvsp[-1])].typ != ints)
            yyerror("the right 'receive' parameter must be of type 'int'");
         else {
            if (!expr[(yyvsp[-1])].arelt)
               emit_push_addr(expr[(yyvsp[-1])]);
            emit(RECEIVE_ID);
         }
         free_expr((yyvsp[-3])); free_expr((yyvsp[-1]));
         (yyval) = new_expr();
         expr[(yyval)].obj = function;
         expr[(yyval)].typ = ints;
         expr[(yyval)].normal = 1; /* the value will be left on the stack */
         expr[(yyval)].ref = 0;
            /* make sure no one uses these */
         expr[(yyval)].adr = expr[(yyval)].lev = -1;
      }
#line 3449 "y.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 1612 "bapascal.y" /* yacc.c:1646  */
    {
            (yyval) = new_expr();
            expr[(yyval)].obj = function;
            expr[(yyval)].typ = ints;
            expr[(yyval)].normal = 1; /* the value will be left on the stack */
            expr[(yyval)].ref = 0;
               /* make sure no one uses these */
            expr[(yyval)].adr = expr[(yyval)].lev = -1;
            /* push the parm count for interp's SSCANF_OP to use */
            emit1(PUSH_LIT,sscanf_cnt); 
            emit1(SSCANF_OP,(yyvsp[-3]));
         }
#line 3466 "y.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 1627 "bapascal.y" /* yacc.c:1646  */
    { 
            if (expr[(yyvsp[-2])].typ != strings)
               yyerror("Leftmost sscanf parameter must be of type 'string'");
            (yyval) = expr[(yyvsp[0])].adr;  /* pass rawstring index up the tree */
            emit_push_addr(expr[(yyvsp[-2])]);
            free_expr((yyvsp[-2])); free_expr((yyvsp[0]));
            sscanf_cnt = 0;
         }
#line 3479 "y.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 1638 "bapascal.y" /* yacc.c:1646  */
    {  
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != strings))
               yyerror(
            "sscanf parameter must be either of type 'int' or type 'string'");
            emit_push_addr(expr[(yyvsp[0])]);
            sscanf_cnt++;
            free_expr((yyvsp[0]));
         }
#line 3492 "y.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 1647 "bapascal.y" /* yacc.c:1646  */
    {  
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != strings))
               yyerror(
            "sscanf parameter must be either of type 'int' or type 'string'");
            emit_push_addr(expr[(yyvsp[0])]);
            sscanf_cnt++;
            free_expr((yyvsp[0]));
         }
#line 3505 "y.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 1658 "bapascal.y" /* yacc.c:1646  */
    {
         expr[(yyvsp[-3])].ref = last_aref;
         expr[(yyvsp[-3])].arelt = 1;
         expr[(yyvsp[-3])].typ = last_eltyp;
         topars--;
         if (topars >= 0) {
            last_aref = arstack[topars].aref;
            last_eltyp = arstack[topars].eltyp;
         }
      }
#line 3520 "y.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 1671 "bapascal.y" /* yacc.c:1646  */
    {
         if (tab[(yyvsp[0])].typ != arrays) {
            sprintf(pbuf,"'%s' is not of type ARRAY",tab[(yyvsp[0])].name);
            yyerror(pbuf);
            last_aref = 0;
            last_eltyp = notyp;
         }
         else {
            last_aref = tab[(yyvsp[0])].ref;
            last_eltyp = atab[last_aref].eltyp;
            /* put addr of array base on the stack */
            emit2((tab[(yyvsp[0])].normal ? LOAD_ADDR : LOAD_VALUE),
               tab[(yyvsp[0])].lev,tab[(yyvsp[0])].adr);
         }
         if (++topars > MAXARNEST) 
            yyerror("Maximum array nesting exceeded");
         else {
            arstack[topars].aref = last_aref;
            arstack[topars].eltyp = last_eltyp;
         }
         (yyval) = new_expr();
         expr[(yyval)].obj = tab[(yyvsp[0])].obj;
         expr[(yyval)].typ = last_eltyp;
         expr[(yyval)].adr = tab[(yyvsp[0])].adr;
         expr[(yyval)].ref = tab[(yyvsp[0])].ref;
         expr[(yyval)].lev = tab[(yyvsp[0])].lev;
         expr[(yyval)].normal = tab[(yyvsp[0])].normal;
      }
#line 3553 "y.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 1702 "bapascal.y" /* yacc.c:1646  */
    { 
         gen_exprval((yyvsp[0]));
         last_aref = arstack[topars].aref;
         last_eltyp = arstack[topars].eltyp;
         index_expr((yyvsp[0]),&last_aref,&last_eltyp); 
         arstack[topars].aref = last_aref;
         arstack[topars].eltyp = last_eltyp;
         free_expr((yyvsp[0])); 
      }
#line 3567 "y.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 1712 "bapascal.y" /* yacc.c:1646  */
    { 
         gen_exprval((yyvsp[0]));
         last_aref = arstack[topars].aref;
         last_eltyp = arstack[topars].eltyp;
         index_expr((yyvsp[0]),&last_aref,&last_eltyp); 
         arstack[topars].aref = last_aref;
         arstack[topars].eltyp = last_eltyp;
         free_expr((yyvsp[0])); 
      }
#line 3581 "y.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 1724 "bapascal.y" /* yacc.c:1646  */
    {
         expr[(yyvsp[-1])].typ = ints;
         (yyval) = (yyvsp[-1]);
      }
#line 3590 "y.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 1731 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = loc(lastident,level); }
#line 3596 "y.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 1735 "bapascal.y" /* yacc.c:1646  */
    { (yyval) = enter(lastident,constant,level,level); }
#line 3602 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3606 "y.tab.c" /* yacc.c:1646  */
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
#line 1739 "bapascal.y" /* yacc.c:1906  */
   /*start of routines*/

void process_var_inits(int levl)
{
   int i;
   if (curr_var >= 0) {
      for (i = 0; i <= curr_var; i++) {
         if (levl == var_init[i].lev) {
            emit2(LOAD_ADDR,tab[var_init[i].tix].lev,tab[var_init[i].tix].adr);
            emit1(PUSH_LIT,var_init[i].value);
            emit(STORE);
            var_init[i].lev = -1;
         }  /* if */
      }  /* for */  
   }  /* if */
}

void add_var_init(int* curr_var, int tix, int level, int value)
{
   if (++(*curr_var) == MAXVARINIT) {
      sprintf(pbuf,
         "No more than %d variables in a program can be initialized",
         MAXVARINIT);
      yyerror(pbuf); 
   }
   else {
      var_init[*curr_var].value = value;
      var_init[*curr_var].lev = level;
      var_init[*curr_var].tix = tix;   
   }
}   

/*
 *
 *  $Id: bapascal.y,v 2.21 2007/06/01 18:47:03 bynum Exp $
 *
 *
 */
