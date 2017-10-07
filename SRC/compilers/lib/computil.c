  /* utilities for BenAri PCODE compilers */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "../include/globdata.h"
#include "../include/globtabs.h"
#include "../include/compdata.h"
#include "../include/computil.h"
#include "../include/writetab.h"
#include "../include/incfiles.h"
#include "../include/pcode.h"

extern int fprintf(FILE*,const char*,...);

static char ubuf[128];

extern void yyerror(char *);
extern void exit(int);
extern int vfprintf(FILE  *stream, const char *format, va_list ap);


void cfatal(const char *fmt, ...)
   /* fmt is the format string                  */
   /* ... is the list of things to be printed   */
   /* output is written to stderr & yyout       */
   /* named 'cfatal' to avoid name clash with 'fatal' in genutil.c */
   /* that only writes to stderr                                   */
{
   va_list ap;

   fprintf(stderr, "** Error: ");
   va_start(ap,fmt);
   vfprintf(stderr,fmt,ap);
   va_end(ap);
   fprintf(stderr,"\n");

   fprintf(yyout, "** Error: ");
   va_start(ap,fmt);
   vfprintf(yyout,fmt,ap);
   va_end(ap);
   fprintf(yyout,"\n");
   exit(1);
}

void cnonfatal(const char *fmt, ...)
   /* fmt is the format string                  */
   /* ... is the list of things to be printed   */
   /* output is written to stderr & yyout       */
   /* named 'cnonfatal' to avoid name clash with 'nonfatal' in genutil.c */
   /* that only writes to stderr                                   */
{
   va_list ap;

   fprintf(stderr, "++ Warning: ");
   va_start(ap,fmt);
   vfprintf(stderr,fmt,ap);
   va_end(ap);
   fprintf(stderr,"\n");

   fprintf(yyout, "++ Warning: ");
   va_start(ap,fmt);
   vfprintf(yyout,fmt,ap);
   va_end(ap);
   fprintf(yyout,"\n");
}

void enterstid(char *thisid, int thisobj, TYPES thistyp, int thisadr)
   /* enter 'standard' id into symbol table without a duplicate check */
{
   last_tab++;
   if (last_tab > TMAX) 
      cfatal("Symbol table space full with %d entries",TMAX);
   strncpy(tab[last_tab].name,thisid,ALNG);
   if (tab[last_tab].name[ALNG] != '\0') tab[last_tab].name[ALNG] = '\0';
   tab[last_tab].link = last_tab - 1;
   tab[last_tab].obj = thisobj;
   tab[last_tab].typ = thistyp;
   tab[last_tab].ref = 0;
   tab[last_tab].normal = TRUE; /* only VAR parms are not NORMAL */
   tab[last_tab].lev = -1;      /* all std id's are marked by -1 level */
   tab[last_tab].adr = thisadr; 
   tab[last_tab].mon = 0; 
   btab[0].last = last_tab;
}  /* enterstid */
    

int   enter(char *id,OBJTYPE obj,int level,int vislevel)
  /* search for enter "id" name at static level "vislevel". If found,    */
  /* declare duplicate id & return 0.  If not found, then place id at    */
  /* "vislevel" level, set the "lev" field to "level" (id's REAL level)  */
  /* and return tab index of "id".  This level manipulation is used to   */
  /* make monitor procs (and nothing else ) visible from the outer scope */
{
int   j,l;
ALFA  tmpid;
   if (last_tab == TMAX) 
      cfatal("Symbol table full with %d entries",TMAX);
   strncpy(tmpid,id,ALNG);
   if (tmpid[ALNG] != '\0') tmpid[ALNG] = '\0';
   strcpy(tab[0].name,tmpid);
   j = btab[display[vislevel]].last; /* name is at "vislevel" static level */
   l = j;
   while (strcmp(tab[j].name,tmpid)) j = tab[j].link;
   if (j != 0) {
      sprintf(ubuf,"Duplicate identifier '%s' at level %d",tmpid,vislevel);
      yyerror(ubuf);
      return(0);
   }
   last_tab++;
   strcpy(tab[last_tab].name,tmpid);
   tab[last_tab].link = l;
   tab[last_tab].obj = obj;
   tab[last_tab].typ = notyp;    /* will probably be changed later */
   tab[last_tab].normal = TRUE;  /* only VAR parms are not normal */
   tab[last_tab].lev = level; /* set lev to the id's REAL level */
   tab[last_tab].ref = tab[last_tab].adr = tab[last_tab].mon = 0;
   btab[display[vislevel]].last = last_tab; /* this id is at "vislevel" level */
   return(last_tab);
}  /* enter */
 
int loc(char *id,int level)
   /* searches for 'id' from block 'level' out to level 0      */
   /* expecting to find it.  return tab index of id if found,  */
   /* else declare error & return 0                            */
{
int i,j;
ALFA  tmpid;
   strncpy(tmpid,id,ALNG);
   if (tmpid[ALNG] != '\0') tmpid[ALNG] = '\0';
   strcpy(tab[0].name,tmpid);
   i = level;
   do {
      j = btab[display[i]].last;
      while (strcmp(tab[j].name,tmpid) != 0) 
         j = tab[j].link;
      i--;
   } while ((i >= 0) && (j == 0));
   if (j == 0){
      sprintf(ubuf,"Undeclared identifier '%s' at level %d",tmpid,level);
      yyerror(ubuf);
      return(0);
   }
   return(j);  /* returns 0 if not found */
}  /* loc */

void  enter_block(int *b, int *level, int t)
   /* enter new code block and make a record of it in btab */
{
   if (++*level > LMAX) 
      cfatal("Maximum nesting level of %d exceeded",LMAX);
   if (*b == BMAX)
      cfatal("Block table full with %d entries",BMAX);
   (*b)++;
   btab[*b].psize = btab[*b].vsize = 5; /* fixed part of stack frame */
   btab[*b].last = btab[*b].lastpar = 0;
   display[*level] = *b;
   tab[t].ref = *b;
}  /* enter_block */

void  leave_block(int *level)
   /* leave the current block by going back to the the next level out */
{
   if (*level > 0)
      (*level)--;
   else
      cfatal("Attempt to decrement below 0 level");
}  /* leave_block */

void  emit(int fct)
   /* create a new pcode instruction, (fct,0,0) */
{
   if (lc == CMAX) 
      cfatal("Code table full with %d entries",CMAX);
   code[lc].f = fct;
   code[lc].x = 0;
   code[lc].y = 0;
   if ((curr_infile >= 0) &&
         (infile[curr_infile].line_no > infile[curr_infile].last_line_no)) {
      last_dbg++;
      dbg[last_dbg].lc = lc;
      dbg[last_dbg].fix = curr_infile;
      dbg[last_dbg].flno = infile[curr_infile].line_no;
      infile[curr_infile].last_line_no = infile[curr_infile].line_no;
   }
   lc++;
}  /* emit */

void  emit1(int fct,int b)
   /* create a new pcode instruction  (fct,0,b) */
{
   if (lc == CMAX) 
      cfatal("Code table full with %d entries",CMAX);
   code[lc].f = fct;
   code[lc].x = 0;
   code[lc].y = b;
   if ((curr_infile >= 0) &&
         (infile[curr_infile].line_no > infile[curr_infile].last_line_no)) {
      last_dbg++;
      dbg[last_dbg].lc = lc;
      dbg[last_dbg].fix = curr_infile;
      dbg[last_dbg].flno = infile[curr_infile].line_no;
      infile[curr_infile].last_line_no = infile[curr_infile].line_no;
   }
   lc++;
}  /* emit1 */

void  emit2(int fct,int a,int b)
   /* create a new pcode instruction  (fct,a,b) */
{
   if (lc == CMAX) 
      cfatal("Code table full with %d entries",CMAX);
   code[lc].f = fct;
   code[lc].x = a;
   code[lc].y = b;
   if ((curr_infile >= 0) &&
         (infile[curr_infile].line_no > infile[curr_infile].last_line_no)) {
      last_dbg++;
      dbg[last_dbg].lc = lc;
      dbg[last_dbg].fix = curr_infile;
      dbg[last_dbg].flno = infile[curr_infile].line_no;
      infile[curr_infile].last_line_no = infile[curr_infile].line_no;
   }
   lc++;
}  /* emit2 */


int   get_constant(char *id, int level, TYPES *ctype)
   /* look for a constant at the specified level                   */
   /* return the value of it, if found, else make an error message */
{
int   i;
   if ((i = loc(id,level)) == 0) {
      *ctype = notyp;
      return(0);
   }
   if (tab[i].obj != constant) {
      sprintf(ubuf,"Identifier '%s' not of constant type",tab[i].name);
      yyerror(ubuf);
      *ctype = notyp;
      return(0);
   }
   *ctype = tab[i].typ;
   return(tab[i].adr);
}  /* get_constant */

int   new_expr()
   /* rehabilitate an entry of the 'expr' array to hold an expression */
   /* being parsed and return its index                               */
{
int   i;
   if (expr_cnt == EXPRSIZE)
      cfatal("EXPR array exhausted");
   expr_cnt++;
   for (i = 0; i < EXPRSIZE; i++)
      if (expr[i].free){
         expr[i].obj = constant;
         expr[i].typ = notyp;
         expr[i].tix = -1;
         expr[i].adr = expr[i].ref = 0; 
         expr[i].lev = expr[i].normal = expr[i].result_used = 0;
         expr[i].arelt = expr[i].isval = expr[i].free = 0;
         return(i);
      }
   cfatal("EXPR array inconsistency %d expressions in use",expr_cnt);
   return (0);    /* should never happen */
}  /* new_expr */

void  free_expr(int i)
   /* free entry 'i' of the 'expr' array */
{
   if ((i >= 0)&&(i <= MAXEXPR))
      if (!expr[i].free){
         expr_cnt--;
         expr[i].free = 1;
      }
}  /* free_expr */

void  index_expr(int eix,int *aref,TYPES *eltyp)
/* multidimensional arrays are stored as multiple entries of  */
/* the array table, one for each dimension.  The element      */
/* type of each section is 'arrays' until the last section,   */
/* which has the actual element type of the array.  The 'ref' */
/* field contains the index in the 'atab' array of the next   */
/* section of the array, until the last section, which is 0   */ 
/* This proc works its way across the dimensions of a multi-  */
/* dimensional array to find the element type of the array    */
/* In addition, it checks the index type of the index & emits */
/* the required INDEX PCODE.                                  */
{
   if (*aref >= 0) {
      if (atab[*aref].inxtyp != expr[eix].typ)
         yyerror("Type conflict in array index");
      emit1(INDEX,*aref);
      *eltyp = atab[*aref].eltyp;
      *aref = atab[*aref].elref;
   }
}  /* index_expr */

void gen_loadvar(int ex)
/* 
   emits correct LOAD instruction, depending on what
   sort of variable expr[ex] is
*/
{
int f;
   if ((expr[ex].typ == arrays)||(expr[ex].typ == strings))
      f = (expr[ex].normal ? LOAD_ADDR : LOAD_VALUE);
   else    /* one of the standard types */
      f = (expr[ex].normal ? LOAD_VALUE : LOAD_INDIR);
   emit2(f,expr[ex].lev,expr[ex].adr);
}

void gen_exprval(int ex)
/* The grammar encoded in the recursive descent BenAri
   parser is not context-free, because the actions applied
   to 'expressions' as VAR parameters are different from those
   applied to expressions as value paramters.  Consequently, it is 
   necessary to postpone code emission for expressions as long as 
   possible, until the expression is used.  
      This procedure emits the code necessary to leave the value of 
   the expression on the TOS.  In two cases, some code emission has 
   occurred previously:
      function:  The MARKSTACK has to be emitted before the expressions
                 that are the parameters of the call are placed on the
                 stack.
      array ref  The address calculation for an array reference leaves
                 the address of the indexed element on the stack.  Only
                 a VALUE_AT is required to load the value.
*/
{
   if (expr[ex].isval) return;
   expr[ex].isval = 1; 
   if (expr[ex].arelt){
      if ((expr[ex].typ != arrays)&&(expr[ex].typ != strings)) emit(VALUE_AT);
      expr[ex].arelt = 0; 
      return;
   }
   switch (expr[ex].obj) {
   case constant:
      emit1(PUSH_LIT,expr[ex].adr);
      break;
   case ext_variable:
   case variable:
      gen_loadvar(ex);
      break;
   case ext_function:
   case function: 
      break;  /* nothing more need be done */
   default:
      yyerror("A 'proc', 'monitor', or 'type' cannot appear in an expression");
   } /* switch */
}  /* gen_exprval */


void  proc_parm(int px,int ex,int pct)
   /* used in parser to check parms for a regular call */
   /* 'px'    tab index of this proc or func           */
   /* 'expr'  index of the parm                        */
   /* 'pct'   current count of parms                   */
{
int   cp = px + pct;
int   lp = btab[tab[px].ref].lastpar;  /* tab ix of proc's last parm */
   if ((cp > lp)&&(first_parmcterr)){
      yyerror("More actual parameters than formal parameters");
      first_parmcterr = 0;
   }
   else if (expr[ex].typ == strings){  /* strings are always passed by ref */
      if (!expr[ex].arelt) /* array ref ==> addr is already on the stack */
         emit2(LOAD_ADDR,expr[ex].lev,expr[ex].adr);
   }
   else {
      if (tab[cp].normal) { /* value parameter */
         gen_exprval(ex);
         if (expr[ex].typ == tab[cp].typ) {
            if (expr[ex].typ == arrays)
               emit1(LOAD_BLOCK,atab[expr[ex].ref].size);
         }
         else if (expr[ex].typ != notyp)
            yyerror("Parameter type mismatch");
      }
      else { /* var parameter */
         if ((expr[ex].obj != variable)&&(expr[ex].obj != ext_variable))
            yyerror("Reference (VAR) parameter must be a 'variable'");
         if (!expr[ex].arelt)
            emit2((expr[ex].normal ? LOAD_ADDR : LOAD_VALUE),
                  expr[ex].lev, expr[ex].adr);
      }
   }
}  /* proc_parm */

int   binsem = 0;

void  stdproc_parm(int px, int ex, int pct)
   /* used in parser to check parms for a call of a 'standard' proc */
   /* 'px'    tab index of this proc or func                        */
   /* 'ex'    expr index of the parm                                */
   /* 'pct'   current count of parms                                */
   /* Since standard procs don't have a btab entry and their parms  */
   /* are not in the tab array, this information must be included   */
   /* in the code.  This is the place for most of it.               */
   /* The only PCODE that is emitted here is for each parameter of  */
   /* a READ/READLN or WRITE/WRITELN call, and the WAITs & SIGNALs  */
   /* (because binary semaphores need to be checked for).           */
   /* PCODE for all other std procs & functions is emitted in       */
   /* 'stdproc_pcode'                                               */
{
int   too_many=0;
int   pcode; 
   /* take care of the standard procs first */
   if (tab[px].obj == procedure){
      switch (tab[px].adr) {
      case SP_READ:
      case SP_READLN:
         if (pct > 0) {
            if ((expr[ex].obj != variable)&&(expr[ex].obj != ext_variable))
               yyerror("Input item must be a 'variable'");
            else if ((expr[ex].typ != ints)&&(expr[ex].typ != chars)&&
                     (expr[ex].typ != notyp))
               yyerror("Input item must be of type 'int' or 'integer' or 'char'");
            else if (expr[ex].arelt)
               emit1(READ,expr[ex].typ);
            else { 
               emit2((expr[ex].normal ? LOAD_ADDR : LOAD_VALUE),
                  expr[ex].lev, expr[ex].adr);
               emit1(READ,expr[ex].typ);
            }
         }
         break;
      case SP_WRITE:
      case SP_WRITELN:
         if (pct > 0) {
            if (expr[ex].typ == arrays) /* presently our only str type */
               yyerror("Cannot 'write' a structured type");
            else if (expr[ex].typ == strings) {
               if (!expr[ex].arelt)
                  emit2((expr[ex].normal ? LOAD_ADDR : LOAD_VALUE),
                     expr[ex].lev, expr[ex].adr);
               emit(WRITE_STRING);
            }
            else{
               gen_exprval(ex);
               emit1(WRITE,expr[ex].typ);
            }
         }
         break;
      case SP_INITSEM:
         if (pct == 1) {
            if ((expr[ex].obj != variable) && (expr[ex].obj != ext_variable))
               yyerror("Parameter must be a 'variable'");
            else if ((expr[ex].typ != sems)&&(expr[ex].typ != bsems))
               yyerror("Parameter must be a 'semaphore'");
            else
               binsem = (expr[ex].typ == bsems);
            if (!expr[ex].arelt) 
               emit2((expr[ex].normal ? LOAD_ADDR : LOAD_VALUE),
                  expr[ex].lev, expr[ex].adr);
          }
         else if (pct == 2) {
            if (expr[ex].typ == ints){
               gen_exprval(ex);
               emit1(STORE_SEM,binsem);
            }
            else
               yyerror("Parameter must be of type 'int' (or 'integer')");
         }
         else too_many = 1;
         break;
      case SP_PWAIT:
      case SP_VSIGNAL:
         if (pct == 1) {
            if ((expr[ex].obj != variable)&&(expr[ex].obj != ext_variable)) {
               yyerror("Parameter must be a 'variable'");
            }
            else if ((expr[ex].typ != sems)&&(expr[ex].typ != bsems))
               yyerror("Parameter must be a 'semaphore'");
            else {
            /* if it's an array element, it's address is already on stack */
               if (!expr[ex].arelt) 
                  /* pass by reference */
                  emit2((expr[ex].normal ? LOAD_ADDR : LOAD_VALUE),
                     expr[ex].lev, expr[ex].adr);
               pcode = ((tab[px].adr == SP_PWAIT) ? P_WAIT : V_SIGNAL);
               emit1(pcode,(expr[ex].typ == bsems));
            }
         }
         else too_many = 1;
         break;
      case SP_SIGNALC:         
         if (pct == 1) {
            if ((expr[ex].obj != variable)&&(expr[ex].obj != ext_variable))
               yyerror("Parameter must be a 'variable'");
            else if (expr[ex].typ != conds)
               yyerror("Parameter must be a 'condition'");
            else {
               if (!expr[ex].arelt) 
                  /* pass by reference */
                  emit2((expr[ex].normal ? LOAD_ADDR : LOAD_VALUE),
                     expr[ex].lev, expr[ex].adr);
            }
         }
         else too_many = 1;
         break;
      case SP_WAITC:
         if (pct == 1) {
            if ((expr[ex].obj != variable)&&(expr[ex].obj != ext_variable))
               yyerror("Parameter must be a 'variable'");
            else if (expr[ex].typ != conds)
               yyerror("Parameter must be a 'condition'"); 
            /* if it's an array element, it's address is already on stack */
            if (!expr[ex].arelt)
                  /* pass by reference */
               emit2((expr[ex].normal ? LOAD_ADDR : LOAD_VALUE),
                  expr[ex].lev, expr[ex].adr);
            /* pcode can't be emitted here, must look for possible parm 2 */
         }
         else if (pct == 2) {
            if (expr[ex].typ != ints) 
               yyerror("Parameter must be of type 'int' (or 'integer')");
            else
               gen_exprval(ex);
            /* pcode can't be emitted here either. could only be 1 parm */
         }
         else too_many = 1;
         break;
      case SP_EXIT:
         if (pct > 2) too_many = 1; 
         break;
      case SP_SUSPEND:
         if (pct > 0) too_many = 1;
         break;
      case SP_REVIVE:
         if (pct == 1) {
            if (expr[ex].typ != ints) 
               yyerror("Parameter must be of type 'int' (or 'integer')");
            else 
               gen_exprval(ex); /* pass by value */
         }
         else
            too_many = 1;
         break;
      } /* switch */
   }  /* end of the std procs */
   else if (tab[px].obj == function) { /* now do std functions */
      switch (tab[px].adr) {
      case SF_EOF:
      case SF_EOLN:  
         if (pct > 0) too_many = 1; 
         break;
      case SF_EMPTY:
         if (pct == 1) {
            if ((expr[ex].obj != variable)&&(expr[ex].obj != ext_variable))
               yyerror("Parameter must be a VARIABLE");
            else if (expr[ex].typ == conds) {
               if (!expr[ex].arelt) 
                     /* pass by reference */
                  emit2((expr[ex].normal ? LOAD_ADDR : LOAD_VALUE),
                     expr[ex].lev, expr[ex].adr);
            }
            else
               yyerror("Parameter must be a 'condition'");
         }
         else too_many = 1;
         break;
      case SF_WHICHPROC:
         if (pct > 0) too_many = 1;
         break;
      case SF_RANDOM:
         if (pct == 1) {
            if (expr[ex].typ != ints) 
               yyerror("Parameter must be of type 'int' (or 'integer')");
            else 
               gen_exprval(ex); /* pass by value */
         }
         else too_many = 1;
         break;
      } /* switch */
   } /* end of the std functions */
   if((too_many)&&(first_parmcterr)) {
      yyerror("Too many actual parameters");
      first_parmcterr = 0;
   }
}  /* stdproc_parm */

void  stdproc_pcode(int px, int pct)
   /* used in parser to check parms for a call of a 'standard' proc     */
   /* 'px'      tab index of this proc or func                          */
   /* 'pct'     current count of parms                                  */
   /* The PCODE for every standard proc or function is emitted here,    */
   /* with the exception of the READs and WRITEs that are required for  */
   /* the parameters of READ/READLN or WRITE/WRITELN procs.             */
{
int too_few = 0;
   /* take care of the standard procs first */
   if (tab[px].obj == procedure){
      switch (tab[px].adr) {
      case SP_READ:     /* PCODE for these std procs was emitted in */
      case SP_WRITE:    /* std_procparm                             */
      case SP_INITSEM:  /* BUT, do need to check if no parms were   */
      case SP_PWAIT:    /* supplied */
      case SP_VSIGNAL:  (too_few = (pct == 0)); 
                        break;
      case SP_READLN:   emit(READLN); break;
      case SP_WRITELN:  emit(WRITELN); break;
      case SP_SIGNALC:  emit(SIGNALC); 
                        (too_few = (pct == 0)); break;
      case SP_WAITC:    if (pct == 1) emit1(PUSH_LIT, DEFAULT_PRIO);
                        emit(WAITC);
                        (too_few = (pct == 0)); break;
      case SP_EXIT:     emit(HALT); break;
      case SP_SUSPEND:  emit(SUSPEND); break;
      case SP_REVIVE:   emit(REVIVE);
                        (too_few = (pct == 0)); break;
      } /* switch */
   }  /* end of the std procs */
   else if (tab[px].obj == function) { /* now do std functions */
      switch (tab[px].adr) {
      case SF_EOF:
      case SF_EOLN:        emit2(EOLN_OR_EOF,0,tab[px].adr); break;
      case SF_EMPTY:       emit(EMPTY);
                           (too_few = (pct == 0)); break;
      case SF_WHICHPROC:   emit(WHICH_PROC); break;
      case SF_RANDOM:      emit(RANDOM);
                           (too_few = (pct == 0)); break;
      } /* switch */
   } /* end of the std functions */
   if(too_few) yyerror("One parameter is required");
}  /* stdproc_pcode */

TYPES resulttype(TYPES a, TYPES b)
   /* used for checking the result type of a binary operator */
{
   if ((a > ints)||(b > ints)) {
      yyerror("type 'int' (or 'integer') expected");
      return(notyp);
   }
   if ((a == notyp) || (b == notyp))
      return(notyp);
   return(ints);
}  /* resulttype */

unsigned int strtoi(char *str, int base)
/* 
   interprets 'str' string as an unsigned integer in the base 'base'
   for 'base' == 2, 8, 10, or 16
   no overflow check
   complains and returns 0 if invalid digit is seen
*/
{
int test;
char nextdig;
unsigned int tmp=0;
   switch(base) {
   case 2: 
      while (*str)
      {
         nextdig = *str++;
         if ((nextdig > '1')||(nextdig < '0')){
            yyerror("invalid binary digit");
            return(0);
         } 
         tmp = 2*tmp + nextdig - '0';
      }
      break;
   case 8:         
      while (*str)
      {
         nextdig = *str++;
         if ((nextdig > '7')||(nextdig < '0')){
            yyerror("invalid octal digit");
            return(0);
         } 
         tmp = 8*tmp + nextdig - '0';
      }
      break;
   case 10:         
      while (*str)
      {
         nextdig = *str++;
         if ((nextdig > '9')||(nextdig < '0')){
            yyerror("invalid decimal digit");
            return(0);
         } 
         tmp = 10*tmp + nextdig - '0';
      }
      break;
   case 16:
      while (*str)
      {
         nextdig = *str++;
         if ((nextdig <= 'f') && (nextdig >= 'a'))
            test = nextdig - 'a' + 10;
         else if ((nextdig <= 'F') && (nextdig >= 'A'))
            test = nextdig - 'A' + 10;
         else if ((nextdig <= '9') && (nextdig >= '0'))
            test = nextdig - '0';
         else {
            yyerror("invalid hexadecimal digit");
            return(0);
         } 
         tmp = 16*tmp + test;
      }
      break;
   }  /* switch */
   return (tmp);
}  /*end of strtoi */


int store_string(char * str, int* sx)
   /* store the string 'str' in stab, if it's not already there, and */
   /* update the global index into stab                              */
   /* returns index in stab[] where the string was either found or   */
   /* was stored                                                     */
{

   char buf[512];
   extern char laststring[];
   int t;
   int slen, oldsx, newsx;
   int i;
   int len = strlen(str);
   /* first ASCII-ize the string */
   for (i = 0; i< len; i++)
      if (!isascii(str[i])) str[i] = '?';
   if (*sx > 0) {
      t = 0;
      while (t < *sx) {
         if (strcmp(str,&stab[t]) == 0)
            return t;
         while ((t < *sx)&&(stab[t] != '\0')) t++;
         t++;
      }
   }   
   /* it wasn't there -- must add it */
   slen = strlen(str);
   oldsx = *sx;
   newsx = *sx + slen + 1;
   if (newsx <= SMAX)
      strcpy(&stab[*sx],laststring);
   else
      cfatal("String table space exhausted");
   *sx = newsx;
   return oldsx; 
}

void yyerror(char *msg)
{
   extern char lasttoken[];

   char *p = ((lasttoken[0] == '\0') ? "???" : lasttoken);
   int ci = curr_infile;
   if (ci < 0) ci = 0;
   errcount++;
    if(msg==NULL || *msg=='\0'){
      fprintf(stderr, "Error near '%s', line %d of file %s\n", p,
         infile[ci].line_no, infile[ci].fname);
      fprintf(yyout, "Error near '%s', line %d of file %s\n", p,
         infile[curr_infile].line_no, infile[curr_infile].fname);
   }
   else {
      fprintf(stderr, "Error near '%s', line %d of file %s:\n**   %s\n", p,
         infile[ci].line_no, infile[ci].fname, msg);
      fprintf(yyout,  "Error near '%s', line %d of file %s:\n**   %s \n", p,
         infile[ci].line_no, infile[ci].fname, msg);
   }
   if (errcount >= MAXERROR)
      cfatal("Compilation aborted.  Too many (%d) errors.",MAXERROR);
}
   
int yywrap()
{
   return 1;
}

void check_unresolved_externs()
{
   extern int last_tab;
   extern void yyerror();
   int ix;
   char buf[256];
   char name[30];
   char what[30];
   name[0] = '\0';
   for(ix = 0; ix <= last_tab; ix++) {
      switch (tab[ix].obj) {
         case ext_procedure:
            strcpy(name,tab[ix].name);
            strcpy(what,"void function");
            break;
         case ext_function:
            strcpy(name,tab[ix].name);
            strcpy(what,"function");
            break;
         case ext_variable:
            strcpy(name,tab[ix].name);
            strcpy(what,"variable");
            break;
         case ext_monitor:
            strcpy(name,tab[ix].name);
            strcpy(what,"monitor");
            break;
         default:  /* nothing */ ;
      }
      if (name[0]) {
         sprintf(buf,"Source file contains unresolved external %s: %s\n",
            what,name);
         yyerror(buf);
         name[0] = '\0';
      }
   }     
}  /* check_unresolved_externs */


   /* code used only by the BACI compilers to write the BACI object files */

extern int fclose(FILE*);
extern int fputc(int,FILE*);
extern int fprintf(FILE*,const char*,...);

static void  write_identifier_table(FILE *f)
   /* used only by the two BACI compilers to write the tab[] array */
   /* only the non-predeclared id's are saved                      */
{
   extern int last_tab;
   extern int last_predeclared;
   extern TAB_STRUCT tab[];
   int   ix, jx;
   int   link;
   int   last = last_tab - last_predeclared;
   fprintf(f,"1  %d            IDENTIFIER table\n",last);
   fprintf(f,"index   identifier    link  obj  type  ref normal   lev\
   adr   mon atomic\n");
   /* tab[0] is used as the 'sentinel' value for some searches */
   for (ix = 1; ix <= last; ix++) {
      jx = ix + last_predeclared;
      link = (tab[jx].link > 0 ? tab[jx].link - last_predeclared : 0);
      fprintf(f,"%4d  %-12s  %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
         ix,tab[jx].name,link,tab[jx].obj,tab[jx].typ,
         tab[jx].ref,tab[jx].normal,tab[jx].lev,tab[jx].adr,tab[jx].mon,
         tab[jx].atomic);
   }
}  /* write_identifier_table */ 
 
static void  write_block_table(FILE *f)
   /* Used only by the two BACI compilers to write the btab[] array  */
   /* The last and lastpar fields have to be adjusted because only   */
   /* the non-predeclared id's are saved in the tab array            */
{
   int   ix;
   int   last, lastpar;
   extern int last_predeclared;
   extern int last_btab;
   fprintf(f,"0  %d         BLOCK table\n",last_btab);
   fprintf(f,"index last lastpar psize vsize\n");
/*          1234567890123456789012345678901234567890  */
   for (ix = 0; ix <= last_btab; ix++) {
      if (btab[ix].last < btab[ix].lastpar)  /* assert: last >= lastpar */
         btab[ix].last = btab[ix].lastpar;
      last = (btab[ix].last > last_predeclared ? 
             btab[ix].last - last_predeclared : btab[ix].last);
      lastpar = (btab[ix].lastpar > last_predeclared ? 
             btab[ix].lastpar - last_predeclared : btab[ix].lastpar);
      fprintf(f,"%4d %4d %7d %4d %4d\n",ix,
         last,lastpar,btab[ix].psize,btab[ix].vsize);
   }
}  /* write_block_table */


static void write_pcode(char *pcode_fname, char *comp_pname, char *filename_line)
   /* 
      Used by the BACI compilers to 
      write pcode & tables to pcode file named by 'pcode_fname'
      First line of the pcode file is the name of the compiler
      Second line is the name of the file being compiled
   */
{
   FILE  *pcode;
   int lastlc = lc - 1;
   extern int last_infile;
   extern InputFile *infile;
   
   extern void exit(int);
   if ((pcode = fopen(pcode_fname,"w")) == NULL) {
      fprintf(stderr,"Can't open pcode file %s\n",pcode_fname);
      exit(2);
   }
   fprintf(pcode,"%s\n",comp_pname);
   fprintf(pcode,"%s",filename_line); /* filename_line has an \n already */
   write_code(pcode,code,lastlc);
   write_identifier_table(pcode);
   write_block_table(pcode);
   write_atab(pcode,atab,last_atab);
   write_stab(pcode,stab,stab_size);
   write_infile(pcode,infile,last_infile);
   if (dbg[last_dbg].lc > lastlc) dbg[last_dbg].lc = lastlc;
   write_dbg(pcode,dbg,last_dbg);
   fprintf(stderr,"Pcode and tables are stored in %s\n",pcode_fname);
   fclose(pcode);
}  /*  of write_pcode */ 

void check_expr_list()
{
   extern int last_predeclared;
   int i;
   printf("List of in-use exprs\n");
   for (i = 0; i < EXPRSIZE; i++)
      if (!expr[i].free) {
         printf("Expression %d in use:  obj %s  type %s  tix %d name %s\n",
            i,objnames[expr[i].obj],typenames[expr[i].typ],
            expr[i].tix-last_predeclared,
            tab[expr[i].tix].name);
         printf("    adr %x   ref %d  lev %d  arelt %d  isval %d  result_used %d\n",
            expr[i].adr,expr[i].ref,expr[i].lev,expr[i].arelt,expr[i].isval,
            expr[i].result_used);
      }
}

  /* main pgm for the BenAri Compilers */

int main(int argc,char **argv)
{
   int   retval = 0;
   extern int yyparse(void);
   extern int making_objfile;
   extern void global_init(int,char**);

   global_init(argc,argv);
   yyparse();
   if (!making_objfile) check_unresolved_externs();
   if (errcount > 0) {
      retval = 1;
      code[tab[btab[0].lastpar].adr].f = HALT;  /* render PCODE harmless */
      if (errcount == 1) {
         fprintf(stderr,"Because of 1 error");
         fprintf(yyout,"Because of 1 error");
      }
      else {
         fprintf(stderr,"Because of %d errors,", errcount);
         fprintf(yyout,"Because of %d errors,", errcount);
      }
      fprintf(stderr," the PCODE file will not execute\n");
      fprintf(yyout," the PCODE file will not execute\n");
   }
   write_pcode(pcode_fname,comp_pname,filename_line);
   fprintf(stderr,"Compilation listing is stored in %s\n",list_fname);
   /*check_expr_list(); uncomment to check for non-free expr's */
   return(retval);
}

/*
 *
 *  $Id: computil.c,v 1.23 2007/06/01 17:49:03 bynum Exp $
 *
 */
