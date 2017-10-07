/* modification of the archive file definition (ar.h) for GNU software 
 * used by the BACI archiver
 *    Bill Bynum
 *    May 2000
 */

#ifndef __BACI_AR_H__
#define __BACI_AR_H__

/* Note that the usual '\n' in magic strings may translate to different
   characters, as allowed by ANSI.  '\012' has a fixed value, and remains
   compatible with existing BSDish archives. */

#define BARMAG "!<BACI>\012"	/* For BACI archives */
#define ARMAG  "!<arch>\012"	/* For COFF and a.out archives */
#define ARMAGB "!<bout>\012"	/* For b.out archives */
#define SARMAG 8
#define ARFMAG "`\012"
#define SARFMAG 2

/* The ar_date field of the armap (__.SYMDEF) member of an archive
   must be greater than the modified date of the entire file, or
   BSD-derived linkers complain.  We originally write the ar_date with
   this offset from the real file's mod-time.  After finishing the
   file, we rewrite ar_date if it's not still greater than the mod date.  */


#define ARM_NAME_OFFSET    0
#define ARM_DATE_OFFSET    16
#define ARM_UID_OFFSET     28
#define ARM_GID_OFFSET     34
#define ARM_MODE_OFFSET    40
#define ARM_SIZE_OFFSET    48
#define ARM_FMAG_OFFSET    58

struct ar_hdr {
  char ar_name[16];		/* name of this member */
  char ar_date[12];		/* file mtime */
  char ar_uid[6];		/* owner uid; printed as decimal */
  char ar_gid[6];		/* owner gid; printed as decimal */
  char ar_mode[8];		/* file mode, printed as octal   */
  char ar_size[10];		/* file size, printed as decimal */
  char ar_fmag[2];		/* should contain ARFMAG */
};
/* sizeof(struct ar_hdr) = 60 bytes */
/*
   If one of the member names is longer than 16 chars, then GNU
   ar utility creates the second member of the archive (after __.SYMDEF) 
   named "ARFILENAMES/" that has an otherwise blank header, followed 
   by the member names of the archive in the order that the members
   appear in the archive.  The Sun ar utility simply truncates the
   name to 15 chars with one trailing blank.  This is the algorithm
   that is used here.
*/

#define MAX_AR_NAME 15

/*       
               Format of the baar archive

   The archive can be in several states:

1. EMPTY (no members)

   The archive consists of only BARMAG

2. NON-EMPTY and positive number of symbols 

   BARMAG, followed by the members of the archive.
   
   a. The first member of the archive named "__.SYMDEF".
      This member has the following format:

      (i)   an ar_hdr struct describing the member
      (ii)  a 4-byte longword containing the length of the table of 
            symbol name strings
      (iii) zero or more symbol name strings (each followed by a null
            terminating byte). 
      (iv)  a 4-byte longword containing the size of the 'symdef' 
            structs of symbols defined in members of the archive 
      (v)   zero or more such 'symdef' structs
      (vi)  a 4-byte longword containing the size of the 'symdef' 
            structs of external symbols referenced in members of 
            the archive
      (vii) zero or more such 'symdef' structs

      struct symdef {    size = 12 bytes 
          unsigned long string_offset; 
               file offset to symbol name in the string table (see iii above)
          unsigned long file_offset;
               the file offset to the beginning of the struct ar_hdr
               of the member owning this symbol
          unsigned char B[4]; 
            where
               B[0]  BACI typ field
               B[1]  BACI obj field
               B[2]  is in a monitor? (1 or 0)
               B[3]  unused
            };

      This symdef format was adopted to facilitate the work that the BACI
      archiver and linker must do.  The format differs from the Sun4
      and GNU ar formats.  Only the symdef structs for the symbols
      defined by members in the archive are included.  

   b. Subsequent members of the archive have the format:
      (i)   an ar_hdr struct describing the member
      (ii)  the content of the member

3. NON-EMPTY, no global symbols
   This is a rare situation in which no archive member has any
   globally visible symbols, and are thus useless for linking.

   In this case, the archive consists of BARMAG, the __.SYMDEF member,
   and the three long words of (a), (c), and (e), all zero (a total of
   8 + 60 + 3*4 = 80 bytes).

*/

#define SYMDEF_STRUCT_SIZE 12    /* 3 * sizeof(long) */
#define STRINGTABSIZE_OFFSET 68  /* SARMAG + sizeof(struct ar_hdr) __.SYMDEF */

#if defined(DOS)
#ifndef off_t
#define off_t long int
#endif
#ifndef uid_t
#define uid_t int
#endif
#ifndef mode_t
#define mode_t int
#endif
#endif

#endif /* __BACI_AR_H__ */

/*
 *
 *  $Id: ar.h,v 1.7 2007/06/01 19:46:03 bynum Exp $
 *
 */
