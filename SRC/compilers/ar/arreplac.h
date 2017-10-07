#ifndef AR_REPLACE
#define AR_REPLACE
/*
 *  include file for public arreplac.c functions
 *  BACI archiver
 *     Bill Bynum
 *     May 2000
 */

void replace_members(Archive* ar,char** files,int num_files,int verbose);
/*  
 *  replace members named in the list of files 'files' (there
 *  are 'num_files' filenames in the list) from the archive file
 *  described by '*ar'
 *
 *  if (verbose) perform the action verbosely
 *
 *  if (num_files > 0) and there is no SYMDEF member, then add one
 */

/*
 *
 *  $Id: arreplac.h,v 1.6 2007/06/01 19:46:03 bynum Exp $
 *
 */
#endif
