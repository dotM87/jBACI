#ifndef AR_EXTRACT
#define AR_EXTRACT
/*
 *  include file for public arextrac.c functions
 *  BACI archiver
 *     Bill Bynum
 *     May 2000
 */

#include "artables.h"

void extract_members(Archive* ar,char** files, int num_files,
                     int verbose, int preserve_dates);
/*
 *  extract members named in the list of files 'files' (there
 *  are 'num_files' filenames in the list) from the archive file
 *  described by '*ar'.
 *  if (verbose) perform the action verbosely
 *  if (preserve_dates) preserve the original mtime of the file
 *  else use current time for mtime
 */

void named_extract(Archive* ar, Member* m, char* fname, int verbose);
/*
 *  extract member '*m' from archive '*ar' to a file named
 *  'fname', if this pointer is non-null; otherwise, use the
 *  original name of the member file.
 *  if (verbose) perform the action verbosely
 */
 
/*
 *
 *  $Id: arextrac.h,v 1.8 2007/06/01 19:46:03 bynum Exp $
 *
 */
#endif
