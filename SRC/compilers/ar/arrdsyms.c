/*
 * source code for reading symbols for the BACI archiver
 * from the .pob or .pco files produced by two BACI compilers
 *
 *    Bill Bynum
 *    May 2000
 */

#include <stdio.h>
#include <string.h>
#if !defined(DOS)
#include <unistd.h>
#endif

#include "../include/bacicnty.h"
#include "../include/readtab.h"
#include "../include/genutil.h"

#include "ar.h"
#include "artables.h"
#include "arutils.h"

static TAB_STRUCT tab[TSIZE]; /* .pob file's identifier table */ 

static char buf[MAX_FNAME];

FILE* open_member_file(char* member_fname)
/*
 *  open for reading a member file named 'member_fname' and
 *  return a file ptr to it
 *  abort if not possible  
 */
{  
#if defined(DOS)
FILE* f = fopen(member_fname,"rb");
#else
FILE* f = fopen(member_fname,"r");
#endif
   if (f == NULL) {
      fatal("Cannot open member file %s",member_fname);
   }
   return f;
}  /* open_member_file */


void collectMemberSymbols(FILE* mf, 
   SymbolList* defsym,  /* the declared symbols */
   SymbolList* refsym,  /* the external references  */
   Member* m, int verbose)
/*
 *  collect the defined & external symbols of the Member pointed to by 'm' 
 *  from the file described by 'mf' and store the symbols in the 
 *  '*defsym' or '*refsym' SymbolLists.  verbose == 1 => do verbosely
 */
{
   int ix;
   int defined,external,inmon;
   int last_tab;
   Symbol* s;
   fgets(buf,MAX_FNAME,mf);
   fgets(buf,MAX_FNAME,mf);
   skip_code_table(mf);
   last_tab = read_identifier_table(mf,tab);
   /* tab[0] is empty, tab[1] is outer block */
   for (ix = 2; ix <= last_tab; ix++) {
      if (tab[ix].lev > 1) continue;
      defined = external = inmon = 0;
      if (tab[ix].lev == 1) {
         if (tab[ix].mon) {
            switch (tab[ix].obj) {
               case procedure:  
               case function:  
                  defined = 1;
                  inmon = 1;
                  break;
               case ext_procedure:  
               case ext_function:  
                  defined = 1;
                  inmon = 1;
                  external = 1;
                  break;
               default: ;
            }  /* switch */
         }  /* if tab[ix].mon */
      } /* if tab[ix].lev == 1 */
      else {   /* at level 0 here */
         switch (tab[ix].obj) {
            case constant: 
            case type: 
            case variable:
            case procedure:
            case function:
            case monitor:
            case mainproc:
               defined = 1;
               break;
            case ext_procedure:
            case ext_function:
            case ext_monitor:
            case ext_variable:
               defined = 1;
               external = 1;
               break;
            default:
               ;  /* ignore all else */
         }  /* switch */
      }  /* else */
      if (defined) {
         s = newSymbol();
         s->name = save_string(tab[ix].name);
         s->m = m;
         s->typ = tab[ix].typ;
         s->obj = tab[ix].obj;
         s->inmon = inmon;
         if (external)
            appendSymbol(refsym,s);
         else 
            appendSymbol(defsym,s);
         if (verbose) {
            printSymbol(s);printf(" in %s\n",m->name);
         }
      }
   }  /* for */
}  /* collectMemberSymbols */

void collectArchiveSymbols(Archive* ar, int verbose)
/*
 *  collect all of the Symbols from existing members of the Archive 
 *  into the Archive's SymbolList.  verbose == 1 =>do verbosely
 */
{
   Member* m;
   Member* start;
   FILE* af = ar->rf;
   if ((af == NULL)||(af == NO_ARCHIVE))
      fatal("can't collect symbols from non-opened archive");
   if (strcmp(ar->ml.head->name,SYMDEF) == 0) 
      start = ar->ml.head->next;
   else
      start = ar->ml.head;
   for (m=start;m;m=m->next) {
      fseek(af,(long)(m->offset+sizeof(struct ar_hdr)),SEEK_SET);
      collectMemberSymbols(af,&ar->sl,&ar->el,m,verbose);
   }
}  /* collectArchiveSymbols */

/*
 *
 *  $Id: arrdsyms.c,v 1.11 2007/06/01 19:46:03 bynum Exp $
 *
 */
