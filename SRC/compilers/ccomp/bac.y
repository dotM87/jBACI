%{
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


%}

%start program

%token UNSIGNED_INT
%token STRING RAWSTRING STRINGCONCAT STRINGCOMPARE STRINGCOPY STRINGLENGTH
%token IDENTIFIER CHARCON
%token INCLUDE FNSTRING
%token NE LE GE NE EQ OR AND PLPL MIMI
%token IF ELSE SWITCH CASE DEFAULT
%token WHILE DO FOR
%token CBEGIN CONST TYPEDEF VOID INT CHAR
%token CONTINUE BREAK RETURN
%token COUT LTLT ENDL CIN GTGT
%token MAIN MONITOR INIT ATOMIC
%token SEND RECEIVE BROADCAST
%token EXTERN
%token SSCANF SPRINTF

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%   /*start of rules*/
program  :  init_outerscope  outer_decls  opt_main_decl
         { 
            if (!making_objfile&&!main_declared) 
               yyerror("No 'main' function declared");
            if (main_declared)
               emit(SHORTRET);
         }
      |  init_outerscope  opt_main_decl
         { 
            if (!making_objfile&&!main_declared) 
               yyerror("No 'main' function declared");
            if (main_declared)
               emit(SHORTRET);
         }
      ;

init_outerscope  :  /* no syntax */
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
      ;
         
outer_decls  :  outer_decl
      |  outer_decls  outer_decl
      ;
      
outer_decl  :  include_decl
      |  function_decl
      |  monitor_decl
      |  type_decl
      |  const_decl
      |  var_decl
      |  extern_decl
      ;

include_decl  :  start_include  FNSTRING
         {
            if (strcmp(laststring,infile[curr_infile].fname) == 0) 
               yyerror("Recursive file inclusion");
            else {
               curr_infile = open_infile(laststring,curr_infile);
            }
            in_include = 0;
         }
      ;

start_include  :  '#'  INCLUDE
         { in_include = 1; }
      ;

opt_main_decl  :  /* empty */
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
      | main_hdr  compound_stmt
         { 
            int i;
            emit(HALT); 
            btab[tab[$1].ref].last = last_tab;   /* set last var ref in main */
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
      ;

                                 /* no argc, argv */
main_hdr  :  opt_main_type  MAIN  '('  ')'
         {  
            extern_decl = 0;
            enter("main",mainproc,level,level);
            prt = last_tab;
            enter_block(&last_btab,&level,last_tab);
            btab[last_btab].lastpar = last_tab; /* no parms */
            if (maxlevel < level) maxlevel = level;
            main_declared = 1;
            $$ = prt;
            btab[0].lastpar = prt;  /* save main's tabix for the interp */
            tab[prt].adr = lc;      /* main pgm entry point */
            outer_call_lc = lc;
            emit(SHORTCALL);  /* will have to backpatch addr later */
         }
      ;
   
opt_main_type  :  /* empty */
      |  INT
         { $$ = int_loc; }
      |  VOID
      ;

compound_stmt  :  '{'  var_ext_decls  stmt_list  '}'
      |  '{'  stmt_list  '}'
      |  '{'  '}'
      ;

function_decl  :  func_proto  compound_stmt
         {
            if (main_declared)
               yyerror("main() proc must be last");
            if (void_function) {
               if (curr_ret >= 0) process_returns();
               if (tab[$1].mon) emit(EXITMON);
               emit(EXIT_PROC);
            }
            else {
               if (curr_ret >= 0) 
                  process_returns();
               else
                  yyerror("A non-void function must have at least one RETURN statment");
               if (tab[$1].mon) emit(EXITMON);
               emit(EXIT_FCN);
            } 
            leave_block(&level);
            in_func_decl = 0;
            curr_ret = -1;
         }
      ;

func_proto  :  func_hdr
         { tab[$1].atomic = 0; }
      |  ATOMIC  func_hdr
         { tab[$2].atomic = 1; $$ = $2; }
      ;

func_hdr   :  untyped_func_hdr
         { 
            btab[display[level]].psize = curr_parm_offset;
            btab[display[level]].vsize = curr_parm_offset + delta_vsize;
            in_func_decl = 1;
         }
      |  typed_func_hdr
         { 
            btab[display[level]].psize = curr_parm_offset;
            btab[display[level]].vsize = curr_parm_offset + delta_vsize;
            in_func_decl = 1;
         }
      ;

untyped_func_hdr   :  function_id  formal_params
         { 
            tab[$1].typ = ints; 
            cnonfatal("Type of function %s defaults to int",tab[$1].name);
            tab[$2].obj = (extern_decl ? ext_function : function );
            void_function = 0;
         }
      ;

typed_func_hdr   :  VOID  function_id  formal_params
         {
            tab[$2].typ = notyp;
            tab[$2].obj = (extern_decl ? ext_procedure : procedure);
            $$ = $2;
            void_function = 1;
         }
      |  var_typespec  function_id  formal_params
         {
            if ((tab[$1].typ != ints)&&(tab[$1].typ != chars)){
               sprintf(pbuf,"'%s' is not a valid function return type",
                                 tab[$1].name); 
               yyerror(pbuf); 
               tab[$2].typ = notyp;
            } 
            else
               tab[$2].typ = tab[$1].typ;
            tab[$2].obj = (extern_decl ? ext_function : function );
            $$ = $2;
            void_function = 0;
         }
      ;

function_id  :  IDENTIFIER
         { int vis_level,real_level;
            if (in_mon_decl) {
               real_level = level;
               vis_level = ((level > 0) ? level -1 : 0); 
            }
            else {
               vis_level = real_level = level;
            }
            $$ = prt = enter(lastident(),function,real_level,vis_level);
            if (in_mon_decl) tab[prt].mon = mtab[mon] - last_predeclared;
            enter_block(&last_btab,&level,prt);
            if (maxlevel < level) maxlevel = level;
            curr_parm_offset = btab[display[level]].vsize;
            delta_vsize = curr_parm_offset - btab[display[level]].psize;
            tab[$$].adr = (extern_decl ?  -1 : lc);
            if (in_mon_decl&&!extern_decl) emit1(ENTERMON,tab[prt].mon);
         }
      ;

formal_params  :  '('  param_list  ')'
         { btab[display[level]].lastpar = last_tab;}
      ;

param_list  :  /* empty */
      |  VOID
      |  param_groups
      ;

param_groups  :  param_group
      |  param_groups  ',' param_group
      ;

param_group  :  param_typespec  newident
         {
            tab[$2].typ = elem_typ;
            tab[$2].ref = elem_ref;
            tab[$2].obj = variable;
            tab[$2].lev = level;
            tab[$2].normal = by_value;
            tab[$2].adr = curr_parm_offset;
            if (by_value) 
               curr_parm_offset += elem_size;
            else
               curr_parm_offset++;  /* pointers occupy 1 stack word */
         }
      ;

param_typespec  :  var_typespec     /* by value */
         { by_value = 1; }
      |  var_typespec  '&'          /* by reference */
         { by_value = 0; }
      |  STRING 
         { /* allow strings of indefinite length */
            elem_ref = 0;
            elem_size = 1;  /* a pointer */
            elem_typ  = strings;
            elem_tix = -1; /* not in symbol table */
            by_value = 0;  /* pass-by-reference */
            $$ = -1;       /* not in symbol table */
         }
      ;
      

monitor_decl  :  monitor_hdr  '{'  mon_internal_decls  '}'
         {
            leave_block(&level);
            in_mon_decl = 0;
         }
      ;

monitor_hdr  :  MONITOR  newident
         {
            if (mon == MAXMON) 
               cfatal("No more than %d monitors can be declared",MAXMON);
            tab[$2].obj = (extern_decl ? ext_monitor : monitor);
            mtab[++mon] = $2;
            in_mon_decl = 1;
            $$ = $2;
            if (level > 0)
               yyerror("Monitors can only be declared at the global level");
            enter_block(&last_btab,&level,last_tab);
            btab[last_btab].lastpar = last_tab;  /* no parms */
            if (maxlevel < level) maxlevel = level;
         }
      ; 


mon_internal_decls  :  internal_decls  monitor_init
      ;

internal_decls  :  internal_decl
      |  internal_decls  internal_decl
      ;

internal_decl  : function_decl
      |  const_decl
      |  var_decl
      ;

monitor_init  :  /* empty */
         { 
            if (tab[mtab[mon]].obj == monitor) {
               /* don't do the following for ext_monitor objects */
               tab[mtab[mon]].adr = lc;      /* addr of monitor init code */
               process_var_inits(level);
               /* main pgm will CALL_MONINIT, even if no init code */
               emit(RET_MONINIT); 
            }
         }
      |  the_init  '{'  stmt_list  '}'     /* no variable declarations */
         {
            if (tab[mtab[mon]].obj == monitor) {
               /* don't do the following for ext_monitor objects */
               /* main pgm will CALL_MONINIT, even if no init code */
               emit(RET_MONINIT); 
            }
         }
      ;

the_init  :  INIT
         {
            tab[mtab[mon]].adr = lc;      /* addr of monitor init code */
            process_var_inits(level);
         }
         ;

extern_mon_spec  :  monitor_hdr  '{'  mon_func_protos  '}'
            {
               leave_block(&level);
               tab[$1].adr = -1;
               extern_decl = 0;
               in_mon_decl = 0;
            }
         ;

mon_func_protos  :   func_proto  ';'
            { leave_block(&level); }
         |  mon_func_protos  func_proto  ';'
            { leave_block(&level); }
         ;

var_ext_decls  :  var_ext_decl
      |  var_ext_decls  var_ext_decl
      ;

var_ext_decl  :  var_decl
      |  extern_decl
         {
            if (level > 0)
            yyerror("Externals must be declared at the outer (global) level");
         }      
      ;

var_decl  :  var_typespec  var_list  ';'
      ;

var_typespec  :  INT
         { 
            $$ = elem_tix = int_loc; 
            elem_size = stacksize(tab[int_loc].adr); /* tab has byte size */
            elem_typ  = tab[int_loc].typ;
            elem_ref  = tab[int_loc].ref;
         }
      |  CHAR
         { 
            $$ = elem_tix = char_loc; 
            elem_size = stacksize(tab[char_loc].adr);
            elem_typ  = tab[char_loc].typ;
            elem_ref  = tab[char_loc].ref;
         }
      |  STRING  '['  unsigned_const  ']'
         {
            if ($3 <= 0)
               yyerror("A string of non-positive length is not valid");
            else {
               elem_ref = elem_size = stacksize(1 + $3);  /* null terminator */
               elem_typ  = strings;
               elem_tix = -1;
            }
         }
      |  ident
         { 
            if (tab[$1].obj != type){
               sprintf(pbuf,"'%s' is not a valid type",tab[$1].name);
               yyerror(pbuf);
            }
            elem_size = stacksize(tab[$1].adr); /* adr is size of type object */
            elem_typ = tab[$1].typ;
            elem_ref = tab[$1].ref;
            elem_tix = $1;
         }  
      ;

var_list  :  var_name
      |  var_list  ','  var_name
      ;

var_name  :  newident  
         {
            tab[$1].typ = elem_typ;
            tab[$1].obj = (extern_decl ? ext_variable : variable);
            tab[$1].ref = elem_ref;
            tab[$1].adr = btab[display[level]].vsize;
            btab[display[level]].vsize += elem_size;
         }
      |  newident  '='  constant
         {
            if (extern_decl) 
               yyerror("External variables cannot be initialized");
            else {
               tab[$1].typ = elem_typ;
               tab[$1].obj = variable;
               tab[$1].ref = elem_ref;
               tab[$1].adr = btab[display[level]].vsize;
               btab[display[level]].vsize += elem_size;
               if (tab[$1].typ == conds)
                  yyerror("Cannot assign to a 'condition'");
               else if (tab[$1].typ == bsems) {
                  if (($3 < 0)||($3 > 1)) 
                     yyerror("A binary semaphore can only become 0 or 1");
               }
               else if (tab[$1].typ == sems) {
                  if ($3 < 0)
                     yyerror("A semaphore cannot be negative");
               }
               else if ((tab[$1].typ != ints) && (tab[$1].typ != chars)) 
                  yyerror("'int' or 'char' type expected");
               else if (tab[$1].typ != consttype)
                  yyerror("Type mismatch in assignment");
               if (((in_mon_decl)&&(!in_func_decl))||  /* global monitor vars */
                   ((level == 0)&&(!main_declared))){  /* global main vars */
                  /* in this case, must generate the init code later */
                  add_var_init(&curr_var,$1,level,$3); /* $3 is the value */
               }
               else {   /* declaration in a function */
                  emit2(LOAD_ADDR,tab[$1].lev,tab[$1].adr);
                  emit1(PUSH_LIT,$3);
                  emit(STORE);
               }               
            }  /* else not extern */
         }
      |  array_decl
         { in_typedef = 0; }
      ;

array_decl  :  newident  index_list
         { 
         int i;
         TYPES eltyp;
         int   elsize,elref;
            elsize = elem_size;
            eltyp = elem_typ;
            tab[$1].obj = (extern_decl ? ext_variable : variable);
            tab[$1].typ = arrays;
            tab[$1].ref = $$ = $2; /* first atab index is passed up the tree */
            if (eltyp == arrays)
               elref = elem_ref;
            else
               elref = 0;
               /* if the array is multidimensional, then the last elref */
               /* should be zero; otherwise, the last (and first) elref */
               /* must be the atab index of the array                   */
            for (i= last_atab; i >= $2; i--){ /* work from 'back to front' */
               atab[i].elref = elref;
               atab[i].elsize = elsize;
               atab[i].size = (1 + atab[i].high - atab[i].low)*elsize;
               elsize = atab[i].size;
               atab[i].eltyp = eltyp;
               eltyp = arrays;
               elref = i;
            }
            tab[$1].adr = btab[display[level]].vsize;
            if (!in_typedef) 
               btab[display[level]].vsize += elsize;
            array_defined = $1;
         }
      ;

index_list  :  index
         { $$ = $1; /* only want to pass on atab index for first dim */}
      |  index_list  index
      ;

index  :  '['  unsigned_const  ']'
         {
            if (last_atab == AMAX) 
               cfatal("Array table full with %d entries",AMAX);
            atab[++last_atab].inxtyp = consttype;
            if ($2 < 0) {
               sprintf(pbuf,"Array bound %d less than zero",$2);
               yyerror(pbuf);
               atab[last_atab].high = 0;
            }
            else
               atab[last_atab].high = $2 - 1;
            atab[last_atab].low = 0;
            $$ = last_atab;  /* pass array tab index up the parse tree */
         }
      ;

type_decl  :  the_typedef  var_typespec  newident  ';'
         {
            $$ = $3;
            if ($3 > 0) {
               tab[$3].obj = type;
               tab[$3].typ = tab[$2].typ;
               tab[$3].ref = tab[$2].ref; 
               tab[$3].adr = tab[$2].adr;
            }
            in_typedef = 0;
         }
      |  the_typedef  var_typespec  array_decl  ';'
         {
            $$ = array_defined;
            if (array_defined > 0) {
               tab[array_defined].obj = type;
               tab[array_defined].typ = arrays;
               tab[array_defined].ref = $3;  /* its atab index */
               /* adr field is the size fo the type in BYTES */
               /* atab size is in stack words */
               tab[array_defined].adr = sizeof(int)*atab[$3].size;
            }
            in_typedef = 0;
         }
      ;

the_typedef  :  TYPEDEF
         { in_typedef = 1; }
      ;

const_decl  :  CONST  var_typespec  newident  '='  constant  ';'
         {
            if ((tab[$2].typ != ints)&&(tab[$2].typ != chars))
               yyerror("Constants must be of type 'int' or 'char'");
            else {
               tab[$3].typ = tab[$2].typ;
               tab[$3].adr = $5;    /* adr field of constant holds its val */
               tab[$3].ref = 0;
            }
         }
      ;

constant  :  unsigned_const
         { $$ = $1; }
      |  '-'  unsigned_const
         { $$ = - $2; }
      |  '+'  unsigned_const 
         { $$ = $2; }
      | CHARCON
         { consttype = chars; $$ = laststring[0]; }
      ;

unsigned_const  :  unsigned_num
         { consttype = ints; }
      | ident           /*check that it is constant*/
         {  $$ = get_constant(tab[$1].name,level,&consttype); 
            if ((consttype != ints)&&(consttype != chars)) {
               yyerror("'int' or 'char' type expected");
               consttype = notyp;
            }
         }
      ;

unsigned_num  :  UNSIGNED_INT   /* no reals */
         { $$ = strtoi(numbertext,numberbase); }
      ;

extern_decl  :  extern_func_decl
      |  extern_var_decl
      |  extern_array_decl
      |  extern_mon_decl
   ;

extern_func_decl  :  the_extern  func_proto  ';'
         { 
            leave_block(&level);
            in_func_decl = 0;
            extern_decl = 0; 
         }
      ;

the_extern  :  EXTERN
         { extern_decl = 1; }
      ;

extern_var_decl  :  the_extern  var_decl
         { extern_decl = 0; }
      ;
         
extern_array_decl  :  the_extern  array_decl
         { 
            extern_decl = 0; 
            in_typedef = 0;
         }
      ;
         
extern_mon_decl  :  the_extern  extern_mon_spec
         { extern_decl = 0; }
      ;
         
stmt_list  :  stmt
      |  stmt_list  stmt
      |  error  ';'
         { yyerrok; }
      |  error  '}'
         { yyerrok; }
      ;


opt_for_expr_list  :  /* empty */
         { $$ = -1; /* send back news that expr_list was empty */ }
      |  for_expr_list
      ;

for_expr_list  :  expr
         { free_expr($1); }
      |  for_expr_list  ','  expr
         { free_expr($3); }
      ;

stmt  :  selection_stmt
      |  compound_stmt
      |  cobegin_stmt
      |  iteration_stmt
         { 
            process_breaks();
               /* continue loc is passed back on the value stack */
            process_continues($1);
            leave_break_block(&break_level,&curr_break,&curr_cont);
         }
      |  expr_stmt
      |  continue_stmt
      |  break_stmt
      |  return_stmt
      |  output_stmt
      |  input_stmt
      |  null_stmt
      |  special_proc_calls
      ;

special_proc_calls  : send_call
      |  broadcast_call
      |  sprintf_call
      ;

send_call  :  SEND  left_exprparm  ','  expr  ')'  ';'
         {
            $$ = new_expr();
            expr[$$].obj = function;
            expr[$$].typ = ints;
            expr[$$].normal = 1; /* the value will be left on the stack */
            expr[$$].ref = 0;
               /* make sure no one uses these */
            expr[$$].adr = expr[$$].lev = -1;
            if (expr[$4].typ != ints)
               yyerror("right parameter is not of type 'int'");
            else {
                  /* put the value of the id expr on the stack */
               gen_exprval($4);
               if (expr[$2].typ == rawstrings)     /* raw str message */
                  emit1(SEND_RAWSTRING,expr[$2].adr);
               else if (expr[$2].typ == ints)      /* int message */
                  emit(SEND_INT);
               else if (expr[$2].typ == strings)   /* msg is a string var */
                  emit(SEND_STRING);
               else
                  yyerror("parameter is not 'rawstring', 'string', or 'int'");
            }
            free_expr($2); free_expr($4);
         }
      ;   
          
left_exprparm :  '('  rawstring_parm
         { $$ = $2; }
      |  '('  expr   
         {
            if (expr[$2].typ == strings) {
               /* left parm is a string. If it's not an array elt, */
               /* put the address of the string n the stack        */
               if (!expr[$2].arelt)
                  emit_push_addr(expr[$2]);
            }
            else if (expr[$2].typ == ints)
               gen_exprval($2);  /* put the int on the stack */
            $$ = $2;
         }
      ;   
          
          
broadcast_call :  BROADCAST  single_exprparm
         {
            if (expr[$2].typ == rawstrings)     /* raw str message */
               emit1(BROADCAST_RAWSTRING,expr[$2].adr);
            else if (expr[$2].typ == strings)   /* msg is a string var */
               emit(BROADCAST_STRING);
            else if (expr[$2].typ == ints)      /* int msg */
               emit(BROADCAST_INT);
            else
               yyerror("parameter is not 'rawstring', 'string', or 'int'");
            free_expr($2);
         }
      ;   

sprintf_call  :  sprintf_buf_fmt  ','  sprintf_parmlist  ')'
         {
            /* push the parm count for interp's SPRINTF_OP to use */
            emit1(PUSH_LIT,varargs_cnt); 
            emit1(SPRINTF_OP,$1);
         }
      ;

sprintf_buf_fmt  :  SPRINTF  '('  expr  ','  rawstring_parm
         { 
            if (expr[$3].typ != strings)
               yyerror("Leftmost sprintf parameter must be of type 'string'");
            $$ = expr[$5].adr;   /* pass raw string index up the tree */
            emit_push_addr(expr[$3]);
            free_expr($3); free_expr($5);
            varargs_cnt = 0;
         }
      ;

sprintf_parmlist  :  expr
         {  
            if ((expr[$1].typ != ints)&&(expr[$1].typ != strings)&&
                (expr[$1].typ != chars))
               yyerror(
            "sprintf parameter must be either of type 'char', 'int' or type 'string'");
            if (expr[$1].typ == strings)
               emit_push_addr(expr[$1]);
            else
               gen_exprval($1);
            varargs_cnt++;
            free_expr($1);
         }
      |  sprintf_parmlist  ','  expr
         {  
            if ((expr[$3].typ != ints)&&(expr[$3].typ != strings)&&
                (expr[$3].typ != chars))
               yyerror(
            "sprintf parameter must be either of type 'char', 'int' or type 'string'");
            if (expr[$3].typ == strings)
               emit_push_addr(expr[$3]);
            else
               gen_exprval($3);
            varargs_cnt++;
            free_expr($3);
         }
      ; 

selection_stmt  :  if_expr  then_stmt     %prec LOWER_THAN_ELSE
        { code[$1].y = $2; }
      |  if_expr  then_stmt  the_else  stmt
          { code[$1].y = $3; code[$2].y = lc; }
      |  switch_hdr  '{'  case_groups  opt_default  '}'
         { int i;
               /* switch_level is incremented in switch_hdr rule        */
               /* grammar also forces at least one case in every switch */
            i = case_loc[curr_case];
            code[i].y = $4;      /* JZER of the case */
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
      ;

if_expr  :  IF  '('  expr  ')'
         { gen_exprval($3); $$ = lc; emit(JZER); free_expr($3); }
      ;

then_stmt  :  stmt
         { $$ = lc; }
      ;

the_else  :  ELSE
         { emit(JUMP); $$ = lc; }
      ;

switch_hdr  :  SWITCH  '('  expr  ')'
         { 
            enter_break_block(&break_level,switch_blk);
            if (++curr_case == MAXCASELOC)
               cfatal(
       "Can't more than %d 'case' statments in a nest of 'switch' stmts",
                  MAXCASELOC);
            gen_exprval($3);
            switch_level++;
            switch_block[switch_level].first_case = curr_case;
            switch_block[switch_level].expr_index = $3;
         }
      ;

case_groups  :  case_group
      |  case_groups  case_group
      ;

case_group  :  case_hdr_list  patch_case_hdrs  stmt_list
      ;

case_hdr_list  :  case_hdr
         { first_case_hdr = curr_case; case_hdr_cnt = 1; }
      |  case_hdr_list  case_hdr
         {  case_hdr_cnt++; }
      ;
      
case_hdr  :  CASE  begin_case  expr  ':'
      { int i;
         i = switch_block[switch_level].expr_index;
         if ((expr[i].typ != expr[$3].typ)&&(expr[$3].typ != notyp)) 
            yyerror
               ("Case expression type differs from type of switch expression");
         else {
            gen_exprval($3);
            free_expr($3);
            emit(TEST_EQ_KEEP);
            if (++curr_case == MAXCASELOC)
               cfatal(
      "No more than %d 'case' statments in a nest of switch statements",
      MAXCASELOC);
            case_loc[curr_case] = lc;
            emit(JZER);
         }
      }
      ;

begin_case  :  /* nothing */
      {  int j;
         j = switch_block[switch_level].first_case;
         if (case_hdr_cnt > 0) emit(JUMP); /* for multiple case labels */
         if (curr_case > j)
            code[case_loc[curr_case]].y = lc;  /* prev JZER target */
      }
      ;

patch_case_hdrs  :  /* nothing */
      {  int i, j, k;
         for (i = 1, j = first_case_hdr; i < case_hdr_cnt; i++, j++ ) {
            k = case_loc[j]; 
            code[k+1].y = lc;    /* k+1 == address of JMP to stmt_list */
         }
         case_hdr_cnt = 0; /* reload for next time */
         first_case_hdr = -1;
      }
      ;

opt_default  :  /* empty */
         { $$ = lc; }
      |  the_default  stmt_list
      ;

the_default  :  DEFAULT ':'
         { $$ = lc; }
      ;
      
cobegin_stmt  :  cbegin  '{'  stmt_list  '}'
         { if (level == 1) emit(COEND); in_cobegin = 0; }
      ;

iteration_stmt  :  the_while  while_expr  stmt
         {
            $$ = $1;             /* continue target */
            emit1(JUMP,$1); 
            code[$2].y = lc; 
         }
      |  the_do  do_stmt_loc  stmt  the_do_while do_while_expr  ';'
         {  
            $$ = $4;             /* continue target */
            code[$5].y = $2;     /* point JUMP to top of DO */
         }
      |  for_init  opt_continue_expr  for_semic2  opt_iter_exprs  ')'  stmt
         { int i; 
            if ($4) {            /* iteration code was generated */
               $$ = for_info[curr_for].iter_loc;  /* continue target */
               /* jump back to iteration code after stmt */
               emit1(JUMP,for_info[curr_for].iter_loc); 
               if ($2) {         /* if there was continuation code .. */
                  i = for_info[curr_for].stmt_loc - 1;/* JUMP from iter code */
                  code[i].y = for_info[curr_for].cont_loc;  /* to cont code */
               }
               /* will fall through to stmt from iter code if no cond code */
            }
            else {             /* no iteration code was generated */
               $$ = for_info[curr_for].stmt_loc;    /* continue target */ 
               if ($2)        /* if there is continuation code ... */
                  emit1(JUMP,for_info[curr_for].cont_loc);  /* jmp to it */
               else           /* else jmp back to stmt code */
                  emit1(JUMP,for_info[curr_for].stmt_loc);  /* jmp to it */
            }
            if ($2)           /* if there was continuation code... */
               code[for_info[curr_for].cont_jzer].y = lc;   /* jmp out */  
            code[for_info[curr_for].iter_loc-1].y = 
                                             for_info[curr_for].stmt_loc;
            curr_for--;
         }
      ;

the_while  :  WHILE
         { 
            $$ = lc;   /* continue target */ 
            enter_break_block(&break_level,loop_blk);
         }
      ;

while_expr  :  '('  expr  ')'
         { 
            gen_exprval($2); 
            free_expr($2); 
            $$ = lc; 
            emit(JZER);
         }
      ;

the_do  :  DO
         { 
            enter_break_block(&break_level,loop_blk);
         }
      ;

do_stmt_loc  :  /* empty */
         { $$ = lc; /* location of stmt code */ }
      ;

the_do_while :  WHILE
         { $$ = lc;  /* continue target */ }
      ;

do_while_expr  :  '('  expr  ')'
         { 
            gen_exprval($2); 
            free_expr($2); 
            emit1(JZER,lc+2);
            $$ = lc;    /* pass loc of JUMP up parse tree */
            emit(JUMP);
         }
      ;

for_init  :  the_for  opt_for_expr_list  ';'
         { /* init stuff done, continuation testing code is next */
            for_info[curr_for].cont_loc  = lc;  /* continuation test code */
         }
      ;

the_for  :  FOR  '('
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
      ;

opt_continue_expr  :  /* empty */
         { $$ = 0;   /* no continuation code */ }
      |  expr
         { 
            $$ = 1;  /* continuation code */
            gen_exprval($1);  /* expr might not have been generated yet */
            free_expr($1);
            for_info[curr_for].cont_jzer = lc;    /* mark loc of JZER */
            emit(JZER);
         }
      ;

for_semic2  :  ';'
         {
            emit(JUMP);       /* to stmt code, backpatched later */
            for_info[curr_for].iter_loc = lc; /* loc of iteration code */ 
         }
      ;

opt_iter_exprs  :  /* empty */
         { 
            $$ = 0;   /* no iteration code */
            for_info[curr_for].stmt_loc = lc;   /* loc of stmt code */
         }
      |  for_expr_list
         {
            $$ = 1;   /* iteration code */
            if (for_info[curr_for].cont_jzer >= 0) /* there was cont code */
               emit(JUMP);       /* to continuation code, backpatched */
            for_info[curr_for].stmt_loc = lc;   /* loc of stmt code */
         }
      ;

cbegin  :  CBEGIN
         {
            if (level != 1)
               yyerror("'cobegin' block allowed only in 'main'");
            else if (in_cobegin)
               yyerror("'cobegin' blocks cannot be nested");
            else
               emit(COBEGIN);
            in_cobegin = 1;
         }
      ;

continue_stmt  :  CONTINUE  ';'
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
      ;

break_stmt  :   BREAK  ';'
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
      ;

return_stmt  :  the_return  opt_expr  ';'
         {  
            if (void_function) {
               if ($2 < 0) {
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
               if ($2 >= 0) 
                  gen_exprval($2); 
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
         ; 
         
the_return  :  RETURN 
         { 
            if (!in_func_decl)
               yyerror("'return' is inappropriate outside a function");
            else if (!void_function)
                  emit2(LOAD_ADDR,tab[prt].lev+1,0);
         }
      ;

output_stmt  :  COUT  output_list  ';'
      ;

output_list  :  LTLT  output_item
      |  output_list  LTLT  output_item
      ;

output_item  :  expr
         {
            if (expr[$1].typ == arrays) /* presently our only struct type */
               yyerror("Cannot output an array");
            else if(expr[$1].typ == strings) {
               /* an array ref ==> addr is _aldready_ on stack */
               if (!expr[$1].arelt)
                  emit_push_addr(expr[$1]);
               emit(WRITE_STRING);
            }
            else{
               gen_exprval($1);
               emit1(WRITE,expr[$1].typ);
            }
            free_expr($1);
         }
      |  ENDL
         { emit(WRITELN); }
      |  RAWSTRING
         {  
            int tmp = store_string(laststring,&stab_size);
            emit1(WRITE_RAWSTRING,tmp);
         }
      ;

input_stmt  :  CIN  GTGT  expr  ';'
        {
            if ((expr[$3].obj != variable)&& (expr[$3].obj != ext_variable))
               yyerror("input item must be a 'variable'");
            else if ((expr[$3].typ != ints)&&(expr[$3].typ != chars)&&
                     (expr[$3].typ != notyp))
               yyerror("input item must be of type 'int' or 'char'");
            else if (!expr[$3].arelt){ 
               emit_push_addr(expr[$3]);
               emit1(READ,expr[$3].typ);
            } else   /* array element */
               emit1(READ,expr[$3].typ);
            free_expr($3);
         }
      ;

null_stmt   :  ';'
      ;
opt_expr  :  /* empty */
         { $$ = -1; }
      |  expr
         { free_expr($1); }
      ;

expr_stmt  :  expr  ';'
         {  int i = expr[$1].tix;
            if ((tab[i].obj == function) && (!expr[$1].result_used))
               yyerror("A non-void function cannot be called as a void function");
            free_expr($1); 
         }
      ;

expr  :  logical_or_expr
      |  var_assign
      ;

var_assign  :  id_becomes expr
         {
            expr[$2].result_used = 1;
            gen_exprval($2);
            if (tab[$1].typ == expr[$2].typ) {
               if ((tab[$1].typ == sems)||(tab[$1].typ == bsems)|| 
                   (tab[$1].typ == conds))
                  yyerror("Cannot assign to 'semaphore' or 'condition'");
               else if (tab[$1].typ == arrays)
                  emit1(COPY_BLOCK,atab[tab[$1].ref].size);
               else  /* standard types */
                  if (assign_cnt > 1)  /* multiple assignments */
                     emit(STORE_KEEP);
                  else                 /* last assignment */
                     emit(STORE);
            }
            else
               yyerror("Type mismatch in assignment");
            $$ = $2;
            expr[$$].tix = $1;
            assign_cnt--;
         }
      |  array_var_becomes  expr
         {
            gen_exprval($2);
            if (expr[$1].typ == expr[$2].typ) {
               if ((expr[$1].typ == sems)||(expr[$1].typ == bsems)|| 
                   (expr[$1].typ == conds))
                  yyerror("Cannot assign to 'semaphore' or 'condition'");
               else if (expr[$1].typ == arrays)
                  emit1(COPY_BLOCK,atab[expr[$1].ref].size);
               else  /* standard types */
                  if (assign_cnt > 1)  /* multiple assignments */
                     emit(STORE_KEEP);
                  else                 /* last assignment */
                     emit(STORE);
            }
            else
               yyerror("Type mismatch in assignment");
            free_expr($2);
            expr[$1].isval = 1;
            $$ = $1;
            assign_cnt--;
         }
      ;

id_becomes  :  ident  '='
         {
            assign_cnt++;
            if ((tab[$1].obj == variable)||(tab[$1].obj == ext_variable)) {
                  emit_push_addr(tab[$1]);
            }
            else
               yyerror("'variable' identifier expected");
         }
      ;

array_var_becomes  :  array_var  '='
         { assign_cnt++; }
      ;

logical_or_expr  :  logical_and_expr
      |  left_logical_or_expr  OR  logical_and_expr
         {
            if ((expr[$1].typ == ints)&&(expr[$3].typ == ints)) {
               gen_exprval($3);
               emit(DO_OR);
            }
            else if ((expr[$1].typ != notyp)&&(expr[$3].typ != notyp)){
               yyerror("type 'int' expected");
               expr[$1].typ = notyp;
            }
            free_expr($3);
         }
      ;

left_logical_or_expr  :  logical_or_expr
         { gen_exprval($1); }
      ;

logical_and_expr  :  relational_expr
      |  left_logical_and_expr  AND  relational_expr
         {
            if ((expr[$1].typ == ints)&&(expr[$3].typ == ints)) {
               gen_exprval($3);
               emit(DO_AND);
            }
            else if ((expr[$1].typ != notyp)&&(expr[$3].typ != notyp)){
               yyerror("'int' type expected");
               expr[$1].typ = notyp;
            }
            free_expr($3);
         }
      ;

left_logical_and_expr  :  logical_and_expr
         { gen_exprval($1); }
      ;

relational_expr  :  simple_expr
      |  left_relational_expr  relational_op  simple_expr
         {
            gen_exprval($3);
            if (expr[$1].typ == expr[$3].typ){
               if ((expr[$1].typ == ints)||(expr[$1].typ == chars))
                  emit($2);
               else
                  yyerror("Only 'int' and 'char' types can be compared");
            }
            else
               yyerror("Types in comparison are unequal");
            expr[$1].typ = ints;
            expr[$1].isval = 1;
            free_expr($3);
         }
      ;

left_relational_expr  :  simple_expr
         { gen_exprval($1); }
      ;

relational_op  :  EQ   { $$ = TEST_EQ; }
      |  '<'           { $$ = TEST_LT; }
      |  '>'           { $$ = TEST_GT; }
      |  LE            { $$ = TEST_LE; }
      |  GE            { $$ = TEST_GE; }
      |  NE            { $$ = TEST_NE; }
      ;

simple_expr  :  term
      |  '+'  term
         {
            $$ = $2;
            gen_exprval($$);
            if ((expr[$$].typ != ints)&&(expr[$$].typ != notyp)){
               yyerror("'int' type expected");
               expr[$$].typ = notyp;
            }
         }
      |  '-'  term
         {
            $$ = $2;
            gen_exprval($$);
            if ((expr[$$].typ != ints)&&(expr[$$].typ != notyp)){
               yyerror("type 'int' expected");
               expr[$$].typ = notyp;
            }
            else
               emit(NEGATE);
         }
      |  left_simple_expr  add_op  term
         {
            gen_exprval($3);
            switch ($2) {
            case DO_ADD:
            case DO_SUB:
               expr[$1].typ = resulttype(expr[$1].typ,expr[$3].typ);
               if (expr[$1].typ == ints)
                  emit($2);
               break;
            default:
               fatal("grammar error:  bad add_op %d in simp_expr",$2);
            } /* switch */
            free_expr($3);
         }
      ;

left_simple_expr  :  simple_expr
         { gen_exprval($1); }
      ;

add_op  :  '+'  { $$ = DO_ADD; }
      |  '-'    { $$ = DO_SUB; }
      ;

term  :  factor 
      |  left_term  mult_op  factor
         {
            gen_exprval($3);
            switch($2) {
            case DO_MUL:
            case DO_DIV:
            case DO_MOD:
               if ((expr[$1].typ == ints)&&(expr[$3].typ == ints))
                  emit($2);
               else if ((expr[$1].typ != notyp)&&(expr[$3].typ != notyp)){
                  yyerror("type 'int' expected");
                  expr[$1].typ = notyp;
               }
               break;
            default:
               fatal("grammar error:  bad mult_op %d in term",$2);
            } /* switch */
            free_expr($3);
         }
      ;

left_term  :  term
         { gen_exprval($1); }
      ;

mult_op  :  '*'  { $$ = DO_MUL; }
      | '/'      { $$ = DO_DIV; }
      | '%'      { $$ = DO_MOD; }
      ;

factor  :  variable     
      |  unsigned_num
         {
            $$ = new_expr();
            expr[$$].obj = constant;
            expr[$$].adr = $1;
            expr[$$].typ = ints;
            expr[$$].lev = level;
         }
      |  CHARCON
         {
            $$ = new_expr();
            expr[$$].obj = constant;
            expr[$$].adr = laststring[0];
            expr[$$].typ = chars;
            expr[$$].lev = level;
         }
      |  '('  expr  ')'
         { $$ = $2;}
      |  '!'  factor
         {
            $$ = $2;
            gen_exprval($$);
            if (expr[$$].typ == ints)
               emit(COMPLEMENT);
            else if (expr[$$].typ != notyp){
               yyerror("type 'int' expected");
               expr[$$].typ = notyp;
            }
         }
      ;

variable  :  var_ident
      |  var_ident  PLPL
         {
            if ((expr[$1].typ != ints)&&(expr[$1].typ != chars))
               yyerror("'int' or 'char' expected");
            else {
               emit_push_addr(expr[$1]);
               gen_loadvar($1);
               emit1(PUSH_LIT,1);
               emit(DO_ADD);
               emit(STORE);
            }
         }
      |  PLPL  var_ident
         {
            if ((expr[$2].typ != ints)&&(expr[$2].typ != chars))
               yyerror("'int' or 'char' expected");
            else {
               emit_push_addr(expr[$2]);
               gen_loadvar($2);
               emit1(PUSH_LIT,1);
               emit(DO_ADD);
               emit(STORE);
            }
         }
      |  var_ident  MIMI
         {
            if ((expr[$1].typ != ints)&&(expr[$1].typ != chars))
               yyerror("'int' or 'char' expected");
            else {
               emit_push_addr(expr[$1]);
               gen_loadvar($1);
               emit1(PUSH_LIT,1);
               emit(DO_SUB);
               emit(STORE);
            }
         }
      |  MIMI  var_ident
         {
            if ((expr[$2].typ != ints)&&(expr[$2].typ != chars))
               yyerror("'int' or 'char' expected");
            else {
               emit_push_addr(expr[$2]);
               gen_loadvar($2);
               emit1(PUSH_LIT,1);
               emit(DO_SUB);
               emit(STORE);
            }
         }
      |  func_call
      |  array_var
      |  typecast
      ;
   
var_ident  :  ident
         {
            $$ = new_expr();
            expr[$$].obj = tab[$1].obj;
            expr[$$].typ = tab[$1].typ;
            expr[$$].adr = tab[$1].adr;
            expr[$$].ref = tab[$1].ref;
            expr[$$].lev = tab[$1].lev;
            expr[$$].normal = tab[$1].normal;
            expr[$$].tix = $1;
         }
      ;

func_call  :  func_id  actual_params
         { int k; 
            $$ = new_expr();
            expr[$$].obj = tab[$1].obj;
            expr[$$].typ = tab[$1].typ;
            expr[$$].adr = tab[$1].adr;
            expr[$$].ref = tab[$1].ref;
            expr[$$].lev = tab[$1].lev;
            expr[$$].normal = tab[$1].normal;
            expr[$$].tix = $1;
            if (tab[$1].lev == -1)  /* std function unfinished business */
               stdproc_pcode($1,parmct);
            else if ((tab[$1].obj == function)||(tab[$1].obj == procedure)||
               (tab[$1].obj == ext_function)||(tab[$1].obj == ext_procedure)) {
               expr[$$].ref = 0;   /* in a func call ref field of func == 0 */
               /* first check on too few parms */
               k = btab[tab[$1].ref].lastpar - $1;
               if ((k > parmct)&&(first_parmcterr)) {
                  sprintf(pbuf,"Too few actual parameters in the %s call",
                     tab[$1].name);
                  yyerror(pbuf);
                  first_parmcterr = 0;
               }
               if ((in_cobegin)&&(tab[$1].typ != notyp)) 
                  yyerror("Only VOID function calls are allowed in a \
cobegin block");
               k = btab[tab[$1].ref].psize - 1;
               if (tab[$1].mon == 0)
                  emit2(CALL,0,k);
               else
                  emit2(CALL,-1,k); /* CALL pcode: x == -1 => mon call */
               if (tab[$1].lev < level) /* update display, ord nesting*/
                  emit2(UPDATE_DISPLAY,tab[$1].lev,level);
               else if ((tab[$1].mon)&&(level == tab[$1].lev))
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
      |  stringfn_call
      |  receive_call
      ;

func_id  :  ident
         {
            last_pf = $1;
            if ((tab[$1].obj != function)&&(tab[$1].obj != procedure)&&
                (tab[$1].obj != ext_function)&&(tab[$1].obj != ext_procedure))
               yyerror("'function call' expected");
            else { /* tab[$1].obj == procedure or function or stdproc*/
               if ((++toppfs) == MAXPFNEST)
                  yyerror("Maximum function call nesting exceeded");
               else{  /* legal func call */
                  pfstack[toppfs].tix = last_pf; 
                  pfstack[toppfs].pct = parmct = 0;
                  first_parmcterr = 1;
                  if (tab[$1].lev != -1){ /* regular proc call */
                     if ((tab[$1].mon)&&(tab[prt].mon)&&
                         (tab[$1].mon != tab[prt].mon))
                        yyerror("Cross monitor calls not allowed");
                     emit1(MARKSTACK,last_pf-last_predeclared);
                  } /* if regular proc */
               } /* legal func call */ 
            } /* if func */
         }
      ;

actual_params  :  '('  ')'
      |  '(' actuals_list ')'
      ;

actuals_list   : actuals_list ',' actual_param
      | actual_param
      ; 

actual_param   : expr    /* which could be a variable */
         {
            parmct = ++pfstack[toppfs].pct;
            if (tab[last_pf].lev == -1)
               stdproc_parm(last_pf,$1,parmct);
            else
               proc_parm(last_pf,$1,parmct);
            free_expr($1);
         }
      ;

stringfn_call :  stringconcat_call
      |  stringcopy_call 
      |  stringcompare_call
      |  stringlength_call
      |  sscanf_call
      ;

stringconcat_call  :  STRINGCONCAT  left_exprparm  right_stringparm
         {
            if (expr[$2].typ != strings)
               yyerror("left parameter is not of type 'string'");
            $$ = NO_EXPR;     /* for free_expr() call up the parse tree */
            if (expr[$3].typ == rawstrings) 
               emit1(CONCAT_RAWSTRING,expr[$3].adr);
            else 
               emit(CONCAT_STRING);
            free_expr($2); free_expr($3);
         }
      ;

right_stringparm  :  ','  expr  ')'
         {
            $$ = $2;
            if (expr[$2].typ != strings) {
               yyerror("right parameter is not of type 'string'");
               last_eltyp = notyp;
            }
            else {
               if (!expr[$2].arelt)
                  /* push addr of source string */
                  /* addr of array ref is already on the stack */
                  emit_push_addr(expr[$2]);
            }
         }
         |  ','  rawstring_parm  ')'
            { $$ = $2; }
      ;

rawstring_parm  :  RAWSTRING
         {
            $$ = new_expr();
            expr[$$].obj = variable;
            expr[$$].typ = rawstrings;
            expr[$$].adr = store_string(laststring,&stab_size);
            expr[$$].ref = 1 + strlen(laststring); /* includes null char */
            expr[$$].normal = 1;
            expr[$$].tix = -1;
         }
      ;

stringcopy_call  :  STRINGCOPY  left_exprparm  right_stringparm
         {
            if (expr[$2].typ != strings)
               yyerror("left parameter is not of type 'string'");
            $$ = NO_EXPR;     /* for free_expr() call up the parse tree */
            if (expr[$3].typ == rawstrings) 
               emit1(COPY_RAWSTRING,expr[$3].adr);
            else 
               emit(COPY_STRING);
            free_expr($2); free_expr($3);
         }
      ;

stringcompare_call  :  STRINGCOMPARE  left_exprparm  right_stringparm
         {
            if (expr[$2].typ != strings)
               yyerror("left parameter is not of type 'string'");
            $$ = new_expr();
            expr[$$].obj = function;
            expr[$$].typ = ints;
            expr[$$].normal = 1; /* the value will be left on the stack */
            expr[$$].ref = 0;
               /* make sure no one uses these */
            expr[$$].adr = expr[$$].lev = -1;
            if (expr[$3].typ == rawstrings) 
               emit1(COMPARE_RAWSTRING,expr[$3].adr);
            else 
               emit(COMPARE_STRING);
            free_expr($3);
         }
      ;

stringlength_call  :  STRINGLENGTH  single_exprparm
         {
            $$ = new_expr();
            expr[$$].obj = function;
            expr[$$].typ = ints;
            expr[$$].normal = 1; /* the value will be left on the stack */
            expr[$$].ref = 0;
               /* make sure no one uses these */
            expr[$$].adr = expr[$$].lev = -1;
            if (expr[$2].typ == rawstrings)
                  /* user wants length of a rawtring !???!!! */
                  /* let 'em have it */
               emit1(PUSH_LIT,expr[$2].ref - 1);
            else  /* was a string variable */
               emit(LENGTH_STRING);
            free_expr($2);
         }
      ;

single_exprparm  :  '('  expr  ')'
         {
            if (expr[$2].typ == strings) {
                 if (!expr[$2].arelt)
                  /* addr of array ref is already on the stack */
                  emit_push_addr(expr[$2]);
            } else if (expr[$2].typ == ints)
               gen_exprval($2);
            $$ = $2;
            /* parent rule will free_expr($2) */
         }
      |  '('  RAWSTRING  ')'
         {
            $$ = new_expr();
            expr[$$].obj = variable;
            expr[$$].typ = rawstrings;
            expr[$$].adr = store_string(laststring,&stab_size);
            expr[$$].ref = 1 + strlen(laststring); /* includes null char */
            expr[$$].normal = 1;
            expr[$$].tix = -1;
         }
      ;

sscanf_call  :  sscanf_buf_fmt  ','  sscanf_parmlist  ')'
         {
            $$ = new_expr();
            expr[$$].obj = function;
            expr[$$].typ = ints;
            expr[$$].normal = 1; /* the value will be left on the stack */
            expr[$$].ref = 0;
               /* make sure no one uses these */
            expr[$$].adr = expr[$$].lev = -1;
            /* push the parm count for interp's SSCANF_OP to use */
            emit1(PUSH_LIT,varargs_cnt); 
            emit1(SSCANF_OP,$1);
         }
      ;

sscanf_buf_fmt  :  SSCANF  '('  expr  ','  rawstring_parm
         { 
            if (expr[$3].typ != strings)
               yyerror("Leftmost sscanf parameter must be of type 'string'");
            $$ = expr[$5].adr;  /* pass rawstring index up the tree */
            emit_push_addr(expr[$3]);
            free_expr($3); free_expr($5);
            varargs_cnt = 0;
         }
      ;

sscanf_parmlist  :  expr
         {  
            if ((expr[$1].typ != ints)&&(expr[$1].typ != strings))
               yyerror(
            "sscanf parameter must be either of type 'int' or type 'string'");
            emit_push_addr(expr[$1]);
            varargs_cnt++;
            free_expr($1);
         }
      |  sscanf_parmlist  ','  expr
         {  
            if ((expr[$3].typ != ints)&&(expr[$3].typ != strings))
               yyerror(
            "sscanf parameter must be either of type 'int' or type 'string'");
            emit_push_addr(expr[$3]);
            varargs_cnt++;
            free_expr($3);
         }
      ; 

receive_call :  RECEIVE  left_exprparm  ')'
         {
            if (expr[$2].typ != strings)
               yyerror("Left parameter must be of type 'string'");
            emit(RECEIVE_OP);
            free_expr($2);   
            $$ = new_expr();
            expr[$$].obj = function;
            expr[$$].typ = ints;
            expr[$$].normal = 1; /* the value will be left on the stack */
            expr[$$].ref = 0;
               /* make sure no one uses these */
            expr[$$].adr = expr[$$].lev = -1;
         }
      |  RECEIVE  left_exprparm  ','  expr  ')'
         {
            if (expr[$2].typ != strings)
               yyerror("Left parameter must be of type 'string'");
            /* implements receive(string,int&) call */
            if ((expr[$4].obj != variable)&& (expr[$4].obj != ext_variable))
               yyerror("the right 'receive' parameter must be a variable");
            else if (expr[$4].typ != ints)
               yyerror("the right 'receive' parameter must be of type 'int'");
            else {
               if (!expr[$4].arelt)
                  emit_push_addr(expr[$4]);
               emit(RECEIVE_ID);
            }
            free_expr($2); free_expr($4);
            $$ = new_expr();
            expr[$$].obj = function;
            expr[$$].typ = ints;
            expr[$$].normal = 1; /* the value will be left on the stack */
            expr[$$].ref = 0;
               /* make sure no one uses these */
            expr[$$].adr = expr[$$].lev = -1;
         }
      ;   

array_var  :  array_id  index_expr
         {
            expr[$1].ref = last_aref;
            expr[$1].arelt = 1;
            expr[$1].typ = last_eltyp;
            topars--;
            if (topars >= 0) {
               last_aref = arstack[topars].aref;
               last_eltyp = arstack[topars].eltyp;
            }
         }
      ;

array_id  :  ident
         {
            if (tab[$1].typ != arrays) {
               sprintf(pbuf,"'%s' is not of type 'array'",tab[$1].name);
               yyerror(pbuf);
               last_aref = 0;
               last_eltyp = notyp;
            }
            else {
               last_aref = tab[$1].ref;
               last_eltyp = atab[last_aref].eltyp;
               /* put addr of array base on the stack */
               emit_push_addr(tab[$1]);
            }
            if (++topars > MAXARNEST) 
               yyerror("Maximum array nesting exceeded");
            else {
               arstack[topars].aref = last_aref;
               arstack[topars].eltyp = last_eltyp;
            }
            $$ = new_expr();
            expr[$$].obj = tab[$1].obj;
            expr[$$].typ = tab[$1].typ;
            expr[$$].adr = tab[$1].adr;
            expr[$$].ref = tab[$1].ref;
            expr[$$].lev = tab[$1].lev;
            expr[$$].normal = tab[$1].normal;
         }
      ;

index_expr  :  index_expr  '['  expr  ']'
         { 
            gen_exprval($3);
            last_aref = arstack[topars].aref;
            last_eltyp = arstack[topars].eltyp;
            index_expr($3,&last_aref,&last_eltyp); 
            arstack[topars].aref = last_aref;
            arstack[topars].eltyp = last_eltyp;
            free_expr($3); 
         }
      |  '['  expr  ']'
         { 
            gen_exprval($2);
            last_aref = arstack[topars].aref;
            last_eltyp = arstack[topars].eltyp;
            index_expr($2,&last_aref,&last_eltyp); 
            arstack[topars].aref = last_aref;
            arstack[topars].eltyp = last_eltyp;
            free_expr($2); 
         } 
      ;

typecast  :  INT  '('  expr  ')'
         {
            expr[$3].typ = ints;
            $$ = $3;
         }
      ;
             
ident  :  IDENTIFIER
         { $$ = loc(lastident(),level); }
      ;

newident  :  IDENTIFIER
         { $$ = enter(lastident(),constant,level,level); } 
         /* obj field of const will probably be changed */
      ;

%%   /*start of routines*/

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
