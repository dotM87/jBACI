   /* BenAri external declarations for routines to read the tables */

#include "bacicnty.h"


   /* global array sizes */
extern int code_size;    /* actual size of code array */
extern int tab_size;     /* actual size of tab array  */
extern int btab_size;    /* actual size of btab array */ 
extern int atab_size;    /* actual size of atab array */
extern int stab_size;    /* actual size of stab array */
extern int last_infile;  /* index of last file in infile array */
extern int last_dbg_ix;  /* index of last entry in dbg array */

extern int read_code_table(FILE *f, ORDER code[]);
   /* reads code[] array and returns the last index used */

extern void skip_code_table(FILE *f);
   /* skips the code table so the identifier table can be read */

extern int read_identifier_table(FILE *f, TAB_STRUCT tab[]);
   /* reads tab[] array and returns the last index used */

extern int read_block_table(FILE *f, BTAB_STRUCT btab[]);
   /* reads btab[] array and returns the last index used */

extern int read_array_table(FILE *f, ATAB_STRUCT atab[]);
   /* reads atab[] array and returns the last index used */
   
extern int read_string_table(FILE *f, char stab[]);
   /* reads stab[] array and returns the size of the array */

extern InputFile* alloc_infile(int last_infile);
   /* If last_infile >= 0, then callocs space for an array of     */
   /* type InputFile with last_infile + 1 members and returns a   */ 
   /* pointer to it.                                              */
   /* If last_infile < 0, then returns NULL                       */

extern int read_file_info(FILE *f,InputFile** inputfile);
   /*  allocates space for the inputfile[] array, reads in the data, and */
   /*  returns the last index used */

extern int read_debug_info(FILE* f, PCDEBUG dbg[]);
   /* reads the dbg[] array and returns the last index used */

extern void check_header(FILE *f);
   /* check header line of a .pob or .pco file for validity */

extern void read_pcode_file(FILE *f);
   /* read pcode file up to, but not including, file & debug info */

extern void read_entire_pcode_file(FILE *f);
   /* read the entire pcode file */

  
/*
 *
 *  $Id: readtab.h,v 1.7 2007/06/01 17:41:16 bynum Exp $
 *
 */

