   /* global tables used by the BACI system */
   
#include "../include/bacicnty.h"    /* bring in constants & types */

TAB_STRUCT  tab[TSIZE]; /* identifier table */
int   last_tab;         /* index of last tab entry */

ATAB_STRUCT atab[ASIZE];/* array table */
int   last_atab;        /* index of last atab entry */

BTAB_STRUCT btab[BSIZE];/* block table */
int   last_btab;        /* index of last btab entry */

char  stab[SSIZE];      /* string table */
int   stab_size;        /* current size of stab == next loc to be filled */

ORDER code[CSIZE];      /* object code */
int   lc;               /* next loc to be filled, used by the compilers */
int   last_code;        /* last code loc filled, used by interp & disasm */

PCDEBUG dbg[CSIZE];     /* debugging information about pcode */
int   last_dbg;         /* index of last dbg entry */

/*
 *
 * $Id: globtabs.c,v 1.3 2007/06/01 17:49:03 bynum Exp $
 *
 */
