#ifndef AR_UTILS
#define AR_UTILS
/*
 *  include file for arutils.c functions
 *  BACI archiver
 *     Bill Bynum
 *     May 2000
 */

#include <sys/types.h>
#include <stdio.h>
#include "artables.h"

char* new_string(int len);
/*
 *  create space for a string of length 'len' in the heap and
 *  return a pointer to it
 */

char* save_string(char* s);
/* 
 *  save a string on the heap 
 */

char* normalize(char* file);
/* 
 * local equivalent of 'basename', copied from GNU ar 
 * returns a pointer to the next char after the rightmost '/'
 * if there is one or a pointer to the beginning of the string
 */

void open_archive(Archive* ar, char* arfname);
/*
 * If archive file exists, opens it for reading and sets its file descriptor
 * Sets file descriptor to -1 if file doesn't exist.
 * Aborts for any other reason
 */
 
void read_archive(Archive* ar);
/*
 * if 'ar' is non-NULL and '*ar' has been opened for reading, 
 * then reads archive information into the '*ar' struct;
 * otherwise, does nothing
 */

void PositionToRead(Archive* ar, Member* m);
   /* position the read file descriptor so that member *m can be read */
 
void reopen_archive(Archive* ar, char* arfname);
/*
 * opens the archive file with 'open_archive'
 * aborts if archive file doesn't exist
 */

char* make_new_arfname(char* arfname);
/*
 *  creates a name for the new archive file 'new_arfname' that is similar 
 *  to the name of the old archive file 'arfname'
 */

void open_new_archive(Archive* ar, char* new_arfname);
/*
 *  opens for writing an archive file with 'new_arfname' 
 *  and stores file pointer to the file in *ar
 *  'new_arfname' is assumed to point to non-volatile storage!!!
 *  aborts if there is a problem
 */

int next_arstring(Archive* ar, char* buf, int len);
/*
 *  reads up to 'len' characters at the current file offset 
 *  in the file described by 'ar->rf' into the character buffer 'buf'.  
 *  This buffer is expected to be of length at least 'len' + 1, because 
 *  the string is terminated with a null byte as soon as either a blank is 
 *  encountered or 'len' characters are read.
 *  returns 0 on success, returns 1 on EOF, aborts otherwise
 */

int read_arstring(Archive* ar, off_t offset, char *buf,  int len);
/*
 *  reads up to 'len' characters at a file offset 'offset' in the file 
 *  in the file described by 'ar->rf' into the character buffer 'buf'.  
 *  is expected to be of length at least 'len' + 1, because the string 
 *  is terminated with a null byte as soon as either a blank is 
 *  encountered or 'len' characters are read.
 *  returns 0 on success, returns 1 on EOF, aborts otherwise
 */

long read_long(FILE* fd);
/* 
 *  reads a long at current offset of file described by 'fd' 
 *  or dies trying
 */

void printSymbol(Symbol* s);
   /* prints information about symbol to stdout */

/*
 *
 *  $Id: arutils.h,v 1.10 2007/06/01 19:46:03 bynum Exp $
 *
 */
#endif
