/* constants & types for the BenAri Concurrent Pascal PCODE Interpreter */


#ifndef __CONST_TYPES__
#define __CONST_TYPES__ 1

#include <stdio.h>

#define  TRUE  1
#define  FALSE 0
#define  MAX_FNAME   256
#define  SUFFIX_LEN  4
#define  MAX_PREFIX  251      /* MAX_FNAME - SUFFIX_LEN */
#define  LMAX  7     /* max level index */
#define  LSIZE 8     /* display array size */
#define  TMAX  500   /* max tab index */
#define  TSIZE 501   /* tab array size */
#define  BMAX  50    /* max btab index */
#define  BSIZE 51    /* btab array size */
#define  AMAX  50    /* max atab index */
#define  ASIZE 51    /* atab array size */
#define  CMAX  2000  /* max code index */
#define  CSIZE 2001  /* code array size */
#define  SMAX  900   /* max stab index */
#define  SSIZE 901   /* stab array size */
#define  ALNG  12    /* number of significant chars in an id */
#define  ALNGSIZE 13 /* allow for null terminator */
#define  GULPSIZE 60 /* # chars per line in storing stab array */

#define  PMAX     18    /* no. of concurrent processes */
#define  PMAXM1   17    /* PMAX - 1    */
#define  PMAXP1   19    /* should be a prime */

#define MAXMON 20    /* largest mtab array index */
#define MONSIZE 21   /* max number of monitors definable */

#define  LINESIZE  120
#define  MAXLINE   119

#define  BUFSIZE  120
typedef  char  BUFFER[BUFSIZE];  

#define  MAXERROR 4

#define  DEFAULT_PRIO   10    /* default priority in WAITC call */

typedef  char PREFIX_STRING[MAX_PREFIX];
typedef  char FNAME_STRING[MAX_FNAME];
typedef  char ALFA[ALNGSIZE];
typedef  char TYPENAME[6];
typedef  char OBJNAME[10];

typedef enum { constant, variable, type, procedure, 
               function, monitor, mainproc,outerblock,
               ext_procedure, ext_function, ext_monitor,
               ext_variable }  OBJTYPE;

typedef enum { notyp = 0, ints = 1, bools = 2, chars = 3, bsems = 4, 
               sems = 5, conds = 6, arrays = 7, 
               rawstrings  = 10 /* "xxx" */, 
               strings = 11/* string vbl */  }  TYPES;

typedef struct { int f; int x; int y; } ORDER;   /* pcode type */

typedef struct {
   int   lc;      /* value of lc where source file line no changed */
   int   fix;     /* index of the file generating this pcode instr */
   int   flno;    /* line # in the that should be showing for this instr */
}PCDEBUG;         /* debugging information about the pcode */

/* used for include files */
typedef struct{
      char  *fname;           /* name of the include file  */
      int   last_line_no;     /* last line no in it that genned pcode */
      int   line_no;          /* current line number in it */
      int   parent;           /* index in 'infile' array of parent file */
      void* flexbuf;          /* ptr to flex buffer */
      FILE *f;                /* stream for this file */
} InputFile;

/* symbol table structure */
typedef struct {  ALFA     name;    /* symbol name */
                  int      link;    /* tab index of next symbol this level */ 
                  OBJTYPE  obj;     /* sort of object this is */
                  TYPES    typ;     /* type of this object */
                  int      ref;     /* btab index(proc); atab index(array) */
                  int      normal;  /* FALSE for VAR parms, TRUE all else */
                  int      lev;     /* level of this symbol */
                  int      adr;     /* entry pt(proc),size(type),value(vbl)*/
                  int      mon;     /* tab index of monitor (monproc), else 0*/
                  int      atomic;  /* TRUE if proc or func is atomic */
      } TAB_STRUCT;                 

/* array table.  EACH DIMENSION of an array takes 1 entry */
typedef struct {  TYPES    inxtyp;  /* index type for this dimension */
                  TYPES    eltyp;   /* elem type, (arrays if more dims left)*/
                  int      elref;   /* atab index (if arrays) 0 if not */
                  int      low;     /* smallest index this dimension */
                  int      high;    /* largest index this dimension */ 
                  int      elsize;  /* element size */
                  int      size;    /* size of the entire array */
      } ATAB_STRUCT;

typedef struct {  int      last;    /* tab index, last id in this block */
                  int      lastpar; /*  ", last parameter this block */
                  int      psize;   /* stackframe size = 5 (fixed) + parms */
                  int      vsize;   /* psize + size of local variables */
      } BTAB_STRUCT;


/* expressions in the compiler */
typedef struct{
                  OBJTYPE  obj;     /* values transferred from tab entry */
                  TYPES    typ;     /* of object, if it's in the symbol  */
                  int      adr;     /* table                             */
                  int      ref;    
                  int      lev;
                  int      normal;
                  int      tix;     /* tab index, if a var ident */
                  int      arelt; 
                  int      isval; 
                  int      result_used; /* fcn result used, in C-- */
                  int      free;  
               } EXPR;
               
#define  EXPRSIZE 64
#define  MAXEXPR  63

   /* struct for the code block table              */
   /* used by the disassembler and interpreter     */
typedef struct {
   int   tix;     /* index into tab array of this code block */
   int   bix;     /* index into btab array of this code block */
   int   adr;     /* lc of first byte of code for the code block */
      } CODEBLOCK;

   /* ALL_TABLES struct containing all 7 tables in a .pco or .pob file */
   /* used by the BACI linker                               */
   /* all ints or pointers are placed first to avoid alignment problems */
typedef struct {
   int   last_code;              /* last index used in the code array   */
   int   last_tab;               /* last index used in the tab array    */
   int   last_btab;              /* last index used in the btab array   */
   int   last_atab;              /* last index used in the atab array   */
   int   stab_size;              /* byte size of the stab array         */
   InputFile* inputfile;         /* array of InputFile information      */
   int   last_infile;            /* last index used in inputfile array  */
   int   last_dbg;               /* last index used in dbg array        */
   FNAME_STRING header;          /* header line of .pob or .pco file    */
   FNAME_STRING fnameline;       /* filename line of .pob or.pco file   */
   ORDER code[CSIZE];            /* code table                          */
   TAB_STRUCT tab[TSIZE];        /* identifier table                    */
   BTAB_STRUCT btab[BSIZE];      /* block table                         */
   ATAB_STRUCT atab[ASIZE];      /* array table                         */
   char stab[SSIZE];             /* string table                        */
   PCDEBUG dbg[CSIZE];           /* debugging information array         */
} ALL_TABLES;
 
#endif

/*
 *  $Id: bacicnty.h,v 1.19 2007/06/01 17:34:16 bynum Exp $
 */
