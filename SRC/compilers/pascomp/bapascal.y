%{
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

%}

%start program

%token UNSIGNED_INT 
%token STRING RAWSTRING STRINGCONCAT STRINGCOMPARE STRINGCOPY STRINGLENGTH
%token SSCANF SPRINTF
%token IDENTIFIER CHAR
%token INT
%token NE LE GE BECOMES DIV MOD OR AND NOT DOTDOT
%token IF THEN ELSE CASE OF REPEAT UNTIL WHILE DO FOR TO 
%token CBEGIN SBEGIN END CEND CONST VAR TYPE ARRAY FUNCTION
%token MONITOR PROCEDURE PROGRAM INCLUDE ATOMIC FNSTRING
%token SEND RECEIVE BROADCAST
%token EXTERNAL

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%   /*start of rules*/
program     :  init_outerscope  outer_decls
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
      |  init_outerscope  outer_decls  pgm_decl
      { 
         if (!making_objfile&&!main_declared) 
            yyerror("No 'main' function declared");
         if (main_declared)
            emit(SHORTRET);
      }
      |  init_outerscope  pgm_decl
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
         enter(outerblock_name,outerblock,level,level);
         ob_tix = last_tab;
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
               
outer_decl  :  include_dcl_part
      |  proc_dcl_part
      |  mon_dcl_part
      |  type_dcl_part
      |  const_dcl_part
      |  var_dcl_part
      |  extern_decl
      ;

extern_decl  :  extern_var_decl
      |  extern_proc_decl
      |  extern_mon_decl
      ;

extern_var_decl  :  the_external  VAR  variable_dcls  ';'
      { external = 0; } 
      ;

extern_proc_decl  :  the_external  proc_or_func_proto  
      { 
         external = 0; 
      }
      ;

the_external  :  EXTERNAL
      {  external = 1; }
      ;

proc_or_func_proto  :  proc_proto
         { leave_block(&level); }
      |  func_proto
         { leave_block(&level); }
      ;

extern_mon_decl  :  the_external  mon_heading  proto_list  END  ';'
      {  
         tab[$2].adr = -1;
         in_mon_decl = 0; 
         external = 0;
         leave_block(&level);
      }
      ;

proto_list  :  proc_or_func_proto
      |  proto_list  proc_or_func_proto
      ;

pgm_decl  :  pgm_hdg_decls statement_part '.'
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

      ;

pgm_hdg_decls : pgm_heading  ';' opt_declarations
      { 
         tab[$1].adr = lc;          /* main pgm entry point */ 
         process_var_inits(level);  /* process any var inits */
         outer_call_lc = lc;        /* loc of SHORTCALL instr */
         emit(SHORTCALL);           /* target addr is backpatched later */
      }
      ;

pgm_heading :  PROGRAM newident /* no files */
      { /* pgm name is last symbol in outer scope */
         $$ = $2;
         main_declared = 1;
         external = 0;
         tab[last_tab].obj = mainproc;
         prt = last_tab;
         btab[0].lastpar = prt;  /* save main's tabix for the interp */
         enter_block(&last_btab,&level,last_tab);
         btab[last_btab].lastpar = last_tab; /* no parms */
      }
      ;
         
opt_declarations: /*empty*/
      | declarations
      ;

declarations   : declarations declaration 
      | declaration
      ;

declaration : include_dcl_part   /* no labels */
      | const_dcl_part   /* no labels */
      | type_dcl_part
      | var_dcl_part
      | proc_dcl_part
      | mon_dcl_part            /* monitors  */
      ;

include_dcl_part : start_include RAWSTRING
      {
         if (strcmp(laststring,infile[curr_infile].fname) == 0) 
            yyerror("Recursive file inclusion");
         else {
            curr_infile = open_infile(laststring,curr_infile);
            yyin = infile[curr_infile].f;
         }
         in_include = 0;
      }
      ;

start_include : '#' INCLUDE
      { 
         in_include = 1;
      }
      ;

include_dcl_part : start_include FNSTRING
      {
         if (strcmp(laststring,infile[curr_infile].fname) == 0) 
            yyerror("Recursive file inclusion");
         else {
            curr_infile = open_infile(laststring,curr_infile);
            yyin = infile[curr_infile].f;
         }
         in_include = 0;
      }
      ;

const_dcl_part : CONST const_defs ';'
      ;

const_defs  : const_defs ';' const_def
      | const_def
      ;

const_def   : newident '=' constant
      {
         tab[last_tab].typ = consttype;
         tab[last_tab].adr = $3; 
         tab[last_tab].ref = 0;
      } 
      ;

constant : unsigned_num
         { consttype = ints; }
      | '+' unsigned_num
         { $$ = $2; consttype = ints; }
      | '-' unsigned_num
         { $$ = - $2; consttype = ints; }
      | ident           /*check it is constant*/
         {  $$ = get_constant(lastident,level,&consttype); }
      | '+' ident
         {  $$ = get_constant(lastident,level,&consttype);
            if (consttype != ints) {
               sprintf(pbuf,"Identifier '%s' not INTEGER type",lastident);
               yyerror(pbuf);
               consttype = notyp;
            }
         }
      | '-' ident
         {  $$ = - get_constant(lastident,level,&consttype);
            if (consttype != ints) {
               sprintf(pbuf,"Identifier '%s' not INTEGER type",lastident);
               yyerror(pbuf);
               consttype = notyp;
            }
         }
      | CHAR 
         { consttype = chars; $$ = laststring[0]; }
      ;

unsigned_num   : UNSIGNED_INT        /* no reals */
      {$$ = strtoi(numbertext,numberbase);}
      ;

type_dcl_part  : TYPE type_defs ';'
      ;

type_defs   : type_defs ';' type_def
      | type_def
      ;

type_def :  newident '=' type
      {
         tab[$1].obj = type;
         if (expr[$3].typ == arrays) {
            tab[$1].typ = arrays;
            tab[$1].ref = expr[$3].ref;  /* its atab index */
            /* atab sizes are in stack words */
            /* adr field is in bytes */
            tab[$1].adr = sizeof(int)*atab[expr[$3].ref].size;
         }
         else { /* for non-arrays, the expr info is handled identically */
            tab[$1].typ = expr[$3].typ;
            tab[$1].ref = expr[$3].ref; 
            tab[$1].adr = expr[$3].adr;
         }
         free_expr($3);
      }
      ;

type  : simple_type
      | struct_type
      | string_type
      ;

simple_type :  ident
      {  
         if ($1 != 0) {
            if ((tab[$1].obj != type)||(tab[$1].typ == notyp)){
               sprintf(pbuf,"Identifier '%s' is not a valid type",lastident);
               yyerror(pbuf);
               $$ = NO_EXPR;
            }
            else  {
               $$ = new_expr();
               expr[$$].tix = $1;
               expr[$$].adr = tab[$1].adr;
               expr[$$].typ = tab[$1].typ;
               expr[$$].ref = tab[$1].ref;
               expr[$$].lev = tab[$1].lev;
               expr[$$].normal = tab[$1].normal;
               expr[$$].arelt = 0;
            }
         }            
         else /* wasn't there, loc has already complained */
            $$ = NO_EXPR;
      }
      ;

      /* no RECORDs, SETs, or FILEs */
struct_type  :  ARRAY  '['  index_type  ']'  last_atab  OF  type
      {  
      int i;
      TYPES eltyp;
      int   elsize,elref,last;
         $$ = new_expr();
         expr[$$].obj = type;
         expr[$$].typ = arrays;
         expr[$$].ref = $3; /* atab index is passed up through index_type */
         if (expr[$7].typ == arrays) { /* type production has atab index */
            i = expr[$7].ref; /* get the atab index */
            elsize = atab[i].size;  /* atab has size in stack words */
            eltyp = arrays;
            last = $5;     /* last dimension atab */
            elref = i;
         }
         else if (expr[$7].typ == strings) {
               /* ref field of string is size in bytes */
            elsize = stacksize(expr[$7].ref);
            eltyp = strings;
            elref = 0;  /* last non-array elref in atab must be 0 */
            last = last_atab;
         } 
         else { /* type production has tab index and isn't typ arrays */
            i = expr[$7].tix;    /* get tab index */
            elsize = stacksize(tab[i].adr);/* adr is byte size of type object */
            eltyp = tab[i].typ;
            elref = 0;  /* last non-array elref in atab must be 0 */
            last = last_atab;
         }
         for (i= last; i >= $3; i--){ /* work from 'back to front' */
            atab[i].elref = elref;
            atab[i].elsize = elsize;
            atab[i].size = (1 + atab[i].high - atab[i].low)*elsize;
            elsize = atab[i].size;
            atab[i].eltyp = eltyp;
            eltyp = arrays;
            elref = i;
         }
         free_expr($7);
      }
      ;

      /* no subrange or enumerated types for indices */
index_type  :  index_type  ','   dimension  
      /* don't pass along dimension $$; we want the first atab index  */
      |  dimension
      ;

dimension   :  low   constant
      {
         if (consttype != atab[last_atab].inxtyp)
            yyerror("Array index type mismatch");
         else {
            if ($2 < atab[last_atab].low){
               sprintf(pbuf,"Upper array index %d less than lower index %d",
                  $2,atab[last_atab].low);
               yyerror(pbuf);
            }
            else
               atab[last_atab].high = $2;
         }
         $$ = last_atab;
      }
      ;

low   :  constant DOTDOT
      {
         if (last_atab == AMAX) 
            cfatal("Array table full with %d entries",AMAX);
         atab[++last_atab].inxtyp = consttype;
         atab[last_atab].high = atab[last_atab].low = $1;
         $$ = last_atab;  /* pass the array start up the parse tree */
      }
      ;

last_atab:  /* no syntax -- we need to send up the last atab index */
      {  $$ = last_atab; }
      ;

string_type  :  STRING  '['  constant  ']'
      {
         $$ = new_expr();
         if (consttype != ints)
            yyerror("Size of 'string' type must be an integer");
         else if ($3 <= 0)
            yyerror("Size of 'string' type must be positive");
         else {
            expr[$$].ref = 1 + $3;  /* allow for null terminator */
            expr[$$].typ = strings;
            expr[$$].obj = type;
         }
      }
      ;

var_dcl_part   : VAR variable_dcls ';'
      ;

variable_dcls  : variable_dcls ';' variable_dcl
      | variable_dcl
      ;

variable_dcl  :  newident_list  ':'  type  opt_initializer
      {  
      int i,vartyp,varref,varsize,dx = btab[display[level]].vsize;
      int val;
         if ($4 != NO_EXPR) {
            val = expr[$4].adr;
            if (expr[$3].typ == conds)
               yyerror("Cannot assign to a 'condition'");
            else if (expr[$3].typ == bsems) {
               if ((expr[$4].typ != ints)||(val < 0)||(val > 1)) 
                  yyerror("A binary semaphore can only become 0 or 1");
            }
            else if (expr[$3].typ == sems) {
               if (expr[$4].typ != ints)
                  yyerror("integer initialization value expected");
               else if (val < 0)
                  yyerror("A semaphore cannot be negative");
            }
            else if ((expr[$3].typ != ints) && (expr[$3].typ != chars) &&
               (expr[$3].typ != bools))  
               yyerror("'INTEGER', 'CHAR', or 'BOOLEAN' type expected");
            else if (expr[$3].typ != expr[$4].typ)
               yyerror("Type of initializer does not match variable type");
            if (external)
               yyerror("External variables cannot be initialized");
         }
         else
            val = 0;
         if (expr[$3].typ == arrays) { /* type production has atab index */
            varsize = atab[expr[$3].ref].size;
            vartyp = arrays;
            varref = expr[$3].ref;
         }
         else if (expr[$3].typ == strings){
            varsize = stacksize(expr[$3].ref);
            vartyp = strings;
            varref = expr[$3].ref;
         }
         else {  /* type production has tab index */ 
            varsize = stacksize(expr[$3].adr);
            vartyp = expr[$3].typ;
            varref = expr[$3].ref;

         }
         for (i=$1; i<= last_tab; i++){
            tab[i].typ = vartyp;
            tab[i].ref = varref;
            tab[i].adr = dx;
            tab[i].obj = (external ? ext_variable : variable);
            if ($4 != NO_EXPR){
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
         free_expr($3);  free_expr($4);
      } 
      ;

newident_list  : new_id_list 
      ;

new_id_list : new_id_list ',' newident
      | newident { $$ = $1; }
      ;

opt_initializer  :  /* empty */
      { $$ = NO_EXPR; }
      |  BECOMES  constant
      {
         $$ = new_expr();
         expr[$$].typ = consttype;
         expr[$$].adr = $2;
      }
      ;

proc_dcl_part  : proc_or_func
      ;

proc_or_func   : proc_hdg_decl statement_part ';' 
      { 
         if (tab[$1].mon) emit(EXITMON);
         emit(EXIT_PROC);
         leave_block(&level);
         in_proc_decl = 0;
      }
      | func_hdg_decl statement_part ';'  
      { 
         if (tab[$1].mon) emit(EXITMON);
         emit(EXIT_FCN);
         leave_block(&level);
         in_proc_decl = 0;
      }
      ;

proc_hdg_decl   : proc_proto  opt_declarations
      {
         tab[$1].adr = (external? -1 : lc);
         if (tab[$1].mon) emit1(ENTERMON,tab[$1].mon); 
         process_var_inits(level);
      }
      ;

proc_proto  :  opt_atomic  proc_heading  ';'
      {
         $$ = $2;
         tab[$2].atomic = atomic;
      }
      ;

opt_atomic: /* empty */
         { atomic = FALSE; }
      | ATOMIC
         { atomic = TRUE; }
      ;

   /* newident not used here to be able to install mon procs at outer level */
proc_heading   : proc_id   formal_params
      ;

proc_id  : PROCEDURE IDENTIFIER
      { int vis_level,real_level;
         in_proc_decl = 1;
         if (in_mon_decl) {
            real_level = level;
            vis_level = ((level > 0) ? level -1 : 0);
         }
         else {
            vis_level = real_level = level;
         }
         $$ = prt = enter(lastident, (external? ext_procedure : procedure)
                              ,real_level,vis_level);
         if (in_mon_decl) tab[prt].mon = mtab[mon] - last_predeclared;
         enter_block(&last_btab,&level,prt);
      }
      ;

func_hdg_decl  :  func_proto  opt_declarations
      {
         tab[$1].adr = (external? -1 : lc);
         if (tab[$1].mon) emit1(ENTERMON,tab[$1].mon); 
         process_var_inits(level);
      }
      ;

func_proto  :  opt_atomic  func_heading  ';' 
      {
         tab[$2].atomic = atomic;
         $$ = $2;
      }
      ;

func_heading   : func_id   formal_params ':' ident
      { TYPES ft = tab[$4].typ; 
         if (tab[$4].obj != type){
            sprintf(pbuf,"'%s' is not a valid type",tab[$4].name);
            yyerror(pbuf);
         }
         if ((ft != ints)&&(ft != bools)&&(ft != chars)&&(ft != notyp)) 
            yyerror("Function return type must be INTEGER, BOOLEAN, or CHAR");
         tab[$1].typ = tab[$4].typ;
      }
      ;

func_id  : FUNCTION IDENTIFIER
      { int vis_level,real_level;
         in_proc_decl = 1;
         if (in_mon_decl) {
            real_level = level;
            vis_level = ((level > 0) ? level -1 : 0);
         }
         else {
            vis_level = real_level = level;
         }
         $$ = prt = enter(lastident, (external? ext_function: function)
                           ,real_level,vis_level);
         if (in_mon_decl) tab[prt].mon = mtab[mon] - last_predeclared;
         enter_block(&last_btab,&level,prt);
      }
      ;

formal_params  : /*empty*/
         { btab[display[level]].lastpar = last_tab;}
      | '('  formal_p_sects ')' 
         { btab[display[level]].lastpar = last_tab;}
      ;

formal_p_sects : formal_p_sects ';' formal_p_sect
      | formal_p_sect
      ;

formal_p_sect  : param_group
      | var_param_group
      | string_param_group
      ;

param_group : newident_list ':' ident
      { int i,dx,dv,j,elem_size,stack_dx;
         if (tab[$3].obj != type) {
            sprintf(pbuf,"'%s' is not a valid type",tab[$3].name);
            yyerror(pbuf);
         }
         j = display[level];
         elem_size = tab[$3].adr;
         stack_dx = stacksize(elem_size);
         dx = btab[j].vsize;
         dv = dx - btab[j].psize; /* in case vbls have been declared */
         for (i = $1; i <= last_tab; i++) {
            tab[i].typ = tab[$3].typ;
            tab[i].ref = tab[$3].ref;
            tab[i].lev = level;
            tab[i].obj = variable;
            tab[i].adr = dx;
            dx = stack_dx + dx;
         }
         btab[j].psize = dx;
         btab[j].vsize = dx + dv;
      }
      ;

var_param_group : VAR newident_list ':' ident
      { int i,dx,dv,j,elem_size;
         if (tab[$4].obj != type) {
            sprintf(pbuf,"'%s' is not a valid type",tab[$4].name);
            yyerror(pbuf);
         }
         j = display[level];
         elem_size = tab[$4].adr; 
         dx = btab[j].vsize;
         dv = dx - btab[j].psize; /* in case vbls have been declared */
         for (i = $2; i <= last_tab; i++) {
            tab[i].typ = tab[$4].typ;
            tab[i].ref = tab[$4].ref;
            tab[i].lev = level;
            tab[i].obj = variable;
            tab[i].adr = dx;
            tab[i].normal = FALSE; /* VAR parms are not normal */
            dx++;                  /* pointers occupy 1 stack word */
         }
         btab[j].psize = dx;
         btab[j].vsize = dx + dv;
      }
      ;

string_param_group : newident_list ':' STRING
      { int i,dx,dv,j,stack_dx;
         j = display[level];
         stack_dx = 1;            /* strings always pass by reference */
         dx = btab[j].vsize;
         dv = dx - btab[j].psize; /* in case vbls have been declared */
         for (i = $1; i <= last_tab; i++) {
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
      ;

mon_dcl_part  :  mon_heading   mon_decl  opt_stmt_part  ';'
      {
         if (tab[mtab[mon]].obj == monitor) {
            /* don't do the following for ext_monitor objects */
            /* main pgm will CALL_MONINIT, even if no init code */
            emit(RET_MONINIT); 
         }
         leave_block(&level);
      }
      ;

mon_heading :  MONITOR newident  ';'
      {
         if (mon == MAXMON) 
            cfatal("No more than %d monitors can be declared",MAXMON);
         if (level > 1) {
            sprintf(pbuf,
            "Monitors must be declared at level 1 or level 0, not level %d\n",
            level);
            yyerror(pbuf);
         }
         tab[$2].obj = (external ? ext_monitor : monitor);
         mtab[++mon] = $2;
         in_mon_decl = 1;
         $$ = $2;
         enter_block(&last_btab,&level,last_tab);
         btab[last_btab].lastpar = last_tab;  /* no parms */
      }
      ; 

mon_decl   : opt_declarations 
      {
         in_mon_decl = 0;  /* optional declarations are over */
         in_mon_init = 1;  /* could have some init code     */
      }
      ;

opt_stmt_part  :  END  /* no explicit init code, do implicit code */
      {
         tab[mtab[mon]].adr = lc;
         process_var_inits(level);
         in_mon_init = 0; 
      } 
      | compound_stmt
      ;

compound_stmt  : the_begin statements END
      ;

the_begin  :  SBEGIN    
      {
         if (in_mon_init) {   /* beginning of the mon init code */
            tab[mtab[mon]].adr = lc;
            process_var_inits(level);
            in_mon_init = 0; 
         } 
      }
      ;

statement_part : compound_stmt
      ;

statements  : statements ';' statement
      | statement
      | error  ';'
         { yyerrok; }
      | error  END
         { yyerrok; }
      ;

statement   : /*empty*/
      | compound_stmt
      | assignment
      | procedure_call
      | special_proc_call
      | if_expr then_stmt           %prec LOWER_THAN_ELSE
         { code[$1].y = $2; }
      | if_expr then_stmt the_else statement
         { code[$1].y = $3; code[$2].y = lc; }
      | the_while while_expr DO statement
         { emit1(JUMP,$1); code[$2].y = lc; }
      | the_repeat statements UNTIL expr
         { gen_exprval($4); emit1(JZER,$1); free_expr($4); }
      | for_header statement
         { emit1(ENDFOR,1+$1); code[$1].y = lc; }
      | cbegin statements CEND
         { if (level == 1) emit(COEND); in_cobegin = 0; }
      ;

if_expr  : IF expr
      { gen_exprval($2); $$ = lc; emit(JZER); free_expr($2); }
      ;

then_stmt   : THEN statement
      { $$ = lc; }
      ;

the_else    : ELSE
      { emit(JUMP); $$ = lc; }
      ;

the_while   : WHILE
      { $$ = lc; }
      ;

while_expr  : expr
      { gen_exprval($1); free_expr($1); $$ = lc; emit(JZER);}
      ;

the_repeat  : REPEAT 
      { $$ = lc; }
      ;

for_id   :  FOR  ident
      {
         $$ = $2;
         if (tab[$2].obj != variable) 
            yyerror("FOR loop index must be a VARIABLE");
         if (!tab[$2].normal) 
            yyerror("FOR loop index cannot be a VAR parameter");
         emit2(LOAD_ADDR,tab[$2].lev,tab[$2].adr);
         if ((tab[$2].typ != ints)&&(tab[$2].typ != bools)&&
             (tab[$2].typ != chars)&&(tab[$2].typ != notyp))
            yyerror("FOR loop index must be of type INTEGER, BOOLEAN, or CHAR");
      }
      ;

for_header  :  for_id_becomes_expr TO expr DO 
      {
         if (expr[$1].typ != expr[$3].typ)
            yyerror("Type mismatch in FOR loop limits");
         gen_exprval($3);
         $$ = lc; emit(BEGINFOR);
         free_expr($1); free_expr($3);
      }
      ;
            
for_id_becomes_expr  :  for_id BECOMES expr 
      {
         if ((tab[$1].typ != expr[$3].typ)&&(tab[$1].typ != notyp))
            yyerror("Type mismatch in FOR loop limits");
         gen_exprval($3);
         $$ = $3; /* pass expr index back to previous rule for type chk*/
      }
      ;

cbegin   :  CBEGIN
      {
         if (level != 1)
            yyerror("COBEGIN-COEND block allowed only in main program");
         else if (in_cobegin)
            yyerror("COBEGIN-COEND blocks cannot be nested");
         else
            emit(COBEGIN);
         in_cobegin = 1;
      }
      ;

assignment  : id_becomes expr
      {
         gen_exprval($2);
         if (tab[$1].typ == expr[$2].typ) {
            if ((tab[$1].typ == sems)||(tab[$1].typ == bsems)|| 
                (tab[$1].typ == conds))
               yyerror("Cannot assign to SEMAPHORE or CONDITION");
            else if (tab[$1].typ == arrays)
               emit1(COPY_BLOCK,atab[tab[$1].ref].size);
            else  /* standard types */
               emit(STORE);
         }
         else
            yyerror("Type mismatch in assignment");
         free_expr($2);
      }
      | array_var BECOMES expr
      {
         gen_exprval($3);
         if (expr[$1].typ == expr[$3].typ) {
            if ((expr[$1].typ == sems)||(expr[$1].typ == bsems)|| 
                (expr[$1].typ == conds))
               yyerror("Cannot assign to SEMAPHORE or CONDITION");
            else if (expr[$1].typ == arrays)
               emit1(COPY_BLOCK,atab[expr[$3].ref].size);
            else
               emit(STORE);
         }
         else
            yyerror("Type mismatch in assignment");
         free_expr($1); free_expr($3);
      }
      ;

id_becomes  :  ident BECOMES
      {
         switch (tab[$1].obj) {
         case ext_variable:
         case variable:
            if ((tab[$1].typ == sems)||(tab[$1].typ == bsems))
               yyerror("Must use INITIALSEM to initialize a SEMAPHORE");
            else
               emit2((tab[$1].normal ? LOAD_ADDR : LOAD_VALUE),
                  tab[$1].lev,tab[$1].adr);
            break;
         case ext_function:
         case function:
            if ((tab[$1].lev+1) == level) /* fn name declared in outer scope */
               /* s[display[tab[$1].lev+1]+0] is loc of fn return value */
               emit2(LOAD_ADDR,tab[$1].lev+1,0);
            else
               yyerror("FUNCTION identifier inaccessible at this level");
            break;
         default:
            yyerror("VARIABLE or FUNCTION identifier expected");
         } /* switch */
      }
      ;

procedure_call :  pfv_id check_proc actual_params
      { int k;
         if ((tab[$1].obj == procedure)||
             (tab[$1].obj == ext_procedure)) {
            if (tab[$1].lev == -1) { /* std proc unfinished business */
               stdproc_pcode($1,parmct);
            } /* if std proc */
            else { /* regular proc call */
               /* first check on too few parms */
               k = btab[tab[$1].ref].lastpar - $1;
               if ((k > parmct)&&(first_parmcterr)) {
                  yyerror("Too few actual parameters in the call");
                  first_parmcterr = 0;
               }
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
      ;

actual_params  :  /* empty */
      |  '(' actuals_list ')'
      ;

actuals_list   : actuals_list ',' actual_param
      | actual_param
      ; 

actual_param   : expr    /* which could be a variable or a proc/fn id */
      {
         parmct = ++pfstack[toppfs].pct;
         if (tab[last_pf].lev == -1)
            stdproc_parm(last_pf,$1,parmct);
         else
            proc_parm(last_pf,$1,parmct);
         free_expr($1);
      }
      | RAWSTRING    /* for write/writeln parms only */
      { int tmp = store_string(laststring,&stab_size);
         if ((tab[last_pf].lev == -1)&&
            ((tab[last_pf].adr == SP_WRITE)||
             (tab[last_pf].adr == SP_WRITELN))){
               emit1(WRITE_RAWSTRING,tmp);
         }
         parmct++;
      }
      ;

pfv_id   :  ident
      { $$ = last_pfv = $1; }
      ;

check_proc  :  /* no syntax */
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
      ;

special_proc_call  :  send_call
      |  broadcast_call
      |  stringconcat_call 
      |  stringcopy_call
      |  sprintf_call
      ;

send_call  :  SEND  left_exprparm  ','  expr  ')'
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
         if (expr[$2].typ != strings) {
            yyerror("right parameter is not of type 'string'");
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

sprintf_call  :  sprintf_buf_fmt  ','  sprintf_parmlist  ')'
         {
            /* push the parm count for interp's SPRINTF_OP to use */
            emit1(PUSH_LIT,sprintf_cnt); 
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
            sprintf_cnt = 0;
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
            sprintf_cnt++;
            free_expr($1);
         }
      |  sprintf_parmlist  ','  expr
         {  
            if ((expr[$3].typ != ints)&&(expr[$3].typ != strings)&&
                (expr[$3].typ != chars))
               yyerror(
            "sprintf parameter must be either of type 'char', 'int' or type 'string'");
            if (expr[$3].typ == strings)
               emit_push_addr(expr[$1]);
            else
               gen_exprval($3);
            sprintf_cnt++;
            free_expr($3);
         }
      ; 

expr  : simple_expr
      | left_simp_expr relational_op simple_expr
      {
         gen_exprval($3);
         if (expr[$1].typ == expr[$3].typ){
            if ((expr[$1].typ == ints)||(expr[$1].typ == bools)||
                (expr[$1].typ == chars))
               emit($2);
            else
               yyerror("Only INTEGER, BOOLEAN, CHAR types can be compared");
         }
         else
            yyerror("Types in comparison are unequal");
         expr[$1].typ = bools;
         free_expr($3);
      }
      ;

relational_op  : '='  { $$ = TEST_EQ; }
      | '<' { $$ = TEST_LT; }
      | '>' { $$ = TEST_GT; }
      | LE  { $$ = TEST_LE; }
      | GE  { $$ = TEST_GE; }
      | NE  { $$ = TEST_NE; }
      ;

simple_expr : term
      | '+' term
      {
         $$ = $2;
         gen_exprval($$);
         if ((expr[$$].typ != ints)&&(expr[$$].typ != notyp)){
            yyerror("INTEGER type expected");
            expr[$$].typ = notyp;
         }
      }
      | '-' term
      {
         $$ = $2;
         gen_exprval($$);
         if ((expr[$$].typ != ints)&&(expr[$$].typ != notyp)){
            yyerror("INTEGER type expected");
            expr[$$].typ = notyp;
         }
         else
            emit(NEGATE);
      }
      | left_simp_expr add_op term
      {
         gen_exprval($3);
         switch ($2) {
         case DO_OR:
            if ((expr[$1].typ == bools)&&(expr[$3].typ == bools))
               emit(DO_OR);
            else if ((expr[$1].typ != notyp)&&(expr[$3].typ != notyp)){
               yyerror("BOOLEAN type expected");
               expr[$1].typ = notyp;
            }
            break;
         case DO_ADD:
         case DO_SUB:
            expr[$1].typ = resulttype(expr[$1].typ,expr[$3].typ);
            if (expr[$1].typ == ints)
               emit($2);
            break;
         } /* switch */
         free_expr($3);
      }
      ;

left_simp_expr :  simple_expr
      { gen_exprval($1); }
      ;

add_op      : '+' { $$ = DO_ADD; }
      | '-' { $$ = DO_SUB; }
      | OR  { $$ = DO_OR; }
      ;

term     : factor 
      | left_term mult_op factor
      {
         gen_exprval($3);
         switch($2) {
         case DO_AND:
            if ((expr[$1].typ == bools)&&(expr[$3].typ == bools))
               emit(DO_AND);
            else if ((expr[$1].typ != notyp)&&(expr[$3].typ != notyp)){
               yyerror("BOOLEAN type expected");
               expr[$1].typ = notyp;
            }
            break;
         case DO_MUL:
         case DO_DIV:
         case DO_MOD:
            if ((expr[$1].typ == ints)&&(expr[$3].typ == ints))
               emit($2);
            else if ((expr[$1].typ != notyp)&&(expr[$3].typ != notyp)){
               yyerror("INTEGER type expected");
               expr[$1].typ = notyp;
            }
            break;
         } /* switch */
         free_expr($3);
      }
      ;

left_term   : term
      { gen_exprval($1); }
      ;

mult_op     : '*' { $$ = DO_MUL; }
      | DIV    { $$ = DO_DIV; }
      | MOD    { $$ = DO_MOD; }
      | AND    { $$ = DO_AND; }
      ;

factor      : variable     /* could be a const_id, array, or fn_call*/
      | unsigned_num
      {
         $$ = new_expr();
         expr[$$].obj = constant;
         expr[$$].adr = $1;
         expr[$$].typ = ints;
         expr[$$].lev = level;
      }
      | CHAR
      {
         $$ = new_expr();
         expr[$$].obj = constant;
         expr[$$].adr = laststring[0];
         expr[$$].typ = chars;
         expr[$$].lev = level;
      }
      | '(' expr ')' { $$ = $2;}
      | NOT factor
      {
         $$ = $2;
         gen_exprval($$);
         if (expr[$$].typ == bools)
            emit(COMPLEMENT);
         else if (expr[$$].typ != notyp){
            yyerror("BOOLEAN type expected");
            expr[$$].typ = notyp;
         }
      }
      ;

variable : pfv_id check_func actual_params
      { int k; 
         $$ = new_expr();
         expr[$$].obj = tab[$1].obj;
         expr[$$].typ = tab[$1].typ;
         expr[$$].adr = tab[$1].adr;
         expr[$$].ref = tab[$1].ref;
         expr[$$].lev = tab[$1].lev;
         expr[$$].normal = tab[$1].normal;
         if ((tab[$1].obj != variable)&&
             (tab[$1].obj != ext_variable)&&
             (tab[$1].obj != constant)&& 
             (tab[$1].obj != function)&&
             (tab[$1].obj != ext_function))
            yyerror("VARIABLE, CONSTANT, or FUNCTION CALL expected");
         if ((tab[$1].obj == function)||
             (tab[$1].obj == ext_function)) {
            expr[$$].ref = 0;   /* in a func call ref field of func == 0 */
            if (tab[$1].lev == -1) { /* std function unfinished business */
               stdproc_pcode($1,parmct);
            } /* if std proc */
            else { /* regular func call */
               /* first check on too few parms */
               k = btab[tab[$1].ref].lastpar - $1;
               if ((k > parmct)&&(first_parmcterr)) {
                  yyerror("Too few actual parameters in the call");
                  first_parmcterr = 0;
               }
               if (in_cobegin)
                  yyerror("Function calls are not allowed in a COBEGIN block");
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
      |  special_func_call
      |  array_var
      |  typecast
      ;

check_func  :  /* no syntax */
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
      ;

special_func_call  :  stringcompare_call
      |  stringlength_call
      |  receive_call
      |  sscanf_call
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
         if (expr[$4].obj != variable) 
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
            emit1(PUSH_LIT,sscanf_cnt); 
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
            sscanf_cnt = 0;
         }
      ;

sscanf_parmlist  :  expr
         {  
            if ((expr[$1].typ != ints)&&(expr[$1].typ != strings))
               yyerror(
            "sscanf parameter must be either of type 'int' or type 'string'");
            emit_push_addr(expr[$1]);
            sscanf_cnt++;
            free_expr($1);
         }
      |  sscanf_parmlist  ','  expr
         {  
            if ((expr[$3].typ != ints)&&(expr[$3].typ != strings))
               yyerror(
            "sscanf parameter must be either of type 'int' or type 'string'");
            emit_push_addr(expr[$3]);
            sscanf_cnt++;
            free_expr($3);
         }
      ; 

array_var :  array_id '[' index_expr ']'
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

array_id : ident
      {
         if (tab[$1].typ != arrays) {
            sprintf(pbuf,"'%s' is not of type ARRAY",tab[$1].name);
            yyerror(pbuf);
            last_aref = 0;
            last_eltyp = notyp;
         }
         else {
            last_aref = tab[$1].ref;
            last_eltyp = atab[last_aref].eltyp;
            /* put addr of array base on the stack */
            emit2((tab[$1].normal ? LOAD_ADDR : LOAD_VALUE),
               tab[$1].lev,tab[$1].adr);
         }
         if (++topars > MAXARNEST) 
            yyerror("Maximum array nesting exceeded");
         else {
            arstack[topars].aref = last_aref;
            arstack[topars].eltyp = last_eltyp;
         }
         $$ = new_expr();
         expr[$$].obj = tab[$1].obj;
         expr[$$].typ = last_eltyp;
         expr[$$].adr = tab[$1].adr;
         expr[$$].ref = tab[$1].ref;
         expr[$$].lev = tab[$1].lev;
         expr[$$].normal = tab[$1].normal;
      }
      ;

index_expr :  index_expr ',' expr
      { 
         gen_exprval($3);
         last_aref = arstack[topars].aref;
         last_eltyp = arstack[topars].eltyp;
         index_expr($3,&last_aref,&last_eltyp); 
         arstack[topars].aref = last_aref;
         arstack[topars].eltyp = last_eltyp;
         free_expr($3); 
      }
      |  expr
      { 
         gen_exprval($1);
         last_aref = arstack[topars].aref;
         last_eltyp = arstack[topars].eltyp;
         index_expr($1,&last_aref,&last_eltyp); 
         arstack[topars].aref = last_aref;
         arstack[topars].eltyp = last_eltyp;
         free_expr($1); 
      } 
      ;

typecast  :  INT  '('  expr  ')'
      {
         expr[$3].typ = ints;
         $$ = $3;
      }
      ;

ident    : IDENTIFIER
      { $$ = loc(lastident,level); }
      ;

newident : IDENTIFIER
      { $$ = enter(lastident,constant,level,level); } 
      /* obj field of const will probably be changed */
      ;

%%   /*start of routines*/

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
