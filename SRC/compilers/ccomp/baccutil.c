/* utility pgms for BenAri Concurrent C Compiler */

#include <stdlib.h>        /* for malloc  */
#include <time.h>          /* for ctime   */
#include <sys/stat.h>      /* for stat    */
#include <sys/types.h>     /* for stat    */
#include <stdio.h>
#include <string.h>

#include "../include/globdata.h"
#include "../include/globtabs.h"
#include "../include/compdata.h"
#include "../include/computil.h"
#include "../include/pcode.h"
#include "../include/incfiles.h"
#include "../include/writetab.h"

extern   int fprintf(FILE*,const char*,...);

extern   char  lastident[];

extern   char  comp_tail[];
extern   char  date_string[];
FNAME_STRING source_prefix;

extern   int   char_loc;
extern   int   int_loc;

extern   int   making_objfile;

void  show_help(char **argv)
{
   fprintf(stderr,"Usage:   %s [optional_flags]  source_filename\n",argv[0]);
   fprintf(stderr,"Optional flags:\n");
   fprintf(stderr, "   -h  show this help\n");
   fprintf(stderr, "   -c  make a .pob object file for subsequent linking\n");
   fprintf(stderr, "The name of the source file is required.  If missing, you\n");
   fprintf(stderr, "will be prompted for it.  The file suffix \"%s\" will\n",
         source_suffix);
   fprintf(stderr, "be appended to the filename if you don't supply it.\n");
}  /* show_help */

void  get_command_options(int argc,char **argv)
{
int   aix;
char  c;
   /* open the object file */
   if (argc == 1) 
      source_prefix[0] = '\0';
   else {
      /*  process options on command line */
      for (aix= 1;aix < argc;aix++)
      {
         if (argv[aix][0] != '-')
            break;      /* all done with options */
         /* otherwise argv[aix] is '-'*/
         c = argv[aix][1];
         switch (c)
         {
            case 'h': show_help(argv); break;
            case 'c': making_objfile = 1; break;
            default:
               fprintf(stderr,"Invalid option '%s'\n",argv[aix]);
         }  /* switch */
      } /* for processing options */
      if (aix < argc) 
         strcpy(source_prefix,argv[aix]);
   }  /* else process options */
}  /* get_command_options */


void  get_filenames(char *p)
{
   int tlen;
   char *x;
   char *timestr;
   struct stat s;
   comp_pname[0] = '\0';
   sprintf(comp_pname,"%s%s%s",comp_proot,comp_tail,date_string);
   if (source_prefix[0] == '\0'){
      fprintf(stderr,"Enter name of source file: ");
      fgets(source_prefix,MAX_FNAME,stdin);
      tlen = strlen(source_prefix);
      source_prefix[tlen-1]='\0'; /* get rid of \n */
      if (tlen == 1) {
         fprintf(stderr,"Name of the source file is required!\n");
         fprintf(stderr,"%s\n",comp_pname);
         exit(0);
      }
   }
   strcpy(source_fname,source_prefix);
   x = strrchr(source_prefix,'.');  /* look for suffix */
   if (x == NULL) /* if none, concat suffix to source_fname */
      strcat(source_fname,source_suffix);
   else { /* source_prefix has a suffix -- use it */
      *x = '\0';
   }
   sprintf(list_fname,"%s%s",source_prefix,list_suffix);
   sprintf(pcode_fname,"%s%s",source_prefix,
      (making_objfile ? pobject_suffix : pcode_suffix));
   if (stat(source_fname,&s) < 0) {
      fprintf(stderr,"%s: can't open %s\n",p,source_fname);
      fprintf(stderr,"%s\n",comp_pname);
      exit(1);
   }
   timestr = ctime(&s.st_mtime); 
   strcpy(filename_line,"Source file: ");
   strcat(filename_line,source_fname);
   strcat(filename_line,"  ");
   strcat(filename_line,timestr); /* timestr is terminated by \n */
   if ((yyout = fopen(list_fname,"w")) == NULL) {
      fprintf(stderr,"%s: Can't open %s\n",p,list_fname);
      fprintf(stderr,"%s\n",comp_pname);
      exit(1);
   }
}  /* get_filenames */   

void  global_init(int argc, char **argv)
{
int i;
   get_command_options(argc,argv);
   get_filenames(argv[0]);
   curr_infile = open_infile(source_fname,curr_infile);
   fprintf(yyout,"%s\n",comp_pname);
   fprintf(yyout,"%s",filename_line); /* filename_line has an \n already */
   fprintf(yyout," line  pc\n");
   errcount = 0;
   mon = last_atab = last_tab = -1;
   last_btab = stab_size = 0;
   last_dbg = -1;
   for (i = 0;i < EXPRSIZE; i++) expr[i].free = 1;
                /*           id            object  types   adr  */
   enterstid("          ", variable,notyp,0); /* sentinel */
   /* adr field for a constant is the value of the constant */
   enterstid("INT_MSG",constant,ints,INT_MSG);
   enterstid("STRING_MSG",constant,ints,STRING_MSG);
   /* adr field for type objects is byte size for vbls of the type */
   enterstid("char",type,chars,sizeof(int));
   char_loc = last_tab;                    /* save tab index of char id */
   enterstid("int",type,ints,sizeof(int));
   int_loc = last_tab;                     /* save tab index of int id */
   enterstid("condition",type,conds,sizeof(int));
   enterstid("semaphore",type,sems,sizeof(int));
   enterstid("binarysem",type,bsems,sizeof(int));
   /* adr field for std func EOF, EOLN is switch case in pcode 13 */
   /* adr field for std func EMPTY is used in parser */
   enterstid("empty",function,ints,SF_EMPTY);
   /* adr field for std procedure is used in the parser */
   /* enterstid("read",procedure,notyp,SP_READ);*/
   /* enterstid("readln",procedure,notyp,SP_READLN);*/
   /* enterstid("write",procedure,notyp, SP_WRITE); */
   /* enterstid("writeln",procedure,notyp,SP_WRITELN); */
   enterstid("initialsem",procedure,notyp,SP_INITSEM);
   enterstid("p",procedure,notyp,SP_PWAIT);
   enterstid("wait",procedure,notyp,SP_PWAIT);
   enterstid("v",procedure,notyp,SP_VSIGNAL);
   enterstid("signal",procedure,notyp,SP_VSIGNAL);
   enterstid("waitc",procedure,notyp,SP_WAITC);
   enterstid("signalc",procedure,notyp,SP_SIGNALC);
   enterstid("exit",procedure,notyp,SP_EXIT);
   enterstid("suspend",procedure,notyp,SP_SUSPEND);
   enterstid("revive",procedure,notyp,SP_REVIVE);
   enterstid("which_proc",function,ints,SF_WHICHPROC);
   enterstid("random",function,ints,SF_RANDOM);
   last_predeclared = last_tab;  /* save index of last predeclared id */
   /* when name of main pgm is seen in parsing, it's next stid */
} /* global_init */ 


/*
 *
 *  $Log: baccutil.c,v $
 *  Revision 2.6  2004/07/21 19:16:54  bynum
 *  add writetab.h include
 *
 *  Revision 2.5  2003/06/25 17:09:46  bynum
 *  change to Moti Ben-Ari's filename suffix convention
 *
 *  Revision 2.4  2001/07/13 19:27:17  bynum
 *  add globtabs.h include
 *
 *  Revision 2.3  2000/08/04 17:09:43  bynum
 *  replace last_dbg_ix with last_dbg
 *
 *  Revision 2.2  1998/11/26 22:41:54  bynum
 *  add code to allow user to omit .cm suffix
 *
 * Revision 2.1  1997/07/09  17:23:33  bynum
 * change from a to last_atab, b to last_btab, t to last_tab, move
 * check_resolved_externs() to ../lib/computil.c
 *
 * Revision 2.0  1997/07/02  13:06:40  bynum
 * add extern procs, monitors, and variables
 *
 * Revision 1.8  1997/06/20  10:02:58  bynum
 * add INT_MSG, STRING_MSG constants, 'last_predeclared' init
 *
 * Revision 1.7  1997/06/17  05:27:55  bynum
 * change enterstid sizes of builtin types to byte sizes
 *
 * Revision 1.6  1997/03/25  14:48:32  bynum
 * incorporate name changes from include directory, add fprintf prototype
 *
 * Revision 1.5  1997/02/04  07:01:56  bynum
 * switch to FLEX
 *
 * Revision 1.4  1996/03/07  09:21:08  bynum
 * change to 'stat' call for file being compiled
 *
 * Revision 1.3  1995/09/07  14:27:25  bynum
 * move much of the code to one of the files in the ../lib directory,
 * change includes to conform to new structure of the ../include directory
 * and usage of libba.a
 *
 * Revision 1.2  1995/09/05  14:27:16  bynum
 * comment out 'read', 'readln', 'write', 'writeln' std procs
 *
 * Revision 1.1  1995/08/26  11:06:02  bynum
 * Initial revision
 *
 *
 */
