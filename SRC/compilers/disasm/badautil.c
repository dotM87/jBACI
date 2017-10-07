 /* BenAri PCODE disassembler utilities */
 /*
      Bill Bynum
      June 1997
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>      /* for stat    */
#include <sys/types.h>     /* for stat    */
#include <time.h>          /* for ctime   */

#include "../include/globdata.h"
#include "../include/globtabs.h"
#include "../include/genutil.h"
#include "../include/readtab.h"
#include "../include/pcode.h"
#include "../include/incfiles.h"

#include "../include/disutils.h"

extern int fprintf(FILE*,const char*,...);
extern int fputc(int , FILE*);
extern int fputs(const char*,FILE*);
extern int sscanf(const char*,const char*,...);
extern int fclose(FILE*);
extern void rewind(FILE*);

FNAME_STRING prog_name;  /* to hold the name of the disassembler */

char pn_root[]  = 
   {"PCODE Disassembler in C, " };   /* beginning of prog_name */
   
char disasm_hdr[] = { "  lc    f    x    y     PCODE\n" };

extern char date_string[];

FNAME_STRING   buf;  /* input buffer for reading tables */

FNAME_STRING   disasm_fname;  /* name of file to be disassembled */
FILE*          disasm;        /* FILE pointer to the file */

FILE*          pcode;         /* FILE pointer to pcode file */

char disasm_suffix[] = {".dis"};

extern CODEBLOCK cbtab[];       /* code block table */

extern int last_cbtab;   /* index of last element of cbtab array */

int source_wanted;   /* 1 ==> user wants to see source listing, if available */
      
void  show_help(char **argv)
{
   fprintf(stderr,"%s\n",prog_name);
   fprintf(stderr,"Usage:   %s [optional_flags]  pcode_filename\n",argv[0]);
   fprintf(stderr,"Optional flags:\n");
   fprintf(stderr,"  -h    show this help\n");
   fprintf(stderr,"  -s    don't display source code, even if available\n");
   fprintf(stderr, "The name of the PCODE file is required.  The PCODE file\n");
   fprintf(stderr, "is expected to have a \".pco\" or \".pob\" suffix.\n");
}  /* show_help */


void  get_command_options(int argc, char** argv)
{
int   aix;
char  c;
   /* open the object file */
   if (argc == 1) 
      pcode_fname[0] = '\0';
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
            case 'h':   show_help(argv); break;
            case 's':   source_wanted = 0; 
            default:
               fprintf(stderr,"Invalid option '%s'\n",argv[aix]);
         }  /* switch */
      } /* for processing options */
      if (aix < argc) 
         strcpy(pcode_fname,argv[aix]);
   }  /* else process options */
}  /* get_command_options */


void get_filenames(char *p)
{
   int tlen;
   char     *timestr;
   struct stat s;

   char* tp;

      /* create the name of the disassembler */
   sprintf(prog_name,"%s%s%s",comp_proot,pn_root,date_string);
      /* create the name of the PCODE file to be disassembled */
   if (pcode_fname[0] == '\0'){
      fprintf(stderr,
      "Enter name of file to be disassembled ");
      fgets(pcode_fname,MAX_FNAME,stdin);
      tlen = strlen(pcode_fname);
      pcode_fname[tlen-1]='\0'; /* get rid of \n */
      if (tlen == 1) {
         fprintf(stderr,"Name of the file to be disassembled is required!\n");
         fprintf(stderr,"%s\n",prog_name);
         exit(1);
      }
   }
      /* get mod date of the file to put in the disassembly file */
   if (stat(pcode_fname,&s) < 0) {
      fprintf(stderr,"Can't open '%s'\n",pcode_fname);
      fprintf(stderr,"%s\n",prog_name);
      exit(1);
   }
      /* create the name of the disassembly file */
   strcpy(disasm_fname,pcode_fname);
   tp = strrchr(disasm_fname,'.');
   if (tp == NULL) {
      fprintf(stderr,"\"%s\" does not have a \".pco\" or \".pob\" suffix\n",
         pcode_fname);
      exit(1);
   }
   if (strcmp(tp,".pco")&&strcmp(tp,".pob")) {
      fprintf(stderr,"\"%s\" does not have a \".pco\" or \".pob\" suffix\n",
         pcode_fname);
      exit(1);
   }
   tp++;  /* move past the period */
   *tp = 'd';   /* change from .pco or .pob to .dco or .dob */

      /* open the PCODE file */
   if ((pcode = fopen(pcode_fname,"r")) == NULL) {
      fprintf(stderr,"%s: Can't open %s\n",p,pcode_fname);
      fprintf(stderr,"%s\n",prog_name);
      exit(1);
   }
      /* open the disassembly file */
   if ((disasm = fopen(disasm_fname,"w+")) == NULL){
      fprintf(stderr,"%s: Can't open file to hold disassembled PCODE!\n",p);
      fprintf(stderr,"%s\n",prog_name);
      exit(1);
   }
   timestr = ctime(&s.st_mtime); 
   fprintf(disasm,"%s\n",prog_name);
   fprintf(disasm,"PCODE file: %s  %s\n",pcode_fname,timestr);
}  /* get_filenames */

void global_init(int argc, char **argv)
   /* initialize the disassembler */ 
{
   source_wanted = 1;   /* assume that source is wanted */
   get_command_options(argc,argv);
   get_filenames(argv[0]);
   strcpy(tab[0].name,"???"); /* sentinel for search_var */
   tab[0].lev = tab[0].adr = -1;
   read_entire_pcode_file(pcode);
      /* copy first two lines of PCODE file to disasm file              */
      /* must reopen, read_entire_pcode_file closes pcode descriptor    */
   pcode = fopen(pcode_fname,"r");
   if (pcode == NULL) {
      fprintf(stderr,"Can't reopen %s for reading\n",pcode_fname);
      exit(1);
   }
   fgets(buf,MAX_FNAME,pcode);
   fputs(buf,disasm);
   fgets(buf,MAX_FNAME,pcode);
   fputs(buf,disasm);
      /* write a header to explain the columns, if there is no source */
   if (!source_wanted) 
      fputs(disasm_hdr,disasm);
   fclose(pcode);
   last_cbtab = build_cbtab();
   lc = 0;  /* start at the right place */
}  /* global_init */

int level = -1;               /* file include level */

extern InputFile* inputfile;

void open_ifile(FILE* f, int* cur_ifile)
   /* open the file inputfile[*cur_ifile].fname and write ouput */
   /* to file 'f'.  Modify the 'level' global                   */
{
   if (*cur_ifile < 0) return;
   inputfile[*cur_ifile].f = fopen(inputfile[*cur_ifile].fname,"r");
   if (inputfile[*cur_ifile].f == NULL) {
      fprintf(f,"Source file '%s' is not available\n",
         inputfile[*cur_ifile].fname);
   }
   else {
      fprintf(f,"Reading from source file '%s'\n",inputfile[*cur_ifile].fname);
      level++;
   }
}

void close_ifile(FILE* f,int* cur_ifile)
   /* open the file inputfile[*cur_ifile].fname and write ouput */
   /* to file 'f', if necessary.  Modify the 'level' global     */
{
   if (*cur_ifile < 0) return;
   fclose(inputfile[*cur_ifile].f);
   *cur_ifile = inputfile[*cur_ifile].parent;
   if (*cur_ifile >= 0) {
      if (inputfile[*cur_ifile].f != NULL)
         fprintf(f,"Returning to file '%s'\n",inputfile[*cur_ifile].fname);
   }
   level--;
}

static char xbuf[BUFSIZE];

void show_line(FILE* f, int* cur_ifile)
   /* write the source file line inputfile[*cur_ifile].line_no to */
   /* file 'f', indicating the proper file include level.         */
{
   int i;

   for (i = 0; i< level; i++) fputc('>',f);
   fgets(xbuf,BUFSIZE,inputfile[*cur_ifile].f);
   inputfile[*cur_ifile].line_no++;
   fprintf(f,"%7d %s",inputfile[*cur_ifile].line_no,xbuf);
}
   
int first_time = 1;  
   /* flag to ensure that the "lc f x y" header appears only once */
   /* in the disassembly listing                                  */

void show_source(FILE* f, int lc, int* cur_ifile, int* showing_source, 
   int* dbix)
   /* Using the dbg[] array, display as many source lines as required  */
   /* at the current point of the disassembly.                         */
   /* Information in the dbg[] array is retrieved from the .pco file   */
   /* at startup.  The array contains the significant events in the    */
   /* parse that the debugger or disassembler needs to know to rep-    */
   /* resent faithfully the appearance of the source code to the user. */
   /* For a given entry i of dbg[],                                    */
   /* the lines of the file infputfile[dbg[i].fix].f up to, and in-    */
   /* cluding  line # dbg[i].flno must have been shown to the user     */
   /* before the PCODE at dbg[i].lc is disassembled.                   */
   /* The following dbg[] contents are for an actual compilation.      */
   /*          0  11           PCODE debugging information             */
   /*             lc findex flineno                                    */
   /*             0     0     2                                        */
   /*             0     1    -5                                        */
   /*             0     0     5                                        */
   /*             0     2     5                                        */
   /*             5     2     6                                        */
   /*             6     2    -9                                        */
   /*             6     0     9                                        */
   /*             8     0    10                                        */
   /*            12     0    11                                        */
   /*            13     0    15                                        */
   /*            16     0    16                                        */
   /*            17     0   -16                                        */
   /*  This list indicates that lines 1 & 2 of file 0 should be        */
   /*  displayed, then lines 1 through 5 of file 1.  The -5 indicates  */
   /*  that file 1 should be closed after showing line 5.  Then, lines */
   /*  3, 4 and 5 of file 0 should be shown.  File 2 should be opened  */
   /*  and lines 1..5 displayed.  All of these actions will occur in   */
   /*  one call to show_source, since lc == 0 for all of them.  On the */
   /*  next call to show_source, line 6 of file 2 will be displayed.   */
   /*  On the subsequent call to show_source, lines 7, 8, and 9 of     */
   /*  file 2 will be displayed and file 2 closed, then lines 6..9 of  */
   /*  file 0 will be displayed.  The next call to show_source will    */
   /*  display line 10 of file 0, and so it goes until file 0 is       */
   /*  closed after line 16 is shown.                                  */
   /*     The file names and file inclusion order are given in the     */
   /*  inputfile[] array.                                              */
{
   int doclose;
   int stopln;

      /* if the primary file hasn't yet been opened, then do it */
   if ((*cur_ifile == 0)&&(inputfile[0].f == NULL)) {
      open_ifile(f,cur_ifile);
         /* the main pgm wasn't there ==> no source for this disasm */
      if (inputfile[0].f == NULL) 
         *showing_source = 0;
   }
   while((*dbix <= last_dbg)&&(dbg[*dbix].lc <= lc)) {
      if (*showing_source) {
         if (*cur_ifile != dbg[*dbix].fix) {
               /* consistency check */
            if (*cur_ifile >= 0) {
               if (inputfile[dbg[*dbix].fix].parent != *cur_ifile) {
                  fprintf(stderr,"show_source:  Input file index mismatch\n");
                  fprintf(stderr,"Expected parent: %d     Actual: %d\n",
                  inputfile[dbg[*dbix].fix].parent, *cur_ifile);
                  fprintf(stderr,"line # %d of file: %s\n", 
                     inputfile[*cur_ifile].line_no, 
                     inputfile[*cur_ifile].fname);
                  exit(1);
               }
            }
            /* switch to next input file */
            *cur_ifile = dbg[*dbix].fix;
         }
            /* the current input file isn't open, then open it */
         if (inputfile[*cur_ifile].f == NULL)
            open_ifile(f,cur_ifile);
            /* if the file wasn't there, then stop showing source */
         if (inputfile[*cur_ifile].f == NULL) 
            *showing_source = 0;
         if (*showing_source) {
               /* display lines in the current file to line # stopln */
            stopln = dbg[*dbix].flno;
               /* if stopln is negative, then close the current file */
               /* after the display                                  */
            if (stopln < 0) {
               stopln = -stopln;
               doclose = 1;
            }
            else
               doclose = 0;
               /* don't add any output unless it's needed */
            if (inputfile[*cur_ifile].line_no < stopln) {
               fprintf(disasm,"\n");
               while (inputfile[*cur_ifile].line_no < stopln)
                  show_line(disasm,cur_ifile);
               fprintf(disasm,"\n");
            }
            if (doclose)
               close_ifile(f,cur_ifile);
         }  /* inner if showing source */
      }  /* outer if showing source */
      else if (dbg[*dbix].flno < 0) {
            /* if we just reached the end of a file (whether it's */
            /* there or not), then switch back to its parent      */
         *cur_ifile = inputfile[*cur_ifile].parent;
         *showing_source = (inputfile[*cur_ifile].f != NULL);
         if (*showing_source) {
            fprintf(disasm,"\nReturning to file '%s'\n",
               inputfile[*cur_ifile].fname);
         }
      } /* else*/
      (*dbix)++;  /* keep chugging through the dbg[] array */
   }  /* while */
   if (first_time) {
      fputs(disasm_hdr,disasm);
      first_time = 0;
   }
}  /* show_source */


/*
 *
 *  $Id: badautil.c,v 1.16 2007/06/01 18:51:13 bynum Exp $
 *
 */
