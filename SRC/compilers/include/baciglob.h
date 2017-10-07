/* BenAri Concurrent Pascal PCODE Compiler & Interpreter Global Variables */

#if __DEFINING__
TAB_STRUCT  tab[TSIZE];    /* identifier table */
ATAB_STRUCT atab[ASIZE];   /* array table */
BTAB_STRUCT btab[BSIZE];   /* block table */
char  stab[SSIZE];         /* string table */
ORDER code[CSIZE];         /* object code */

PCDEBUG dbg[CSIZE];        /* debugging information about pcode */

FNAME_STRING pcode_fname;

char  comp_proot[]      = {"BACI -- "};

char  pcode_suffix[]    = {".pco"};

TYPENAME typenames[]    = {"notyp","int","bool","char","bsem",
                           "sem","cond","array"};

OBJNAME objnames[]      = {"constant","variable","type","procedure",
                           "function","monitor"};
#else

/* BenAri Concurrent Pascal PCODE Interpreter External Variables */

extern char  comp_proot[];

extern   TAB_STRUCT     tab[];
extern   ATAB_STRUCT    atab[];
extern   BTAB_STRUCT    btab[];
extern   char           stab[];
extern   ORDER          code[];

extern   PCDEBUG        dbg[];        

extern   FNAME_STRING   pcode_fname;

extern   char           pcode_suffix[];

extern   TYPENAME       typenames[];
extern   OBJNAME        objnames[];
extern   char           filename_line[];

#endif

/*
 *
 * $Id: baciglob.h,v 1.6 2007/06/01 17:41:16 bynum Exp $
 *
 */
