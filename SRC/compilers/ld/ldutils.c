/*Scott Mitchell */
/*CS710 Linker Project */
/*7 August 2000*/
/*ld_untils.c source file */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/bacicnty.h"
#include "../include/pcode.h"
#include "../include/readtab.h"
#include "../include/writetab.h"
#include "../include/globdata.h"
#include "../include/genutil.h"
#include "ld.h"
#include "ldresolv.h"


/*Prcedure is to merge the two code tables from t1 and t2 into one
  code table stored in finalcodetable.  This is done by finding the
  outer blocks of each program in order to split the program into
  two sections.  This needs to be done because the non-outer block
  code for the 2nd program must be out into the finalcodetable BEFORE
  the outer block code for the 1st file is put in.  Once the dividing
  of each file is done, the final code table is built by copying the
  lines corresponding to the non-outer block code of file 1, followed
  by the non-outer block code of file 2, followed by the outer block
  code of file1, and then the outer block code of file 2 into the
  data structure finalcodetable.  Once this is done, the remaining
  tables from t1 are copied into tfinal.  Then the entries in the
  various tables of t2 are compared with those of t1 and appropriate
  adjustments are made*/

int block1; /*number of code lines in code array of 1st argument file
              excluding the outer block
              = t1->tab[1].adr, the addr of t1's outer block   */
int block2; /*number of code lines in the code  array of 2nd argument
              file excluding the outer block
              = t2->tab[1].adr, the addr of t2's outer block   */
int outer1; /*number of code lines in outer block of 1st argument file*/
   /* = t1->last_code - block1 + 1    */
int outer2; /*number of code lines in outer block of 2nd argument file*/
   /* = t2->last_code - block2 + 1    */


int newcode2_start;  /* addr in tfinal->code of start of t2->code */
   /* = block1 = t1->tab[1].adr, the addr of t1's outer block     */
int newcode2_stop;   /* 1 + addr in tfinal->code of end of t2->code */
   /* = block1 + block2                                             */
int newouter2_start; /* addr in tfinal->code of start of t2 outer block */
   /* = block1 + outer1 + block2 - 1 = t1->last_code + t1->tab[1].adr   */
int newouter2_stop; /* 1 + addr in tfinal->code of end of t2 outer block */
   /* = newouter2_start + outer2 = block1 + outer1 + block2 + outer2 -1  */
   /* = t1->last_code + t2->last_code + 1                                */ 

int mainproc_tix = -1;
   /* index in tfinal->tab of mainproc, when it is seen */
   /* remains -1 until this event                       */

extern int last_linkfile;
extern char* linkfile[];

   /* data structure for keeping record of symbol references */
RefData* R[TSIZE];

static Ref* newRef()
   /* create a new Ref struct, if possible, & return a ptr to it */
{
   Ref* tmp;
   if ((tmp = (Ref*) calloc(1,sizeof(Ref))) == (Ref*) 0)
      fatal("Can't allocate space for new Ref struct");
   tmp->ed = 'U';
   return tmp;
}  /* newRef */

static RefData* newRefData()
   /* create a new Ref struct, if possible, & return a ptr to it */
{
   RefData* tmp;
   if ((tmp = (RefData*) calloc(1,sizeof(RefData))) == (RefData*) 0)
      fatal("Can't allocate space for new RefData struct");
   return tmp;
}  /* newRefData */

void addRef(int tix, OBJTYPE obj, int lfix)
   /* add entry in the R[] array at tab index tix.  Set status field       */
   /* according to obj parm.  Set lfix field with lfix (linked file index  */
{
   RefData* d;
   Ref* r;
   if (R[tix] == (RefData*) 0) R[tix] = newRefData();
   d = R[tix];
   r = newRef();
   switch (obj) {
      case ext_variable:
      case ext_procedure:
      case ext_function:
      case ext_monitor:
         r->ed = 'E';
         if (d->status != DEFINED) d->status = UNRESEXT;
         break;
      default:
         r->ed = 'D';
         d->status = DEFINED;
   }  /* switch */
   r->lfix = lfix;
   if (d->tail) { /* a non-empty list */
      d->tail->next = r;
      d->tail = r;
   } 
   else { /* list is empty, start it */
      d->head = d->tail = r;
   }
}  /* addRef */
      
static void append_atab(ALL_TABLES* tfinal, int place, ALL_TABLES* t2, int ix)
   /* adds t2->atab[t2->tab[ix].ref] to tfinal->atab, if possible */
   /* array symbol owning the atab entry is at tfinal->tab[place] */
{
   int lastatab;
   int ref2;
   ATAB_STRUCT* af;
   ATAB_STRUCT* a2;
   if (tfinal->last_atab == AMAX) fatal("Array table size %d exceeded",ASIZE);
   ref2 = t2->tab[ix].ref;
   /*This is a simple copy if we are not dealing with an array of
     arrays*/
   if (tfinal->atab[tfinal->last_atab].eltyp != arrays) {
      lastatab = ++(tfinal->last_atab);
      af = &(tfinal->atab[lastatab]);
      a2 = &(t2->atab[ref2]);
      af->inxtyp = a2->inxtyp;
      af->eltyp  = a2->eltyp;
      af->elref  = a2->elref;
      af->high   = a2->high;
      af->low    = a2->low;
      af->elsize = a2->elsize;
      af->size   = a2->size;
      tfinal->tab[place].ref = lastatab;
   }
   else {
      /*Need to do something a little different if we are dealing
        with an array of arrays*/

       /*the elref field is updated first and all subsequent fields
         are updated based on this new index.  This prevents the wrong
         index into the array table being added to the ref field in the
         tab table for this entry*/
      tfinal->atab[tfinal->last_atab].elref = (tfinal->last_atab)++;
      lastatab = tfinal->last_atab;
      af = &(tfinal->atab[lastatab]);
      a2 = &(t2->atab[ref2]);
      af->inxtyp = a2->inxtyp;
      af->eltyp  = a2->eltyp;
      af->elref  = a2->elref;
      af->high   = a2->high;
      af->low    = a2->low;
      af->elsize = a2->elsize;
      af->size   = a2->size;
   }
}  /* append_atab */

static void append_btab(ALL_TABLES* tfinal, ALL_TABLES* t2, int place, int ix)
 /* adds t2->btab[ref] to tfinal->btab, if possible */
{
   BTAB_STRUCT* bf;
   BTAB_STRUCT* b2;
   int delta = place-ix;
   int lastbtab;
   if (tfinal->last_btab == BMAX) fatal("Block table size %d exceeded",BSIZE);
   lastbtab = ++(tfinal->last_btab);
   bf = &(tfinal->btab[lastbtab]);
   b2 = &(t2->btab[t2->tab[ix].ref]);
   bf->last    = b2->last + delta;
   bf->lastpar = b2->lastpar + delta;
   bf->psize   = b2->psize;
   bf->vsize   = b2->vsize;
}  /* append_btab */

char typname [12][10];

void check_atab_dimensions(ALL_TABLES* tfinal, ALL_TABLES* t2, 
                                             int place, int ix) {
    /* Compares the array dimensions of the two arrays */
   int dim = 0;
   int tfref = tfinal->tab[place].ref;
   int t2ref = t2->tab[ix].ref;
   ATAB_STRUCT* af;
   ATAB_STRUCT* a2;
   do {
      af = &(tfinal->atab[tfref]);
      a2 = &(t2->atab[t2ref]);
      if (af->size != a2->size) {
         fatal("Size mismatch in dimension %d:\narray %s of linkfile %s has size %d\nbut array %s of linkfile %s has size %d\n",
             dim, tfinal->tab[place].name,linkfile[R[place]->head->lfix],
                  af->size,
             t2->tab[ix].name,linkfile[last_linkfile],a2->size);
      }
      if (af->elsize != a2->elsize) {
         fatal("Element size mismatch in dimension %d:\narray %s of linkfile %s has size %d\nbut array %s of linkfile %s has size %d\n",
             dim, tfinal->tab[place].name,linkfile[R[place]->head->lfix],
                  af->elsize,
             t2->tab[ix].name,linkfile[last_linkfile],a2->elsize);
      }
      if (af->low != a2->low) {
         fatal("Low limit mismatch in dimension %d:\narray %s of linkfile %s has low %d\nbut array %s of linkfile %s has low %d\n",
             dim, tfinal->tab[place].name,linkfile[R[place]->head->lfix],
                  af->low,
             t2->tab[ix].name,linkfile[last_linkfile],a2->low);
      }
      if (af->high != a2->high) {
         fatal("High limit mismatch in dimension %d:\narray %s of linkfile %s has high %d\nbut array %s of linkfile %s has high %d\n",
             dim, tfinal->tab[place].name,linkfile[R[place]->head->lfix],
                  af->high,
             t2->tab[ix].name,linkfile[last_linkfile],a2->high);
      }
      if (af->eltyp != a2->eltyp) {
         fatal("Element type mismatch in dimension %d:\narray %s of linkfile %s\nand array %s of linkfile %s has type %s\nbut array %s of linkfile has type %s\n",
             dim, tfinal->tab[place].name,linkfile[R[place]->head->lfix],
             typname[af->eltyp],
             t2->tab[ix].name,linkfile[last_linkfile],typname[a2->eltyp]);
      }
      tfref = af->elref;
      t2ref = a2->elref;
      dim++;
   } while ((af->eltyp == arrays)&&(a2->eltyp == arrays));
}  /* end check_atab_dimensions */

void fix_atabindex(ALL_TABLES* tfinal, ALL_TABLES* t2, int place, int ix) {
    /* Adjusts tfinal->code to reflect changes in the indices of arrays */
    /* t2->code is never changed                                        */
   int i;
   int ref2 = t2->tab[ix].ref;
   for (i=0; i <= t2->last_code; i++) {
      if ((t2->code[i].f == INDEX) &&
          (t2->code[i].y == ref2)) {
         tfinal->code[i+newcode2_start].y = tfinal->tab[place].ref;
      }/*end if*/
   }/*end for*/
}/*end fix_atabindex*/

void read_tables (FILE* f, ALL_TABLES* t)
    /*reads all tables from stream f into ALL_TABLES struct */
    /*pointed to by t)                                      */
{
   fgets(t->header, MAX_FNAME, f);
   fgets(t->fnameline, MAX_FNAME, f);
   t->last_code = read_code_table(f, t->code);
   t->last_tab = read_identifier_table(f, t->tab);
   t->last_btab = read_block_table(f, t->btab);
   t->last_atab = read_array_table(f, t->atab);
   t->stab_size = read_string_table(f, t->stab);
   t->last_infile = read_file_info(f, &t->inputfile);
   t->last_dbg = read_debug_info(f, t->dbg);
   /* fclose removed, so that read_tables can be used to read directly */
   /* from an open archive file                                        */
}  /* end read_tables */

static void fix_shortcall(ALL_TABLES* tfinal, int mainproc_tix)
/* fix the shortcall instruction and set the entry pt of main into btab */
{
   int shortcallloc = tfinal->tab[mainproc_tix].adr;
   if (tfinal->code[shortcallloc].f != SHORTCALL) 
      fatal("fix_shortcall: Malformed main proc -- no SHORTCALL instr");
   else  /* insert entry pt of outer block in the SHORTCALL instr */
      tfinal->code[shortcallloc].y = tfinal->tab[1].adr;
      /* entry pt of outer block has already been fixed */
      /* bainterp expects lastpar entry of block table entry 
         for the outer block to be the tab index of the mainproc */
   tfinal->btab[0].lastpar = mainproc_tix;
}  /* fix_shortcall */
          
int search_tab(ALL_TABLES* tfinal, ALFA t2name)
    /* Searches the outer block for t2name.  Returns index i     */
    /* in tfinal->tab[] such that tfinal->tab[i].name == t2name  */
    /* if there is such an i; otherwise, returns 0               */
{
   int ix = tfinal->btab[0].last;
   while (ix) {
      if (strncmp(tfinal->tab[ix].name,t2name,ALNG) == 0) break;
      ix = tfinal->tab[ix].link;
   }
   return ix;
}  /* search_tab */

void merge_codetables (ALL_TABLES* tfinal, ALL_TABLES* t1, ALL_TABLES* t2) {
      /*copy pcode tables from t1 and t2 into tfinal*/
    
   int shortreturnt1 = 0;
   int shortreturnt2 = 0;
   int i,indext,indexfinal;
    /*compute how big table will be*/
   tfinal->last_code = (t1->last_code + t2->last_code + 1);
    
    /*set up loop variables*/
   block1 = t1->tab[1].adr;
   block2 = t2->tab[1].adr;
   outer1 = t1->last_code - block1 + 1;
   outer2 = t2->last_code - block2 + 1;
   newcode2_start = block1; 
   newcode2_stop = block1 + block2; 
   newouter2_start = block1 + outer1 + block2 - 1;
   newouter2_stop = newouter2_start + outer2;

    /*begin copy routines*/
   for (i=0; i<block1; i++) {
         /*copy pcode fields from 0 to outerblock of t1 to tfinal*/
      indexfinal = i;
      indext = i;
      tfinal->code[indexfinal].f = t1->code[indext].f;
      tfinal->code[indexfinal].x = t1->code[indext].x;
      tfinal->code[indexfinal].y = t1->code[indext].y;
   }/* end for*/

   for (i=0; i<block2; i++) {
         /*copy pcode fields from 0 to outerblock of t2 to tfinal*/
      indexfinal = i+block1;
      indext = i;
      tfinal->code[indexfinal].f = t2->code[indext].f;
      tfinal->code[indexfinal].x = t2->code[indext].x;
      tfinal->code[indexfinal].y = t2->code[indext].y;
   }/*end for*/

   if (t1->code[t1->last_code].f == SHORTRET) { shortreturnt1 = 1;}
   if (t2->code[t2->last_code].f == SHORTRET) { shortreturnt2 = 1;}

   if (shortreturnt1 && shortreturnt2) {
      fatal("Both link files %s and %s contain a main proc",
         linkfile[(mainproc_tix >= 0) ? R[mainproc_tix]->head->lfix : 
            last_linkfile-1],
         linkfile[last_linkfile]);
   }/*end if*/
   else if (!shortreturnt1) {
        /* append outer1 and outer2 as is*/
      for (i=0; i<outer1; i++) {
            /*copy outerblock of t1 to tfinal*/
         indexfinal = i+block1+block2;
         indext = i + block1;
         tfinal->code[indexfinal].f = t1->code[indext].f;
         tfinal->code[indexfinal].x = t1->code[indext].x;
            tfinal->code[indexfinal].y = t1->code[indext].y;
      }/* end for*/
      for (i=0; i<outer2; i++) {
            /*copy outerblock of t2 to tfinal*/
         indexfinal = i+block1+block2+outer1;
         indext = i+block2;
         tfinal->code[indexfinal].f = t2->code[indext].f;
         tfinal->code[indexfinal].x = t2->code[indext].x;
         tfinal->code[indexfinal].y = t2->code[indext].y;
      }/* end for*/
   }/* end if*/
   else if ((shortreturnt1) && (!(shortreturnt2))) {
      for (i=0; i<outer1; i++) { 
      /*copy outerblock less SHORTRET statement of t1 to tfinal*/
         indexfinal = i+block1+block2;
         indext = i + block1;
         tfinal->code[indexfinal].f = t1->code[indext].f;
         tfinal->code[indexfinal].x = t1->code[indext].x;
         tfinal->code[indexfinal].y = t1->code[indext].y;
      }/* end for*/
      for (i=0; i<outer2; i++) {
            /*copy outerblock of t2 to tfinal*/
          indexfinal = i+block1+block2+outer1-1;
          indext = i+block2;
          tfinal->code[indexfinal].f = t2->code[indext].f;
          tfinal->code[indexfinal].x = t2->code[indext].x;
          tfinal->code[indexfinal].y = t2->code[indext].y;
      }/* end for*/
         /* now  install SHORTRET that has to be there */
      tfinal->code[tfinal->last_code].f = SHORTRET;
      tfinal->code[tfinal->last_code].x = 0;
      tfinal->code[tfinal->last_code].y = 0;
   }/*end if*/
}/*end merge_codetables*/

void merge_infiles (ALL_TABLES* tfinal, ALL_TABLES* t1, ALL_TABLES* t2) {
    /*This both copies and merges the infiles tables which are
      essentially nothing more than a listing of the input files in
      the order in which they appear on the command line*/

   int i,j;
   int first_t2infile = t1->last_infile+1; /* array offset for t2 */
    /* need to alloc space for t2 also        */
   tfinal->last_infile = first_t2infile + t2->last_infile;

   tfinal->inputfile = alloc_infile(tfinal->last_infile);
   for (i=0; i<=t1->last_infile; i++) {  /* copy t1->inputfile */
      tfinal->inputfile[i].fname        = savestr(t1->inputfile[i].fname);
      tfinal->inputfile[i].parent       = t1->inputfile[i].parent;
   }
   for (i=0; i<=t2->last_infile; i++) {  /* copy t2->inputfile */
      j = i + t1->last_infile + 1;
      tfinal->inputfile[j].fname        = savestr(t2->inputfile[i].fname);
     /* might as well fix parent field now, rather than later */
      tfinal->inputfile[j].parent       = 
         (t2->inputfile[i].parent == -1) ?
            0 : t2->inputfile[i].parent + first_t2infile;
       /* parent of "first" *t2 file is big-daddy *t1 file, else it's  */
       /* the t2 parent, offset by the proper amount                   */
   }
}/*end merge_infiles*/


void copy_five_tables( ALL_TABLES* tfinal, ALL_TABLES* t1 )
{     /* copy tab, btab, atab, stab, and dbg tables from *t1 to *tfinal  */
      /* the code and inputfile tables are handled by the corresponding  */
      /* merge procs                                                     */

   int i;
   TAB_STRUCT* taf;
   TAB_STRUCT* ta1;
   ATAB_STRUCT* af;
   ATAB_STRUCT* a1;
   BTAB_STRUCT* bf;
   BTAB_STRUCT* b1;
   PCDEBUG* pf;
   PCDEBUG* p1;
      /* 1. Copy tab table from t1 to tfinal*/
      
   tfinal->last_tab = t1->last_tab;
   for (i=1; i<= tfinal->last_tab; i++) {
      taf = &(tfinal->tab[i]);
      ta1 = &(t1->tab[i]);
      strncpy (taf->name, ta1->name, ALNG);
      taf->link   = ta1->link;
      taf->obj    = ta1->obj;
      if (taf->obj == mainproc) 
         if (mainproc_tix < 0) 
            mainproc_tix = i;
      taf->typ    = ta1->typ;
      taf->ref    = ta1->ref;
      taf->normal = ta1->normal;
      taf->lev    = ta1->lev;
      taf->adr    = ta1->adr;
      taf->mon    = ta1->mon;
      taf->atomic = ta1->atomic;
   } /*end for*/
   tfinal->tab[1].adr = block1+block2; /* can set entry pt of outer block */

      /* 2. copy btab table from t1 to tfinal*/
   tfinal->last_btab = t1->last_btab;
   for (i=0; i<=tfinal->last_btab; i++) {
      bf = &(tfinal->btab[i]);
      b1 = &(t1->btab[i]);
      bf->last    = b1->last;
      bf->lastpar = b1->lastpar;
      bf->psize   = b1->psize;
      bf->vsize   = b1->vsize;
   }    /*end for*/
   if (mainproc_tix >= 0)  /* shortcall instr must be fixed during each link */
      fix_shortcall(tfinal,mainproc_tix);

      /* 3. copy atab from t1 to tfinal*/
   tfinal->last_atab = t1->last_atab;
   for (i=0; i<= tfinal->last_atab; i++) {
       af = &(tfinal->atab[i]);
       a1 = &(t1->atab[i]);
       af->inxtyp  = a1->inxtyp;
       af->eltyp   = a1->eltyp;
       af->elref   = a1->elref;
       af->low     = a1->low;
       af->high    = a1->high;
       af->elsize  = a1->elsize;
       af->size    = a1->size;
   }   /*end for*/

      /* 4. copy stab from t1 to tfinal*/

   tfinal->stab_size = t1->stab_size;
   for (i=0; i<=tfinal->stab_size; i++) {
      tfinal->stab[i] = t1->stab[i];
   }   /*end for*/

      /* 5. Copy debug table from t1 to tfinal*/

   tfinal->last_dbg = t1->last_dbg;
   for (i=0; i<=tfinal->last_dbg; i++) {
      pf = &(tfinal->dbg[i]);
      p1 = &(t1->dbg[i]);
      pf->lc    = p1->lc;
      pf->fix   = p1->fix;
      pf->flno  = p1->flno;
   }/*end for*/

}/*end copy_five_tables */

   /* forward reference */
void append_tab(ALL_TABLES* tfinal, ALL_TABLES* t2, int ix, int lix2);

   /* 
      merge t2->tab into tfinal->tab, resolving any symbols with
      identical names
   */
void merge_symbol_table(ALL_TABLES* tfinal, ALL_TABLES* t2, int lix)  {
   int ref, ix, place;
    /*match tab names to check for duplicates that need
     name resolution or new ones that need to be added*/
   ref = t2->tab[1].ref;
   ix = t2->btab[ref].last;
   while (ix) {
      place = search_tab(tfinal, t2->tab[ix].name);
      if (place) {
         /*tab table names are the same so resolve
           conflict and make the right table adjustments*/
         tab_name_resolution(tfinal, place, t2, ix, lix);
      }
      else { 
         /*tab table names are different so add the new member
           to the tab array and make appropriate adjustments to
           the remaining tables*/
         append_tab (tfinal, t2, ix, lix);
      }
        /*follow the link to make the next check*/
      ix = t2->tab[ix].link;
   }/*end while*/
}  /* end merge_symbol_table */

int add_tab_entry(ALL_TABLES* tfinal, TAB_STRUCT* t, int link)
   /* obtains a entry in tfinal->tab, if there's room, and copies all 
      TAB_STRUCT fields from *t to the new entry except link, copy
      the 'link' parm into the link field.  Returns index of the entry  */
{ 
   int lasttab = ++(tfinal->last_tab);
   TAB_STRUCT* tf;
   if (lasttab >=  TMAX) fatal("tab table size %d exceeded",TSIZE);
   tf = &(tfinal->tab[lasttab]);
   strncpy (tf->name, t->name, ALNG);
   if (tf->name[ALNG] != '\0') tf->name[ALNG] = '\0';
   tf->link   = link;
   tf->obj    = t->obj;
   tf->typ    = t->typ;
   tf->ref    = t->ref;
   tf->normal = t->normal;
   tf->lev    = t->lev;
   tf->adr    = t->adr;
   tf->mon    = t->mon;
   tf->atomic = t->atomic;
   return lasttab;
}  /* add_tab_entry */

#define NOT_IN_MONITOR 0
#define IN_MONITOR 1

static void absorb_variable(ALL_TABLES* tfinal, ALL_TABLES* t2, 
   int ix, int place, int in_monitor)
   /* absorb the variable from t2->tab[ix] that has been added to 
      tfinal->tab[place] into the other tables.
      If the variable is not a monitor variable, then adjust
      tfinal->btab[0].vsize, the size of the global space */
{
   int size;
    /*Arrays are treated as variables.
      If we have an array we have some work to do to make
      sure the stack space is allocated and indexed properly.
      Once you make a shift, it make impact the indicies of
      other params.  This is effectively where we are going
      to add the new array entries into the structure and
      adjust for their inclusion*/
   if (t2->tab[ix].typ == arrays) {
      if (!in_monitor) {   
           /*get the space needed on the stack for the array*/
         tfinal->tab[place].adr = tfinal->btab[0].vsize;
           /*update tfinal btab to add the new space*/
         tfinal->btab[0].vsize += t2->atab[t2->tab[ix].ref].size;
         fix_loadaddresses(tfinal, t2, place, ix);
      }
        /*make the addition to the tfinal atab*/
      append_atab(tfinal, place,t2, ix);
        /*now fix the y field of the tfinal code table for
             array-related opcodes */ 
      fix_atabindex(tfinal, t2, place, ix);
   }
   else {
      if (!in_monitor) {
       /*if the type is not an array we just need to update the
         address field of the newly added tab entry and then
         increment the tfinal->btab[0].vsize to reflect the addition
         of another parameter/variable and then go fix the code table
         addresses of the final code table*/
         tfinal->tab[place].adr = tfinal->btab[0].vsize;
            /* strings can occupy several stack words.  The byte size 
               of a string variable is kept in the ref field.  All other
               BACI types occupy one stack word */
         size = (t2->tab[ix].typ == strings) ? stacksize(t2->tab[ix].ref) : 1;
         tfinal->btab[0].vsize +=  size;
         fix_loadaddresses(tfinal, t2, place, ix);
      }
   }
      /*  fix the final code table for any opcodes that use load addresses  */
}/*end  absorb_variable */

   /* by following the link fields in the tab array, the parms of a    */
   /* procedure or function are encountered in reverse order (opposite */
   /* of declaration order).  The tixstack is used to stack the tab     */
   /* indices of the parms, so they can be entered in the tfinal->tab  */
   /* array in the correct order                                       */
static int tixstack[TSIZE];
static int toptxs;

void reset_tixstack()
   /* reset tab index stack for use */
{
   toptxs = -1;
}  /* reset_tixstack */

void push_tix(int i)
   /* push a tab index on the tab index stack */
{
   if (toptxs == TMAX)
      fatal("push_tix: internal error -- tab index stack exhausted");
   toptxs++;
   tixstack[toptxs] = i;
}  /* push_tix */

int pop_tix()
   /* pop a tab index from the tab index stack */
{
   int i;
   if (toptxs < 0)
      fatal("pop_tix: internal error -- top of tab index stack is negative");
   i = tixstack[toptxs];
   toptxs--;
   return i;
}  /* pop_tix */

static void append_parameters(ALL_TABLES* tfinal, int place, ALL_TABLES* t2, 
   int ix)
   /* append the parameters of t2->tab[ix] (presumed to be procedure
      or function object) to tfinal->tab */
{
   int b2 = t2->tab[ix].ref;
   int bfinal = tfinal->tab[place].ref;
   int loop1 = t2->btab[b2].lastpar;
   BTAB_STRUCT* btfin;
   int prev;
   int i,n,j;
   if (loop1 == ix) /* proc or func has no parms */
      return;
   reset_tixstack();
   for (i=loop1,n=0; i; n++) { /* stack up the tab indices of the parms */
      push_tix(i);
      i = t2->tab[i].link;
   }
   for (i=0,prev = 0; i < n; i++) {
      j = pop_tix();
      prev = add_tab_entry(tfinal,&(t2->tab[j]),prev);
   }
   btfin = &(tfinal->btab[bfinal]);
   btfin->lastpar = prev;   
   if (btfin->last < prev) btfin->last = prev;
}  /* append_parameters */

static void absorb_proc(ALL_TABLES* tfinal, ALL_TABLES* t2, int ix, int place)
   /* absorb the function or procedure from t2->tab[ix] that has 
      been added to tfinal->tab[place] into the other tables */ 
{
      /* update adr field of new tab table addition, add it to the
         tfinal btab table, and update the ref field */ 
   tfinal->tab[place].adr = t2->tab[ix].adr + block1;
   append_btab(tfinal, t2, place, ix);
   tfinal->tab[place].ref = tfinal->last_btab;
      /*get the params into the tfinal tab and btab*/
   append_parameters(tfinal,place,t2,ix);
   get_locals(tfinal, t2, place, ix,NOT_EXTERNAL);
      /*fix markstack opcode instruction in final code table*/
   fix_markstack(tfinal, t2, ix, place);
}  /* end absorb_proc */

void fix_loops(ALL_TABLES* tfinal) {
   /* Adjust addresses for opcode instructions that have jumps and loops */
   /* The code to be modified includes the t1.code outerblock, plus      */
   /* all of the t2.code, non outerblock and outerblock                  */
   /* The proc should only be called ONCE!                               */
   int i;
   for (i=newcode2_start; i< newouter2_stop; i++) {
      switch (tfinal->code[i].f) {
         case JUMP:
         case JZER:
         case BEGINFOR:
         case ENDFOR:
            tfinal->code[i].y += block1;
      }/*end switch*/
   }/*end for*/
}  /* end fix_loops */

static void fix_entermons(ALL_TABLES* tfinal, ALL_TABLES* t2, 
   int ix, int place)
  /* Updates the x field in tfinal->code of all ENTERMONs coming 
     from the new code.  The t2->code array is never modified, so
     it isused to find the ENTERMONs to avoid changing an ENTERMON
     that has alredy been fixed */
{
   int i;
   for (i=0; i <= t2->last_code; i++) {
      if (t2->code[i].f == ENTERMON)
         if (t2->code[i].y == ix) 
            tfinal->code[newcode2_start+i].y = place;
   }  /* end for */
}  /* fix_entermons */

void append_monitor(ALL_TABLES* tfinal, ALL_TABLES* t2, int place, int ix)
   /* Copies local procs, functions, variables, and constants of
      the monitor t2->tab[ix] into tfinal->tab. monitor's tfinal->tab 
      index is place */
{
   int i,j;
   int loop1 = t2->btab[t2->tab[ix].ref].last;
   int prevlast = 0;
   int tmp;
   if (loop1 == ix) 
      return;  /* no local procs, funcs, and vbls in t2->tab[ix] */
   for (i=loop1; i; i = t2->tab[i].link) {
      switch (t2->tab[i].obj) {
         case constant:       /* as far as the tables go, constants and */
         case type:           /* types are like variables               */
            break;            /* omit them, they're compiled in         */
         case variable:
            tmp = add_tab_entry(tfinal, &(t2->tab[i]), prevlast);
            absorb_variable(tfinal,t2,i,tmp,IN_MONITOR); 
            prevlast = tmp;
            break;
         /* 
            procedures and functions of a monitor are at the global level
            and should never appear here.  If anyh show up, then let them
            fall into the default case.
         */
         default: /* anything else is a no-no */
            fatal("append_monitor: Unexpected object type %d of symbol %s)",
               t2->tab[ix].obj, t2->tab[ix].name);
      }  /* switch */
   }  /* for */
      /* add btab entry and set last and vsize fields */
   append_btab(tfinal, t2, place, ix);
   i = tfinal->tab[place].ref = tfinal->last_btab;
   tfinal->btab[i].last = prevlast;   
   j = t2->tab[ix].ref;
   tfinal->btab[i].vsize = t2->btab[j].vsize;
   fix_entermons(tfinal, t2, ix, place);
      /* fix entry point of mon init code */
   tfinal->tab[place].adr = t2->tab[ix].adr + block1;
      /* fix the markstack for the mon init code */
   fix_markstack(tfinal,t2,ix,place);
}  /* append_monitor */

void append_tab(ALL_TABLES* tfinal, ALL_TABLES* t2, int ix, int lix2)
   /* adds t2->tab[ix] to tfinal->tab, if possible       */
   /* returns index in tfinal->tab where entry was added */
{
   int lasttab;
   int mix;
   extern int last_level0;
   TAB_STRUCT* ta2 = &(t2->tab[ix]);
   /*if level is 0 or level is 1 and this object is a proc or 
     function in a monitor, add to tfinal->tab a level 0.
     Then set the btab last and lastparam fields */
   if (ta2->lev == 0)  { /* run-of-the-mill global object */
      lasttab = add_tab_entry(tfinal, ta2, last_level0);
      last_level0 = lasttab;
      tfinal->btab[0].last = last_level0;
   }  
   else if ((ta2->lev == 1)&&(ta2->mon)) { /* monitor proc!! */
      lasttab = add_tab_entry(tfinal, ta2, last_level0);
      last_level0 = lasttab;
      tfinal->btab[0].last = last_level0;
      mix = search_tab(tfinal,t2->tab[ta2->mon].name);
      if (mix > 0) /* fix the mon field now */
         tfinal->tab[lasttab].mon = mix;
      else  /* mark this tab entry as a mon field to be fixed */
         tfinal->tab[lasttab].mon = - t2->tab[ix].mon;
   }
   else {
      /*if level is not 0, just link to the previous last*/
      lasttab = tfinal->last_tab;
      lasttab = add_tab_entry(tfinal, ta2, lasttab);
   }
   addRef(lasttab,ta2->obj,lix2);  /* note reference to symbol */

   /*now we haveto check all of the special cases to make sure
     conflicts are resolved and the correct things are being
     put in the correct place*/

   switch (ta2->obj) {
      case constant:       /* as far as the tables go, constants and */
      case type:           /* types are like variables               */
      case ext_variable:
      case variable:
         absorb_variable(tfinal,t2,ix,lasttab,NOT_IN_MONITOR); 
         break;
      case mainproc:
         /*main proc must be handled by itself if the main routine is in the
          2nd file.  If it is in the 1st file, then the SHORTCALL will be
          fixed in merge_codetables */
          /*correct address of main block in tab table*/
         tfinal->tab[lasttab].adr = t2->tab[ix].adr + block1;
             /*add main proc to tfinal's btab*/
         append_btab(tfinal, t2, lasttab, ix);
       /*update the ref field in the tab table entry for the main proc
         to correct the new index value*/
         mainproc_tix = lasttab;
         tfinal->tab[lasttab].ref = (tfinal->last_btab);
         fix_shortcall(tfinal,mainproc_tix);
         break;
      case procedure:
      case ext_procedure:
      case function:
      case ext_function:
         absorb_proc(tfinal,t2,ix,lasttab); 
         break;
      case monitor:
      case ext_monitor:
         append_monitor(tfinal,t2,lasttab,ix);
         break;
      default: /* anything else is a no-no */
         fatal("append_tab: Unexpected object type %d of symbol %s)",ta2->obj,
            ta2->name);
   }  /* switch */
}  /* append_tab */

void append_stab(ALL_TABLES* tfinal, ALL_TABLES* t2)
    /*adds t2->stab[SSIZE] to tfinal[SSIZE], if possible*/
{
    int i;
    int tfinalstringsize = tfinal->stab_size;
    int t2stringsize = t2->stab_size;

    /*update size by adding the two together*/
    tfinal->stab_size = tfinalstringsize + t2stringsize;

    if (tfinal->stab_size >= SMAX)
        {fatal("String table size %d  exceeded",SSIZE);}
    /*shouldn't this be SMAX?*/

    /*do a char by char append of each t2 string table entry to the
      tfinal string table*/
   
    for (i=0; i<t2->stab_size; i++) {
        tfinal->stab[i+tfinalstringsize] = t2->stab[i];
    }/*end for*/

    /*now go fix the addresses in the code table for all opcodes in the
      the code table that deal with strings that came from the 2nd file*/
    fix_stringcodeaddresses(tfinal, tfinalstringsize);
}/*end append_stab*/


void append_dbgs(ALL_TABLES* tfinal, ALL_TABLES* t1, ALL_TABLES* t2)
    /*add PCODE debugging info from t2 to tfinal*/
{
   int i, tfinaldebug, t2debug, filecount, tmp;
   PCDEBUG* pf;
   PCDEBUG* p2;
   filecount = t1->last_infile + 1;
   tfinaldebug = (tfinal->last_dbg + 1);
   t2debug = (t2->last_dbg + tfinaldebug);
   tfinal->last_dbg = (tfinal->last_dbg + t2->last_dbg + 1);
   for (i=tfinaldebug; i<=t2debug; i++) {
      pf = &(tfinal->dbg[i]);
      tmp = i-tfinaldebug;
      p2 = &(t2->dbg[tmp]);
      pf->lc   = p2->lc + block1;
      pf->fix  = filecount + p2->fix;
      pf->flno = p2->flno;
   }/*end for*/
}/*end append_dbgs*/

void write_tables (FILE* f, ALL_TABLES* t)
    /*writes all tables from ALL_TABLES struct pointed to by */
    /*t into stream f                                        */
{
   extern FNAME_STRING ldnamestring;
   fprintf(f,"%s\n", ldnamestring);
   fprintf(f,"%s\n", t->fnameline);

   write_code(f, t->code, t->last_code);
   write_tab(f, t->tab, t->last_tab);
   write_btab(f, t->btab, t->last_btab);
   write_atab(f, t->atab, t->last_atab);
   write_stab(f, t->stab, t->stab_size);
   write_infile(f, t->inputfile, t->last_infile);
   write_dbg(f, t->dbg, t->last_dbg);
   fclose(f);
}  /* write-tables */

int al[TSIZE];  /* index array to alphabetize final tab array */
int last_al;    /* last entry of the al array in use          */

int global_index(ALL_TABLES* t, int gi[], int stop)
   /* creates the list of indices encountered in t->tab[]          */
   /* by starting at t->btab[0].last and threading the link fields */
   /* until 'stop' is seen.  stop == 0, include outerblock, stop   */
   /* == 1, don't include outer block                              */
   /* returns the last index of the array formed                   */
{
   int i,j;
   for (i=0,j=t->btab[0].last; j != stop; i++, j = t->tab[j].link) 
      gi[i] = j;
   return i - 1;
}  /* global_index */

static void sort_tab(ALL_TABLES* t)
   /* bubble sort t->tab[i] on the name field */
{
   int i,j,k,tmp;
   int sorting;
   last_al = global_index(t,al,1);  /* 1 => don't include outer block */
   sorting = 1;
   while (sorting) {
      sorting =0;
      for (i= 0; i < last_al; i++) {
         for (j = i; j < last_al; j++) {
            k = j+1;
            if (strncmp(t->tab[al[j]].name,t->tab[al[k]].name,ALNG) > 0) {
               sorting = 1;
               tmp = al[j];
               al[j] = al[k];
               al[k] = tmp;
            }  /* if strncmp */
         }  /* for j */
      } /* for i */
   }  /* while sorting */
}  /* sort_tab */

char objname[12][11] = {
 /* 1234567890   */
   "constant",        /* 0 constant      */
   "variable",        /* 1 variable      */
   "type",            /* 2 type          */
   "procedure",       /* 3 procedure     */
   "function",        /* 4 function      */
   "monitor",         /* 5 monitor       */
   "main proc",       /* 6 mainproc      */
   "outerblock",      /* 7 outerblock    */
   "procedure",      /* 8 ext_procedure */
   "function",       /* 9 ext_function  */
   "monitor",        /* 10 ext_monitor  */
   "variable"        /* 11 ext_variable */
  };

char real_objname[12][19] = {
 /* 123456789012345678    */
   "constant",                /* 0 constant      */
   "variable",                /* 1 variable      */
   "type",                    /* 2 type          */
   "procedure",               /* 3 procedure     */
   "function",                /* 4 function      */
   "monitor",                 /* 5 monitor       */
   "main proc",               /* 6 mainproc      */
   "outerblock",              /* 7 outerblock    */
   "external procedure",      /* 8 ext_procedure */
   "external function",       /* 9 ext_function  */
   "external monitor",        /* 10 ext_monitor  */
   "external variable"        /* 11 ext_variable */
  };

char typname[12][10] = {
 /* 123456789     */
   "void",        /* 0 notyp       */
   "int",         /* 1 ints        */
   "boolean",     /* 2 bools       */
   "char",        /* 3 chars       */
   "binarysem",   /* 4 bsems       */
   "semaphore",   /* 5 sems        */
   "condition",   /* 6 conds       */
   "array",       /* 7 arrays      */
   " ",           /* 8             */
   " ",           /* 9             */
   "rawstring",   /* 10 rawstrings */
   "string"       /* 11 strings    */
};

void show_globals(ALL_TABLES* t, int lfix)
   /* show all global symbols in t->tab[] from file linkfile[lfix] */
{
   int i, k; 
   TAB_STRUCT* tb;
   sort_tab(t);
   fprintf(stdout,"++ Global symbols in the link file %s\n", linkfile[lfix]);
   fprintf(stdout,"  ('E' = external, 'D' = defined)\n");
   fprintf(stdout,"  name                 object        type\n");
   for(i = 0; i <= last_al; i++) {
      /*          12 10  9 = 31 (+3 sp) = 34 */
      k = al[i];
      tb = &(t->tab[k]);
      fprintf(stdout,"%-12s ",tb->name);
      if (tb->mon)
         fprintf(stdout,"mon. ");
      else 
         fprintf(stdout,"     ");
      fprintf(stdout,"%-18s %-10s %c\n",real_objname[tb->obj],typname[tb->typ],
         (tb->obj < ext_procedure) ? 'D' : 'E');
   }  /* for i */
}  /* show_globals */


static void show_linkfile_list(FILE* f) 
   /* show on file f an indexed list of files used in the link.*/
{
   int i;
   fprintf(f,"Files included in the link\n");
   fprintf(f,"index     file\n");
   for(i = 0; i <= last_linkfile; i++)
      fprintf(f,"%-3d    %s\n",i,linkfile[i]);
}  /* show_linkfile_list */

static void show_symbolref_header(FILE* f) 
   /* show on file f the header explaining the symbol references */
{
   fprintf(f,"List of symbol references\n");
   fprintf(f,"  ('E' = external, 'D' = defined, 'U' = unknown)\n");
   fprintf(f,"  The integer shown is an index into the link file list above\n");
   fprintf(f,"  name         object                type           references\n");
      /*          12 10  9 = 31 (+3 sp) = 34 */
}  /* show_symbolref_header */

static void show_reference_list(FILE* f, ALL_TABLES* t, int k)
   /* show on file f the reference list for the symbol at t->tab[k] */
{
   int j;
   TAB_STRUCT* tb;
   Ref* r;
   tb = &(t->tab[k]);
   fprintf(f,"%-12s ",tb->name);
   if (tb->mon)
      fprintf(f,"mon. ");
   else 
      fprintf(f,"     ");
   fprintf(f,"%-18s %-9s ", real_objname[tb->obj],typname[tb->typ]); 
   r = R[k]->head; j = 0;
   while (r != (Ref*) 0) {
      fprintf(f,"%2d %c ",r->lfix,r->ed);
      j++;
      if (j > 9) {
         fprintf(f,"\n          ");
         j = 0;
      } /* if newline */
      r = r->next;
   }  /* while r */
   fprintf(f,"\n");
}  /* show_reference_list */

void show_externals(ALL_TABLES* t)
   /* show on stdout all external symbols left */
{
   extern FNAME_STRING linkfname;
   int i, k; 
   sort_tab(t);
   fprintf(stdout,"External symbols in the %s link file\n",linkfname);
   show_linkfile_list(stdout);
   show_symbolref_header(stdout);
   for(i = 0; i <= last_al; i++) {
      k = al[i];
      if ((R[k]->status == UNRESEXT)||(R[k]->status == UNRESEXT)) {
         show_reference_list(stdout,t,k);
      }  /* if unresolved */
   }  /* for i */
}  /* show_externals */

void map_tab(ALL_TABLES* t)
   /* write to map file a ref map of  t->tab, using the R[] RefData array */
{
   int i,k;
   extern FNAME_STRING linkfname;
   extern FNAME_STRING ldnamestring;
   FNAME_STRING mname;
   FILE* mf;
   sort_tab(t);
   strncpy(mname,linkfname,MAX_FNAME);
   i = strlen(linkfname);
   mname[i-3] = 'm';
   mname[i-2] = 'a';
   mname[i-1] = 'p';
   if ((mf = fopen(mname,"w")) == NULL)
      fatal("Can't write map file %s\n",mname);
   fprintf(mf,"%s\n",ldnamestring);
   fprintf(mf,"Symbol map for the %s link file\n",linkfname);
   show_linkfile_list(mf);
   show_symbolref_header(mf);
   for(i = 0; i <= last_al; i++) {
      k = al[i];
      show_reference_list(mf,t,k);
   }  /* for i */
   printf("Map file stored in %s\n",mname);
}  /* map_tab */ 

         
/*
 *
 *     $Id: ldutils.c,v 2.16 2007/06/02 12:28:26 bynum Exp $
 *
 */

