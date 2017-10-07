/*
 * utilities for the BACI baar archiver
 *    Bill Bynum
 *    May 2000
 */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(DOS)
#include <unistd.h>
#endif
#include <utime.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "../include/genutil.h"

#include "ar.h"
#include "artables.h"
#include "arutils.h"
#include "arwrite.h"

static void removeSymbols(Archive* ar, Member* tm)
/* 
 *  remove all Symbols from the __.SYMDEF archive member belonging
 *  to the archive Member pointed to by 'tm'
 *
 */
{
   Symbol *ts;
   Symbol *oldts;
   Symbol *ps;

   for (ts = ar->sl.head, ps = 0; ts; ) 
      if (ts->member_offset == tm->offset) {
         if (ps) ps->next = ts->next;
         if (ar->sl.head == ts) ar->sl.head = ts->next;
         if (ar->sl.tail == ts) ar->sl.tail = ps;
         oldts = ts;
         ts = ts->next;
         free(oldts);      /* be tidy */
      }
      else {
         ps = ts; /* no dangling refs! only move ps if ts is not deleted */
         ts = ts->next;
      }
   for (ts = ar->el.head, ps = 0; ts; ) 
      if (ts->member_offset == tm->offset) {
         if (ps) ps->next = ts->next;
         if (ar->el.head == ts) ar->el.head = ts->next;
         if (ar->el.tail == ts) ar->el.tail = ps;
         oldts = ts;
         ts = ts->next;
         free(oldts);      /* be tidy */
      }
      else {
         ps = ts; /* no dangling refs! only move ps if ts is not deleted */
         ts = ts->next;
      }
}  /* removeSymbols */

void deleteMember(Archive* ar, Member* tm, Member* prev, int verbose)
/*
 *  delete the member pointed to by 'tm' from the MemberList of the
 *  archive pointed to by 'ar'
 *  'prev' points to the member of the Member list prior to 
 *  the Member to be deleted
 *  if (verbose) perform the action verbosely
 */
{
      /* first snip out the Member from the Member list */
   if (prev) prev->next = tm->next;
   if (ar->ml.head == tm) ar->ml.head = tm->next;
   if (ar->ml.tail == tm) ar->ml.tail = prev;
      /* now snip out all of the Member's Symbols from the Symbol list */
   if (symdef_present(ar)) removeSymbols(ar,tm);
   if (verbose) printf("d- %s\n",tm->name);
   free(tm);      /* not strictly necessary, but why not be tidy? */
}  /* deleteMember */

void delete_members(Archive* ar, char** files, int num_files, int verbose)
/*
 *  delete members named in the list of files 'files' (there
 *  are 'num_files' filenames in the list) from the archive file
 *  described by '*ar'
 *  if (verbose) perform the action verbosely
 *  If no SYMDEF member is there, then add it
 */
{
   Member* tm;
   Member* prev;
   int unchanged = 1;
   read_all_members(ar);
   if (symdef_present(ar)) 
      read_symdef(ar);
   else { /* no symdef */
      fatal("Malformed archive!  No SYMDEF member!!!");
   }  /* else no symdef */
   while (num_files > 0) {
      if (strcmp(*files, SYMDEF) == 0) {
         nonfatal("Cannot delete %s member\n",SYMDEF);
      }
      else {
         tm = findMember(&ar->ml,&prev,*files);
         if (tm == 0) {
            nonfatal("Member %s not in archive",*files);
         }
         else {
            deleteMember(ar,tm,prev,verbose);
            unchanged = 0;
         }
      }
      num_files--;
      files++;
   }
   if (unchanged) {
      fclose(ar->rf);
      exit(0);
   }
   recalc_offsets(ar);
   write_archive(ar);
}  /* delete_members */


/*
 *
 *  $Id: ardelete.c,v 1.14 2007/06/01 19:46:03 bynum Exp $
 *
 */
