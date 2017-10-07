#ifndef __LD_H_
#define __LD_H_
/* header file for the BACI Linker */
/* data structures that all parts of the linker need */

   /* to keep track of symbol references */
typedef struct r{
   char ed;             /* 'E' (external) 'D' (defined)  'U' (unknown) */
   int lfix;            /* index of linkfile containing this reference */
   struct r *next;      /* linked list machinery */
} Ref;


   /* to keep track of symbol references */
typedef struct {
   int status;       /* status of this symbol */
   Ref* head;        /* ptr to head of Ref list */
   Ref* tail;        /* ptr to tail of Ref list */
}  RefData;


/* values for the status field above */

#define UNKNOWN   0  
   /* all symbols will be this only briefly at startup */
#define DEFINED   1  
   /* symbol has been defined                          */ 
#define UNRESEXT  2  
   /* unresolved external symbol                       */
#define TESTEDEXT 3  
   /* external symbol not found in all link libraries  */

/*
 *
 * $Id: ld.h,v 2.2 2007/06/01 20:07:44 bynum Exp $
 *
 */
#endif
