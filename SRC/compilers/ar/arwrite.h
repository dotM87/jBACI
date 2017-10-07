#ifndef AR_WRITE
#define AR_WRITE
/*
 *  include file for public arwrite.c functions
 *  BACI archiver
 *     Bill Bynum
 *     May 2000
 */

#include "artables.h"

void write_archive(Archive* ar);
/*
 *  write the new archive file to disk
 *
 *  if ar->rf== NO_ARCHIVE, no old archive exists;
 *  otherwise, ar->rf is the file descriptor of the (open) old archive file
 *  (having name 'ar->fname')
 */

/*
 *
 *  $Id: arwrite.h,v 1.7 2007/06/01 19:46:03 bynum Exp $
 *
 */
#endif
