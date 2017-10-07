/*
 * BenAri Concurrent PCODE system
 * Code to handle include files
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/bacicnty.h"
#include "../include/genutil.h"
#include "../include/computil.h"

extern FILE* yyin;

extern char *yy_create_buffer(FILE*,int);
extern void yy_switch_to_buffer(void *);
extern void yy_delete_buffer(void *);
extern int fclose(FILE*);

InputFile *infile = NULL;     /* the array */

static int   num_infiles = 0;

int   curr_infile = -1;                /* original file has index 0 */
extern int last_infile;                /* original file has index 0 */
   /* last_infile never decreases, curr_infile floats up and down   */
   /* Declared in readtab.c                                         */

int   include_level = -1;     /* original file is at level 0 */

#define MORE_INFILES 20       /* # add'l array infile entries realloc-ed */

#define FLEXBUF_SIZE 256


extern int last_dbg;
extern int lc;
extern PCDEBUG dbg[];

static int new_infile()
{
InputFile *tmp;
int new_numinfiles;
extern void* memset(void *,int,size_t);
   if (infile == NULL) {
      num_infiles = MORE_INFILES;
      tmp = (InputFile *) calloc(num_infiles, sizeof(InputFile));
      if (tmp == (InputFile *) NULL)
         fatal("new_infile: realloc failed -- out of memory");
      infile = tmp;
      return (last_infile = 0);
   }
   last_infile++;
   if (last_infile == num_infiles){ /* allocate more space */
      new_numinfiles = num_infiles + MORE_INFILES;
      tmp = (InputFile *) realloc(infile, new_numinfiles*sizeof(InputFile));
      if (tmp == (InputFile *) NULL)
         fatal("new_infile: realloc failed -- out of memory");
      memset((char *)&tmp[num_infiles],0,MORE_INFILES*sizeof(InputFile));
      free(infile);
      infile = tmp;
      num_infiles = new_numinfiles;
   }
   return last_infile;
} /* new_infile */

int open_infile(FNAME_STRING fname,int curr_inf)
{
FILE *tf;
int tcif = curr_inf;
   if (tcif >= 0) {
      last_dbg++;
      dbg[last_dbg].flno = infile[tcif].line_no;
      dbg[last_dbg].fix = tcif;
      dbg[last_dbg].lc = lc;
      infile[tcif].last_line_no = infile[tcif].line_no;
   }
   tf = fopen(fname,"r");
   if (tf== NULL) fatal("Can't open file %s",fname);
   curr_inf = new_infile();
   infile[curr_inf].fname = savestr(fname);
   infile[curr_inf].line_no = 0;
   infile[curr_inf].parent = tcif;
   yyin = infile[curr_inf].f = tf;
   infile[curr_inf].flexbuf = (void *) yy_create_buffer(yyin,FLEXBUF_SIZE);
   yy_switch_to_buffer(infile[curr_inf].flexbuf);
   include_level++;
   return curr_inf;
}  /* open_infile */

int close_infile(int curr_inf)
{
int parent;   
   if (curr_inf < 0) return curr_inf;
   last_dbg++;
   dbg[last_dbg].flno = - infile[curr_inf].line_no; /* close event */
   dbg[last_dbg].fix = curr_inf;
   dbg[last_dbg].lc = lc;
   fclose(infile[curr_inf].f);
   yy_delete_buffer(infile[curr_inf].flexbuf);
   parent = infile[curr_inf].parent;
   if (parent >= 0) 
   {
      yy_switch_to_buffer(infile[parent].flexbuf);
      yyin = infile[parent].f;
   }
   include_level--;
   return parent;
}  /* close_infile */
  
/*
 *
 *  $Id: incfiles.c,v 1.14 2007/06/01 17:49:03 bynum Exp $
 *
 */
