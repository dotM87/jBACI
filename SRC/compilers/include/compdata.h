   /* Data structures for the BenAri Concurrent PCODE compilers */


extern FNAME_STRING   comp_pname;
extern FNAME_STRING   source_fname;
extern FNAME_STRING   list_fname;

extern char  source_suffix[];
extern char  list_suffix[];

extern int   last_predeclared;
   /* tab index of last predeclared id */

extern int   display[];

extern int   mtab[];
extern int   mon;

extern FILE  *yyin;                 /* supplied by lex.c */
extern FILE  *yyout;                /* supplied by lex.c */

extern int   errcount;
extern int   first_stringerr;
extern int   first_parmcterr;

extern EXPR  expr[];
extern int   expr_cnt;

/*
 *
 *  $Id: compdata.h,v 1.5 2007/06/01 17:41:16 bynum Exp $
 *
 */
