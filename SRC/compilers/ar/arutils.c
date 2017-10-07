/*
 * utilities for the BACI archiver
 *    Bill Bynum
 *    May 2000
 */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#if !defined(DOS)
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdarg.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "../include/genutil.h"

#include "ar.h"
#include "artables.h"
#include "arrdsyms.h"

extern char *program_name;

int next_arstring(Archive* ar, char * buf, int len)
/*
 *  reads up to 'len' characters at the current file offset 
 *  in the file described by 'ar->rf' into the character buffer 'buf'.  
 *  This buffer is expected to be of length at least 'len' + 1, because 
 *  the string is terminated with a null byte as soon as either a blank is 
 *  encountered or 'len' characters are read.
 *  returns 0 on success, returns 1 on EOF, aborts otherwise
 */
{
   char *p;
   int i;
   FILE* fd = ar->rf;
   i = fread(buf,1,len,fd);
   if (i == 0) return 1;
   if (i != len) {
      fatal("Cannot read string at offset %lX in archive %s", 
         ftell(fd),ar->fname);
   }
   p = (char *) buf;
   i = 0;
   while ((*p != ' ')&&(i < len)) {
      p++; i++;
   }
   *p = '\0';
   return 0;
}  /* next_arstring */

static char *new_string(int len)
/*
 *  create space for a string of length 'len' in the heap and
 *  return a pointer to it
 */
{
   char *tmp;
   tmp = malloc(len+1);
   if (tmp == (char *)0) 
      fatal("Couldn't malloc string space");
   return tmp;
}  /* new_string */


char *save_string(char *s)
/* 
 *  save a string on the heap 
 */
{
   char *tmp;
   tmp = new_string(strlen(s));
   strcpy(tmp,s);
   return(tmp);
}

char* normalize(char* file)
/* 
 * local equivalent of 'basename', copied from GNU ar 
 * returns a pointer to the next char after the rightmost '/'
 * if there is one or a pointer to the beginning of the string
 */
{
   char* fname = strrchr(file,'/');
   if (fname != (char *) NULL)
      fname++;
   else
      fname = file;
   return fname;
}  /* normalize */

int read_arstring(Archive* ar, off_t offset, char *buf,  int len)
/*
 *  reads up to 'len' characters at a file offset 'offset' in the file 
 *  in the file described by 'ar->rf' into the character buffer 'buf'.  
 *  is expected to be of length at least 'len' + 1, because the string 
 *  is terminated with a null byte as soon as either a blank is 
 *  encountered or 'len' characters are read.
 *  returns 0 on success, returns 1 on EOF, aborts otherwise
 */
{
   int i;
   fseek(ar->rf,offset,SEEK_SET);
   i = next_arstring(ar,buf,len);
   return i;
}  /* read_arstring */

void open_archive(Archive* ar, char* arfname)
/*
 * If archive file exists, opens it for reading and sets its file descriptor
 * Sets file descriptor to NO_ARCHIVE if file doesn't exist.
 * Aborts for any other reason
 */
{
   FILE* fd;
   struct stat ar_stat;
   char ar_magic[SARMAG+1];
   ar->fname = save_string(arfname);
#if defined(DOS)
   if ((fd = fopen(arfname,"rb")) == NULL) {
#else
   if ((fd = fopen(arfname,"r")) == NULL) {
#endif
      if (errno == ENOENT) {
         ar->rf = NO_ARCHIVE;
      }
      else {
         fatal("Cannot open archive file '%s'", arfname);
      }
   }
   else {
      ar->rf = fd;
      if (stat(arfname,&ar_stat) < 0)
         fatal("Cannot stat archive file");
      ar->fsize = ar_stat.st_size;
      if (read_arstring(ar,0,ar_magic,SARMAG))
         fatal("Invalid archive file");
      if (strcmp(ar_magic,BARMAG) != 0) {
         fatal("Bad magic string '%s' in archive %s", ar_magic,ar->fname);
      }
   }
   ar->ml.head = ar->ml.tail = (Member*) 0;
   ar->el.head = ar->el.tail = ar->sl.head = ar->sl.tail = (Symbol*) 0;
   ar->num_defsyms = ar->num_extsyms = 0;
   ar->stab_size = 0;
   ar->next = (Archive*) 0;
}  /* open_archive */

void read_archive(Archive* ar)
/*
 * if 'ar' is non-NULL and '*ar' has been opened for reading, 
 * then reads archive information into the '*ar' struct;
 * otherwise, does nothing.
 * This proc is used by the linker, not the archiver.
 */
{
   if (ar == (Archive*) 0) return;
   if (ar->rf == NO_ARCHIVE) return;
   read_all_members(ar);
   read_symdef(ar);
}  /* read_archive */

void PositionToRead(Archive* ar, Member* m)
   /* position the read file descriptor so that member *m can be read */
{
    /* skip over the member's header */
   fseek(ar->rf,m->offset + sizeof(struct ar_hdr),SEEK_SET); 
}  /* PositionToRead */

void reopen_archive(Archive* ar, char* arfname)
/*
 * opens the archive file with 'open_archive'
 * aborts if archive file doesn't exist
 */
{
   open_archive(ar,arfname);
   if (ar->rf == NO_ARCHIVE){
      fatal("Archive file '%s' does not exist", arfname);
   }
}  /* reopen_archive */

char* make_new_arfname(char* arfname)
/*
 *  creates a name for the new archive file 'new_arfname' that is similar 
 *  to the name of the old archive file 'arfname'
 */
{
   char* t;
   t = new_string(strlen(arfname)+3);
   strcpy(t,arfname);
   strcat(t,"-a");
   return t;
}  /* make_new_arfname */

void open_new_archive(Archive* ar, char *new_arfname)
/*
 *  opens for writing an archive file with 'new_arfname' 
 *  and stores file pointer to the file in *ar
 *  'new_arfname' is assumed to point to non-volatile storage!!!
 *  aborts if there is a problem
 */
{
   FILE* naf;
#if defined(DOS)
   naf = fopen(new_arfname,"wb");
#else
   naf = fopen(new_arfname,"w");
#endif
   if (naf == NULL) {
      fatal("Cannot open file %s",new_arfname);
   }
   ar->wf = naf;
   ar->new_fname = new_arfname;
}  /* open_new_archive */

long read_long(FILE* fd)
/* 
 *  reads a long at current offset of file described by 'fd' 
 *  (AS LITTLE ENDIAN)
 *  or dies trying
 */
{
   long tmp;
   unsigned char c[4];
   if (fread(c,1,4,fd) != 4) {
      fatal("Cannot read long at offset %lX",ftell(fd)); 
   }
   tmp = (c[3]<<24)+(c[2]<<16)+(c[1]<<8)+c[0];
   return tmp;
}  /* read_long */

static char* typename[] =
   {"unknown","int","boolean","character","binary semaphore",
   "semaphore", "condition","array","","","rawstring", "string"};


void printSymbol(Symbol* s)
   /* prints information about symbol on stdout */
{
   char* p = (s->inmon ? "monitor " : "" );
   switch (s->obj) {
      case procedure:
         printf("%svoid function %s",p,s->name);
         break;
      case function:
         printf("%s%s function %s",p,typename[s->typ],s->name);
         break;
      case ext_procedure:
         printf("external %svoid function %s",p,s->name);
         break;
      case ext_function:
         printf("external %s%s function %s",p,typename[s->typ],s->name);
         break;
      case ext_variable:
         printf("external %s variable %s",typename[s->typ],s->name);
         break;
      case variable:
         printf("%s variable %s",typename[s->typ],s->name);
         break;
      case monitor:
         printf("monitor %s",s->name);
         break;
      case mainproc:
         printf("mainproc %s",s->name);
         break;
      case ext_monitor:
         printf("external monitor %s",s->name);
         break;
      case constant:
         printf("constant %s %s", typename[s->typ], s->name);
         break;
      case type:
         printf("type %s",s->name);
         break;
      default: ;  /* print nothing */
   }
}

/*
 *
 *  $Id: arutils.c,v 1.16 2007/06/01 19:46:03 bynum Exp $
 *
 */
