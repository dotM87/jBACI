/*Scott Mitchell */
/*CS710 Linker Project */
/*18 August 2000*/
/*ld_resolution.h file */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/bacicnty.h"

extern void tab_name_resolution(ALL_TABLES* tfinal, int place, 
                                 ALL_TABLES* t2, int ix, int lix2);
/*resolves name conflicts and corrects code_table y field */
/*entries for variables and procedures in the tab table */

extern void update_monfields(ALL_TABLES* tfinal, ALL_TABLES* t2);
/* check tfinal->tab for entries with a negative mon field 
   When found, look for the monitor from t2->tab in 
   tfinal->tab.  If found, fix the negative mon field,
   else die */

extern void fix_markstack (ALL_TABLES* tfinal, ALL_TABLES* t2, int ix,
                                      int correctmark); 
/*search lines of tfinal code that came from the second argument program */
/*and modify the y field of the markstack instruction to correspond to */
/*the  correct value in the tab table*/

extern void fix_loadaddresses(ALL_TABLES* tfinal, ALL_TABLES* t2,
                                          int place, int ix);
/*search lines of tfinal code that came from the second argument program */
/*and modify the y field of the address type instructions to correspond to */
/*the  correct value in the tab table*/

extern void fix_stringcodeaddresses(ALL_TABLES* tfinal, int tfinalstringsize);
/*Correct y field values in finaltable->code for opcodes WRITE_RAWSTRING,*/
/*SEND_RAWSTRING, BROADCAST_RAWSTRING, COPY_RAWSTRING, CONCAT_RAWSTRING,*/
/*and COMPARE_RAWSTRING */

/* values for the isExternal flag below */
#define IS_EXTERNAL  1
#define NOT_EXTERNAL 0

void get_locals(ALL_TABLES* tfinal, ALL_TABLES* t2, int place, int ix, 
                                 int isExternal);
    /*Function to get the local variables from added functions and
      procedure's into the Identifier Table.  Changes btab 'vsize'
      field if isExternal flag is true */

/*
 *
 * $Id: ldresolv.h,v 2.5 2007/06/01 20:07:44 bynum Exp $
 *
 */
