   /* code to write the BenAri data tables */

#include <stdio.h>
#include "../include/globdata.h"    /* most externs declared here */
#include "../include/globtabs.h"    /* global tables declared here */
#include "../include/pcode.h"


extern int fclose(FILE*);
extern int fputc(int,FILE*);
extern int fprintf(FILE*,const char*,...);

void write_code(FILE *fl, ORDER code[], int last_code)
   /* used by all BACI programs to write the code[] array */
{
   int   ix;
   fprintf(fl,"0  %d                 PCODE table\n",last_code);
   fprintf(fl," lc   f    x    y\n");
   for (ix = 0; ix <= last_code; ix++)
         fprintf(fl,"%3d %3d %4d %4d\n",ix,code[ix].f,code[ix].x,code[ix].y);
}  /* write_code */ 

void  write_tab(FILE *f, TAB_STRUCT tab[], int last_tab)
   /* used by all BACI programs other than two compilers to write
      the tab[] array */
{
   int   ix;
   fprintf(f,"1  %d            IDENTIFIER table\n",last_tab);
   fprintf(f,"index   identifier    link  obj  type  ref normal   lev\
   adr   mon atomic\n");
   /* tab[0] is used as the 'sentinel' value for some searches */
   for (ix = 1; ix <= last_tab; ix++) {
      fprintf(f,"%4d  %-12s  %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
         ix,tab[ix].name,tab[ix].link,tab[ix].obj,tab[ix].typ,
         tab[ix].ref,tab[ix].normal,tab[ix].lev,tab[ix].adr,tab[ix].mon,
         tab[ix].atomic);
   }
}  /* write_tab */ 
 
void  write_btab(FILE *f, BTAB_STRUCT btab[], int last_btab)
   /* used by all BACI programs orhter than the two compilers 
      to write the btab[] array */
{
   int   ix;
   fprintf(f,"0  %d         BLOCK table\n",last_btab);
   fprintf(f,"index last lastpar psize vsize\n");
/*          1234567890123456789012345678901234567890  */
   for (ix = 0; ix <= last_btab; ix++) {
      fprintf(f,"%4d %4d %7d %4d %4d\n",ix,
         btab[ix].last,btab[ix].lastpar,btab[ix].psize,btab[ix].vsize);
   }
}  /* write_btab */


void write_atab(FILE *f, ATAB_STRUCT atab[], int last_atab)
   /* used by all BACI programs to write the atab[] array */
{
   int   ix;
   fprintf(f,"0  %d           ARRAY table\n",last_atab);
      /*      12345678901234567890123456789012345678901234567890123456789012345678901234567890 */
   fprintf(f,"index inxtype eltyp elref  low  high  elsize  size\n");
   for (ix = 0; ix <= last_atab; ix++)
      fprintf(f," %3d   %3d   %3d  %5d %5d %5d %5d %5d\n",
          ix,atab[ix].inxtyp,atab[ix].eltyp,atab[ix].elref,atab[ix].low,
          atab[ix].high,atab[ix].elsize,atab[ix].size);
}  /* write_atab */
  
void write_stab(FILE *f, char stab[], int stab_size)
   /* used by all BACI programs to write the stab[] array */
{
   int start,stop;
   fprintf(f,"0 %3d %3d                  STRING table\n",stab_size,GULPSIZE);
   start = 0;
   if (stab_size > 0)
      do { 
         stop = start + GULPSIZE;
         if (stop > stab_size) stop = stab_size;
         while (start < stop)
            fputc(stab[start++],f);
         fputc('\n',f);
      } while (stop != stab_size);
}  /* write_stab */
   
void write_dbg(FILE *f, PCDEBUG dbg[], int last_dbg)
   /* used by all BACI programs to write the dbg[] array */
{
   int ix;
   fprintf(f,"0  %d           PCODE debugging information\n",last_dbg);
      /*      12345678901234567890123456789012345678901234567890123456789012345678901234567890 */
   fprintf(f,"   lc findex flineno\n");
   for (ix = 0; ix <= last_dbg; ix++)
      fprintf(f," %3d   %3d   %3d\n",dbg[ix].lc,dbg[ix].fix,dbg[ix].flno);
}  /* write_dbg */


void write_infile(FILE *f, InputFile infile[], int last_infile)
   /* used by all BACI programs to write the infile[] array */
{
   int   ix;
   fprintf(f,"0  %d           Input File array\n",last_infile);
      /*      123456789012345678901234567890 */
   fprintf(f,"index parent      file name\n");
   for (ix = 0; ix <= last_infile; ix++)
      fprintf(f," %3d   %3d   %s\n",ix,infile[ix].parent,infile[ix].fname);
}  /* write_infile */


/*
 *
 *  $Id: writetab.c,v 1.13 2007/06/01 17:49:03 bynum Exp $
 *
 */
