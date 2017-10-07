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
#line 1 "bac.y" /* yacc.c:339  */

       /* yacc grammar for BenAri Concurrent C */
       /* based on the grammar for the BenAri Concurrent Pascal compiler */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "../include/bacicnty.h"
#include "../include/globdata.h"
#include "../include/globtabs.h"
#include "../include/genutil.h"
#include "../include/compdata.h"
#include "../include/computil.h"
#include "../include/incfiles.h"
#include "../include/pcode.h"

extern   int   yydebug;
extern   char  yytext[];
extern   char  numbertext[80];
extern   int   numberbase;
extern   char  laststring[];
extern   char  lasttoken[];
extern   void  global_init(int argc, char** argv);
extern   char  *lastident();

extern   int fprintf(FILE*,const char*,...);
extern   int printf(const char*,...);
/* extern   int free(void*); */
extern   void yyerror(char*); 
extern   int yylex(void); 

void process_breaks();
void process_continues();
void process_returns();

void process_var_inits(int level);
void add_var_init(int* curr_var, int tix, int level, int value);

#define YYDEBUG 1

char  comp_tail[] = {"C-- to PCODE Compiler in C, "};
extern char  comp_date[];

char  source_suffix[] = {".cm"};
char  list_suffix[] = {".lst"};

BUFFER   pbuf;   /* string buffer for parsing */
/* constants for passing synthesized information back up the parse tree */
int   dx,level=0;
int maxlevel = 0;
/* for passing type of constant back to constant productions */
TYPES consttype;
/* for passing back information to productions using 'type' prod */

#define NO_EXPR -1
   /* to pass up the parse tree when no entry in the expr array has */
   /* been generated                                                */

int   array_defined;

#define MAXARNEST 50    /* max nesting in array declarations */
struct {
   int aref;         /* atab index */
   TYPES  eltyp;     /* element type for this atab index */
} arstack[MAXARNEST];
int   topars = -1;   /* index of top item on arstack */
int   last_aref;     /* for calculating array references */
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

int   in_cobegin = 0;  /* 1 when in COBEGIN-COEND block */

int   in_include = 0;  /* to signal yylex to change its tactics */

int   by_value;      /* 0 for pass-by-reference parameters, 1 by-value */

int   main_declared = 0;  /* 1 when main proc is declared */

int   making_objfile = 0;  /* 0 unless -c option is used, then 1 */


int   elem_size;     /* size in stack words of variable being declared */
TYPES elem_typ;      /* type of variable being declared */
int   elem_ref;      /* ref value of variable being declared */
int   elem_tix;      /* tab index of type, when it has one */
int   curr_parm_offset;  /* offset to parms in stack frame of curr func */
int   delta_vsize;   /* change in vize parm in a func block due to parms */

int   assign_cnt = 0;   /* for handling multiple assignments */

TYPES formal_type;      /* holds typ field of formal parms */

int   in_func_decl;     /* 1 if declaring a function, 0 otherwise */
int   void_function;    /* 1 if function has VOID return type, 0 otherwise */

#define MAXBREAK  200   
   /* max # breaks in a nest of loop and switch stmts */
#define MAXBRNEST  20   
   /* max depth of the nesting */
int   break_loc[MAXBREAK]; /* loc of 'break' instr */
   /* each 'break' instruction generates a JUMP PCODE instruction.  */
   /* The target of this jump has to be filled in, once it is known.*/
   /* break_loc[curr_break] holds the lc in the 'code' array of the */
   /* JUMP instr of the current break instruction being parsed.     */
   /* For both switch and loop statement, the 'break' instruction   */
   /* generates a JUMP out of the loop or switch.  The action is    */
   /* the same for either an enclosing switch or loop block.        */
int   curr_break = -1;  
   /* index of top of 'break_loc' array */

#define MAXCONT   200   
   /* max # continues in a nest of loop stmts */
int   cont_loc[MAXCONT]; 
   /* each 'continue' instruction generates a JUMP PCODE instr.     */
   /* The target of this jump is always known, but we use the same  */
   /* machinery as is used in the 'break' statement, because cont's */
   /* can occur any any loop, and loops can be nested arbitrarily.  */
   /* cont_loc[curr_break] holds the lc in the 'code' array of the  */
   /* JUMP instr of the current continue instruction being parsed.  */
int   curr_cont = -1;
   /* index of top of 'cont_loc' array */

enum brk_block_type {loop_blk, switch_blk};
typedef enum brk_block_type break_block_type;

   /* breaks can occur in the switch and loop statements and can be */
   /* nested arbitrarily.  We keep the information about breaks, as */
   /* well as continues, in an array-based implementation of a      */
   /* stack of the following structs, to handle the nesting.        */ 
struct {
   int first_break;
      /* index in the break_loc array of the first break in this */
      /* block, if there is one, or -1, if there is none         */
   int first_cont;
      /* index in the cont_loc array of the first break in this  */
      /* block, if there is one, or -1, if there is none         */
   break_block_type bb_type;
      /* type of this break_block:  loop or switch               */
} break_block[MAXBRNEST];

int break_level = -1;
   /* top of the break_block array */

   /* procs for managing the break blocks */
void enter_break_block(int* break_level, break_block_type bbt);
void leave_break_block(int* break_level, int* curr_break, int* curr_cont);

#define MAXCASELOC  200    /* max # 'case' stmts in a nest of switch stmts */
#define MAXSWNEST  20      /* max depth of the nesting */
int   case_loc[MAXCASELOC];   
   /* Each 'case' stmt in a 'switch' block generates a JZER to the     */
   /* code for the next 'case' stmt in the 'switch'.  That is, if the  */
   /* TEST_EQ instruction that checks the case value fails, then a     */
   /* jump to the next case block must occur.  The target of this JZER */
   /* is not known until the next case block is encountered.           */
int   curr_case = -1;      
   /* index of top of case_loc array */

   /* information used to compile switch statements */
struct {
   int expr_index;
      /* index into expr array of the expression in the switch stmt */
   int first_case;
      /* index in the case_loc array of the first case in this switch */
      /* block, if there is one, or -1, if there is none              */
} switch_block[MAXSWNEST];
int   switch_level = -1;  
  /* index of the top of the switch_block array, 1 less than the      */
  /* switch nesting level */
int   case_hdr_cnt = 0;    /* number of case labels for one code block */ 
int   first_case_hdr = -1; /* case_loc index of first label of a codeblk */

#define MAXFORNEST  20
   /* information used to compile for loops */
struct {
   int   iter_loc;      /* lc of iteration code */
   int   cont_loc;      /* lc of continuation test code */
   int   cont_jzer;     /* lc of JZER of continuation test */
   int   stmt_loc;      /* lc of stmt code */
} for_info[MAXFORNEST];
int   curr_for = -1;    /* top of 'for_info' stack */

#define MAXRETURN  50      /* max # returns in a function */
int   return_loc[MAXRETURN];  /* lc of JUMP stmt at return */
int   curr_ret = -1;

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

int   in_mon_decl;      /* 1 if in a monitor declaration, else 0 */

int   char_loc;   /* tab index of char declaration */
int   int_loc;    /* tab index of int declaration */

extern int last_predeclared;  /* tab index of last predeclared id */

int   ob_tix;           /* tab index of outer block */ 
int   outer_call_lc;;   /* lc of outer block SHORTCALL */


int   extern_decl = 0;  /* 1 if in an extern declaration, else 0 */


int   in_typedef = 0;   /* 1 if in an array typedef, 0 otherwise */

int  varargs_cnt;       /* to count the number of args in a varargs call */
                        /* e.g., sprintf, sscanf                         */

#define emit_push_addr(x)  (\
emit2(((x).normal ? LOAD_ADDR : LOAD_VALUE), (x).lev, (x).adr))



#line 304 "y.tab.c" /* yacc.c:339  */

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
    IDENTIFIER = 265,
    CHARCON = 266,
    INCLUDE = 267,
    FNSTRING = 268,
    NE = 269,
    LE = 270,
    GE = 271,
    EQ = 272,
    OR = 273,
    AND = 274,
    PLPL = 275,
    MIMI = 276,
    IF = 277,
    ELSE = 278,
    SWITCH = 279,
    CASE = 280,
    DEFAULT = 281,
    WHILE = 282,
    DO = 283,
    FOR = 284,
    CBEGIN = 285,
    CONST = 286,
    TYPEDEF = 287,
    VOID = 288,
    INT = 289,
    CHAR = 290,
    CONTINUE = 291,
    BREAK = 292,
    RETURN = 293,
    COUT = 294,
    LTLT = 295,
    ENDL = 296,
    CIN = 297,
    GTGT = 298,
    MAIN = 299,
    MONITOR = 300,
    INIT = 301,
    ATOMIC = 302,
    SEND = 303,
    RECEIVE = 304,
    BROADCAST = 305,
    EXTERN = 306,
    SSCANF = 307,
    SPRINTF = 308,
    LOWER_THAN_ELSE = 309
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
#define IDENTIFIER 265
#define CHARCON 266
#define INCLUDE 267
#define FNSTRING 268
#define NE 269
#define LE 270
#define GE 271
#define EQ 272
#define OR 273
#define AND 274
#define PLPL 275
#define MIMI 276
#define IF 277
#define ELSE 278
#define SWITCH 279
#define CASE 280
#define DEFAULT 281
#define WHILE 282
#define DO 283
#define FOR 284
#define CBEGIN 285
#define CONST 286
#define TYPEDEF 287
#define VOID 288
#define INT 289
#define CHAR 290
#define CONTINUE 291
#define BREAK 292
#define RETURN 293
#define COUT 294
#define LTLT 295
#define ENDL 296
#define CIN 297
#define GTGT 298
#define MAIN 299
#define MONITOR 300
#define INIT 301
#define ATOMIC 302
#define SEND 303
#define RECEIVE 304
#define BROADCAST 305
#define EXTERN 306
#define SSCANF 307
#define SPRINTF 308
#define LOWER_THAN_ELSE 309

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

#line 463 "y.tab.c" /* yacc.c:358  */

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
#define YYLAST   1059

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  75
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  144
/* YYNRULES -- Number of rules.  */
#define YYNRULES  261
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  437

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   309

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    74,     2,    55,     2,    73,    61,     2,
      56,    57,    71,    67,    60,    66,     2,    72,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    68,    62,
      69,    65,    70,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    63,     2,    64,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    58,     2,    59,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   260,   260,   267,   277,   292,   293,   296,   297,   298,
     299,   300,   301,   302,   305,   316,   321,   333,   356,   373,
     374,   376,   379,   380,   381,   384,   407,   409,   413,   419,
     427,   436,   443,   459,   479,   483,   484,   485,   488,   489,
     492,   507,   509,   511,   523,   530,   547,   550,   551,   554,
     555,   556,   560,   569,   579,   586,   595,   597,   601,   602,
     605,   606,   613,   616,   623,   630,   640,   653,   654,   657,
     665,   701,   705,   738,   740,   743,   760,   771,   786,   790,
     802,   804,   806,   808,   812,   814,   823,   827,   828,   829,
     830,   833,   841,   845,   849,   856,   860,   861,   862,   864,
     870,   871,   874,   876,   880,   881,   882,   883,   890,   891,
     892,   893,   894,   895,   896,   897,   900,   901,   902,   905,
     932,   934,   949,   963,   971,   982,   995,  1010,  1012,  1014,
    1032,  1036,  1040,  1044,  1058,  1059,  1062,  1065,  1067,  1071,
    1092,  1101,  1112,  1113,  1116,  1120,  1124,  1130,  1135,  1162,
    1169,  1178,  1185,  1188,  1192,  1202,  1208,  1222,  1223,  1233,
    1241,  1245,  1254,  1266,  1286,  1305,  1335,  1344,  1347,  1348,
    1351,  1367,  1369,  1376,  1392,  1395,  1396,  1400,  1408,  1409,
    1412,  1434,  1458,  1469,  1473,  1474,  1488,  1492,  1493,  1507,
    1511,  1512,  1529,  1533,  1534,  1535,  1536,  1537,  1538,  1541,
    1542,  1551,  1562,  1579,  1583,  1584,  1587,  1588,  1609,  1613,
    1614,  1615,  1618,  1619,  1627,  1635,  1637,  1650,  1651,  1663,
    1675,  1687,  1699,  1700,  1701,  1704,  1717,  1762,  1763,  1766,
    1790,  1791,  1794,  1795,  1798,  1809,  1810,  1811,  1812,  1813,
    1816,  1829,  1843,  1847,  1859,  1872,  1891,  1910,  1921,  1933,
    1948,  1959,  1968,  1979,  1993,  2018,  2031,  2061,  2071,  2083,
    2090,  2094
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "UNSIGNED_INT", "STRING", "RAWSTRING",
  "STRINGCONCAT", "STRINGCOMPARE", "STRINGCOPY", "STRINGLENGTH",
  "IDENTIFIER", "CHARCON", "INCLUDE", "FNSTRING", "NE", "LE", "GE", "EQ",
  "OR", "AND", "PLPL", "MIMI", "IF", "ELSE", "SWITCH", "CASE", "DEFAULT",
  "WHILE", "DO", "FOR", "CBEGIN", "CONST", "TYPEDEF", "VOID", "INT",
  "CHAR", "CONTINUE", "BREAK", "RETURN", "COUT", "LTLT", "ENDL", "CIN",
  "GTGT", "MAIN", "MONITOR", "INIT", "ATOMIC", "SEND", "RECEIVE",
  "BROADCAST", "EXTERN", "SSCANF", "SPRINTF", "LOWER_THAN_ELSE", "'#'",
  "'('", "')'", "'{'", "'}'", "','", "'&'", "';'", "'['", "']'", "'='",
  "'-'", "'+'", "':'", "'<'", "'>'", "'*'", "'/'", "'%'", "'!'", "$accept",
  "program", "init_outerscope", "outer_decls", "outer_decl",
  "include_decl", "start_include", "opt_main_decl", "main_hdr",
  "opt_main_type", "compound_stmt", "function_decl", "func_proto",
  "func_hdr", "untyped_func_hdr", "typed_func_hdr", "function_id",
  "formal_params", "param_list", "param_groups", "param_group",
  "param_typespec", "monitor_decl", "monitor_hdr", "mon_internal_decls",
  "internal_decls", "internal_decl", "monitor_init", "the_init",
  "extern_mon_spec", "mon_func_protos", "var_ext_decls", "var_ext_decl",
  "var_decl", "var_typespec", "var_list", "var_name", "array_decl",
  "index_list", "index", "type_decl", "the_typedef", "const_decl",
  "constant", "unsigned_const", "unsigned_num", "extern_decl",
  "extern_func_decl", "the_extern", "extern_var_decl", "extern_array_decl",
  "extern_mon_decl", "stmt_list", "opt_for_expr_list", "for_expr_list",
  "stmt", "special_proc_calls", "send_call", "left_exprparm",
  "broadcast_call", "sprintf_call", "sprintf_buf_fmt", "sprintf_parmlist",
  "selection_stmt", "if_expr", "then_stmt", "the_else", "switch_hdr",
  "case_groups", "case_group", "case_hdr_list", "case_hdr", "begin_case",
  "patch_case_hdrs", "opt_default", "the_default", "cobegin_stmt",
  "iteration_stmt", "the_while", "while_expr", "the_do", "do_stmt_loc",
  "the_do_while", "do_while_expr", "for_init", "the_for",
  "opt_continue_expr", "for_semic2", "opt_iter_exprs", "cbegin",
  "continue_stmt", "break_stmt", "return_stmt", "the_return",
  "output_stmt", "output_list", "output_item", "input_stmt", "null_stmt",
  "opt_expr", "expr_stmt", "expr", "var_assign", "id_becomes",
  "array_var_becomes", "logical_or_expr", "left_logical_or_expr",
  "logical_and_expr", "left_logical_and_expr", "relational_expr",
  "left_relational_expr", "relational_op", "simple_expr",
  "left_simple_expr", "add_op", "term", "left_term", "mult_op", "factor",
  "variable", "var_ident", "func_call", "func_id", "actual_params",
  "actuals_list", "actual_param", "stringfn_call", "stringconcat_call",
  "right_stringparm", "rawstring_parm", "stringcopy_call",
  "stringcompare_call", "stringlength_call", "single_exprparm",
  "sscanf_call", "sscanf_buf_fmt", "sscanf_parmlist", "receive_call",
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
     305,   306,   307,   308,   309,    35,    40,    41,   123,   125,
      44,    38,    59,    91,    93,    61,    45,    43,    58,    60,
      62,    42,    47,    37,    33
};
# endif

#define YYPACT_NINF -303

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-303)))

#define YYTABLE_NINF -262

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -303,    32,   278,  -303,   -21,    45,    42,  -303,    49,    78,
    -303,    82,   191,  -303,    85,   278,  -303,  -303,   123,  -303,
      64,    96,  -303,    64,  -303,  -303,  -303,    99,  -303,   118,
    -303,   162,  -303,    42,  -303,  -303,  -303,   240,  -303,  -303,
    -303,  -303,    48,  -303,  -303,    82,  -303,    99,  -303,  -303,
      49,  -303,    49,  -303,  -303,  -303,  -303,   334,  -303,   125,
    -303,   293,  -303,   387,   135,    99,    92,  -303,  -303,   129,
      82,     6,   122,   145,  -303,  -303,  -303,   130,  -303,   140,
    -303,  -303,   141,  -303,    47,   158,   158,   158,   163,  -303,
     217,   217,   177,   178,  -303,  -303,   183,  -303,   185,   180,
     184,  -303,   207,   205,   158,   158,   163,   193,   196,   934,
    -303,  -303,   985,   985,   985,  -303,   403,  -303,  -303,    82,
    -303,  -303,   527,  -303,  -303,  -303,  -303,  -303,   197,  -303,
     775,   195,  -303,  -303,   208,  -303,   934,   934,   209,  -303,
    -303,  -303,   934,  -303,  -303,  -303,  -303,   201,  -303,   934,
     934,   247,   248,   252,   253,  -303,   147,   113,     1,   114,
     117,  -303,  -303,    84,  -303,   221,  -303,  -303,  -303,  -303,
    -303,  -303,   218,  -303,   215,   226,  -303,     8,   233,   -21,
    -303,   234,   232,  -303,    82,   238,  -303,   236,   812,  -303,
    -303,  -303,  -303,    82,  -303,    48,    12,   130,  -303,   244,
      69,  -303,   853,  -303,    12,  -303,  -303,   845,   254,   254,
     254,   864,  -303,  -303,  -303,  -303,   934,   934,  -303,   934,
    -303,  -303,   202,    -6,   934,   256,    54,  -303,   934,   934,
     185,   243,    24,   114,  -303,   -20,   114,  -303,  -303,   589,
    -303,  -303,   934,  -303,   284,   294,   934,   775,   775,   262,
    -303,   268,   274,  -303,   465,   285,  -303,  -303,  -303,  -303,
     934,   934,  -303,  -303,  -303,  -303,  -303,  -303,   934,  -303,
    -303,   985,  -303,  -303,  -303,   985,  -303,  -303,   915,  -303,
     934,  -303,   934,   273,  -303,  -303,  -303,    91,  -303,  -303,
    -303,  -303,  -303,  -303,   288,  -303,   286,  -303,    48,    48,
    -303,  -303,  -303,  -303,  -303,   287,    27,   289,  -303,  -303,
    -303,   845,  -303,  -303,  -303,   291,   295,   296,   300,   302,
    -303,  -303,  -303,  -303,   202,  -303,   298,   934,  -303,   934,
     305,   306,  -303,  -303,    61,  -303,  -303,   775,  -303,   171,
    -303,   294,  -303,   310,  -303,   347,  -303,   934,  -303,   934,
     651,  -303,   252,  -303,    50,   114,  -303,  -303,  -303,    81,
    -303,  -303,    93,   311,   934,  -303,   465,  -303,  -303,  -303,
    -303,  -303,   315,  -303,   321,   322,  -303,  -303,  -303,  -303,
    -303,  -303,  -303,   323,   324,   383,   383,  -303,   934,  -303,
     934,   326,  -303,   330,   465,  -303,   465,  -303,  -303,   339,
     274,   341,  -303,  -303,  -303,   934,  -303,   934,  -303,   335,
     713,  -303,  -303,  -303,   340,  -303,  -303,  -303,  -303,   337,
    -303,  -303,   775,   775,   934,   353,   775,  -303,  -303,  -303,
    -303,  -303,  -303,   346,  -303,  -303,  -303
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,    16,     1,     0,    33,     0,    78,    21,    20,
      64,     0,     0,    92,     0,    16,     5,     7,     0,     3,
       0,     0,     8,     0,    26,    28,    29,     0,     9,     0,
      12,     0,    10,     0,    11,    13,    87,     0,    88,    89,
      90,    66,     0,   260,    63,     0,    33,     0,   261,    45,
       0,    27,     0,    15,     6,     2,    14,     0,    17,     0,
      25,    35,    30,     0,   261,     0,     0,    67,    71,    69,
       0,    33,     0,     0,    95,    93,    94,     0,    86,     0,
      84,    85,     0,    31,     0,     0,     0,     0,     0,   214,
       0,     0,     0,     0,   149,   151,     0,   162,    63,     0,
       0,   166,     0,     0,     0,     0,     0,     0,     0,     0,
      24,   174,     0,     0,     0,   105,     0,    58,    60,     0,
     213,    61,     0,    96,   115,   116,   117,   118,     0,   104,
       0,     0,   106,   107,     0,   152,   157,   100,     0,   109,
     110,   111,   175,   112,   113,   114,   108,     0,   179,     0,
       0,   178,     0,   184,     0,   187,     0,   190,     0,   199,
       0,   206,   212,   217,   222,     0,   227,   235,   236,   237,
     238,   239,     0,   228,   223,     0,   224,   225,     0,    43,
      36,     0,    37,    38,     0,    41,    49,     0,    52,    47,
      51,    50,    32,     0,    62,     0,     0,    72,    73,     0,
       0,    91,     0,    65,     0,    99,    98,     0,     0,     0,
       0,     0,   246,   219,   225,   221,     0,     0,   156,     0,
     163,   164,     0,     0,     0,     0,     0,   122,     0,     0,
       0,     0,   225,   201,   223,   225,   200,   216,    59,     0,
      23,    97,     0,   131,   127,     0,     0,     0,     0,     0,
     158,     0,   101,   102,     0,     0,   176,   177,   180,   181,
       0,     0,   198,   196,   197,   193,   194,   195,     0,   205,
     204,     0,   209,   210,   211,     0,   218,   220,     0,   226,
       0,   183,     0,   255,   182,    18,    34,     0,    40,    42,
      44,    54,    48,    46,     0,    68,     0,    83,     0,     0,
      70,    80,    74,    77,    76,     0,     0,     0,   243,   121,
     120,     0,   240,   245,   244,     0,     0,     0,     0,     0,
     172,   171,   168,   170,     0,   167,     0,     0,   253,     0,
       0,     0,   215,    22,     0,   125,   132,     0,   140,   142,
     134,   141,   137,     0,   146,     0,   159,   160,   155,     0,
       0,   165,   185,   188,   191,   202,   207,   230,   234,     0,
     233,   251,     0,     0,     0,    39,     0,    75,    81,    82,
      56,    55,     0,    79,     0,     0,   248,   247,   130,   133,
     259,   169,   173,     0,     0,     0,     0,   123,     0,   128,
       0,     0,   135,     0,     0,   138,     0,   150,   153,     0,
     161,     0,   103,   145,   231,     0,   249,     0,   258,     0,
       0,    57,   241,   242,     0,   254,   250,   124,   126,     0,
     144,   129,   143,   136,     0,     0,     0,   232,   252,   257,
      53,   119,   139,     0,   147,   148,   154
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -303,  -303,  -303,  -303,   401,  -303,  -303,   402,  -303,  -303,
     128,   -53,   -36,   407,  -303,  -303,    41,     0,  -303,  -303,
     139,  -303,  -303,   391,  -303,  -303,   241,  -303,  -303,  -303,
    -303,  -303,   319,   -13,    -4,  -303,   250,   -16,  -303,   239,
    -303,  -303,   -49,   242,   -37,   -39,   -31,  -303,  -303,  -303,
    -303,  -303,  -110,  -303,    97,  -105,  -303,  -303,    -5,  -303,
    -303,  -303,  -303,  -303,  -303,  -303,  -303,  -303,  -303,   108,
    -303,   107,  -303,  -303,  -303,  -303,  -303,  -303,  -303,  -303,
    -303,  -303,  -303,  -303,  -303,  -303,  -303,  -303,  -303,  -303,
    -303,  -303,  -303,  -303,  -303,  -303,   126,  -303,  -303,  -303,
    -303,  -109,  -303,  -303,  -303,  -303,  -303,   189,  -303,   199,
    -303,  -303,   190,  -303,  -303,   -93,  -303,  -303,  -107,  -303,
     109,  -303,  -303,  -303,  -303,    52,  -303,  -303,    21,  -302,
    -303,  -303,  -303,   356,  -303,  -303,  -303,  -303,  -101,  -303,
    -303,  -303,    33,    -7
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    15,    16,    17,    18,    19,    20,    21,
     115,    22,    23,    24,    25,    26,    27,    62,   181,   182,
     183,   184,    28,    29,   187,   188,   189,   293,   294,    74,
     306,   116,   117,    30,    31,    66,    67,    68,   197,   198,
      32,    33,    34,   300,   301,   120,    35,    36,    37,    38,
      39,    40,   122,   251,   252,   123,   124,   125,   208,   126,
     127,   128,   334,   129,   130,   244,   337,   131,   339,   340,
     341,   342,   390,   396,   393,   394,   132,   133,   134,   247,
     135,   248,   399,   425,   136,   137,   249,   347,   401,   138,
     139,   140,   141,   142,   143,   223,   322,   144,   145,   255,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   268,   157,   158,   271,   159,   160,   275,   161,   162,
     163,   164,   165,   279,   359,   360,   166,   167,   312,   310,
     168,   169,   170,   212,   171,   172,   362,   173,   174,   175,
     283,   176,   232,    69
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     231,    72,    45,    80,    49,    79,   239,   237,    52,   375,
     186,   234,   234,   234,   191,    78,  -260,   241,   -66,   233,
     236,    76,    43,   297,    75,   243,   121,   250,   253,    70,
      77,     4,     3,   256,   324,    41,  -229,     5,    82,    41,
     258,   259,    42,  -256,   118,    41,     4,    83,    41,    47,
     190,    78,    43,   119,   199,  -260,   325,   185,    43,    46,
      50,    44,    10,   200,  -229,   192,    41,   269,   270,  -261,
      41,  -256,    65,   284,    12,    81,    44,    10,   298,   299,
    -229,   209,   210,   416,   417,   121,   371,  -256,   -63,   284,
     177,    47,    48,    65,    41,   179,    41,    53,   309,   225,
     226,    43,   316,   118,   276,   277,   205,   317,   318,   206,
     319,   328,   119,   323,   329,   326,  -203,  -203,   387,   330,
     331,   388,    57,   214,   214,    44,    10,  -192,  -192,  -192,
    -192,   304,   195,   335,   241,   186,    56,   343,   404,   191,
      59,   405,   344,   345,   350,   235,   235,   235,    58,   177,
     406,    60,   193,   407,   194,    61,    80,    80,   296,   234,
     234,   262,   263,   264,   265,    80,   305,   234,   356,   358,
     234,   361,    64,   363,   234,   190,    63,   288,   355,  -203,
    -203,   178,  -192,  -192,   201,  -208,  -208,  -208,   272,   273,
     274,   -33,   195,   195,   196,     4,   338,   391,    52,   213,
     215,     5,   374,   202,   203,    78,   204,   320,    85,    86,
      87,    88,    43,    89,   207,   323,   266,   267,   383,   211,
     384,    41,    90,    91,    50,    44,    10,    43,    81,    81,
     313,   314,   389,   216,   217,    41,   230,    81,   253,   218,
     402,   219,   220,   321,     4,   241,   221,   222,   224,   228,
      71,   105,   229,   245,   107,   409,   410,   242,   109,    80,
      80,   368,   369,   257,   246,  -186,   260,   254,   112,   113,
     372,  -189,   261,    50,    44,    10,   114,   278,   280,   418,
     281,   419,     4,   185,   422,    11,   423,    12,     5,   282,
     285,   286,   287,   235,   235,   290,   358,   179,   428,   289,
     332,   235,    52,    43,   235,   241,   303,   336,   235,     6,
       7,     8,     9,    10,   311,   433,   327,   241,   241,   338,
      41,   435,   -19,    11,   346,    12,   180,    44,    10,    13,
     348,    81,    81,    14,   349,    84,   364,    78,     4,    41,
      85,    86,    87,    88,    43,    89,   366,   351,   376,   370,
     367,   373,   377,   378,    90,    91,    92,   379,    93,   380,
     382,    94,    95,    96,    97,   385,   386,   397,    98,    10,
      99,   100,   101,   102,   398,   408,   103,   411,   412,   413,
     414,   415,   104,   105,   106,    13,   107,   108,   308,   421,
     109,     4,    57,   110,   420,   424,   111,     5,   426,   429,
     112,   113,   431,   436,    84,   432,    78,     4,   114,    85,
      86,    87,    88,    43,    89,   434,    54,    55,     6,    51,
      50,    44,    10,    90,    91,    92,   365,    93,    73,   292,
      94,    95,    96,    97,    12,   238,   302,    98,    10,    99,
     100,   101,   102,   295,   400,   103,   307,   392,   395,   352,
     381,   104,   105,   106,    13,   107,   108,   427,   354,   109,
     353,    57,   227,     0,     0,   111,    84,     0,    78,   112,
     113,    85,    86,    87,    88,    43,    89,   114,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,     0,   230,
       0,    99,   100,   101,   102,     0,     0,   103,     0,     0,
       0,     0,     0,   104,   105,   106,     0,   107,   108,     0,
       0,   109,     0,    57,     0,     0,     0,   111,     0,     0,
      78,   112,   113,    85,    86,    87,    88,    43,    89,   114,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
       0,   230,     0,    99,   100,   101,   102,     0,     0,   103,
       0,     0,     0,     0,     0,   104,   105,   106,     0,   107,
     108,     0,     0,   109,     0,    57,   240,     0,     0,   111,
       0,     0,    78,   112,   113,    85,    86,    87,    88,    43,
      89,   114,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,     0,   230,     0,    99,   100,   101,   102,     0,
       0,   103,     0,     0,     0,     0,     0,   104,   105,   106,
       0,   107,   108,     0,     0,   109,     0,    57,   333,     0,
       0,   111,     0,     0,    78,   112,   113,    85,    86,    87,
      88,    43,    89,   114,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,     0,   230,     0,    99,   100,   101,
     102,     0,     0,   103,     0,     0,     0,     0,     0,   104,
     105,   106,     0,   107,   108,     0,     0,   109,     0,    57,
     403,     0,     0,   111,     0,     0,    78,   112,   113,    85,
      86,    87,    88,    43,    89,   114,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,     0,   230,     0,    99,
     100,   101,   102,     0,     0,   103,     0,     0,     0,     0,
       0,   104,   105,   106,     0,   107,   108,     0,     0,   109,
       0,    57,   430,     0,     0,   111,     0,     0,    78,   112,
     113,    85,    86,    87,    88,    43,    89,   114,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,     0,   230,
       0,    99,   100,   101,   102,     0,     4,   103,     0,     0,
       0,     0,     5,   104,   105,   106,     0,   107,   108,     0,
       0,   109,     0,    57,     0,     0,     0,   111,     0,     0,
       0,   112,   113,     6,     0,    50,    44,    10,    78,   114,
     308,    85,    86,    87,    88,    43,    89,     4,   291,    12,
       0,     0,     0,     5,     0,    90,    91,    78,     0,   315,
      85,    86,    87,    88,    43,    89,     0,     0,     0,   230,
       0,     0,     0,     0,    90,    91,    50,    44,    10,     0,
       0,     0,     0,     0,   105,     0,     0,   107,   230,     0,
      12,   109,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,   113,   105,     0,     0,   107,     0,    78,   114,
     109,    85,    86,    87,    88,    43,    89,     0,     0,     0,
     112,   113,     0,     0,     0,    90,    91,    78,   114,     0,
      85,    86,    87,    88,    43,    89,     0,     0,     0,   230,
       0,     0,     0,     0,    90,    91,     0,     0,     0,     0,
       0,     0,     0,     0,   105,     0,     0,   107,   230,     0,
       0,   109,   357,     0,     0,     0,     0,     0,     0,     0,
       0,   112,   113,   105,     0,     0,   107,     0,    78,   114,
     109,    85,    86,    87,    88,    43,    89,     0,     0,     0,
     112,   113,     0,     0,     0,    90,    91,     0,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   105,     0,     0,   107,     0,     0,
       0,   109,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114
};

static const yytype_int16 yycheck[] =
{
     109,    37,     6,    42,    11,    42,   116,   114,    12,   311,
      63,   112,   113,   114,    63,     3,    10,   122,    10,   112,
     113,    37,    10,    11,    37,   130,    57,   136,   137,    33,
      37,     4,     0,   142,    40,     2,    56,    10,    45,     6,
     149,   150,    63,    63,    57,    12,     4,    47,    15,     8,
      63,     3,    10,    57,    70,    10,    62,    61,    10,    10,
      33,    34,    35,    70,    56,    65,    33,    66,    67,    63,
      37,    63,    31,    65,    47,    42,    34,    35,    66,    67,
      56,    86,    87,   385,   386,   116,    59,    63,    10,    65,
      57,    50,    10,    52,    61,     4,    63,    12,   207,   104,
     105,    10,   211,   116,    20,    21,    59,   216,   217,    62,
     219,    57,   116,   222,    60,   224,    66,    67,    57,   228,
     229,    60,    58,    90,    91,    34,    35,    14,    15,    16,
      17,    62,    63,   242,   239,   188,    13,   246,    57,   188,
      44,    60,   247,   248,   254,   112,   113,   114,    20,   116,
      57,    23,    60,    60,    62,    56,   195,   196,   195,   260,
     261,    14,    15,    16,    17,   204,   202,   268,   275,   278,
     271,   280,    10,   282,   275,   188,    58,   184,   271,    66,
      67,    56,    69,    70,    62,    71,    72,    73,    71,    72,
      73,    56,    63,    63,    65,     4,    25,    26,   202,    90,
      91,    10,   311,    58,    64,     3,    65,     5,     6,     7,
       8,     9,    10,    11,    56,   324,    69,    70,   327,    56,
     329,   188,    20,    21,    33,    34,    35,    10,   195,   196,
     209,   210,   337,    56,    56,   202,    34,   204,   347,    56,
     349,    56,    62,    41,     4,   350,    62,    40,    43,    56,
      10,    49,    56,    58,    52,   364,   366,    60,    56,   298,
     299,   298,   299,    62,    56,    18,    18,    58,    66,    67,
     306,    19,    19,    33,    34,    35,    74,    56,    60,   388,
      65,   390,     4,   287,   394,    45,   396,    47,    10,    63,
      57,    57,    60,   260,   261,    59,   405,     4,   407,    61,
      57,   268,   306,    10,   271,   410,    62,    23,   275,    31,
      32,    33,    34,    35,    60,   424,    60,   422,   423,    25,
     287,   426,    44,    45,    62,    47,    33,    34,    35,    51,
      62,   298,   299,    55,    60,     1,    63,     3,     4,   306,
       6,     7,     8,     9,    10,    11,    58,    62,    57,    62,
      64,    62,    57,    57,    20,    21,    22,    57,    24,    57,
      62,    27,    28,    29,    30,    60,    60,    57,    34,    35,
      36,    37,    38,    39,    27,    64,    42,    62,    57,    57,
      57,    57,    48,    49,    50,    51,    52,    53,     5,    59,
      56,     4,    58,    59,    68,    56,    62,    10,    57,    64,
      66,    67,    62,    57,     1,    68,     3,     4,    74,     6,
       7,     8,     9,    10,    11,    62,    15,    15,    31,    12,
      33,    34,    35,    20,    21,    22,   287,    24,    37,   188,
      27,    28,    29,    30,    47,   116,   197,    34,    35,    36,
      37,    38,    39,   193,   347,    42,   204,   339,   341,   260,
     324,    48,    49,    50,    51,    52,    53,   405,   268,    56,
     261,    58,   106,    -1,    -1,    62,     1,    -1,     3,    66,
      67,     6,     7,     8,     9,    10,    11,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    22,    -1,    24,
      -1,    -1,    27,    28,    29,    30,    -1,    -1,    -1,    34,
      -1,    36,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    52,    53,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    62,    -1,    -1,
       3,    66,    67,     6,     7,     8,     9,    10,    11,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,
      -1,    24,    -1,    -1,    27,    28,    29,    30,    -1,    -1,
      -1,    34,    -1,    36,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    -1,    56,    -1,    58,    59,    -1,    -1,    62,
      -1,    -1,     3,    66,    67,     6,     7,     8,     9,    10,
      11,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      21,    22,    -1,    24,    -1,    -1,    27,    28,    29,    30,
      -1,    -1,    -1,    34,    -1,    36,    37,    38,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    -1,    56,    -1,    58,    59,    -1,
      -1,    62,    -1,    -1,     3,    66,    67,     6,     7,     8,
       9,    10,    11,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    22,    -1,    24,    -1,    -1,    27,    28,
      29,    30,    -1,    -1,    -1,    34,    -1,    36,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    -1,    56,    -1,    58,
      59,    -1,    -1,    62,    -1,    -1,     3,    66,    67,     6,
       7,     8,     9,    10,    11,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    21,    22,    -1,    24,    -1,    -1,
      27,    28,    29,    30,    -1,    -1,    -1,    34,    -1,    36,
      37,    38,    39,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    -1,    56,
      -1,    58,    59,    -1,    -1,    62,    -1,    -1,     3,    66,
      67,     6,     7,     8,     9,    10,    11,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    22,    -1,    24,
      -1,    -1,    27,    28,    29,    30,    -1,    -1,    -1,    34,
      -1,    36,    37,    38,    39,    -1,     4,    42,    -1,    -1,
      -1,    -1,    10,    48,    49,    50,    -1,    52,    53,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    66,    67,    31,    -1,    33,    34,    35,     3,    74,
       5,     6,     7,     8,     9,    10,    11,     4,    46,    47,
      -1,    -1,    -1,    10,    -1,    20,    21,     3,    -1,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    20,    21,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    52,    34,    -1,
      47,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    49,    -1,    -1,    52,    -1,     3,    74,
      56,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    20,    21,     3,    74,    -1,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    52,    34,    -1,
      -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    49,    -1,    -1,    52,    -1,     3,    74,
      56,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    20,    21,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    76,    77,     0,     4,    10,    31,    32,    33,    34,
      35,    45,    47,    51,    55,    78,    79,    80,    81,    82,
      83,    84,    86,    87,    88,    89,    90,    91,    97,    98,
     108,   109,   115,   116,   117,   121,   122,   123,   124,   125,
     126,   217,    63,    10,    34,   109,    10,    91,    10,   218,
      33,    88,   109,    12,    79,    82,    13,    58,    85,    44,
      85,    56,    92,    58,    10,    91,   110,   111,   112,   218,
     109,    10,    87,    98,   104,   108,   112,   218,     3,   119,
     120,   217,   218,    92,     1,     6,     7,     8,     9,    11,
      20,    21,    22,    24,    27,    28,    29,    30,    34,    36,
      37,    38,    39,    42,    48,    49,    50,    52,    53,    56,
      59,    62,    66,    67,    74,    85,   106,   107,   108,   109,
     120,   121,   127,   130,   131,   132,   134,   135,   136,   138,
     139,   142,   151,   152,   153,   155,   159,   160,   164,   165,
     166,   167,   168,   169,   172,   173,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   187,   188,   190,
     191,   193,   194,   195,   196,   197,   201,   202,   205,   206,
     207,   209,   210,   212,   213,   214,   216,   217,    56,     4,
      33,    93,    94,    95,    96,   109,    86,    99,   100,   101,
     108,   117,    92,    60,    62,    63,    65,   113,   114,   112,
     218,    62,    58,    64,    65,    59,    62,    56,   133,   133,
     133,    56,   208,   195,   217,   195,    56,    56,    56,    56,
      62,    62,    40,   170,    43,   133,   133,   208,    56,    56,
      34,   176,   217,   190,   213,   217,   190,   193,   107,   127,
      59,   130,    60,   130,   140,    58,    56,   154,   156,   161,
     176,   128,   129,   176,    58,   174,   176,    62,   176,   176,
      18,    19,    14,    15,    16,    17,    69,    70,   186,    66,
      67,   189,    71,    72,    73,   192,    20,    21,    56,   198,
      60,    65,    63,   215,    65,    57,    57,    60,   218,    61,
      59,    46,   101,   102,   103,   111,   119,    11,    66,    67,
     118,   119,   114,    62,    62,    87,   105,   118,     5,   176,
     204,    60,   203,   203,   203,     5,   176,   176,   176,   176,
       5,    41,   171,   176,    40,    62,   176,    60,    57,    60,
     176,   176,    57,    59,   137,   176,    23,   141,    25,   143,
     144,   145,   146,   176,   130,   130,    62,   162,    62,    60,
     127,    62,   182,   184,   187,   190,   193,    57,   176,   199,
     200,   176,   211,   176,    63,    95,    58,    64,   119,   119,
      62,    59,    87,    62,   176,   204,    57,    57,    57,    57,
      57,   171,    62,   176,   176,    60,    60,    57,    60,   130,
     147,    26,   144,   149,   150,   146,   148,    57,    27,   157,
     129,   163,   176,    59,    57,    60,    57,    60,    64,   176,
     127,    62,    57,    57,    57,    57,   204,   204,   176,   176,
      68,    59,   127,   127,    56,   158,    57,   200,   176,    64,
      59,    62,    68,   176,    62,   130,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    75,    76,    76,    77,    78,    78,    79,    79,    79,
      79,    79,    79,    79,    80,    81,    82,    82,    83,    84,
      84,    84,    85,    85,    85,    86,    87,    87,    88,    88,
      89,    90,    90,    91,    92,    93,    93,    93,    94,    94,
      95,    96,    96,    96,    97,    98,    99,   100,   100,   101,
     101,   101,   102,   102,   103,   104,   105,   105,   106,   106,
     107,   107,   108,   109,   109,   109,   109,   110,   110,   111,
     111,   111,   112,   113,   113,   114,   115,   115,   116,   117,
     118,   118,   118,   118,   119,   119,   120,   121,   121,   121,
     121,   122,   123,   124,   125,   126,   127,   127,   127,   127,
     128,   128,   129,   129,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   131,   131,   131,   132,
     133,   133,   134,   135,   136,   137,   137,   138,   138,   138,
     139,   140,   141,   142,   143,   143,   144,   145,   145,   146,
     147,   148,   149,   149,   150,   151,   152,   152,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   161,   162,
     163,   163,   164,   165,   166,   167,   168,   169,   170,   170,
     171,   171,   171,   172,   173,   174,   174,   175,   176,   176,
     177,   177,   178,   179,   180,   180,   181,   182,   182,   183,
     184,   184,   185,   186,   186,   186,   186,   186,   186,   187,
     187,   187,   187,   188,   189,   189,   190,   190,   191,   192,
     192,   192,   193,   193,   193,   193,   193,   194,   194,   194,
     194,   194,   194,   194,   194,   195,   196,   196,   196,   197,
     198,   198,   199,   199,   200,   201,   201,   201,   201,   201,
     202,   203,   203,   204,   205,   206,   207,   208,   208,   209,
     210,   211,   211,   212,   212,   213,   214,   215,   215,   216,
     217,   218
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     2,     0,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     0,     2,     4,     0,
       1,     1,     4,     3,     2,     2,     1,     2,     1,     1,
       2,     3,     3,     1,     3,     0,     1,     1,     1,     3,
       2,     1,     2,     1,     4,     2,     2,     1,     2,     1,
       1,     1,     0,     4,     1,     4,     2,     3,     1,     2,
       1,     1,     3,     1,     1,     4,     1,     1,     3,     1,
       3,     1,     2,     1,     2,     3,     4,     4,     1,     6,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     2,     2,     2,     1,     2,     2,     2,
       0,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       2,     2,     2,     4,     5,     1,     3,     2,     4,     5,
       4,     1,     1,     4,     1,     2,     3,     1,     2,     4,
       0,     0,     0,     2,     2,     4,     3,     6,     6,     1,
       3,     1,     0,     1,     3,     3,     2,     0,     1,     1,
       0,     1,     1,     2,     2,     3,     1,     3,     2,     3,
       1,     1,     1,     4,     1,     0,     1,     2,     1,     1,
       2,     2,     2,     2,     1,     3,     1,     1,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     2,     1,     2,     2,
       2,     2,     1,     1,     1,     1,     2,     1,     1,     1,
       2,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     1,     3,     3,     2,     3,     3,     4,
       5,     1,     3,     3,     5,     2,     1,     4,     3,     4,
       1,     1
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
#line 261 "bac.y" /* yacc.c:1646  */
    { 
            if (!making_objfile&&!main_declared) 
               yyerror("No 'main' function declared");
            if (main_declared)
               emit(SHORTRET);
         }
#line 2032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 268 "bac.y" /* yacc.c:1646  */
    { 
            if (!making_objfile&&!main_declared) 
               yyerror("No 'main' function declared");
            if (main_declared)
               emit(SHORTRET);
         }
#line 2043 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 277 "bac.y" /* yacc.c:1646  */
    { 
            level = 0;
            extern_decl = 0;
            enter(outerblock_name,outerblock,level,level);
            ob_tix = last_tab;
            tab[ob_tix].adr = lc;
            /* initialize the block table index with the outer block*/
            tab[ob_tix].ref = last_btab = 0;
            btab[last_btab].lastpar = 0;
            btab[last_btab].psize = btab[last_btab].vsize = 0;
            /* initialize the static display */
            display[last_btab] = level;
         }
#line 2061 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 306 "bac.y" /* yacc.c:1646  */
    {
            if (strcmp(laststring,infile[curr_infile].fname) == 0) 
               yyerror("Recursive file inclusion");
            else {
               curr_infile = open_infile(laststring,curr_infile);
            }
            in_include = 0;
         }
#line 2074 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 317 "bac.y" /* yacc.c:1646  */
    { in_include = 1; }
#line 2080 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 321 "bac.y" /* yacc.c:1646  */
    {  int i;
            tab[ob_tix].adr = lc;  /* outerblock entry pt */
            for (i = 0; i <= mon; i++){
               /* don't emit CALL_MONINIT for ext_monitor objects */
               if (tab[mtab[i]].obj == monitor) {
                  emit1(MARKSTACK,mtab[i]-last_predeclared);
                  emit(CALL_MONINIT);
               }
            }
            assert(level == 0);
            process_var_inits(level);
         }
#line 2097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 334 "bac.y" /* yacc.c:1646  */
    { 
            int i;
            emit(HALT); 
            btab[tab[(yyvsp[-1])].ref].last = last_tab;   /* set last var ref in main */
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
            assert(level == 1);
            process_var_inits(level);
            process_var_inits(level-1);   /* outer block variables, too */
         }
#line 2121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 357 "bac.y" /* yacc.c:1646  */
    {  
            extern_decl = 0;
            enter("main",mainproc,level,level);
            prt = last_tab;
            enter_block(&last_btab,&level,last_tab);
            btab[last_btab].lastpar = last_tab; /* no parms */
            if (maxlevel < level) maxlevel = level;
            main_declared = 1;
            (yyval) = prt;
            btab[0].lastpar = prt;  /* save main's tabix for the interp */
            tab[prt].adr = lc;      /* main pgm entry point */
            outer_call_lc = lc;
            emit(SHORTCALL);  /* will have to backpatch addr later */
         }
#line 2140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 375 "bac.y" /* yacc.c:1646  */
    { (yyval) = int_loc; }
#line 2146 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 385 "bac.y" /* yacc.c:1646  */
    {
            if (main_declared)
               yyerror("main() proc must be last");
            if (void_function) {
               if (curr_ret >= 0) process_returns();
               if (tab[(yyvsp[-1])].mon) emit(EXITMON);
               emit(EXIT_PROC);
            }
            else {
               if (curr_ret >= 0) 
                  process_returns();
               else
                  yyerror("A non-void function must have at least one RETURN statment");
               if (tab[(yyvsp[-1])].mon) emit(EXITMON);
               emit(EXIT_FCN);
            } 
            leave_block(&level);
            in_func_decl = 0;
            curr_ret = -1;
         }
#line 2171 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 408 "bac.y" /* yacc.c:1646  */
    { tab[(yyvsp[0])].atomic = 0; }
#line 2177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 410 "bac.y" /* yacc.c:1646  */
    { tab[(yyvsp[0])].atomic = 1; (yyval) = (yyvsp[0]); }
#line 2183 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 414 "bac.y" /* yacc.c:1646  */
    { 
            btab[display[level]].psize = curr_parm_offset;
            btab[display[level]].vsize = curr_parm_offset + delta_vsize;
            in_func_decl = 1;
         }
#line 2193 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 420 "bac.y" /* yacc.c:1646  */
    { 
            btab[display[level]].psize = curr_parm_offset;
            btab[display[level]].vsize = curr_parm_offset + delta_vsize;
            in_func_decl = 1;
         }
#line 2203 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 428 "bac.y" /* yacc.c:1646  */
    { 
            tab[(yyvsp[-1])].typ = ints; 
            cnonfatal("Type of function %s defaults to int",tab[(yyvsp[-1])].name);
            tab[(yyvsp[0])].obj = (extern_decl ? ext_function : function );
            void_function = 0;
         }
#line 2214 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 437 "bac.y" /* yacc.c:1646  */
    {
            tab[(yyvsp[-1])].typ = notyp;
            tab[(yyvsp[-1])].obj = (extern_decl ? ext_procedure : procedure);
            (yyval) = (yyvsp[-1]);
            void_function = 1;
         }
#line 2225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 444 "bac.y" /* yacc.c:1646  */
    {
            if ((tab[(yyvsp[-2])].typ != ints)&&(tab[(yyvsp[-2])].typ != chars)){
               sprintf(pbuf,"'%s' is not a valid function return type",
                                 tab[(yyvsp[-2])].name); 
               yyerror(pbuf); 
               tab[(yyvsp[-1])].typ = notyp;
            } 
            else
               tab[(yyvsp[-1])].typ = tab[(yyvsp[-2])].typ;
            tab[(yyvsp[-1])].obj = (extern_decl ? ext_function : function );
            (yyval) = (yyvsp[-1]);
            void_function = 0;
         }
#line 2243 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 460 "bac.y" /* yacc.c:1646  */
    { int vis_level,real_level;
            if (in_mon_decl) {
               real_level = level;
               vis_level = ((level > 0) ? level -1 : 0); 
            }
            else {
               vis_level = real_level = level;
            }
            (yyval) = prt = enter(lastident(),function,real_level,vis_level);
            if (in_mon_decl) tab[prt].mon = mtab[mon] - last_predeclared;
            enter_block(&last_btab,&level,prt);
            if (maxlevel < level) maxlevel = level;
            curr_parm_offset = btab[display[level]].vsize;
            delta_vsize = curr_parm_offset - btab[display[level]].psize;
            tab[(yyval)].adr = (extern_decl ?  -1 : lc);
            if (in_mon_decl&&!extern_decl) emit1(ENTERMON,tab[prt].mon);
         }
#line 2265 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 480 "bac.y" /* yacc.c:1646  */
    { btab[display[level]].lastpar = last_tab;}
#line 2271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 493 "bac.y" /* yacc.c:1646  */
    {
            tab[(yyvsp[0])].typ = elem_typ;
            tab[(yyvsp[0])].ref = elem_ref;
            tab[(yyvsp[0])].obj = variable;
            tab[(yyvsp[0])].lev = level;
            tab[(yyvsp[0])].normal = by_value;
            tab[(yyvsp[0])].adr = curr_parm_offset;
            if (by_value) 
               curr_parm_offset += elem_size;
            else
               curr_parm_offset++;  /* pointers occupy 1 stack word */
         }
#line 2288 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 508 "bac.y" /* yacc.c:1646  */
    { by_value = 1; }
#line 2294 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 510 "bac.y" /* yacc.c:1646  */
    { by_value = 0; }
#line 2300 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 512 "bac.y" /* yacc.c:1646  */
    { /* allow strings of indefinite length */
            elem_ref = 0;
            elem_size = 1;  /* a pointer */
            elem_typ  = strings;
            elem_tix = -1; /* not in symbol table */
            by_value = 0;  /* pass-by-reference */
            (yyval) = -1;       /* not in symbol table */
         }
#line 2313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 524 "bac.y" /* yacc.c:1646  */
    {
            leave_block(&level);
            in_mon_decl = 0;
         }
#line 2322 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 531 "bac.y" /* yacc.c:1646  */
    {
            if (mon == MAXMON) 
               cfatal("No more than %d monitors can be declared",MAXMON);
            tab[(yyvsp[0])].obj = (extern_decl ? ext_monitor : monitor);
            mtab[++mon] = (yyvsp[0]);
            in_mon_decl = 1;
            (yyval) = (yyvsp[0]);
            if (level > 0)
               yyerror("Monitors can only be declared at the global level");
            enter_block(&last_btab,&level,last_tab);
            btab[last_btab].lastpar = last_tab;  /* no parms */
            if (maxlevel < level) maxlevel = level;
         }
#line 2340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 560 "bac.y" /* yacc.c:1646  */
    { 
            if (tab[mtab[mon]].obj == monitor) {
               /* don't do the following for ext_monitor objects */
               tab[mtab[mon]].adr = lc;      /* addr of monitor init code */
               process_var_inits(level);
               /* main pgm will CALL_MONINIT, even if no init code */
               emit(RET_MONINIT); 
            }
         }
#line 2354 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 570 "bac.y" /* yacc.c:1646  */
    {
            if (tab[mtab[mon]].obj == monitor) {
               /* don't do the following for ext_monitor objects */
               /* main pgm will CALL_MONINIT, even if no init code */
               emit(RET_MONINIT); 
            }
         }
#line 2366 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 580 "bac.y" /* yacc.c:1646  */
    {
            tab[mtab[mon]].adr = lc;      /* addr of monitor init code */
            process_var_inits(level);
         }
#line 2375 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 587 "bac.y" /* yacc.c:1646  */
    {
               leave_block(&level);
               tab[(yyvsp[-3])].adr = -1;
               extern_decl = 0;
               in_mon_decl = 0;
            }
#line 2386 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 596 "bac.y" /* yacc.c:1646  */
    { leave_block(&level); }
#line 2392 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 598 "bac.y" /* yacc.c:1646  */
    { leave_block(&level); }
#line 2398 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 607 "bac.y" /* yacc.c:1646  */
    {
            if (level > 0)
            yyerror("Externals must be declared at the outer (global) level");
         }
#line 2407 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 617 "bac.y" /* yacc.c:1646  */
    { 
            (yyval) = elem_tix = int_loc; 
            elem_size = stacksize(tab[int_loc].adr); /* tab has byte size */
            elem_typ  = tab[int_loc].typ;
            elem_ref  = tab[int_loc].ref;
         }
#line 2418 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 624 "bac.y" /* yacc.c:1646  */
    { 
            (yyval) = elem_tix = char_loc; 
            elem_size = stacksize(tab[char_loc].adr);
            elem_typ  = tab[char_loc].typ;
            elem_ref  = tab[char_loc].ref;
         }
#line 2429 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 631 "bac.y" /* yacc.c:1646  */
    {
            if ((yyvsp[-1]) <= 0)
               yyerror("A string of non-positive length is not valid");
            else {
               elem_ref = elem_size = stacksize(1 + (yyvsp[-1]));  /* null terminator */
               elem_typ  = strings;
               elem_tix = -1;
            }
         }
#line 2443 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 641 "bac.y" /* yacc.c:1646  */
    { 
            if (tab[(yyvsp[0])].obj != type){
               sprintf(pbuf,"'%s' is not a valid type",tab[(yyvsp[0])].name);
               yyerror(pbuf);
            }
            elem_size = stacksize(tab[(yyvsp[0])].adr); /* adr is size of type object */
            elem_typ = tab[(yyvsp[0])].typ;
            elem_ref = tab[(yyvsp[0])].ref;
            elem_tix = (yyvsp[0]);
         }
#line 2458 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 658 "bac.y" /* yacc.c:1646  */
    {
            tab[(yyvsp[0])].typ = elem_typ;
            tab[(yyvsp[0])].obj = (extern_decl ? ext_variable : variable);
            tab[(yyvsp[0])].ref = elem_ref;
            tab[(yyvsp[0])].adr = btab[display[level]].vsize;
            btab[display[level]].vsize += elem_size;
         }
#line 2470 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 666 "bac.y" /* yacc.c:1646  */
    {
            if (extern_decl) 
               yyerror("External variables cannot be initialized");
            else {
               tab[(yyvsp[-2])].typ = elem_typ;
               tab[(yyvsp[-2])].obj = variable;
               tab[(yyvsp[-2])].ref = elem_ref;
               tab[(yyvsp[-2])].adr = btab[display[level]].vsize;
               btab[display[level]].vsize += elem_size;
               if (tab[(yyvsp[-2])].typ == conds)
                  yyerror("Cannot assign to a 'condition'");
               else if (tab[(yyvsp[-2])].typ == bsems) {
                  if (((yyvsp[0]) < 0)||((yyvsp[0]) > 1)) 
                     yyerror("A binary semaphore can only become 0 or 1");
               }
               else if (tab[(yyvsp[-2])].typ == sems) {
                  if ((yyvsp[0]) < 0)
                     yyerror("A semaphore cannot be negative");
               }
               else if ((tab[(yyvsp[-2])].typ != ints) && (tab[(yyvsp[-2])].typ != chars)) 
                  yyerror("'int' or 'char' type expected");
               else if (tab[(yyvsp[-2])].typ != consttype)
                  yyerror("Type mismatch in assignment");
               if (((in_mon_decl)&&(!in_func_decl))||  /* global monitor vars */
                   ((level == 0)&&(!main_declared))){  /* global main vars */
                  /* in this case, must generate the init code later */
                  add_var_init(&curr_var,(yyvsp[-2]),level,(yyvsp[0])); /* $3 is the value */
               }
               else {   /* declaration in a function */
                  emit2(LOAD_ADDR,tab[(yyvsp[-2])].lev,tab[(yyvsp[-2])].adr);
                  emit1(PUSH_LIT,(yyvsp[0]));
                  emit(STORE);
               }               
            }  /* else not extern */
         }
#line 2510 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 702 "bac.y" /* yacc.c:1646  */
    { in_typedef = 0; }
#line 2516 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 706 "bac.y" /* yacc.c:1646  */
    { 
         int i;
         TYPES eltyp;
         int   elsize,elref;
            elsize = elem_size;
            eltyp = elem_typ;
            tab[(yyvsp[-1])].obj = (extern_decl ? ext_variable : variable);
            tab[(yyvsp[-1])].typ = arrays;
            tab[(yyvsp[-1])].ref = (yyval) = (yyvsp[0]); /* first atab index is passed up the tree */
            if (eltyp == arrays)
               elref = elem_ref;
            else
               elref = 0;
               /* if the array is multidimensional, then the last elref */
               /* should be zero; otherwise, the last (and first) elref */
               /* must be the atab index of the array                   */
            for (i= last_atab; i >= (yyvsp[0]); i--){ /* work from 'back to front' */
               atab[i].elref = elref;
               atab[i].elsize = elsize;
               atab[i].size = (1 + atab[i].high - atab[i].low)*elsize;
               elsize = atab[i].size;
               atab[i].eltyp = eltyp;
               eltyp = arrays;
               elref = i;
            }
            tab[(yyvsp[-1])].adr = btab[display[level]].vsize;
            if (!in_typedef) 
               btab[display[level]].vsize += elsize;
            array_defined = (yyvsp[-1]);
         }
#line 2551 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 739 "bac.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); /* only want to pass on atab index for first dim */}
#line 2557 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 744 "bac.y" /* yacc.c:1646  */
    {
            if (last_atab == AMAX) 
               cfatal("Array table full with %d entries",AMAX);
            atab[++last_atab].inxtyp = consttype;
            if ((yyvsp[-1]) < 0) {
               sprintf(pbuf,"Array bound %d less than zero",(yyvsp[-1]));
               yyerror(pbuf);
               atab[last_atab].high = 0;
            }
            else
               atab[last_atab].high = (yyvsp[-1]) - 1;
            atab[last_atab].low = 0;
            (yyval) = last_atab;  /* pass array tab index up the parse tree */
         }
#line 2576 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 761 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-1]);
            if ((yyvsp[-1]) > 0) {
               tab[(yyvsp[-1])].obj = type;
               tab[(yyvsp[-1])].typ = tab[(yyvsp[-2])].typ;
               tab[(yyvsp[-1])].ref = tab[(yyvsp[-2])].ref; 
               tab[(yyvsp[-1])].adr = tab[(yyvsp[-2])].adr;
            }
            in_typedef = 0;
         }
#line 2591 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 772 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = array_defined;
            if (array_defined > 0) {
               tab[array_defined].obj = type;
               tab[array_defined].typ = arrays;
               tab[array_defined].ref = (yyvsp[-1]);  /* its atab index */
               /* adr field is the size fo the type in BYTES */
               /* atab size is in stack words */
               tab[array_defined].adr = sizeof(int)*atab[(yyvsp[-1])].size;
            }
            in_typedef = 0;
         }
#line 2608 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 787 "bac.y" /* yacc.c:1646  */
    { in_typedef = 1; }
#line 2614 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 791 "bac.y" /* yacc.c:1646  */
    {
            if ((tab[(yyvsp[-4])].typ != ints)&&(tab[(yyvsp[-4])].typ != chars))
               yyerror("Constants must be of type 'int' or 'char'");
            else {
               tab[(yyvsp[-3])].typ = tab[(yyvsp[-4])].typ;
               tab[(yyvsp[-3])].adr = (yyvsp[-1]);    /* adr field of constant holds its val */
               tab[(yyvsp[-3])].ref = 0;
            }
         }
#line 2628 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 803 "bac.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2634 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 805 "bac.y" /* yacc.c:1646  */
    { (yyval) = - (yyvsp[0]); }
#line 2640 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 807 "bac.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2646 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 809 "bac.y" /* yacc.c:1646  */
    { consttype = chars; (yyval) = laststring[0]; }
#line 2652 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 813 "bac.y" /* yacc.c:1646  */
    { consttype = ints; }
#line 2658 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 815 "bac.y" /* yacc.c:1646  */
    {  (yyval) = get_constant(tab[(yyvsp[0])].name,level,&consttype); 
            if ((consttype != ints)&&(consttype != chars)) {
               yyerror("'int' or 'char' type expected");
               consttype = notyp;
            }
         }
#line 2669 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 824 "bac.y" /* yacc.c:1646  */
    { (yyval) = strtoi(numbertext,numberbase); }
#line 2675 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 834 "bac.y" /* yacc.c:1646  */
    { 
            leave_block(&level);
            in_func_decl = 0;
            extern_decl = 0; 
         }
#line 2685 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 842 "bac.y" /* yacc.c:1646  */
    { extern_decl = 1; }
#line 2691 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 846 "bac.y" /* yacc.c:1646  */
    { extern_decl = 0; }
#line 2697 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 850 "bac.y" /* yacc.c:1646  */
    { 
            extern_decl = 0; 
            in_typedef = 0;
         }
#line 2706 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 857 "bac.y" /* yacc.c:1646  */
    { extern_decl = 0; }
#line 2712 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 863 "bac.y" /* yacc.c:1646  */
    { yyerrok; }
#line 2718 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 865 "bac.y" /* yacc.c:1646  */
    { yyerrok; }
#line 2724 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 870 "bac.y" /* yacc.c:1646  */
    { (yyval) = -1; /* send back news that expr_list was empty */ }
#line 2730 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 875 "bac.y" /* yacc.c:1646  */
    { free_expr((yyvsp[0])); }
#line 2736 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 877 "bac.y" /* yacc.c:1646  */
    { free_expr((yyvsp[0])); }
#line 2742 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 884 "bac.y" /* yacc.c:1646  */
    { 
            process_breaks();
               /* continue loc is passed back on the value stack */
            process_continues((yyvsp[0]));
            leave_break_block(&break_level,&curr_break,&curr_cont);
         }
#line 2753 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 906 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = new_expr();
            expr[(yyval)].obj = function;
            expr[(yyval)].typ = ints;
            expr[(yyval)].normal = 1; /* the value will be left on the stack */
            expr[(yyval)].ref = 0;
               /* make sure no one uses these */
            expr[(yyval)].adr = expr[(yyval)].lev = -1;
            if (expr[(yyvsp[-2])].typ != ints)
               yyerror("right parameter is not of type 'int'");
            else {
                  /* put the value of the id expr on the stack */
               gen_exprval((yyvsp[-2]));
               if (expr[(yyvsp[-4])].typ == rawstrings)     /* raw str message */
                  emit1(SEND_RAWSTRING,expr[(yyvsp[-4])].adr);
               else if (expr[(yyvsp[-4])].typ == ints)      /* int message */
                  emit(SEND_INT);
               else if (expr[(yyvsp[-4])].typ == strings)   /* msg is a string var */
                  emit(SEND_STRING);
               else
                  yyerror("parameter is not 'rawstring', 'string', or 'int'");
            }
            free_expr((yyvsp[-4])); free_expr((yyvsp[-2]));
         }
#line 2782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 933 "bac.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 2788 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 935 "bac.y" /* yacc.c:1646  */
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
#line 2804 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 950 "bac.y" /* yacc.c:1646  */
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
#line 2820 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 964 "bac.y" /* yacc.c:1646  */
    {
            /* push the parm count for interp's SPRINTF_OP to use */
            emit1(PUSH_LIT,varargs_cnt); 
            emit1(SPRINTF_OP,(yyvsp[-3]));
         }
#line 2830 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 972 "bac.y" /* yacc.c:1646  */
    { 
            if (expr[(yyvsp[-2])].typ != strings)
               yyerror("Leftmost sprintf parameter must be of type 'string'");
            (yyval) = expr[(yyvsp[0])].adr;   /* pass raw string index up the tree */
            emit_push_addr(expr[(yyvsp[-2])]);
            free_expr((yyvsp[-2])); free_expr((yyvsp[0]));
            varargs_cnt = 0;
         }
#line 2843 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 983 "bac.y" /* yacc.c:1646  */
    {  
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != strings)&&
                (expr[(yyvsp[0])].typ != chars))
               yyerror(
            "sprintf parameter must be either of type 'char', 'int' or type 'string'");
            if (expr[(yyvsp[0])].typ == strings)
               emit_push_addr(expr[(yyvsp[0])]);
            else
               gen_exprval((yyvsp[0]));
            varargs_cnt++;
            free_expr((yyvsp[0]));
         }
#line 2860 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 996 "bac.y" /* yacc.c:1646  */
    {  
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != strings)&&
                (expr[(yyvsp[0])].typ != chars))
               yyerror(
            "sprintf parameter must be either of type 'char', 'int' or type 'string'");
            if (expr[(yyvsp[0])].typ == strings)
               emit_push_addr(expr[(yyvsp[0])]);
            else
               gen_exprval((yyvsp[0]));
            varargs_cnt++;
            free_expr((yyvsp[0]));
         }
#line 2877 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1011 "bac.y" /* yacc.c:1646  */
    { code[(yyvsp[-1])].y = (yyvsp[0]); }
#line 2883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1013 "bac.y" /* yacc.c:1646  */
    { code[(yyvsp[-3])].y = (yyvsp[-1]); code[(yyvsp[-2])].y = lc; }
#line 2889 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1015 "bac.y" /* yacc.c:1646  */
    { int i;
               /* switch_level is incremented in switch_hdr rule        */
               /* grammar also forces at least one case in every switch */
            i = case_loc[curr_case];
            code[i].y = (yyvsp[-1]);      /* JZER of the case */
            /* opcode of last test of swtich must pop the switch expr */
            code[i-1].f = TEST_EQ;
               /* the current case of the enclosing switch block is one */
               /* less than the first case of this block                */
            curr_case = switch_block[switch_level].first_case - 1;
            free_expr(switch_block[switch_level].expr_index);
            switch_level--;
            process_breaks();
            leave_break_block(&break_level,&curr_break,&curr_cont);
         }
#line 2909 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1033 "bac.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[-1])); (yyval) = lc; emit(JZER); free_expr((yyvsp[-1])); }
#line 2915 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1037 "bac.y" /* yacc.c:1646  */
    { (yyval) = lc; }
#line 2921 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1041 "bac.y" /* yacc.c:1646  */
    { emit(JUMP); (yyval) = lc; }
#line 2927 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1045 "bac.y" /* yacc.c:1646  */
    { 
            enter_break_block(&break_level,switch_blk);
            if (++curr_case == MAXCASELOC)
               cfatal(
       "Can't more than %d 'case' statments in a nest of 'switch' stmts",
                  MAXCASELOC);
            gen_exprval((yyvsp[-1]));
            switch_level++;
            switch_block[switch_level].first_case = curr_case;
            switch_block[switch_level].expr_index = (yyvsp[-1]);
         }
#line 2943 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1066 "bac.y" /* yacc.c:1646  */
    { first_case_hdr = curr_case; case_hdr_cnt = 1; }
#line 2949 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 1068 "bac.y" /* yacc.c:1646  */
    {  case_hdr_cnt++; }
#line 2955 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 1072 "bac.y" /* yacc.c:1646  */
    { int i;
         i = switch_block[switch_level].expr_index;
         if ((expr[i].typ != expr[(yyvsp[-1])].typ)&&(expr[(yyvsp[-1])].typ != notyp)) 
            yyerror
               ("Case expression type differs from type of switch expression");
         else {
            gen_exprval((yyvsp[-1]));
            free_expr((yyvsp[-1]));
            emit(TEST_EQ_KEEP);
            if (++curr_case == MAXCASELOC)
               cfatal(
      "No more than %d 'case' statments in a nest of switch statements",
      MAXCASELOC);
            case_loc[curr_case] = lc;
            emit(JZER);
         }
      }
#line 2977 "y.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 1092 "bac.y" /* yacc.c:1646  */
    {  int j;
         j = switch_block[switch_level].first_case;
         if (case_hdr_cnt > 0) emit(JUMP); /* for multiple case labels */
         if (curr_case > j)
            code[case_loc[curr_case]].y = lc;  /* prev JZER target */
      }
#line 2988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 1101 "bac.y" /* yacc.c:1646  */
    {  int i, j, k;
         for (i = 1, j = first_case_hdr; i < case_hdr_cnt; i++, j++ ) {
            k = case_loc[j]; 
            code[k+1].y = lc;    /* k+1 == address of JMP to stmt_list */
         }
         case_hdr_cnt = 0; /* reload for next time */
         first_case_hdr = -1;
      }
#line 3001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 1112 "bac.y" /* yacc.c:1646  */
    { (yyval) = lc; }
#line 3007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 1117 "bac.y" /* yacc.c:1646  */
    { (yyval) = lc; }
#line 3013 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1121 "bac.y" /* yacc.c:1646  */
    { if (level == 1) emit(COEND); in_cobegin = 0; }
#line 3019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 1125 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-2]);             /* continue target */
            emit1(JUMP,(yyvsp[-2])); 
            code[(yyvsp[-1])].y = lc; 
         }
#line 3029 "y.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 1131 "bac.y" /* yacc.c:1646  */
    {  
            (yyval) = (yyvsp[-2]);             /* continue target */
            code[(yyvsp[-1])].y = (yyvsp[-4]);     /* point JUMP to top of DO */
         }
#line 3038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 1136 "bac.y" /* yacc.c:1646  */
    { int i; 
            if ((yyvsp[-2])) {            /* iteration code was generated */
               (yyval) = for_info[curr_for].iter_loc;  /* continue target */
               /* jump back to iteration code after stmt */
               emit1(JUMP,for_info[curr_for].iter_loc); 
               if ((yyvsp[-4])) {         /* if there was continuation code .. */
                  i = for_info[curr_for].stmt_loc - 1;/* JUMP from iter code */
                  code[i].y = for_info[curr_for].cont_loc;  /* to cont code */
               }
               /* will fall through to stmt from iter code if no cond code */
            }
            else {             /* no iteration code was generated */
               (yyval) = for_info[curr_for].stmt_loc;    /* continue target */ 
               if ((yyvsp[-4]))        /* if there is continuation code ... */
                  emit1(JUMP,for_info[curr_for].cont_loc);  /* jmp to it */
               else           /* else jmp back to stmt code */
                  emit1(JUMP,for_info[curr_for].stmt_loc);  /* jmp to it */
            }
            if ((yyvsp[-4]))           /* if there was continuation code... */
               code[for_info[curr_for].cont_jzer].y = lc;   /* jmp out */  
            code[for_info[curr_for].iter_loc-1].y = 
                                             for_info[curr_for].stmt_loc;
            curr_for--;
         }
#line 3067 "y.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 1163 "bac.y" /* yacc.c:1646  */
    { 
            (yyval) = lc;   /* continue target */ 
            enter_break_block(&break_level,loop_blk);
         }
#line 3076 "y.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 1170 "bac.y" /* yacc.c:1646  */
    { 
            gen_exprval((yyvsp[-1])); 
            free_expr((yyvsp[-1])); 
            (yyval) = lc; 
            emit(JZER);
         }
#line 3087 "y.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 1179 "bac.y" /* yacc.c:1646  */
    { 
            enter_break_block(&break_level,loop_blk);
         }
#line 3095 "y.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 1185 "bac.y" /* yacc.c:1646  */
    { (yyval) = lc; /* location of stmt code */ }
#line 3101 "y.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 1189 "bac.y" /* yacc.c:1646  */
    { (yyval) = lc;  /* continue target */ }
#line 3107 "y.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 1193 "bac.y" /* yacc.c:1646  */
    { 
            gen_exprval((yyvsp[-1])); 
            free_expr((yyvsp[-1])); 
            emit1(JZER,lc+2);
            (yyval) = lc;    /* pass loc of JUMP up parse tree */
            emit(JUMP);
         }
#line 3119 "y.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 1203 "bac.y" /* yacc.c:1646  */
    { /* init stuff done, continuation testing code is next */
            for_info[curr_for].cont_loc  = lc;  /* continuation test code */
         }
#line 3127 "y.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 1209 "bac.y" /* yacc.c:1646  */
    { 
            enter_break_block(&break_level,loop_blk);
            if (++curr_for == MAXFORNEST)
               cfatal("No more than %d 'for' statements can be nested",
                  MAXFORNEST);
            else {
               for_info[curr_for].iter_loc = for_info[curr_for].cont_loc = -1;
               for_info[curr_for].cont_jzer = for_info[curr_for].stmt_loc = -1;
            }
         }
#line 3142 "y.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 1222 "bac.y" /* yacc.c:1646  */
    { (yyval) = 0;   /* no continuation code */ }
#line 3148 "y.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 1224 "bac.y" /* yacc.c:1646  */
    { 
            (yyval) = 1;  /* continuation code */
            gen_exprval((yyvsp[0]));  /* expr might not have been generated yet */
            free_expr((yyvsp[0]));
            for_info[curr_for].cont_jzer = lc;    /* mark loc of JZER */
            emit(JZER);
         }
#line 3160 "y.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 1234 "bac.y" /* yacc.c:1646  */
    {
            emit(JUMP);       /* to stmt code, backpatched later */
            for_info[curr_for].iter_loc = lc; /* loc of iteration code */ 
         }
#line 3169 "y.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 1241 "bac.y" /* yacc.c:1646  */
    { 
            (yyval) = 0;   /* no iteration code */
            for_info[curr_for].stmt_loc = lc;   /* loc of stmt code */
         }
#line 3178 "y.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 1246 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = 1;   /* iteration code */
            if (for_info[curr_for].cont_jzer >= 0) /* there was cont code */
               emit(JUMP);       /* to continuation code, backpatched */
            for_info[curr_for].stmt_loc = lc;   /* loc of stmt code */
         }
#line 3189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 1255 "bac.y" /* yacc.c:1646  */
    {
            if (level != 1)
               yyerror("'cobegin' block allowed only in 'main'");
            else if (in_cobegin)
               yyerror("'cobegin' blocks cannot be nested");
            else
               emit(COBEGIN);
            in_cobegin = 1;
         }
#line 3203 "y.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 1267 "bac.y" /* yacc.c:1646  */
    {
            if ((break_level < 0)||
               (break_block[break_level].bb_type != loop_blk))
               yyerror("'continue' is inappropriate outside a loop");
            else {
               if (++curr_cont == MAXCONT)
                  cfatal(
                     "Can't have more than %d 'continue' statements in a loop",
                     MAXCONT);
               else {
                  cont_loc[curr_cont] = lc;
                  if (break_block[break_level].first_cont < 0)
                     break_block[break_level].first_cont = curr_cont;
                  emit(JUMP);
               }
            }
         }
#line 3225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 1287 "bac.y" /* yacc.c:1646  */
    {
            if (break_level < 0)
   yyerror("'break' is inappropriate outside a 'loop' or 'switch' statement");
            else {
               if (++curr_break == MAXBREAK)
                  cfatal(
         "Can't have more than %d 'breaks' in a nest of loop or switch stmts",
                     MAXBREAK);
               else {
                  break_loc[curr_break] = lc;
                  if (break_block[break_level].first_break < 0) 
                     break_block[break_level].first_break = curr_break;
                  emit(JUMP);
               }
            }
         }
#line 3246 "y.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 1306 "bac.y" /* yacc.c:1646  */
    {  
            if (void_function) {
               if ((yyvsp[-1]) < 0) {
                  if (++curr_ret == MAXRETURN)
                     cfatal(
   "No more thn %d 'return' statements can appear in a function",MAXRETURN);
                  else
                     return_loc[curr_ret] = lc;    /* mark the JUMP loc */
                  emit(JUMP);
               }
               else
                  yyerror("Cannot return a value from a 'void' function");
            }
            else {   // non void function
               if ((yyvsp[-1]) >= 0) 
                  gen_exprval((yyvsp[-1])); 
               else  /* no expression to push */
                  emit1(PUSH_LIT,0); 
               emit(STORE); 
               if (++curr_ret == MAXRETURN)
                  cfatal(
   "No more thn %d 'return' statements can appear in a function",MAXRETURN);
               else
                  return_loc[curr_ret] = lc;    /* mark the JUMP loc */
               emit(JUMP);   /* jump to fcn exit*/ 
            } 
         }
#line 3278 "y.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 1336 "bac.y" /* yacc.c:1646  */
    { 
            if (!in_func_decl)
               yyerror("'return' is inappropriate outside a function");
            else if (!void_function)
                  emit2(LOAD_ADDR,tab[prt].lev+1,0);
         }
#line 3289 "y.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 1352 "bac.y" /* yacc.c:1646  */
    {
            if (expr[(yyvsp[0])].typ == arrays) /* presently our only struct type */
               yyerror("Cannot output an array");
            else if(expr[(yyvsp[0])].typ == strings) {
               /* an array ref ==> addr is _aldready_ on stack */
               if (!expr[(yyvsp[0])].arelt)
                  emit_push_addr(expr[(yyvsp[0])]);
               emit(WRITE_STRING);
            }
            else{
               gen_exprval((yyvsp[0]));
               emit1(WRITE,expr[(yyvsp[0])].typ);
            }
            free_expr((yyvsp[0]));
         }
#line 3309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 1368 "bac.y" /* yacc.c:1646  */
    { emit(WRITELN); }
#line 3315 "y.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 1370 "bac.y" /* yacc.c:1646  */
    {  
            int tmp = store_string(laststring,&stab_size);
            emit1(WRITE_RAWSTRING,tmp);
         }
#line 3324 "y.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 1377 "bac.y" /* yacc.c:1646  */
    {
            if ((expr[(yyvsp[-1])].obj != variable)&& (expr[(yyvsp[-1])].obj != ext_variable))
               yyerror("input item must be a 'variable'");
            else if ((expr[(yyvsp[-1])].typ != ints)&&(expr[(yyvsp[-1])].typ != chars)&&
                     (expr[(yyvsp[-1])].typ != notyp))
               yyerror("input item must be of type 'int' or 'char'");
            else if (!expr[(yyvsp[-1])].arelt){ 
               emit_push_addr(expr[(yyvsp[-1])]);
               emit1(READ,expr[(yyvsp[-1])].typ);
            } else   /* array element */
               emit1(READ,expr[(yyvsp[-1])].typ);
            free_expr((yyvsp[-1]));
         }
#line 3342 "y.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 1395 "bac.y" /* yacc.c:1646  */
    { (yyval) = -1; }
#line 3348 "y.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 1397 "bac.y" /* yacc.c:1646  */
    { free_expr((yyvsp[0])); }
#line 3354 "y.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 1401 "bac.y" /* yacc.c:1646  */
    {  int i = expr[(yyvsp[-1])].tix;
            if ((tab[i].obj == function) && (!expr[(yyvsp[-1])].result_used))
               yyerror("A non-void function cannot be called as a void function");
            free_expr((yyvsp[-1])); 
         }
#line 3364 "y.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 1413 "bac.y" /* yacc.c:1646  */
    {
            expr[(yyvsp[0])].result_used = 1;
            gen_exprval((yyvsp[0]));
            if (tab[(yyvsp[-1])].typ == expr[(yyvsp[0])].typ) {
               if ((tab[(yyvsp[-1])].typ == sems)||(tab[(yyvsp[-1])].typ == bsems)|| 
                   (tab[(yyvsp[-1])].typ == conds))
                  yyerror("Cannot assign to 'semaphore' or 'condition'");
               else if (tab[(yyvsp[-1])].typ == arrays)
                  emit1(COPY_BLOCK,atab[tab[(yyvsp[-1])].ref].size);
               else  /* standard types */
                  if (assign_cnt > 1)  /* multiple assignments */
                     emit(STORE_KEEP);
                  else                 /* last assignment */
                     emit(STORE);
            }
            else
               yyerror("Type mismatch in assignment");
            (yyval) = (yyvsp[0]);
            expr[(yyval)].tix = (yyvsp[-1]);
            assign_cnt--;
         }
#line 3390 "y.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 1435 "bac.y" /* yacc.c:1646  */
    {
            gen_exprval((yyvsp[0]));
            if (expr[(yyvsp[-1])].typ == expr[(yyvsp[0])].typ) {
               if ((expr[(yyvsp[-1])].typ == sems)||(expr[(yyvsp[-1])].typ == bsems)|| 
                   (expr[(yyvsp[-1])].typ == conds))
                  yyerror("Cannot assign to 'semaphore' or 'condition'");
               else if (expr[(yyvsp[-1])].typ == arrays)
                  emit1(COPY_BLOCK,atab[expr[(yyvsp[-1])].ref].size);
               else  /* standard types */
                  if (assign_cnt > 1)  /* multiple assignments */
                     emit(STORE_KEEP);
                  else                 /* last assignment */
                     emit(STORE);
            }
            else
               yyerror("Type mismatch in assignment");
            free_expr((yyvsp[0]));
            expr[(yyvsp[-1])].isval = 1;
            (yyval) = (yyvsp[-1]);
            assign_cnt--;
         }
#line 3416 "y.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 1459 "bac.y" /* yacc.c:1646  */
    {
            assign_cnt++;
            if ((tab[(yyvsp[-1])].obj == variable)||(tab[(yyvsp[-1])].obj == ext_variable)) {
                  emit_push_addr(tab[(yyvsp[-1])]);
            }
            else
               yyerror("'variable' identifier expected");
         }
#line 3429 "y.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 1470 "bac.y" /* yacc.c:1646  */
    { assign_cnt++; }
#line 3435 "y.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 1475 "bac.y" /* yacc.c:1646  */
    {
            if ((expr[(yyvsp[-2])].typ == ints)&&(expr[(yyvsp[0])].typ == ints)) {
               gen_exprval((yyvsp[0]));
               emit(DO_OR);
            }
            else if ((expr[(yyvsp[-2])].typ != notyp)&&(expr[(yyvsp[0])].typ != notyp)){
               yyerror("type 'int' expected");
               expr[(yyvsp[-2])].typ = notyp;
            }
            free_expr((yyvsp[0]));
         }
#line 3451 "y.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 1489 "bac.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); }
#line 3457 "y.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 1494 "bac.y" /* yacc.c:1646  */
    {
            if ((expr[(yyvsp[-2])].typ == ints)&&(expr[(yyvsp[0])].typ == ints)) {
               gen_exprval((yyvsp[0]));
               emit(DO_AND);
            }
            else if ((expr[(yyvsp[-2])].typ != notyp)&&(expr[(yyvsp[0])].typ != notyp)){
               yyerror("'int' type expected");
               expr[(yyvsp[-2])].typ = notyp;
            }
            free_expr((yyvsp[0]));
         }
#line 3473 "y.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 1508 "bac.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); }
#line 3479 "y.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 1513 "bac.y" /* yacc.c:1646  */
    {
            gen_exprval((yyvsp[0]));
            if (expr[(yyvsp[-2])].typ == expr[(yyvsp[0])].typ){
               if ((expr[(yyvsp[-2])].typ == ints)||(expr[(yyvsp[-2])].typ == chars))
                  emit((yyvsp[-1]));
               else
                  yyerror("Only 'int' and 'char' types can be compared");
            }
            else
               yyerror("Types in comparison are unequal");
            expr[(yyvsp[-2])].typ = ints;
            expr[(yyvsp[-2])].isval = 1;
            free_expr((yyvsp[0]));
         }
#line 3498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 1530 "bac.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); }
#line 3504 "y.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 1533 "bac.y" /* yacc.c:1646  */
    { (yyval) = TEST_EQ; }
#line 3510 "y.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 1534 "bac.y" /* yacc.c:1646  */
    { (yyval) = TEST_LT; }
#line 3516 "y.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 1535 "bac.y" /* yacc.c:1646  */
    { (yyval) = TEST_GT; }
#line 3522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 1536 "bac.y" /* yacc.c:1646  */
    { (yyval) = TEST_LE; }
#line 3528 "y.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 1537 "bac.y" /* yacc.c:1646  */
    { (yyval) = TEST_GE; }
#line 3534 "y.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 1538 "bac.y" /* yacc.c:1646  */
    { (yyval) = TEST_NE; }
#line 3540 "y.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 1543 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[0]);
            gen_exprval((yyval));
            if ((expr[(yyval)].typ != ints)&&(expr[(yyval)].typ != notyp)){
               yyerror("'int' type expected");
               expr[(yyval)].typ = notyp;
            }
         }
#line 3553 "y.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 1552 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[0]);
            gen_exprval((yyval));
            if ((expr[(yyval)].typ != ints)&&(expr[(yyval)].typ != notyp)){
               yyerror("type 'int' expected");
               expr[(yyval)].typ = notyp;
            }
            else
               emit(NEGATE);
         }
#line 3568 "y.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 1563 "bac.y" /* yacc.c:1646  */
    {
            gen_exprval((yyvsp[0]));
            switch ((yyvsp[-1])) {
            case DO_ADD:
            case DO_SUB:
               expr[(yyvsp[-2])].typ = resulttype(expr[(yyvsp[-2])].typ,expr[(yyvsp[0])].typ);
               if (expr[(yyvsp[-2])].typ == ints)
                  emit((yyvsp[-1]));
               break;
            default:
               fatal("grammar error:  bad add_op %d in simp_expr",(yyvsp[-1]));
            } /* switch */
            free_expr((yyvsp[0]));
         }
#line 3587 "y.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 1580 "bac.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); }
#line 3593 "y.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 1583 "bac.y" /* yacc.c:1646  */
    { (yyval) = DO_ADD; }
#line 3599 "y.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 1584 "bac.y" /* yacc.c:1646  */
    { (yyval) = DO_SUB; }
#line 3605 "y.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 1589 "bac.y" /* yacc.c:1646  */
    {
            gen_exprval((yyvsp[0]));
            switch((yyvsp[-1])) {
            case DO_MUL:
            case DO_DIV:
            case DO_MOD:
               if ((expr[(yyvsp[-2])].typ == ints)&&(expr[(yyvsp[0])].typ == ints))
                  emit((yyvsp[-1]));
               else if ((expr[(yyvsp[-2])].typ != notyp)&&(expr[(yyvsp[0])].typ != notyp)){
                  yyerror("type 'int' expected");
                  expr[(yyvsp[-2])].typ = notyp;
               }
               break;
            default:
               fatal("grammar error:  bad mult_op %d in term",(yyvsp[-1]));
            } /* switch */
            free_expr((yyvsp[0]));
         }
#line 3628 "y.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 1610 "bac.y" /* yacc.c:1646  */
    { gen_exprval((yyvsp[0])); }
#line 3634 "y.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 1613 "bac.y" /* yacc.c:1646  */
    { (yyval) = DO_MUL; }
#line 3640 "y.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 1614 "bac.y" /* yacc.c:1646  */
    { (yyval) = DO_DIV; }
#line 3646 "y.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 1615 "bac.y" /* yacc.c:1646  */
    { (yyval) = DO_MOD; }
#line 3652 "y.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 1620 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = new_expr();
            expr[(yyval)].obj = constant;
            expr[(yyval)].adr = (yyvsp[0]);
            expr[(yyval)].typ = ints;
            expr[(yyval)].lev = level;
         }
#line 3664 "y.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 1628 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = new_expr();
            expr[(yyval)].obj = constant;
            expr[(yyval)].adr = laststring[0];
            expr[(yyval)].typ = chars;
            expr[(yyval)].lev = level;
         }
#line 3676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 1636 "bac.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]);}
#line 3682 "y.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 1638 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[0]);
            gen_exprval((yyval));
            if (expr[(yyval)].typ == ints)
               emit(COMPLEMENT);
            else if (expr[(yyval)].typ != notyp){
               yyerror("type 'int' expected");
               expr[(yyval)].typ = notyp;
            }
         }
#line 3697 "y.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 1652 "bac.y" /* yacc.c:1646  */
    {
            if ((expr[(yyvsp[-1])].typ != ints)&&(expr[(yyvsp[-1])].typ != chars))
               yyerror("'int' or 'char' expected");
            else {
               emit_push_addr(expr[(yyvsp[-1])]);
               gen_loadvar((yyvsp[-1]));
               emit1(PUSH_LIT,1);
               emit(DO_ADD);
               emit(STORE);
            }
         }
#line 3713 "y.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1664 "bac.y" /* yacc.c:1646  */
    {
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != chars))
               yyerror("'int' or 'char' expected");
            else {
               emit_push_addr(expr[(yyvsp[0])]);
               gen_loadvar((yyvsp[0]));
               emit1(PUSH_LIT,1);
               emit(DO_ADD);
               emit(STORE);
            }
         }
#line 3729 "y.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1676 "bac.y" /* yacc.c:1646  */
    {
            if ((expr[(yyvsp[-1])].typ != ints)&&(expr[(yyvsp[-1])].typ != chars))
               yyerror("'int' or 'char' expected");
            else {
               emit_push_addr(expr[(yyvsp[-1])]);
               gen_loadvar((yyvsp[-1]));
               emit1(PUSH_LIT,1);
               emit(DO_SUB);
               emit(STORE);
            }
         }
#line 3745 "y.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1688 "bac.y" /* yacc.c:1646  */
    {
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != chars))
               yyerror("'int' or 'char' expected");
            else {
               emit_push_addr(expr[(yyvsp[0])]);
               gen_loadvar((yyvsp[0]));
               emit1(PUSH_LIT,1);
               emit(DO_SUB);
               emit(STORE);
            }
         }
#line 3761 "y.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1705 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = new_expr();
            expr[(yyval)].obj = tab[(yyvsp[0])].obj;
            expr[(yyval)].typ = tab[(yyvsp[0])].typ;
            expr[(yyval)].adr = tab[(yyvsp[0])].adr;
            expr[(yyval)].ref = tab[(yyvsp[0])].ref;
            expr[(yyval)].lev = tab[(yyvsp[0])].lev;
            expr[(yyval)].normal = tab[(yyvsp[0])].normal;
            expr[(yyval)].tix = (yyvsp[0]);
         }
#line 3776 "y.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1718 "bac.y" /* yacc.c:1646  */
    { int k; 
            (yyval) = new_expr();
            expr[(yyval)].obj = tab[(yyvsp[-1])].obj;
            expr[(yyval)].typ = tab[(yyvsp[-1])].typ;
            expr[(yyval)].adr = tab[(yyvsp[-1])].adr;
            expr[(yyval)].ref = tab[(yyvsp[-1])].ref;
            expr[(yyval)].lev = tab[(yyvsp[-1])].lev;
            expr[(yyval)].normal = tab[(yyvsp[-1])].normal;
            expr[(yyval)].tix = (yyvsp[-1]);
            if (tab[(yyvsp[-1])].lev == -1)  /* std function unfinished business */
               stdproc_pcode((yyvsp[-1]),parmct);
            else if ((tab[(yyvsp[-1])].obj == function)||(tab[(yyvsp[-1])].obj == procedure)||
               (tab[(yyvsp[-1])].obj == ext_function)||(tab[(yyvsp[-1])].obj == ext_procedure)) {
               expr[(yyval)].ref = 0;   /* in a func call ref field of func == 0 */
               /* first check on too few parms */
               k = btab[tab[(yyvsp[-1])].ref].lastpar - (yyvsp[-1]);
               if ((k > parmct)&&(first_parmcterr)) {
                  sprintf(pbuf,"Too few actual parameters in the %s call",
                     tab[(yyvsp[-1])].name);
                  yyerror(pbuf);
                  first_parmcterr = 0;
               }
               if ((in_cobegin)&&(tab[(yyvsp[-1])].typ != notyp)) 
                  yyerror("Only VOID function calls are allowed in a \
cobegin block");
               k = btab[tab[(yyvsp[-1])].ref].psize - 1;
               if (tab[(yyvsp[-1])].mon == 0)
                  emit2(CALL,0,k);
               else
                  emit2(CALL,-1,k); /* CALL pcode: x == -1 => mon call */
               if (tab[(yyvsp[-1])].lev < level) /* update display, ord nesting*/
                  emit2(UPDATE_DISPLAY,tab[(yyvsp[-1])].lev,level);
               else if ((tab[(yyvsp[-1])].mon)&&(level == tab[(yyvsp[-1])].lev))
               /* must fix display differently for monitor call */
                  emit2(UPDATE_DISPLAY,level-1,level);
            }
            toppfs--;
            if (toppfs < 0 )
               parmct = last_pf = 0;
            else {
               parmct = pfstack[toppfs].pct;
               last_pf = pfstack[toppfs].tix;
            }
         }
#line 3825 "y.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1767 "bac.y" /* yacc.c:1646  */
    {
            last_pf = (yyvsp[0]);
            if ((tab[(yyvsp[0])].obj != function)&&(tab[(yyvsp[0])].obj != procedure)&&
                (tab[(yyvsp[0])].obj != ext_function)&&(tab[(yyvsp[0])].obj != ext_procedure))
               yyerror("'function call' expected");
            else { /* tab[$1].obj == procedure or function or stdproc*/
               if ((++toppfs) == MAXPFNEST)
                  yyerror("Maximum function call nesting exceeded");
               else{  /* legal func call */
                  pfstack[toppfs].tix = last_pf; 
                  pfstack[toppfs].pct = parmct = 0;
                  first_parmcterr = 1;
                  if (tab[(yyvsp[0])].lev != -1){ /* regular proc call */
                     if ((tab[(yyvsp[0])].mon)&&(tab[prt].mon)&&
                         (tab[(yyvsp[0])].mon != tab[prt].mon))
                        yyerror("Cross monitor calls not allowed");
                     emit1(MARKSTACK,last_pf-last_predeclared);
                  } /* if regular proc */
               } /* legal func call */ 
            } /* if func */
         }
#line 3851 "y.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1799 "bac.y" /* yacc.c:1646  */
    {
            parmct = ++pfstack[toppfs].pct;
            if (tab[last_pf].lev == -1)
               stdproc_parm(last_pf,(yyvsp[0]),parmct);
            else
               proc_parm(last_pf,(yyvsp[0]),parmct);
            free_expr((yyvsp[0]));
         }
#line 3864 "y.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1817 "bac.y" /* yacc.c:1646  */
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
#line 3879 "y.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1830 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = (yyvsp[-1]);
            if (expr[(yyvsp[-1])].typ != strings) {
               yyerror("right parameter is not of type 'string'");
               last_eltyp = notyp;
            }
            else {
               if (!expr[(yyvsp[-1])].arelt)
                  /* push addr of source string */
                  /* addr of array ref is already on the stack */
                  emit_push_addr(expr[(yyvsp[-1])]);
            }
         }
#line 3897 "y.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1844 "bac.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 3903 "y.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1848 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = new_expr();
            expr[(yyval)].obj = variable;
            expr[(yyval)].typ = rawstrings;
            expr[(yyval)].adr = store_string(laststring,&stab_size);
            expr[(yyval)].ref = 1 + strlen(laststring); /* includes null char */
            expr[(yyval)].normal = 1;
            expr[(yyval)].tix = -1;
         }
#line 3917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1860 "bac.y" /* yacc.c:1646  */
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
#line 3932 "y.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1873 "bac.y" /* yacc.c:1646  */
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
#line 3953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1892 "bac.y" /* yacc.c:1646  */
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
#line 3974 "y.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1911 "bac.y" /* yacc.c:1646  */
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
#line 3989 "y.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1922 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = new_expr();
            expr[(yyval)].obj = variable;
            expr[(yyval)].typ = rawstrings;
            expr[(yyval)].adr = store_string(laststring,&stab_size);
            expr[(yyval)].ref = 1 + strlen(laststring); /* includes null char */
            expr[(yyval)].normal = 1;
            expr[(yyval)].tix = -1;
         }
#line 4003 "y.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1934 "bac.y" /* yacc.c:1646  */
    {
            (yyval) = new_expr();
            expr[(yyval)].obj = function;
            expr[(yyval)].typ = ints;
            expr[(yyval)].normal = 1; /* the value will be left on the stack */
            expr[(yyval)].ref = 0;
               /* make sure no one uses these */
            expr[(yyval)].adr = expr[(yyval)].lev = -1;
            /* push the parm count for interp's SSCANF_OP to use */
            emit1(PUSH_LIT,varargs_cnt); 
            emit1(SSCANF_OP,(yyvsp[-3]));
         }
#line 4020 "y.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1949 "bac.y" /* yacc.c:1646  */
    { 
            if (expr[(yyvsp[-2])].typ != strings)
               yyerror("Leftmost sscanf parameter must be of type 'string'");
            (yyval) = expr[(yyvsp[0])].adr;  /* pass rawstring index up the tree */
            emit_push_addr(expr[(yyvsp[-2])]);
            free_expr((yyvsp[-2])); free_expr((yyvsp[0]));
            varargs_cnt = 0;
         }
#line 4033 "y.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1960 "bac.y" /* yacc.c:1646  */
    {  
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != strings))
               yyerror(
            "sscanf parameter must be either of type 'int' or type 'string'");
            emit_push_addr(expr[(yyvsp[0])]);
            varargs_cnt++;
            free_expr((yyvsp[0]));
         }
#line 4046 "y.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1969 "bac.y" /* yacc.c:1646  */
    {  
            if ((expr[(yyvsp[0])].typ != ints)&&(expr[(yyvsp[0])].typ != strings))
               yyerror(
            "sscanf parameter must be either of type 'int' or type 'string'");
            emit_push_addr(expr[(yyvsp[0])]);
            varargs_cnt++;
            free_expr((yyvsp[0]));
         }
#line 4059 "y.tab.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1980 "bac.y" /* yacc.c:1646  */
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
#line 4077 "y.tab.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1994 "bac.y" /* yacc.c:1646  */
    {
            if (expr[(yyvsp[-3])].typ != strings)
               yyerror("Left parameter must be of type 'string'");
            /* implements receive(string,int&) call */
            if ((expr[(yyvsp[-1])].obj != variable)&& (expr[(yyvsp[-1])].obj != ext_variable))
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
#line 4104 "y.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 2019 "bac.y" /* yacc.c:1646  */
    {
            expr[(yyvsp[-1])].ref = last_aref;
            expr[(yyvsp[-1])].arelt = 1;
            expr[(yyvsp[-1])].typ = last_eltyp;
            topars--;
            if (topars >= 0) {
               last_aref = arstack[topars].aref;
               last_eltyp = arstack[topars].eltyp;
            }
         }
#line 4119 "y.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 2032 "bac.y" /* yacc.c:1646  */
    {
            if (tab[(yyvsp[0])].typ != arrays) {
               sprintf(pbuf,"'%s' is not of type 'array'",tab[(yyvsp[0])].name);
               yyerror(pbuf);
               last_aref = 0;
               last_eltyp = notyp;
            }
            else {
               last_aref = tab[(yyvsp[0])].ref;
               last_eltyp = atab[last_aref].eltyp;
               /* put addr of array base on the stack */
               emit_push_addr(tab[(yyvsp[0])]);
            }
            if (++topars > MAXARNEST) 
               yyerror("Maximum array nesting exceeded");
            else {
               arstack[topars].aref = last_aref;
               arstack[topars].eltyp = last_eltyp;
            }
            (yyval) = new_expr();
            expr[(yyval)].obj = tab[(yyvsp[0])].obj;
            expr[(yyval)].typ = tab[(yyvsp[0])].typ;
            expr[(yyval)].adr = tab[(yyvsp[0])].adr;
            expr[(yyval)].ref = tab[(yyvsp[0])].ref;
            expr[(yyval)].lev = tab[(yyvsp[0])].lev;
            expr[(yyval)].normal = tab[(yyvsp[0])].normal;
         }
#line 4151 "y.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 2062 "bac.y" /* yacc.c:1646  */
    { 
            gen_exprval((yyvsp[-1]));
            last_aref = arstack[topars].aref;
            last_eltyp = arstack[topars].eltyp;
            index_expr((yyvsp[-1]),&last_aref,&last_eltyp); 
            arstack[topars].aref = last_aref;
            arstack[topars].eltyp = last_eltyp;
            free_expr((yyvsp[-1])); 
         }
#line 4165 "y.tab.c" /* yacc.c:1646  */
    break;

  case 258:
#line 2072 "bac.y" /* yacc.c:1646  */
    { 
            gen_exprval((yyvsp[-1]));
            last_aref = arstack[topars].aref;
            last_eltyp = arstack[topars].eltyp;
            index_expr((yyvsp[-1]),&last_aref,&last_eltyp); 
            arstack[topars].aref = last_aref;
            arstack[topars].eltyp = last_eltyp;
            free_expr((yyvsp[-1])); 
         }
#line 4179 "y.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 2084 "bac.y" /* yacc.c:1646  */
    {
            expr[(yyvsp[-1])].typ = ints;
            (yyval) = (yyvsp[-1]);
         }
#line 4188 "y.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 2091 "bac.y" /* yacc.c:1646  */
    { (yyval) = loc(lastident(),level); }
#line 4194 "y.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 2095 "bac.y" /* yacc.c:1646  */
    { (yyval) = enter(lastident(),constant,level,level); }
#line 4200 "y.tab.c" /* yacc.c:1646  */
    break;


#line 4204 "y.tab.c" /* yacc.c:1646  */
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
#line 2099 "bac.y" /* yacc.c:1906  */
   /*start of routines*/

void enter_break_block(int* break_level, break_block_type bbt)
{
   (*break_level)++;
   if (*break_level == MAXBRNEST)
      cfatal(
"Can't nest for, do, while, or switch stmts more deeply than %d levels",
MAXBRNEST);
   break_block[*break_level].first_break = -1;
   break_block[*break_level].first_cont  = -1;
   break_block[*break_level].bb_type     = bbt;
}


void leave_break_block(int* break_level, int* curr_break, int* curr_cont)
{
   if (*break_level < 0) {
      *curr_cont = *curr_break = -1;
      return;
   }
      /* the current continue of the outer block is one less than the */
      /* first continue of this block                                 */
   if (break_block[*break_level].first_cont >= 0)
      *curr_cont  = break_block[*break_level].first_cont - 1;
      /* same holds for break stmts, too */
   if (break_block[*break_level].first_break >= 0)
      *curr_break = break_block[*break_level].first_break - 1;
   (*break_level)--;
}


void
process_breaks()
{ 
   int i;
   if (break_level < 0) return;
   if (break_block[break_level].first_break < 0 ) return;
      /* backpatch all of the break targets in this block */
   for (i = break_block[break_level].first_break; i <= curr_break; i++) 
      code[break_loc[i]].y = lc;
}

void process_continues(int lc)
{
   int i;
   if (break_level < 0) return;
   if (break_block[break_level].first_cont < 0 ) return;
      /* backpatch all continue targets in this block */
   for (i = break_block[break_level].first_cont; i <= curr_cont; i++) 
      code[cont_loc[i]].y = lc;
}

void process_returns()
{
   int i;
   if (curr_ret >= 0) {
      for (i = 0; i <= curr_ret; i++)
         code[return_loc[i]].y = lc;   /* point JUMP @ RETURN to fcn exit */
   }
}

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
 *  $Id: bac.y,v 2.30 2007/06/01 18:40:34 bynum Exp $
 *
 */
