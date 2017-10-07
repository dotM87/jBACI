   /* BenAri external declarations for routines to write the tables */
#include "../include/bacicnty.h"

extern void write_code(FILE *fl, ORDER code[], int last_code);
   /* used by all BACI programs to write the code[] array */

extern void  write_tab(FILE *f, TAB_STRUCT tab[], int last_tab);
   /* used by all BACI programs other than two compilers to write    */
   /*   the tab[] array                                              */

extern void  write_btab(FILE *f, BTAB_STRUCT btab[], int last_btab);
   /* used by all BACI programs other than the two compilers  */
   /*  to write the btab[] array                              */

extern void write_atab(FILE *f, ATAB_STRUCT atab[], int last_atab);
   /* used by all BACI programs to write the atab[] array */

extern void write_stab(FILE *f, char stab[], int stab_size);
   /* used by all BACI programs to write the stab[] array */

extern void write_dbg(FILE *f, PCDEBUG dbg[], int last_dbg);
   /* used by all BACI programs to write the dbg[] array */

extern void write_infile(FILE *f, InputFile infile[], int last_infile);
   /* used by all BACI programs to write the infile[] array */

  
/*
 *
 *  $Id: writetab.h,v 1.6 2007/06/01 17:41:16 bynum Exp $
 *
 */

