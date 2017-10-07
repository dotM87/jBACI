   /* code for reading in the BenAri global data structures */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/globdata.h"
#include "../include/globtabs.h"
#include "../include/genutil.h"

extern void exit(int);
extern int fprintf(FILE*,const char*,...);
extern int fgetc(FILE*);
extern int fclose(FILE*);
extern int sscanf(const char*,const char*,...);

int last_infile;        /* globals used by read_entire_pcode_file */
InputFile* inputfile;

static BUFFER buf;

static void index_error(char *proc, int expected, int found)
{
   fprintf(stderr,"\n%s:  index mismatch  %d expected   %d found\n",
      proc,expected,found);
   exit(1);
}  /* index_error */

int read_code_table(FILE *f, ORDER code[])
   /* reads code[] array and returns the last index used */
{
int   tmp, ix, org_lc, last_lc;
   fgets(buf,MAX_FNAME,f);
   sscanf(buf," %d %d",&org_lc,&last_lc);
   if (org_lc != 0) index_error("read_code_table",0,org_lc); 
   if (last_lc > CMAX) index_error("read_code_table",CMAX,last_lc);
   fgets(buf,MAX_FNAME,f); /* header line */
   for (ix = org_lc; ix <= last_lc; ix++) {
      fgets(buf,MAX_FNAME,f);
      sscanf(buf," %d %d %d %d",&tmp,&code[ix].f,&code[ix].x,&code[ix].y);
      if (tmp != ix) index_error("read_code_table",tmp,ix);
   }
   return last_lc;
}  /* read_code_table */

void skip_code_table(FILE *f)
   /* skips the code[] array */
{
int   tmp, ix, org_lc, last_lc, f1, f2, f3;
   fgets(buf,MAX_FNAME,f);
   sscanf(buf," %d %d",&org_lc,&last_lc);
   if (org_lc != 0) index_error("skip_code_table",0,org_lc); 
   if (last_lc > CMAX) index_error("skip_code_table",CMAX,last_lc);
   fgets(buf,MAX_FNAME,f); /* header line */
   for (ix = org_lc; ix <= last_lc; ix++) {
      fgets(buf,MAX_FNAME,f);
      sscanf(buf," %d %d %d %d",&tmp,&f1,&f2,&f3);
      if (tmp != ix) index_error("skip_code_table",tmp,ix);
   }
}  /* skip_code_table */

int read_identifier_table(FILE *f, TAB_STRUCT tab[])
   /* reads tab[] array and returns the last index used */
{
int   tmp, ix, first_ix, last_ix;
   fgets(buf,MAX_FNAME,f);
   sscanf(buf," %d %d",&first_ix,&last_ix);
   /* the interpreter doesn't use tab[0], but the compiler DOES    */
   if (first_ix != 1) index_error("read_identifier_table",1,first_ix); 
   if (last_ix > TMAX) index_error("read_identifier_table",TMAX,last_ix);
   fgets(buf,MAX_FNAME,f); /* header line */
   for (ix = 1; ix <= last_ix; ix++) {
      fgets(buf,MAX_FNAME,f);
      sscanf(buf," %d %s %d %d %d %d %d %d %d %d %d",
         &tmp,(char *)&tab[ix].name,&tab[ix].link,(int *)&tab[ix].obj,
         (int *) &tab[ix].typ,&tab[ix].ref,&tab[ix].normal,
         &tab[ix].lev,&tab[ix].adr,&tab[ix].mon, &tab[ix].atomic);
      if (tmp != ix) index_error("read_identifier_table",tmp,ix);
      if (tab[ix].name[0] == '\0'){
         fprintf(stderr,"read_identifier_table:  empty id, index = %d\n", ix);
         exit(1);
      }
   } /* for */
   return last_ix;
}  /* read_identifier_table */

int read_block_table(FILE *f, BTAB_STRUCT btab[])
   /* reads btab[] array and returns the last index used */
{
int   tmp, ix, first_b, last_b;
   fgets(buf,MAX_FNAME,f);
   sscanf(buf," %d %d",&first_b,&last_b);
   if (first_b != 0) index_error("read_block_table",0,first_b); 
   if (last_b > BMAX) index_error("read_block_table",BMAX,last_b);
   fgets(buf,MAX_FNAME,f); /* header line */
   for (ix = 0; ix <= last_b; ix++) {
      fgets(buf,MAX_FNAME,f); /* header line */
      sscanf(buf," %d %d %d %d %d",&tmp,&btab[ix].last,
         &btab[ix].lastpar,&btab[ix].psize,&btab[ix].vsize);
      if (tmp != ix) index_error("read_block_table",tmp,ix);
   } /* for */
   return last_b;
}  /* read_block_table */

int read_array_table(FILE *f, ATAB_STRUCT atab[])
   /* reads atab[] array and returns the last index used */
{
int   tmp, ix, first_a, last_a;
   fgets(buf,MAX_FNAME,f); 
   sscanf(buf," %d %d",&first_a,&last_a);
   if (first_a != 0) index_error("read_array_table",0,first_a); 
   if (last_a > AMAX) index_error("read_array_table",AMAX,last_a);
   fgets(buf,MAX_FNAME,f); /* header line */
   for (ix = 0; ix <= last_a; ix++) {
      fgets(buf,MAX_FNAME,f); 
      sscanf(buf," %d %d %d %d %d %d %d %d",&tmp,(int *)&atab[ix].inxtyp,
         (int *)&atab[ix].eltyp,&atab[ix].elref,&atab[ix].low,&atab[ix].high,
         &atab[ix].elsize,&atab[ix].size);
      if (tmp != ix) index_error("read_array_table",tmp,ix);
   } /* for */
   return last_a;
}  /* read_array_table */

int read_string_table(FILE *f, char stab[])
   /* reads stab[] array and returns the size of the array */
{
char c;
int   ix, stop_sx, stop_ix, first_sx, gulpsize;
   fgets(buf,MAX_FNAME,f);
   sscanf(buf," %d %d %d",&first_sx,&stop_sx,&gulpsize); 
   if (first_sx != 0) index_error("read_string_table",0,first_sx); 
   if (stop_sx > SSIZE) index_error("read_string_table",SSIZE,stop_sx);
   if (stop_sx == 0) return 0;
   /* write_string_table() adds a newline '\n' character after each */
   /* group of 'gulpsize' chars.  Only these newlines have to be    */
   /* removed. Any other newlines in stab must be kept.             */
   c = fgetc(f);
   for (ix = 0; ix < stop_sx; ) {
      stop_ix = ix + gulpsize;
      if (stop_ix > stop_sx) stop_ix = stop_sx;
      while (ix < stop_ix) {
         stab[ix++] = c;
         c = fgetc(f);
      }
#if defined(DOS)
      if (c == 0x0d) c = fgetc(f);
      if ((ix < stop_sx) &&(c == 0x0a)) c = fgetc(f);
#else
      if ((ix < stop_sx) &&(c == '\n')) c = fgetc(f);
#endif
      if (ix != stop_ix) 
         index_error("read_string_table",stop_ix,ix);
   }   
   return stop_sx;
}  /* read_string_table */

InputFile* alloc_infile(int last_infile)
   /* If last_infile >= 0, then callocs space for an array of     */
   /* type InputFile with last_infile + 1 members and returns a   */ 
   /* pointer to it.                                              */
   /* If last_infile < 0, then returns NULL                       */
{
   InputFile* tmp;
   int infile_size = last_infile + 1; 
   if (infile_size <= 0) 
      tmp = (InputFile*) NULL;
   else {
      tmp = (InputFile *) calloc((infile_size), sizeof(InputFile));
      if (tmp == NULL) 
         fatal("Can't get space for infile array");
   }
   return tmp;
}  /* alloc_infile */

int read_file_info(FILE* f, InputFile** inputfile)
   /*  allocates space for the inputfile[] array, reads in the data, and */
   /*  returns the last index used */
{
   InputFile* tmp;
   char tstring[256];
   int ix, dummy, first_infile, last_infile;
   fgets(buf,MAX_FNAME,f); 
   sscanf(buf," %d %d",&first_infile,&last_infile);
   if (first_infile != 0) index_error("read_file_info",0,first_infile);
   *inputfile = tmp = alloc_infile(last_infile);
   fgets(buf,MAX_FNAME,f); /* header line */
   for (ix = 0; ix <= last_infile; ix++) {
      fgets(buf,MAX_FNAME,f);
      sscanf(buf," %d %d %s ", &dummy, &tmp[ix].parent,tstring);
      if (dummy != ix) index_error("read_file_info",ix,dummy);
      tmp[ix].fname = savestr(tstring);
      tmp[ix].line_no = 0;
      tmp[ix].f = NULL;
   }
   return last_infile;
}  /* read_file_info */


int read_debug_info(FILE* f, PCDEBUG dbg[])
   /* reads the dbg[] array and returns the last index used */
{
   int   ix, org_lc, last_lc;
   fgets(buf,MAX_FNAME,f);
   sscanf(buf," %d %d",&org_lc,&last_lc);
   if (org_lc != 0) index_error("read_debug_info",0,org_lc); 
   if (last_lc > CMAX) index_error("read_debug_info",CMAX,last_lc);
   fgets(buf,MAX_FNAME,f); /* header line */
   for (ix = org_lc; ix <= last_lc; ix++) {
      fgets(buf,MAX_FNAME,f);
      sscanf(buf," %d %d %d",&dbg[ix].lc,&dbg[ix].fix,&dbg[ix].flno);
   }
   return last_lc;
}  /* read_debug_info */

void check_header(FILE* f)
   /* check header line of a .pob or .pco file */
{
   FNAME_STRING   header;
   fgets(header,MAX_FNAME,f);
   if (strspn(comp_proot,header) != strlen(comp_proot)) {
      fprintf(stderr,"Bad PCODE file header\n");
      fprintf(stderr,"expected: %s\n",comp_proot);
      fprintf(stderr,"found   : %s",header);
      exit(1);
   }
}

void read_pcode_file(FILE *f)
   /* reads the .pco file except for the inputfile[] and dbg[] arrays */
{
   check_header(f);
   fgets(filename_line,MAX_FNAME,f);
   last_code = read_code_table(f,code);
   last_tab = read_identifier_table(f,tab);
   last_btab = read_block_table(f,btab);
   last_atab = read_array_table(f,atab);
   stab_size = read_string_table(f,stab);
   fclose(f);
}  /* read_pcode_file */

void read_entire_pcode_file(FILE *f)
   /* reads the entire .pco file */
{
   check_header(f);
   fgets(filename_line,MAX_FNAME,f);
   last_code = read_code_table(f,code);
   last_tab = read_identifier_table(f,tab);
   last_btab = read_block_table(f,btab);
   last_atab = read_array_table(f,atab);
   stab_size = read_string_table(f,stab);
   last_infile = read_file_info(f,&inputfile);
   last_dbg = read_debug_info(f,dbg);
   fclose(f);
}  /* read_entire_pcode_file */


/*
 *
 *  $Id: readtab.c,v 1.16 2007/06/01 17:49:03 bynum Exp $
 *
 */
