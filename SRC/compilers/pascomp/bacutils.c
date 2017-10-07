/* utility pgms for BenAri Concurrent Pascal Compiler */

#include <stdlib.h>  /* for malloc */
#include <time.h>    /* for ctime */
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

extern   int fprintf(FILE*,const char*,...);

extern   char  lastident[];

extern   char  comp_tail[];
extern   char  date_string[];
FNAME_STRING source_prefix;

extern   int   last_predeclared;

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

void  get_command_options(int argc, char **argv)
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
   char *timestr;
   struct stat s;
   char *x;

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
   else { /* source_prefix has a suffix -- is it the one we want? */
      if (strcmp(x,source_suffix) == 0) /* if so, truncate the prefix */
         *x = '\0';
      else  /* otherwise, add the desired suffix to the existing suffix */
         strcat(source_fname,source_suffix);
   }
   sprintf(list_fname,"%s%s",source_prefix,list_suffix);
   sprintf(pcode_fname,"%s%s",source_prefix,
      (making_objfile ? pobject_suffix : pcode_suffix));
   if ((yyin = fopen(source_fname,"r")) == NULL) {
      fprintf(stderr,"%s: Can't open %s\n",p,source_fname);
      fprintf(stderr,"%s\n",comp_pname);
      exit(1);
   }
   if (stat(source_fname,&s) < 0) {
      fprintf(stderr,"%s: can't stat %s\n",p,source_fname);
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
   extern int yydebug;

   yydebug = 0;
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
   enterstid("false",constant,bools,0);
   enterstid("true",constant,bools,1);
   enterstid("INT_MSG",constant,ints,INT_MSG);
   enterstid("STRING_MSG",constant,ints,STRING_MSG);
   /* adr field for type objects is stack size for vbls of the type */
   enterstid("char",type,chars,sizeof(int));
   enterstid("boolean",type,bools,sizeof(int));
   enterstid("integer",type,ints,sizeof(int));
   enterstid("condition",type,conds,sizeof(int));
   enterstid("semaphore",type,sems,sizeof(int));
   enterstid("binarysem",type,bsems,sizeof(int));
   /* adr field for std func EOF, EOLN is switch case in pcode 13 */
   enterstid("eof",function,bools,SF_EOF); 
   enterstid("eoln",function,bools,SF_EOLN);
   /* adr field for std func EMPTY is used in parser */
   enterstid("empty",function,bools,SF_EMPTY);
   /* adr field for std procedure is used in the parser */
   enterstid("read",procedure,notyp,SP_READ);
   enterstid("nbread",procedure,notyp,SP_NBREAD);  /*Moti*/ /* Nonblocking read */
   enterstid("readln",procedure,notyp,SP_READLN);
   enterstid("write",procedure,notyp, SP_WRITE);
   enterstid("writeln",procedure,notyp,SP_WRITELN);
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
   last_predeclared = last_tab;   /* tab index of the last predeclared id */
   /* when name of main pgm is seen in parsing, it's next stid */
} /* global_init */ 


/*
 *
 *  $Log: bacutils.c,v $
 *  Revision 2.4  2004/02/16 Moti
 *  Nonblocking read added
 *
 *  Revision 2.3  2001/07/13 19:28:59  bynum
 *  add globtabs.h include
 *
 *  Revision 2.2  2000/08/04 17:10:49  bynum
 *  replace last_dbg_ix with last_dbg
 *
 *  Revision 2.1  1998/11/26 22:49:09  bynum
 *  allow user to enter the .pm file suffix
 *
 * Revision 2.0  1997/07/09  11:53:20  bynum
 * change from t to last_tab, b to last_btab, a to last_atab, sx to stab_size
 *
 * Revision 1.12  1997/06/20  10:05:37  bynum
 * add INT_MSG, STRING_MSG constants, last_predeclared init
 *
 * Revision 1.11  1997/06/17  05:44:03  bynum
 * for enterstid() calls of builtin types, switch size to byte size
 *
 * Revision 1.10  1997/03/25  14:51:33  bynum
 * incorporate name changes in the include directory, add fprintf prototype
 *
 * Revision 1.9  1997/03/01  05:34:32  bynum
 * remove yylineno (FLEX doesn't need it)
 *
 * Revision 1.8  1996/03/07  09:24:02  bynum
 * change to 'stat' call to obtain date for file being compiled
 *
 * Revision 1.7  1995/09/07  14:32:31  bynum
 * move code to appropriate ../lib file, change includes to new
 * structure of the ../include directory
 *
 * Revision 1.6  1995/08/29  16:51:04  bynum
 * add 'write_debug_info' code, necessary 'dbg' assignments
 *
 * Revision 1.5  1995/07/09  19:23:19  bynum
 * add commenting to 'index_expr', change parm 'typ' to 'eltyp', and
 * emit INDEX even in the case of an indexing error
 *
 * Revision 1.4  1995/07/06  14:13:04  bynum
 * add 'stdproc_pcode' to emit as much of the PCODE as possible for
 * the std procs, change 'stdproc_parms' so that REVIVE & RANDOM parms
 * are pass-by-value (use 'gen_exprval' calls)
 *
 * Revision 1.3  1995/06/30  14:27:04  bynum
 * add code and variables for 'atomic' keyword, put back 'comp_proot' variable
 *
 * Revision 1.2  1995/06/22  06:41:39  bynum
 * remove 'comp_proot' variable, add 'open_infile' call to open original
 * source file
 *
 *
 */
