#ifndef AR_DELETE
#define AR_DELETE
/*
 *  include file for public ardelete.c functions
 *  BACI archiver
 *     Bill Bynum
 *     May 2000
 */
#include "artables.h"

void deleteMember(Archive* ar, Member* tm, Member* prev, int verbose);
/*
 *  delete the member pointed to by 'tm' from the MemberList of tghe
 *  archive pointed to by 'ar'
 *  'prev' points to the member of the Member list prior to 
 *  the Member to be deleted
 *  if (verbose) perform the action verbosely
 */

void delete_members(Archive* ar, char** files, int num_files, int verbose);
/*
 *  delete members named in the list of files 'files' (there
 *  are 'num_files' filenames in the list) from the archive file
 *  described by '*ar'
 *  if (verbose) perform the action verbosely
 *  if no SYMDEF member is present, then add it.
 */

/*
 *
 *  $Id: ardelete.h,v 1.8 2007/06/01 19:46:03 bynum Exp $
 *
 */
#endif
