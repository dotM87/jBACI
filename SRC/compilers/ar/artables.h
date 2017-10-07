#ifndef AR_TABLES
#define AR_TABLES
/*
 *  include file for artables.c functions
 *  BACI archiver
 *     Bill Bynum
 *     May 2000
 */

#include <sys/types.h>
#include <stdio.h>

#include "../include/bacicnty.h"

#define SYMDEF "__.SYMDEF"

   /* struct to hold the information from the member's ar_hdr */
typedef struct M{
   off_t    offset;           /* offset of this member in the old archive */
   off_t    new_offset;       /* offset of this member in the new archive */
   char     *name;            /* name of this member */
   char     *ext_fname;       /* external file name where member is stored */
   time_t   mtime;            /* mod time of member */
   uid_t    uid;              /* uid of member creator */
   uid_t    gid;              /* gid of member creator */
   long     mode;             /* mode of member        */
   off_t    size;             /* size of the member (not including ar_hdr) */
   struct M *next;            /* link to next member */
} Member ;

   /* struct to hold the information about each symbol from __.SYMDEF */
typedef struct S{
   char     *name;            /* name of the symbol */
   off_t    name_offset;      /* offset of the name in the string table */
   off_t    member_offset;    /* file offset of member owning this symbol */
                              /* as read from __.SYMDEF                  */
   TYPES    typ;              /* BACI type of this symbol */
   OBJTYPE  obj;              /* BACI object type of this symbol */
   unsigned char inmon;       /* 1 ==> proc or func declared in a monitor */
   Member   *m;               /* ptr to Member owning the symbol */
   struct S *next;            /* link to next symbol */
}Symbol;

typedef struct {
   Member*  head;
   Member*  tail;
      } MemberList;                 /* for lists of Members */

typedef struct {
   Symbol*  head;
   Symbol*  tail;
      } SymbolList;                 /* for lists of Symbols */

#define NO_ARCHIVE   (FILE *) 1     /* read fd if archive file dosen't exist */

   /* everything that ar and ld need to know about an archive */
typedef struct A{
      /* used for reading an existing archive */
   char*       fname;               /* archive file name */
   FILE*       rf;                  /* read file descriptor */
                                    /* NO_ARCHIVE if archive doesn't exist */
      /* used for writing a revised archive */
   char*       new_fname;           /* name of new archive file */
   FILE*       wf;                  /* write file descriptor for new archive */
      /* archive information */
   int         fsize;               /* size of the original archive file */
   MemberList  ml;                  /* current MemberList of the archive */
   SymbolList  sl;                  /* current SymbolList of defined syms */
   SymbolList  el;                  /* current SymbolList of external syms */
   long        num_defsyms;         /* # symbols defined by archive's members */
   long        num_extsyms;         /* # extern syms ref-ed by archive's members */
   long        stab_size;           /* size of string tab in symdef member */
   struct A*   next;                /* link to next archive (ld only)     */
} Archive;


   /* used in ar_extract and ar_write to write member files */
#define XFERBUFSIZE 8192
extern char xfer_buf[];

   /* used to write the BACI symdef struct information */
typedef union X{
      unsigned char B[4];
      long l;
} PackedLong;

/* array locs of the information */
#define PLtyp 0
#define PLobj 1
#define PLinmon 2
#define PLunused 3

int empty_archive(Archive* ar);
/*
 *  checks for the magic string and the __.SYMDEF name in the
 *  first file of the archive.  Aborts if things aren't right.
 *  returns 1 if archive is empty, 0 if not
 */

Symbol* newSymbol();
/*
 *  create a new Symbol
 */


Member* newMember();
/*
 *  create a new Member
 */

void appendMember(MemberList* ml, Member* m);
/*
 *  append Member pointed to by 'm' at the tail of the Member list
 *  pointed to by 'ml'
 */

void appendSymbol(SymbolList* sl, Symbol* s);
/*
 *  append Symbol pointed to by 's' at the tail of the Symbol list
 *  pointed to by 'sl'
 */

void read_all_members(Archive* ar);
/*
 *  read all members in the archive file described by 'ar'
 *  to the archive's internal Member list
 */

Member *findMember(MemberList* ml, Member** prev, char *memname);
/* 
 *    searches MemberList pointed to by 'ml' for a member named 'memname'
 *    returns ptr to first Member having name 'memname', if one was found
 *            0 otherwise
 *   'prev' points to member previous to Member found, if there was one, 
 *          else 'prev' is 0
 */

Member* findMemberBySymbol(Archive* ar, char* symname, OBJTYPE obj, TYPES typ);
/* 
 *    searches SymbolList pointed to by 'ar->sl' for a symbol named 
 *    'symname' with OBJTYPE 'obj' and of TYPES 'typ'
 *    returns a pointer to the member owning such a symbol, if one exists,
 *    otherwise, returns NULL (Member* 0).
 */    

int  symdef_present(Archive* ar);
/*
 *  returns 1  if the first Member in the Archive's MemberList is
 *             the __.SYMDEF member
 *          0  otherwise
 */

void read_symdef(Archive* ar);
/*
 *  read the __.SYMDEF member (the first) member of the archive
 *  create the internal SymbolList for the archive and add new 
 *  Members encountered in the process to the member list of
 *  the archive
 */

void add_symdef_member(Archive* ar, int verbose);
/*
 * creates a symdef member and adds it to the head of the 
 * ar->ml MemberList of the archive.  If 'verbose', then
 * performs the action verbosely
 */

void recalc_offsets(Archive* ar);
/* 
 *  recalculate the file offsets of all Members in the Archive
 *  struct pointed to by 'ar'.  Store results in all Member and 
 *  Symbol structs.
 */ 

void show_members(Archive* ar, char** files, int num_files, int verbose);
/*  
 * show information about members of the archive on stdout
 * top-level proc called in response to the 't' option
 * if (verbose) perform the action verbosely
 */   

void write_symbols(Archive* ar, int verbose);
/*
 *  writes all symbols in the archive described by 'ar'
 *  to stdout
 *  if (verbose) then the member name owning the symbol is listed, too
 *  if there is no SYMDEF member, then it is added before listing the symbols
 */

/*
 *
 *  $Id: artables.h,v 1.9 2007/06/01 19:46:03 bynum Exp $
 *
 */
#endif
