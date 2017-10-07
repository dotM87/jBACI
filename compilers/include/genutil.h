   /* General utility routines for the BenAri Concurrent PCODE programs */


extern char *savestr(char *s);
   /* copy a string s to new space & return a pointer to it */

void fatal(const char *fmt, ...);
   /* fmt is the format string                        */
   /* ... is the list of things to be printed         */
   /* output is written to stderr, then exit          */
   /* 'cfatal' in computil.c writes to stderr & yyout */

void nonfatal(const char *fmt, ...);
   /* fmt is the format string                        */
   /* ... is the list of things to be printed         */
   /* warning output is written to stderr, no exit    */

int stacksize(int bytesize);
/* convert a size in bytes to number of BACI interpreter stack words */

/*
 *
 *  $Id: genutil.h,v 1.5 2007/06/01 17:41:16 bynum Exp $
 *
 */
