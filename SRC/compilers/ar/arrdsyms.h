#ifndef AR_READSYMS
#define AR_READSYMS
/*
 *  include file for arrdsyms.c functions
 *  BACI archiver
 *     Bill Bynum
 *     May 2000
 */

#include <stdio.h>

FILE* open_member_file(char* member_fname);
/*
 *  open for reading a member file named 'member_fname' and
 *  return a file ptr to it
 *  abort if not possible  
 */

void collectMemberSymbols(FILE* mf, SymbolList* defsym, 
   SymbolList* refsym, Member* m, int verbose);
/*
 *  collect the Def (defined) symbols of the Member pointed to by 'm' 
 *  from the file described by 'mf' and store the defined symbols in the 
 *  '*defsym' SymbolList and the external refs in the '*refsym' SymbolList
 *  verbose == 1 => do it verbosely
 */

void collectArchiveSymbols(Archive* ar, int verbose);
/*
 *  collect all of the Symbols from existing members of the Archive 
 *  into the Archive's SymbolList
 *  verbose == 1 => do it verbosely
 */

/*
 *
 *  $Id: arrdsyms.h,v 1.7 2007/06/01 19:46:03 bynum Exp $
 *
 */
#endif
