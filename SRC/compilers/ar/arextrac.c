/* * code to implement the 'extract' command * for the BACI archiver *    Bill Bynum
 *    May 2000
 */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#if !defined(DOS)
#include <unistd.h>
#endif
#include <utime.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../include/genutil.h"

#include "ar.h" 
#include "arutils.h" 
#include "artables.h" 
#include "arextrac.h" 

/* for extracting a file or rewriting the archive , declared in ar_write.c */
extern char xfer_buf[];

static void extractMember(Archive* ar, Member* tm, char* ext_name, int
                   verbose, int preserve_dates)
/* 
 *  extract member pointed to by 'tm' from the archive file 
 * described by '*ar' into the external file named 'ext_name', 
 *  if this pointer is non-NULL; otherwise, extract the member 
 *  to the file tm->name.  
 *  if (verbose) perform the action verbosely 
 *  if (preserve_dates) preserve the original mtime of the file 
 *  else use current time for mtime 
 */
{ 
   FILE* xfd; 
   char* loc_xname; 
   off_t tocopy; 
   off_t ncopied; 
   int nread;
   struct utimbuf tb; 
   FILE* afd = ar->rf;
   if (ext_name) 
      loc_xname = ext_name;
   else 
      loc_xname = tm->name;
   if (verbose) printf("x- %s\n",loc_xname); 
#if defined(DOS)
   if ((xfd = fopen(loc_xname,"wb+")) == NULL) {
#else   
   if ((xfd = fopen(loc_xname,"w+")) == NULL) {
#endif
      fatal("Cannot open file %s",loc_xname);
   } /* skip over the member's header */
   fseek(afd,tm->offset + sizeof(struct ar_hdr),SEEK_SET); 
   ncopied = 0; 
   while (ncopied < tm->size) {
      tocopy = tm->size - ncopied; 
      if (tocopy > XFERBUFSIZE) 
         tocopy = XFERBUFSIZE; 
      nread = fread(xfer_buf,1,tocopy,afd); 
      if (nread != tocopy) {
         fatal("Cannot read at offset %lX of archive", ftell(afd)); 
      } 
      if (fwrite(xfer_buf,1,tocopy,xfd) != tocopy) {
         fatal("Cannot write member %s at offset %lX",loc_xname, ftell(xfd));
      }
      ncopied += tocopy;
   }  /* copy while loop */ 
   fclose(xfd);
   if (strcmp(tm->name,SYMDEF))  /* if not __.SYMDEF entry ... */
      chmod(loc_xname,tm->mode); 
   else
      chmod(loc_xname,0644); 
   if (preserve_dates) {
      tb.actime = tb.modtime = tm->mtime; 
      if (utime(loc_xname,&tb)<0) {
         perror("extractMember");
         nonfatal("unable to set original mod time & date");
      }
   } 
}  /* extractMember */

void extract_members(Archive* ar,char** files, int num_files, 
                     int verbose, int preserve_dates)
/*
 *  extract members named in the list of files 'files' (there
 *  are 'num_files' filenames in the list) from the archive file
 *  described by '*ar'.
 *  if (verbose) perform the action verbosely
 *  if (preserve_dates) preserve the original mtime of the file
 *  else use current time for mtime
 */
{
   Member* tm;
   Member* prev;
   char *tp;
   read_all_members(ar);
   if (num_files) {
      while (num_files) {
         tp = normalize(*files); 
         tm = findMember(&ar->ml,&prev,tp);
         if (tm == 0) {
            nonfatal("Member %s not in archive",tp);
         }
         else 
            extractMember(ar,tm,*files,verbose,preserve_dates);
         files++;
         num_files--;
      }
   }
   else {   /* extract all members */
      for (tm = ar->ml.head; tm; tm = tm->next) 
         extractMember(ar,tm,(char*) 0,verbose,preserve_dates);
   }               
}  /* extract_members */

void named_extract(Archive* ar, Member* m, char* fname, int verbose)
/*
 *  extract member '*m' from archive '*ar' to a file named
 *  'fname', if this pointer is non-null; otherwise, use the
 *  original name of the member file.
 *  if (verbose) perform the action verbosely
 */
{
   extractMember(ar,m,fname,verbose,1 /* preserve_dates */);
}  /* named_extract */

/*
 *
 *  $Id: arextrac.c,v 1.10 2007/06/01 19:46:03 bynum Exp $
 *
 */
