/*
 * archiver program for the BACI system
 *    Bill Bynum
 *    May 2000
 */

#include <fcntl.h>
#include <stdlib.h>
#if !defined(DOS)
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "../include/genutil.h"

#include "ar.h"
#include "ardelete.h"
#include "arextrac.h"
#include "arrdsyms.h"
#include "arreplac.h"
#include "artables.h"
#include "arutils.h"
#include "arwrite.h"
/*
  the command line parsing is borrowed from the GNU ar.c source
  in the binutils package
*/

/* flags & globals */

char program_name[] = {"baar"};
char program_version[128];
extern char comp_proot[];
extern char date_string[];

int   show_version = 0;

int verbose = 0;              /* 1 -> describe each action performed */
int silent_create = 0;        /* 1 -> don't talk about creating archive file */ 
int preserve_dates = 0;       /* 1 -> when extracting, preserve file dates  */ 


/* 'operation' set by command line options */
enum { 
   none = 0, delete, replace, print_table, extract, write_syms 
     } operation = none;

char *archive_name;        /* name of the archive */ 

char **files;

Archive ar;

void do_show_version ()
{
  printf("%s\n", program_version);
  exit (0);
}  /* do_show_version */

void usage (int ecode)
{
   printf("%s\n",program_version);
   printf("Usage: %s [-]{drtwxh}[covV] archive-file file1 file2 ...\n",program_name);
   printf("  commands (at least one must be present):\n");
   printf("    d     delete named files from the archive\n"); 
   printf("    r     insert files into archive (with Replacement)\n"); 
   printf("    t     display list of files in the archive\n"); 
   printf("    w     list all symbols in the archive\n"); 
   printf("    x     extract named file from the archive\n"); 
   printf("    h     show this help and exit\n");
   printf("  optional modifiers:\n");
   printf("    c     if archive didn't exist previously, create the new archive silently\n"); 
   printf("    o     preserve the original dates of extracted files\n"); 
   printf("    v     do everything verbosely\n"); 
   printf("    V     show version number of program and exit\n"); 
   exit(ecode);
}  /* usage */


int main(int argc, char** argv)
{
   char *arg_ptr;
   char c;
   int num_files;

   sprintf(program_version,"%s%s  PCODE archiver in C, %s",comp_proot,program_name,date_string);
   if (argc < 2) usage(1);
   arg_ptr = argv[1];
   if (*arg_ptr == '-') ++arg_ptr;        /* compatibility */
   while ((c = *arg_ptr++) != '\0') {
      switch (c) {
      case 'd':
      case 'r':
      case 't':
      case 'x':
      case 'w':
      case 'h':
         if (operation != none)
            fatal ("two different operation options specified");
         switch (c) {
         case 'd':
            operation = delete;
            break;
         case 'r':
            operation = replace;
            break;
         case 't':
            operation = print_table;
            break;
         case 'w':
            operation = write_syms;
            break;
         case 'x':
            operation = extract;
            break;
         case 'h':
            usage(0);
         }  /* inner switch */
         break;
      case 'c':
         silent_create = 1;
         break;
      case 'o':
         preserve_dates = 1;
         break;
      case 'V':
         show_version = 1;
         break;
      case 'v':
         verbose = 1;
         break;
      case 'l':
      case 'm':
      case 'p':
      case 'q':
      case 'u':
      case 'a':
      case 'b':
      case 'i':
         printf ("%s: unimplemented option -- %c\n", program_name, c);
         usage(1);
         break;
      default:
         printf ("%s: illegal option -- %c\n", program_name, c);
         usage (1);
      }  /* outer switch */
   }  /* while */ 
   if (show_version) do_show_version ();
   if (argc < 3) usage (1);
   archive_name = argv[2];
   num_files = (argc > 3) ? argc - 3 : 0;
   files = (num_files) ? argv + 3 : NULL;
   switch (operation)
   {
   case none:
      printf("No operation specified!\n");
      exit(1);
      break;

   case write_syms:
      reopen_archive(&ar,archive_name);
      if (empty_archive(&ar)) {
         if (verbose) printf("Archive is empty\n");
      }
      else 
         write_symbols(&ar,verbose);
      break;

   case print_table:
      reopen_archive(&ar,archive_name);
      if (empty_archive(&ar)) {
         if (verbose) printf("Archive is empty\n");
      }
      else
         show_members(&ar,files,num_files,verbose);
     break;

   case extract:
      reopen_archive(&ar,archive_name);
      if (empty_archive(&ar)) {
         if (verbose) printf("Archive is empty\n");
      }
      else
         extract_members(&ar,files,num_files,verbose,preserve_dates);
     break;

   case delete:
      reopen_archive(&ar,archive_name);
      if (empty_archive(&ar)) {
         if (verbose) printf("Archive is empty\n");
      }
      else if (num_files) {
         delete_members(&ar,files,num_files,verbose);
      }
      else
         if (verbose) printf("No archive members specified\n");
      break;

   case replace:
      open_archive(&ar,archive_name);
      if ((ar.rf == NO_ARCHIVE)&&(!silent_create)) 
         printf("Creating archive %s\n",archive_name);
      replace_members(&ar,files,num_files,verbose);
      break;

     /* Shouldn't happen! */
   default:
      printf ("%s: Internal error -- this option not implemented\n",
         program_name);
      exit (1);
   }  /* switch(operation) */
  return 0;
}

/*
 *
 *  $Id: armain.c,v 1.15 2007/06/01 19:46:03 bynum Exp $
 *
 */
