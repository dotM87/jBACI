   /* general utility programs for the BenAri Concurrent PCODE sytem */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern FILE *yyout;
extern void exit(int);
extern int fprintf(FILE*,const char*,...);
extern int vfprintf(FILE  *stream, const char *format, va_list ap);

char *savestr(char *s)
   /* copy a string s to new space & return a pointer to it */
{
char *t;
   t = (char *) malloc(1+strlen(s));
   if (t == (char *) 0) {
      fprintf(stderr,"savestr: malloc failed -- out of memory\n");
      exit(1);
   }
   strcpy(t,s);
   return(t);
}

void fatal(const char *fmt, ...)
   /* fmt is the format string                        */
   /* ... is the list of things to be printed         */
   /* error output is written to stderr, then exits   */
   /* 'cfatal' in computil.c writes to stderr & yyout */
{
   va_list ap;

   fprintf(stderr, "** Error: ");
   va_start(ap,fmt);
   vfprintf(stderr,fmt,ap);
   va_end(ap);
   fprintf(stderr,"\n");
   exit(1);
}  /* fatal */


void nonfatal(const char *fmt, ...)
   /* fmt is the format string                        */
   /* ... is the list of things to be printed         */
   /* warning output is written to stderr, no exit    */
{
   va_list ap;

   fprintf(stderr, "++ Warning: ");
   va_start(ap,fmt);
   vfprintf(stderr,fmt,ap);
   va_end(ap);
   fprintf(stderr,"\n");
}  /* nonfatal */


int stacksize(int bytesize)
/* convert a size in bytes to number of BACI interpreter stack words */
{
   int words = 0;
   int bsize = bytesize;
   while (bsize > 0) {
      words++;
      bsize -= sizeof(int);
   }
   return words;
}  /* stacksize */

/*
 *
 *  $Id: genutil.c,v 1.10 2007/06/01 17:49:03 bynum Exp $
 *
 */
