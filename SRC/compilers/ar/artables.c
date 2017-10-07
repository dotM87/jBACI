/*
 *  source code to manipulate internal tables
 *  for the BACI archiver
 *    Bill Bynum
 *    May 2000
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(DOS)
#include <unistd.h>
#include <sys/time.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>

#include "../include/genutil.h"

#include "ar.h"
#include "artables.h"
#include "arutils.h"
#include "arwrite.h"
#include "arrdsyms.h"

int empty_archive(Archive* ar)
/*
 *  returns 1 if archive either doesn't exist or is empty, else returns 0
 */
{
   if (ar->rf == NO_ARCHIVE) return 1; /* no archive */
   if (ar->fsize == SARMAG) return 1;  /* empty archive */
   return 0;
}  /* empty_archive */

Symbol* newSymbol() 
/* 
 *  create a new Symbol 
 */
{
   Symbol* tmp;
   if ((tmp = (Symbol *) calloc(1,sizeof(Symbol))) == (Symbol *) 0) 
      fatal("can't allocate space for a new Symbol");
   return tmp;
}  /* newSymbol */

Member* newMember() 
/* 
 *  create a new Member 
 */
{
   Member* tmp;
   if ((tmp = (Member *) calloc(1,sizeof(Member))) == (Member *) 0)
      fatal("can't allocate space for a new Member");
   return tmp;
}  /* newMember */

char *new_arname(Archive* ar, off_t member_off)
/*  
 *  make a copy on the heap of the name of the archive member at 
 *  file offset 'member_off' of the archive file described by 'ar'
 */
{
   char b[17];
   if (read_arstring(ar,member_off,b,16)) {
      fatal("Unexpected EOF in archive %s at offset %lX", 
         ar->fname,ftell(ar->rf));
   }
   return (save_string(b));
}  /* new_arname */

void appendMember(MemberList* ml, Member* m)
/*
 *  append Member pointed to by 'm' at the tail of the Member list
 *  pointed to by 'ml'
 */
{
   if (ml->tail) {
      ml->tail->next = m;
      ml->tail = m;
   }
   else
      ml->head = ml->tail = m;
}  /* appendMember */

void appendSymbol(SymbolList* sl, Symbol* s)
/*
 *  append Symbol pointed to by 's' at the tail of the Symbol list
 *  pointed to by 'sl'
 */
{
   if (sl->tail) {
      sl->tail->next = s;
      sl->tail = s;
   }
   else
      sl->tail = sl->head = s;
}  /* appendSymbol */

static Member* readMember(Archive* ar, off_t member_off)
/* 
 *  read the Member information for an archive member with ar_hdr at 
 *  file offset 'member_off' of the archive file described by 'ar'
 *  into the archive's MemberList, if the Member hasn't been seen before.
 *  return a pointer to the Member struct
 */
{
   Member *tm;
   char tmp[80];
   tm = newMember();
   tm->name = new_arname(ar,member_off);
   tm->offset = member_off;
   next_arstring(ar,tmp,12);        /* get mtime */
   sscanf(tmp,"%ld",&tm->mtime);
   next_arstring(ar,tmp,6);         /* get uid */
   tm->uid = 0;                      // skip uid
   //sscanf(tmp,"%d",&tm->uid);
   next_arstring(ar,tmp,6);         /* get gid */
   tm->gid = 0;
   // sscanf(tmp,"%d",&tm->gid);    // skip gid
   next_arstring(ar,tmp,8);         /* get mode */
   sscanf(tmp,"%lo",&tm->mode);
   next_arstring(ar,tmp,10);        /* get size */
   sscanf(tmp,"%ld",&tm->size);
   appendMember(&ar->ml,tm);
   return tm;
}  /* readMember */

void read_all_members(Archive* ar)
/*
 *  read all members in the archive file described by 'ar'
 *  to the archive's internal Member list
 */
{
   Member *tm;
   off_t next_offset = SARMAG;
   while (next_offset < ar->fsize) {
      tm = readMember(ar,next_offset);
      next_offset = next_offset + tm->size + sizeof(struct ar_hdr);
      if (next_offset % 2) next_offset++; /* member offsets must be even */
   }
}  /* read_all_members */

Member *findMember(MemberList* ml, Member** prev, char *memname)
/* 
 *    searches MemberList pointed to by 'ml' for a member named 'memname'
 *    returns ptr to first Member having name 'memname', if one was found
 *            0 otherwise
 *   'prev' points to member previous to Member found, if there was one, 
 *          else 'prev' is 0
 */
{
   Member *tm;
   for(tm = ml->head,*prev = 0; tm; *prev = tm,tm = tm->next)
      if (strcmp(tm->name,memname) == 0) return tm;
   return 0;
}  /* findMember */

Member* findMemberBySymbol(Archive* ar, char* symname, OBJTYPE obj, TYPES typ)
/* 
 *    searches SymbolList pointed to by 'ar->sl' for a symbol named 
 *    'symname' with OBJTYPE 'obj' and of TYPES 'typ'
 *    returns a pointer to the member owning such a symbol, if one exists,
 *    otherwise, returns NULL (Member* 0).
 */    
{
   Symbol* ts;
   for(ts = ar->sl.head; ts; ts = ts->next)
      if (strcmp(ts->name,symname) == 0) {
         if ((ts->typ == typ) && (ts->obj == obj) && (ts->inmon == 0))
            return ts->m;  /* return ptr to member who owns symbol */
      }
   return (Member *) 0;    /* wasn't there -- return NULL */
}  /* findMemberBySymbol */

static Member *locateMember(Archive* ar, off_t offset)
/* 
 *    searches MemberList pointed to by 'ar->ml' for a member having
 *    offset 'offset'.  If one is found returns pointer to Member
 *    else reads Member at that offset into the MemberList and
 *    returns a ptr to it
 */
{
   Member *tm;
   for(tm = ar->ml.head; tm; tm = tm->next)
      if (tm->offset == offset) return tm;
   return readMember(ar,offset);
}  /* locateMember */

int  symdef_present(Archive* ar)
/*
 *  returns 1  if the first Member in the Archive's MemberList is
 *             the __.SYMDEF member
 *          0  otherwise
 */ 
{
   int k,tmp = 0;
   if (ar->ml.head) {
      k = strcmp(ar->ml.head->name,SYMDEF); 
      tmp = (k==0);
   }
   return tmp;
}  /* symdef_present */
      
#define READ_DEFSYMS 1
#define READ_EXTSYMS 0

static void read_symdef_structs(Archive* ar, int readsyms, off_t stab_offset)
/* if readsyms == 1, then is reading list of defined symbols  */
/*                   else is reading list of external symbols */
/* stab_offset is the byte offset in the archive file to the string table */
{
   off_t tot_symdef_size; /* size of the symdef struct portion being read */
   off_t bytes_read;
   off_t curr_offset;
   Symbol *tmp;
   FILE* fd = ar->rf;
   PackedLong x;
   tot_symdef_size = (long) read_long(fd);
   if (readsyms == READ_DEFSYMS) 
      ar->num_defsyms = tot_symdef_size / SYMDEF_STRUCT_SIZE;
   else
      ar->num_extsyms = tot_symdef_size / SYMDEF_STRUCT_SIZE;
   for(bytes_read = 0; bytes_read < tot_symdef_size; 
      bytes_read += SYMDEF_STRUCT_SIZE) {
      tmp = newSymbol();
      tmp->name_offset = (long) read_long(fd);
      tmp->member_offset = (long) read_long(fd);
      x.l = (long) read_long(fd);
      tmp->typ = (TYPES) x.B[PLtyp]; 
      tmp->obj= (OBJTYPE) x.B[PLobj]; 
      tmp->inmon = x.B[PLinmon]; 
      curr_offset = ftell(fd);      /* must remember curr offset */
      tmp->m = locateMember(ar,tmp->member_offset);
      tmp->name = new_arname(ar,stab_offset+tmp->name_offset);
      if (readsyms == READ_DEFSYMS) {
         switch (tmp->obj) {
            case ext_procedure:
            case ext_function:
            case ext_monitor:
            case ext_variable:
               fatal("External symbol %s @ offset 0x%X when reading defsyms",
                  tmp->name,curr_offset);
               break;
            default:
               appendSymbol(&ar->sl,tmp);
         }
      }
      else {
         switch (tmp->obj) {
            case ext_procedure:
            case ext_function:
            case ext_monitor:
            case ext_variable:
               appendSymbol(&ar->el,tmp);
               break;
            default:
               fatal("Defined symbol %s @ offset 0x%X when reading extsyms",
                  tmp->name,curr_offset);
               break;
         }  /* switch */
      }
      fseek(fd,curr_offset,SEEK_SET);
   }  /* for loop */
}  /* read_symdef_structs */

void read_symdef(Archive* ar)
/*
 *  read the __.SYMDEF member (the first) member of the archive
 *  create the internal SymbolList for the archive 
 */
{
   off_t stab_offset;
   if (ar->fsize == SARMAG) return; /* empty archive */
   /* position to read the defined symdef structs */
   fseek(ar->rf,SARMAG+sizeof(struct ar_hdr),SEEK_SET); 
   ar->stab_size = (long) read_long(ar->rf);
   stab_offset = SARMAG+sizeof(struct ar_hdr) + sizeof(long);
   /* position the file ptr to read the symdef structs */
   fseek(ar->rf,(long)(stab_offset+ar->stab_size),SEEK_SET);
   read_symdef_structs(ar,READ_DEFSYMS,stab_offset);
   read_symdef_structs(ar,READ_EXTSYMS,stab_offset);
   /* fix mode of __.SYMDEF entry to 0644 */
   if (ar->ml.head)
      if (ar->ml.head->mode == 0) ar->ml.head->mode = 0100644;
}  /* read_symdef */

void add_symdef_member(Archive* ar, int verbose)
/*
 * creates a symdef member and adds it to the head of the 
 * ar->ml MemberList of the archive.  If 'verbose', then
 * performs the action verbosely
 */
{
   Member *tm;
   tm = newMember();
   tm->name = save_string(SYMDEF);
   tm->mode = 0100644;
   tm->uid = tm->gid = 0;
   tm->mtime = time( (time_t *) 0);
   if (ar->ml.head) {   /* non-empty MemberList, add symdef at the head */
      tm->next = ar->ml.head;
      ar->ml.head = tm;
   }
   else {               /* empty MemberList, symdef IS the head & tail */
      ar->ml.head = ar->ml.tail = tm;
   }
   if (verbose)
      printf("Adding SYMDEF member (the archive symbol table)\n");
}  /* add_symdef_member */


static void resize_stringtab(Archive* ar)
/*
 *  calculate current size of string table and the number of strings
 *  in it and store this information in the Archive struct pointed to
 *  by 'ar'.
 */
{
   long s_off = 0;
   long tmp = 0;
   Symbol* ts;
   if (ar->sl.head) {
      for (ts = ar->sl.head; ts; ts = ts->next) {
         tmp++;
         ts->name_offset = s_off;
         s_off += (strlen(ts->name) + 1);
      }
      s_off++;    /* last null after last string */     
   }
   ar->num_defsyms = tmp;
   tmp = 0;
   if (ar->el.head) {
      for (ts = ar->el.head; ts; ts = ts->next) {
         tmp++;
         ts->name_offset = s_off;
         s_off += (strlen(ts->name) + 1);
      }
      s_off++;    /* last null after last string */     
   }
   ar->num_extsyms = tmp;
   ar->stab_size = s_off;
}  /* resize_stringtab */

void recalc_offsets(Archive* ar)
/* 
 *  recalculate the file offsets of all Members in the Archive
 *  struct pointed to by 'ar'.  Store results in all Member and 
 *  Symbol structs.
 */ 
{
   Member* tm;
   Symbol* ts;
   off_t m_offset = SARMAG;
   resize_stringtab(ar);
   if (!ar->ml.head) return;      /* no members left, nothing else to do */
      /* establish size of 'new' symdef member */
   if (symdef_present(ar)) 
      ar->ml.head->size = 
            /* the symdef structs */
            + SYMDEF_STRUCT_SIZE*(ar->num_defsyms+ar->num_extsyms)
            + 3*sizeof(long) /* the three size longs: stab, defsyms, extsyms */
            + ar->stab_size;         /* the strings themselves */ 
   for (tm = ar->ml.head; tm; tm = tm->next) {
      tm->new_offset = m_offset;
      m_offset += (tm->size + sizeof(struct ar_hdr));
      if (m_offset % 2) m_offset++;    /* members on even boundaries */
   }
   for (ts = ar->sl.head; ts; ts = ts->next)
      ts->member_offset = ts->m->new_offset;
   for (ts = ar->el.head; ts; ts = ts->next)
      ts->member_offset = ts->m->new_offset;
}  /* recalc_offsets */

static void print_mode(long m)
/* 
 *  convert the mode integer 'm' to rwx format on stdout 
 *  Starting with the Java version in July 2006, only 
 *  owner "r" and "w" flags are displayed.
 */
{
   if (m&0400)          /* owner readable? */
      printf("yes   ");
   else
      printf(" no   ");
   if (m&0200)          /* owner writeable? */
      printf("yes  ");
   else
      printf(" no  ");
}  /* print_mode */

static void printMember(Member* tm, int verbose)
/* 
 *  print information about the Member pointed to by 'tm' on stdout 
 *  if (verbose) perform the action verbosely
 */
{
   char* tp;
   char timebuf[40];
   if (verbose){
      print_mode(tm->mode);
      printf(" %10ld",tm->size);
      tp = ctime(&tm->mtime);
	   sprintf (timebuf, "%.12s %.4s", tp + 4, tp + 20);
      printf(" %s %s\n",timebuf,tm->name);
   }
   else
      printf("%s\n",tm->name);
}  /* printMember */

void show_members(Archive* ar, char** files, int num_files, int verbose)
/* 
 * show information about members of the archive on stdout
 * top-level proc called in response to the 't' option
 * if (verbose) perform the action verbosely
 */
{
   Member* tm;
   Member* prev;
   read_all_members(ar);
   if (verbose) {
      printf("read write        size\n");
      printf("able  able       bytes   last modified   file name\n");
      printf("--------------------------------------------------------\n");
   }
   else {
      printf("file name\n");
      printf("---------------\n");  /* member names can be 15 chars */
   }
   if (num_files) {
      while (num_files) {
         tm = findMember(&ar->ml,&prev,*files);
         if (tm == 0){
            nonfatal("%s not in archive",*files);
         }
         else 
            printMember(tm,verbose);
         files++;
         num_files--;
      }
   }
   else {   /* show all members */
      for (tm = ar->ml.head; tm; tm = tm->next) 
         printMember(tm,verbose);
   }               
}  /* show_members */

void write_symbols(Archive* ar, int verbose)
/*
 *  writes all symbols in the archive described by 'ar'
 *  to stdout
 *  if (verbose) then the member name owning the symbol is listed, too
 *  if there is no SYMDEF member, then it isadded before listing the symbols
 */
{
   Symbol* ts;
   read_all_members(ar);
   if (symdef_present(ar)) 
      read_symdef(ar);
   else {   /* no symdef */
      add_symdef_member(ar,verbose);
      collectArchiveSymbols(ar,verbose);
      recalc_offsets(ar);
      write_archive(ar);
   }  /* else no symdef */
   if ((ar->sl.head)||(ar->el.head)) {
      if (verbose) printf("Archive index\n");
      if (ar->sl.head) {
         printf("Declared symbols\n");
         for (ts = ar->sl.head; ts; ts = ts-> next)
            if (verbose) {
               printSymbol(ts); printf(" in %s\n",ts->m->name);
            }
            else
               printf("%s\n",ts->name);
      }
      if (ar->el.head) {
         printf("External symbols\n");
         for (ts = ar->el.head; ts; ts = ts-> next)
            if (verbose) {
               printSymbol(ts); printf(" in %s\n",ts->m->name);
            }
            else
               printf("%s\n",ts->name);
      }
   }
   else  /* no symbols in archive */
      if (verbose) printf("Archive has no symbols\n");
}  /* write_symbols */


/*
 *
 *  $Id: artables.c,v 1.13 2007/06/01 19:46:03 bynum Exp $
 *
 */
