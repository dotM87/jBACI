/*
 *  procs to write an archive for the BACI archiver
 *
 *    Bill Bynum
 *    May 2000
 */

#include <errno.h>
#include <stdio.h>
#if !defined(DOS)
#include <unistd.h>
#endif

#include "../include/genutil.h"

#include "ar.h"
#include "artables.h"
#include "arrdsyms.h"
#include "arutils.h"



/* for extracting a file or rewriting the archive */
char xfer_buf[XFERBUFSIZE];

static void write_long(FILE* naf, long value)
/* 
 * write a long to the archive file described by 'naf' at the
 * current file offset as a little-endian long or die trying
 */
{
   unsigned char c[4];
   c[0] = value & 0xff;
   c[1] = (value>>8)&0xff;
   c[2] = (value>>16)&0xff;
   c[3] = (value>>24)&0xff;
   if (fwrite(c,1,4,naf) != 4) {
      fatal("Cannot write long %lX at offset %lX",value,ftell(naf));
   }
}  /* write_long */

#if defined(DOS)

/* The fprintf of the Borland C++ 3.0 compiler sometimes outputs */ 
/* more characters than the width specifier calls for.  The next */
/* two procs are necessary to work around this problem.          */
static void write_buffer(FILE* naf, Member* m,char* buf, int writesize)
{
   int ret = fwrite(buf,writesize,1,naf);
   if (ret != 1) 
      fatal("Can't write %d bytes in ar header of %s at offset %ld\n",
         writesize, m->name,ftell(naf));
} /* write_buffer */
   
static void write_ar_hdr(FILE* naf, Member* m)
/*
 *  write the ar_hdr struct for the archive member pointed to by 'm'
 *  at the current offset of the file described by 'naf'
 */
{
   char buf[30];
   sprintf(buf,"%-16s",m->name);
   write_buffer(naf,m,buf,16);
   sprintf(buf,"%-12ld",m->mtime);
   write_buffer(naf,m,buf,12);
   sprintf(buf,"%-6d",m->uid);
   write_buffer(naf,m,buf,6);
   sprintf(buf,"%-6d",m->gid);
   write_buffer(naf,m,buf,6);
   sprintf(buf,"%-8lo",m->mode);
   write_buffer(naf,m,buf,8);
   sprintf(buf,"%-10ld",m->size);
   write_buffer(naf,m,buf,10);
   strcpy(buf,ARFMAG);
   write_buffer(naf,m,buf,SARFMAG);
}  /* write_ar_hdr */
#else
/* non-DOS version */
static void write_ar_hdr(FILE* naf, Member* m)
/*
 *  write the ar_hdr struct for the archive member pointed to by 'm'
 *  at the current offset of the file described by 'naf'
 */
{
   fprintf(naf,"%-16s",m->name);
   fprintf(naf,"%-12ld",m->mtime);
   fprintf(naf,"%-6d",m->uid);
   fprintf(naf,"%-6d",m->gid);
   fprintf(naf,"%-8lo",m->mode);
   fprintf(naf,"%-10ld",m->size);
   fprintf(naf,"%s",ARFMAG);
}  /* write_ar_hdr */
#endif

static void write_symdef(Archive* ar)
/*  
 *  write the __.SYMDEF archive member to the archive file described
 *  by 'naf'
 */
{
   PackedLong x;
   Symbol* ts;
   FILE* naf = ar->wf;
   write_ar_hdr(naf,ar->ml.head);
   if (ar->stab_size %2) ar->stab_size++; /* must be even */
   write_long(naf,ar->stab_size);
   /* first write the defined symbols */
   for (ts = ar->sl.head; ts; ts = ts->next) {
      fprintf(naf,"%s",ts->name);
      putc('\0',naf);            /* null terminator for name */
   }
   if (ar->sl.head) putc('\0',naf); /* final null terminator for string tab */
   /* next write the external symbols */
   for (ts = ar->el.head; ts; ts = ts->next) {
      fprintf(naf,"%s",ts->name);
      putc('\0',naf);            /* null terminator for name */
   }
   if (ar->el.head) putc('\0',naf); /* final null terminator for string tab */
   if (ar->ml.head->size % 2) putc('\012',naf); /* final \n, if needed      */
   write_long(naf,(long) SYMDEF_STRUCT_SIZE*ar->num_defsyms);
   for (ts = ar->sl.head; ts; ts = ts->next) {
      write_long(naf,(long) ts->name_offset);
      write_long(naf,(long) ts->member_offset);
      x.B[PLtyp] = (TYPES) ts->typ;
      x.B[PLobj] = (OBJTYPE) ts->obj;
      x.B[PLinmon] = (unsigned char) ts->inmon;
      x.B[PLunused] = (unsigned char) 0;
      write_long(naf,(long) x.l);
   }
   write_long(naf,(long) SYMDEF_STRUCT_SIZE*ar->num_extsyms);
   for (ts = ar->el.head; ts; ts = ts->next) {
      write_long(naf,(long) ts->name_offset);
      write_long(naf,(long) ts->member_offset);
      x.B[PLtyp] = (TYPES) ts->typ;
      x.B[PLobj] = (OBJTYPE) ts->obj;
      x.B[PLinmon] = (unsigned char) ts->inmon;
      x.B[PLunused] = (unsigned char) 0;
      write_long(naf,(long) x.l);
   }
}

static void writeMember(Archive* ar, Member* m)
/*  
 *  write the member of the archive described by '*ar' 
 *
 *  if there is no old archive, then ar-> rf == NO_ARCHIVE
 *  else ar->rf is the file descriptor for the open old archive file.
 *
 *  if this is a member that is not being changed, then
 *  ar-> rf is not NULL and is not NO_ARCHIVE (archive exists) 
 *  and m->ext_fname == NULL, so the member will be copied over 
 *  from the old archive into the new archive file
 *
 *  if this is a member that is coming from an external file,
 *  then m->ext_fname != NULL, and the member will be copied 
 *  from the extenal member file into the new archive file.
 */
{
   FILE* mf;
   FILE* naf = ar->wf;
   off_t tocopy;
   off_t thistime;
   FILE* afd  = ar->rf; 
   write_ar_hdr(naf,m);
   if (m->ext_fname)
      mf = open_member_file(m->ext_fname);
   else if (afd == NO_ARCHIVE) {   /* shouldn't happen */
      fatal("No archive from which to retrieve %s",m->name);
   }
   else
      fseek(afd,m->offset+sizeof(struct ar_hdr),SEEK_SET);
   tocopy = m->size;
   while (tocopy > 0) {
      thistime = tocopy;
      if (thistime > XFERBUFSIZE) thistime = XFERBUFSIZE;
      if (m->ext_fname) {
         if (fread(xfer_buf,1,thistime,mf) != thistime) {
            fatal("Cannot read member file at offset %lX",ftell(mf));
         }
      }
      else {
         if (fread(xfer_buf,1,thistime,afd) != thistime) {
            fatal("Cannot read archive file at offset %lX",ftell(afd));
         }
      }
      if (fwrite(xfer_buf,1,thistime,naf) != thistime) {
         fatal("Cannot write archive file at offset %lX",ftell(naf));
      }
      tocopy -= thistime;
   }
   if (m->size % 2) putc('\012',naf);  /* \n filler if needed */
}  /* writeMember */

void write_archive(Archive* ar)
/*
 *  write the new archive file to disk
 *
 *  if ar->rf== NO_ARCHIVE, no old archive exists;
 *  otherwise, ar->rf is the file descriptor of the (open) old archive file
 *  (having name 'ar->fname')
 */
{
   char* new_arfname;
   Member* tm;
   Member* start;
   if (ar->rf == NO_ARCHIVE)
      open_new_archive(ar,ar->fname);
   else {
      new_arfname = make_new_arfname(ar->fname);
      open_new_archive(ar,new_arfname);
   }
   fprintf(ar->wf,"%s",BARMAG);
   if (symdef_present(ar)) {
      start = ar->ml.head->next;
      if (start) write_symdef(ar);
   }
   else 
      start = ar->ml.head;
   for (tm = start; tm; tm = tm->next) writeMember(ar,tm);
   fclose(ar->wf);
   if ((ar->rf != NULL)&&(ar->rf != NO_ARCHIVE)) {  
      /* if there is an old archive file... */
      fclose(ar->rf);
      if (unlink(ar->fname) < 0) {
         fatal("Cannot remove old archive file %s",ar->fname);
      }
      if (rename(new_arfname,ar->fname) < 0) {
         fatal("Cannot rename temporary archive file %s",new_arfname);
      }
   }
}  /* write_archive */


/*
 *
 *  $Id: arwrite.c,v 1.11 2007/06/01 19:46:03 bynum Exp $
 *
 */
