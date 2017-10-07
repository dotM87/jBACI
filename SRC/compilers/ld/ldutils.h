/*Scott Mitchell */
/*CS710 Linker Project */
/*7 August 2000*/
/*ld_untils.h file */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/bacicnty.h"
#include "../include/readtab.h"
#include "../include/writetab.h"
#include "ld.h"

   
extern RefData* R[];          /* contains reference information */
extern char objname[12][11];  /* more verbose than ../lib/globdata.c */  
extern char typname[12][10];  /* more verbose than ../lib/globdata.c */  

extern void addRef(int tix, OBJTYPE obj, int lfix);
   /* add entry in the R[] array at tab index tix.  Set status field       */
   /* according to obj parm.  Set lfix field with lfix (linked file index  */

extern void read_tables (FILE* f, ALL_TABLES* t);
    /*reads all tables from stream f into ALL_TABLES struct */
    /*pointed to by t)                                      */

extern void merge_codetables (ALL_TABLES* tfinal, 
                              ALL_TABLES* t1, ALL_TABLES* t2);
    /*merges PCODE tables from two argument files into one file*/

extern void merge_infiles (ALL_TABLES* tfinal, ALL_TABLES* t1, ALL_TABLES* t2);
    /*merges infile tables from two argument files into one file*/

extern void copy_five_tables( ALL_TABLES* tfinal, ALL_TABLES* t1 );
   /* copy tab, btab, atab, stab, and dbg tables from *t1 to *tfinal  */
   /* the code and inputfile tables are handled by the corresponding  */
   /* merge procs                                                     */

extern void merge_symbol_table(ALL_TABLES* tfinal, ALL_TABLES* t2, int lix);
   /* merge t2->tab into tfinal->tab, resolving any symbols with 
      identical names */

extern int search_tab(ALL_TABLES* tfinal, ALFA t2name);
    /* Searches the outer block for t2name.  Returns index i     */
    /* in tfinal->tab[] such that tfinal->tab[i].name == t2name  */
    /* if there is such an i; otherwise, returns 0               */

extern int add_tab_entry(ALL_TABLES* tfinal, TAB_STRUCT* t, int link);
   /* obtains a entry in tfinal->tab, if there's room, and copies all 
      TAB_STRUCT fields from *t to the new entry except link, copy
      the 'link' parm into the link field.  Returns index of the entry  */

extern void append_tab(ALL_TABLES* tfinal, ALL_TABLES* t2, int ix, int lix2);
   /* adds t2->tab[ix] to tfinal->tab, if possible       */
   /* returns index in tfinal->tab where entry was added */

extern void append_stab(ALL_TABLES* tfinal, ALL_TABLES* t2);
    /*adds t2->stab[SSIZE] to tfinal[SSIZE], if possible*/

extern void append_dbgs(ALL_TABLES* tfinal, ALL_TABLES* t1, ALL_TABLES* t2);
    /*add PCODE debugginginfo from t2 to tfinal*/

extern void write_tables (FILE* f, ALL_TABLES* t );
    /*writes all tabels from ALL_TABLES struct pointed to */
    /*by t to stream f*/

extern int global_index(ALL_TABLES* t, int gi[], int stop);
   /* creates the list of indices encountered in t->tab[]          */
   /* by starting at t->btab[0].last and threading the link fields */
   /* until 'stop' is seen.  stop == 0, include outerblock, stop   */
   /* == 1, don't include outer block                              */
   /* returns the last index of the array formed                   */

extern void fix_loops(ALL_TABLES* tfinal);
    /*adjust addresses for opcode instructions that have jumps and loops*/

extern void check_atab_dimensions(ALL_TABLES* tfinal, ALL_TABLES* t2, 
                                             int place, int ix);
    /* Compares the array dimensions of the two arrays */

extern void fix_atabindex(ALL_TABLES* tfinal, ALL_TABLES* t2, int place, int ix);
    /*adjusts tfinal->code to reflect changes in the indices of arrays*/

extern void append_monitor(ALL_TABLES* tfinal, ALL_TABLES* t2, int ix,
   int place);
   /* Copies local procs, functions, variables, and constants of
      the monitor t2->tab[ix] into tfinal->tab. place is the index
      of the monitor in tfinal->tab */

extern void reset_tixstack();
   /* reset tab index stack for use */

extern void push_tix(int i);
   /* push a tab index on the tab index stack */

extern int pop_tix();
   /* pop a tab index from the tab index stack */

extern void map_tab(ALL_TABLES* t);
   /* write symbol reference map to the map file */

extern void show_externals(ALL_TABLES* t);
   /* show on stdout all external symbols left */

extern void show_globals(ALL_TABLES* t, int lfix);
   /* show all global symbols in t->tab[] from file linkfile[lfix] */
/*
 *
 * $Id: ldutils.h,v 2.4 2007/06/01 20:07:44 bynum Exp $
 *
 */
