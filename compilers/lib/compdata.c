      /* BenAri Concurrent PCODE Compiler data */

#include "../include/bacicnty.h"    /* bring in constants & types */

FNAME_STRING   comp_pname;
FNAME_STRING   source_fname;
FNAME_STRING   list_fname;

int   last_predeclared;
   /* tab index of last predeclared id */

   /* The following initializer forces the linker to load compdata.o   */
   /* from the library when creating the executable.  This ensure that */
   /* the symbol table information is present for these variables.     */
int   display[LMAX]  = { 0 };
int   mtab[MONSIZE];
int   mon;


int   errcount;   /* count of parsing errors */
int   first_parmcterr;
int   first_stringerr;

/* for the expression subtrees of the parsetree */
EXPR  expr[EXPRSIZE];
int   expr_cnt;

/*
 *
 *  $Id: compdata.c,v 1.10 2007/06/01 17:49:03 bynum Exp $
 *
 */
