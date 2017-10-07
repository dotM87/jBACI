/*
 *  source code to implement the 'replace' command
 *  for the BACI archiver
 *    Bill Bynum
 *    May 2000
 */
 
#include <errno.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../include/genutil.h"

#include "ar.h"
#include "ardelete.h"
#include "arrdsyms.h"
#include "artables.h"
#include "arutils.h"
#include "arwrite.h"

static void replaceMember(Archive* ar, char* ext_fname, int verbose)
/*
 * if (ar->rf >= 0) (there is an existing archive), then find
 * an existing member of the archive with the given member name
 * and delete it (quietly)
 * Then stat the external file and if it exists, add that member
 * to the archive.
 *
 * if (verbose) perform the action verbosely
 *
 */
{
   char tmp_name1[1024];
   char* tmp_name2;
   char new_name[MAX_AR_NAME+2];
   int nonverbose = 0;  /* quietly deleteMember */
   Member* tm;
   Member* prev;
   struct stat t_stat;
   FILE* afd = ar->rf;
   if ((afd != NULL)&&(afd != NO_ARCHIVE)) { /* if there's an old archive ... */
      tm = findMember(&ar->ml,&prev,normalize(ext_fname));
      if (tm) deleteMember(ar,tm,prev,nonverbose);
   }
   if (stat(ext_fname,&t_stat) < 0) {
      if (errno == ENOENT) {
         nonfatal("File %s does not exist",ext_fname);
      }
      else {
         fatal("Cannot stat %s",ext_fname);
      }
   }
   else {   /* successful stat */
      tm = newMember();
      strncpy(tmp_name1,ext_fname,1024);
      tmp_name2 = basename(tmp_name1);
      if (strlen(tmp_name2) > MAX_AR_NAME) {
         strncpy(new_name,tmp_name2,MAX_AR_NAME);
         new_name[MAX_AR_NAME-1] = ' ';
         new_name[MAX_AR_NAME] = '\0';
         printf("Member name truncated to '%s'\n",new_name);
         tm->name = save_string(new_name);
      }
      else {   /* not necessary to truncate member name */
         tm->name = save_string(tmp_name2);
      }  /* truncation not necessary */
      tm->mtime = t_stat.st_mtime;
      tm->uid = 0;
      tm->gid = 0;
      tm->mode = t_stat.st_mode;
      tm->size = t_stat.st_size;
      tm->ext_fname = save_string(ext_fname);
      appendMember(&ar->ml,tm);
      afd = open_member_file(ext_fname);
      collectMemberSymbols(afd,&ar->sl,&ar->el,tm,verbose);
      fclose(afd);
      if (verbose) printf("r- %s\n",tm->name);
   }  /* else successful stat */
}  /* replaceMember */


void replace_members(Archive* ar,char** files,int num_files,int verbose)
/*  
 *  replace members named in the list of files 'files' (there
 *  are 'num_files' filenames in the list) from the archive file
 *  described by '*ar'
 *
 *  if (verbose) perform the action verbosely
 *
 *  if (num_files > 0) and there is no SYMDEF member,  then add it.
 */
{
   read_all_members(ar);
   if (symdef_present(ar)) 
      read_symdef(ar);
   else { /* no symdef */
      if (num_files > 0) {
         /* add symdef if archive is empty */
         if (empty_archive(ar)) { 
            add_symdef_member(ar,verbose);
            recalc_offsets(ar);
            if (!empty_archive(ar))
               collectArchiveSymbols(ar,verbose);
         }  /* if archive is empty */
      }  /* if num_files > 0 */
   }  /* else no symdef */
   if (num_files > 0) {
      while (num_files > 0) {
         if (strcmp(*files,SYMDEF) == 0) 
           nonfatal("The SYMDEF member is not user-modifiable");
         else   
            replaceMember(ar,*files,verbose);
         num_files--;
         files++;
      }
      recalc_offsets(ar);
      write_archive(ar);      
   }
   else if (ar->rf == NO_ARCHIVE)   /* new_archive, no files to be added */
      write_archive(ar);            /* this will write an empty archive  */
   else  /* no files specified, but archive exists */
      if (verbose) printf("No archive members specified\n");   
}

/*
 *
 *  $Id: arreplac.c,v 1.11 2007/06/01 19:46:03 bynum Exp $
 *
 */
