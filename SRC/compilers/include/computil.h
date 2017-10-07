   /* Utility routines for the BenAri Concurrent PCODE compilers */

extern void cfatal(const char* fmt,...);
   /* prints an error message on stderr & yyout and exits the program */
   /* fmt is the printf-type format string                            */ 
   /* renamed to 'cfatal' to avoid name clash with 'fatal' in         */
   /* genutil.c                                                       */

extern void cnonfatal(const char* fmt,...);
   /* prints an error message on stderr & yyout and continues         */
   /* fmt is the printf-type format string                            */ 
   /* renamed to 'cnonfatal' to avoid name clash with 'nonfatal' in   */
   /* genutil.c                                                       */

extern void enterstid(char *thisid, int thisobj, TYPES thistyp, int thisadr);
   /* enter 'standard' id into symbol table without a duplicate check */

extern int enter(char *id,OBJTYPE obj,int level,int vislevel);
  /* search for enter "id" name at static level "vislevel". If found,    */
  /* declare duplicate id & return 0.  If not found, then place id at    */
  /* "vislevel" level, set the "lev" field to "level" (id's REAL level)  */
  /* and return tab index of "id".  This level manipulation is used to   */
  /* make monitor procs (and nothing else ) visible from the outer scope */
          
extern int loc(char *id,int level);
   /* searches for 'id' from block 'level' out to level 0      */
   /* expecting to find it.  return tab index of id if found,  */
   /* else declare error & return 0                            */

extern void enter_block(int *b, int *level, int t);
   /* enter new code block and make a record of it in btab */

extern void leave_block(int *level);
   /* leave current code block and go back to the next level out */

extern void emit(int fct);
   /* create a new pcode instruction, (fct,0,0) */

extern void emit1(int fct,int b);
   /* create a new pcode instruction  (fct,0,b) */

extern void emit2(int fct,int a,int b);
   /* create a new pcode instruction  (fct,a,b) */

extern int get_constant(char *id, int level, TYPES *ctype);
   /* look for a constant at the specified level                   */
   /* return the value of it, if found, else make an error message */

extern int new_expr();
   /* rehabilitate an entry of the 'expr' array to hold an expression */
   /* being parsed and return its index                               */  

extern void free_expr(int i);
   /* free entry 'i' of the 'expr' array */

extern void index_expr(int eix,int *aref,TYPES *eltyp);
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

extern void gen_loadvar(int ex);
/*
   emits correct LOAD instruction, depending on what
   sort of variable expr[ex] is
*/

extern void gen_exprval(int ex);
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

extern void proc_parm(int px,int ex,int pct);
   /* used in parser to check parms for a regular call */
   /* 'px'    tab index of this proc or func           */
   /* 'expr'  index of the parm                        */
   /* 'pct'   current count of parms                   */     

extern void stdproc_parm(int px, int ex, int pct);
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

extern void stdproc_pcode(int px, int pct);
   /* used in parser to check parms for a call of a 'standard' proc     */
   /* 'px'      tab index of this proc or func                          */
   /* 'pct'     current count of parms                                  */
   /* The PCODE for every standard proc or function is emitted here,    */
   /* with the exception of the READs and WRITEs that are required for  */
   /* the parameters of READ/READLN or WRITE/WRITELN procs.             */    

extern TYPES resulttype(TYPES a, TYPES b);
   /* used for checking the result type of a binary operator */

extern unsigned int strtoi(char *str, int base);
/*
   interprets 'str' string as an unsigned integer in the base 'base'
   for 'base' == 2, 8, 10, or 16
   no overflow check
   complains and returns 0 if invalid digit is seen
*/

extern int store_string(char * str, int* sx);
   /* store the string 'str' in stab, if it's not already there, and */
   /* update the global index into stab                              */
   /* returns index in stab[] where the string was either found or   */
   /* was stored                                                     */

extern void yyerror(char* msg);
   /* write 'msg' to stderr and yyout and increment the error count  */

extern int main(int argc, char** argv);
   /* main program for the BACI compilers */

/*
 *
 *  $Id: computil.h,v 1.9 2007/06/01 17:41:16 bynum Exp $
 *
 */
