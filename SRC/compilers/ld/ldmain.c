/*Scott Mitchell */
/*CS710 Linker Project */
/*1 August 2000 */
/* file ld_main.c */

/*Files are combined two at a time.  The arg[1] file (the first 
file is handled differently.  Prior to looping through the remaining
arguments and searching the libraries, the argv[1] file is 
opened and read into t1.
Then we enter the loop for all successive files.  The loop
reads in the next argument file and stores it in t2.  The
files t1 and t2 are combined to produce the linked pcode
in finaltables.  If there are no more argument files we are done.
Otherwise, finaltables is identified with t1 (these are pointers
to tables, not the tables themselves)
and we read the next argument file on
the command line into t2 and combine t1 and t2 into finaltables.
This process continues until all of the argument files have
processed.  
Then the library files specified by the user are searched to
resolve any remaining external references.  For each external
ref, a .pob file (the first one found) that defines the
external symbol is obtained from one
of the libraries and linked with the existing pcode file,
as described above.
This process continues until every external reference has
either been resolved, or remains unresolved, but has been 
checked against every library that the user has specified.
If all externals have been resolved, then the output
pcode file is given a .pco suffix, otherwise, it's given
a .pob suffix. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/bacicnty.h"
#include "../include/pcode.h"
#include "../include/readtab.h"
#include "../include/writetab.h"
#include "../include/globdata.h"
#include "../include/genutil.h"
#include "../ar/ar.h"
#include "../ar/artables.h"
#include "../ar/arutils.h"
#include "../ar/arextrac.h"
#include "ldutils.h"
#include "ldresolv.h"

/*main data structures used to produce linked files*/
#if defined(DOS)
extern ALL_TABLES ta, tb, tc;
#else
ALL_TABLES ta, tb, tc;
#endif
ALL_TABLES* finaltables; /* points to the final ALL_TABLES (ta or tb) */
ALL_TABLES* t1;          /* points to left ALL_TABLES (tb or ta)      */
ALL_TABLES* t2;          /* points to right ALL_TABLES (always tc)    */ 
/*initial size of btab table*/
int last_level0;

#define NUMLINKFILES 50
int last_linkfile = -1;
char* linkfile[NUMLINKFILES];

int last_cmdline_file = -1; /* last_linkfile after linking all cmdline files */

#define NUMLIBDIRS 30
int last_libdir = -1;
char* libdir[NUMLIBDIRS];

#define NUMLIBNAMES 30
int last_libname = -1;
char* libname[NUMLIBNAMES];

#define NUMARCHIVES 50
Archive ar[NUMARCHIVES];
int last_ar = -1;

char* newfname;   /* name of the linkfile (defaults to aout.pob or aout.pco) */

extern char date_string[];
char ldname_tail[] = {" PCODE Linker in C, "};
FNAME_STRING ldnamestring;
char program_name[] = {"bald"};   
char lib_suffix[] = {".ba"};
char lib_prefix[] = {"lib"};
FNAME_STRING linkfname;


OBJTYPE found_obj[12] = {
 /* 1234567890    */
   constant,        /* 0 constant      */
   variable,        /* 1 variable      */
   type,            /* 2 type          */
   procedure,       /* 3 procedure     */
   function,        /* 4 function      */
   monitor,         /* 5 monitor       */
   mainproc,        /* 6 mainproc      */
   outerblock,      /* 7 outerblock    */
   procedure,       /* 8 ext_procedure */
   function,        /* 9 ext_function  */
   monitor,         /* 10 ext_monitor  */
   variable         /* 11 ext_variable */
  };

int make_map = 0;    /* user cmdline flags */
int verbose = 0;

static void new_linkfile(char* fn)
   /* add fn to linkfile array, if possible */
{
   last_linkfile++;
   if (last_linkfile >= NUMLINKFILES)
      fatal("Maximum number of %d link files exceeded",NUMLINKFILES);
   linkfile[last_linkfile] = savestr(fn);
}  /* new_linkfile */


#if defined(DOS)
#define DIRSLASH  '\\'
#define DIREXAMPLE "C:\\baci\\lib"
#else
#define DIRSLASH '/'
#define DIREXAMPLE "/home/baci/lib"
#endif

static void new_libdir(char* fn)
   /* add fn to libdir array, if possible */
{
   char tmp[256];
#if defined(DOS)
   int i, n;
#else
   int n;
#endif
   last_libdir++;
   if (last_libdir >= NUMLIBDIRS)
      fatal("Maximum number of %d library directories exceeded",NUMLIBDIRS);
   strncpy(tmp,fn,256);
   tmp[255] = '\0';  /* make sure that tmp has a null terminator */
   n = strlen(tmp);
#if defined(DOS)
   for (i = 0; i < n; i++) 
      if (tmp[i] == '/') tmp[i] = DIRSLASH;
#endif
   if (tmp[n-1] != DIRSLASH) {
      if (n == 256) n--;
      tmp[n] = DIRSLASH;
      tmp[n+1] = '\0';
   }
   libdir[last_libdir] = savestr(tmp);
}  /* new_libdir */

static void new_libname(char* fn)
   /* add fn to libname array, if possible */
{
   char tmp[256];
   last_libname++;
   if (last_libname >= NUMLIBNAMES)
      fatal("Maximum number of %d library files exceeded",NUMLIBNAMES);
   strcpy(tmp,lib_prefix);
#if defined(DOS)
   strncat(tmp,fn,8-strlen(lib_prefix));
#else
   strncat(tmp,fn,256-strlen(lib_prefix)-strlen(lib_suffix));
#endif
   strncat(tmp,lib_suffix,strlen(lib_suffix));
   libname[last_libname] = savestr(tmp);
}  /* new_libname */

static void save_linkfname(char* fn)
   /* save the name of the link output file */
{
   int n = strlen(fn);
   if (n > MAX_FNAME - 5) n = MAX_FNAME - 5; /* .pxx plus null char */
   strncpy(linkfname,fn,n);
   linkfname[n] = '\0';
}  /* save_linkfname */

void usage(int exit_code)
   /* shows usage information */
{
   printf("%s\n\n",ldnamestring);
   printf("Usage: %s [options] List_of_files_to_link\n",program_name);
   printf("   List_of_files_to_link is a list of .pob or .pco files to be linked.\n");
   printf("   This list must contain at least one file and can contain up to %d files.\n\n",NUMLINKFILES);      
   printf("Options (can occur in any order):\n");
   printf("   -Ldirname\n");
   printf("      Look in \"dirname\" for a BACI link library to use to resolve external\n");
   printf("      references.  \"dirname\" should be a fully qualified directory name.\n");
   printf("      The -L option can be repeated up to %d times.\n\n", NUMLIBDIRS);
   printf("   -llibfile\n");
   printf("      Look in the directories given by the -L options for the library file\n");
   printf("      whose name is the concatenation of the 3 strings \"%s\", \"libfile\",\n",lib_prefix);
   printf("      and \"%s\".  For example, \"-lxu\" specifies the library file \"%sxu%s\".\n", lib_suffix,lib_prefix,lib_suffix);
   printf("      BACI library files are created with the BACI archiver, baar.\n");
   printf("      The -l option can be repeated up to %d times.\n\n", NUMLIBNAMES);
   printf("   -o outfile_prefix\n");
   printf("      Store the linked output in the file \"outfile_prefix.pco\" (if all externals\n");
   printf("      externals were resolved) or \"outfile_prefix.pob\" (if external symbols\n");
   printf("      remain).  Default output file is \"aout.pco\" or \"aout.pob\".\n\n");
   printf("   -m\n");
   printf("      Make a symbol map of the linked file.\n\n");
   printf("   -v\n");
   printf("      Do everything verbosely.\n\n");
   printf("   -h\n");
   printf("      Show this help information.\n\n");
   printf("Example Usage:\n\n");
   printf("   %s -o prog diskmon.pob diskusr.pob -L.. -L%s -ldisk -lfsem\n\n",program_name,DIREXAMPLE);
   printf("Link the two files diskmon.pob disusr.pob, resolving external references with\n");
   printf("the libraries %sdisk%s and %sfsem%s, located in the parent directory (..)\n",lib_prefix,lib_suffix,lib_prefix,lib_suffix);
   printf("or in the directory %s.  The linked file will be named\n",DIREXAMPLE);
   printf("prog.pco if all externals were resolved, or prog.pob, if not.\n");
   exit(exit_code);
}  /* usage */

void parse_cmdline(int argc, char* argv[]) 
    /*takes the command line arguments as input and peels off the flag
     * options from the files to be linked.  Stores directories to be
     * searched in the global array of strings called libdir[].
     * Stores library files to be searched for in the global array of
     * strings called libfile[].  Stores the files to be linked in the
     * global array of strings called linkfile[].*/ 
{
   char* arg;
   int i;
   linkfname[0] = '\0';
   new_libdir("."); /* current dir is always searched for lib files */
    /*loop through all command line arguments*/
   for (i=1; i<argc; i++) {
      arg = argv[i];
      if (arg[0] != '-') {
         /*we have a file to be linked so add it to the list of files
          * to compile and bump the counter by one*/
         new_linkfile(arg);
      }/*end if != '-'*/
      else {
         /* we have a command line option so lets ID the flag and
          * take appropriate action*/
         switch (arg[1]) {
            case 'o':
               if (i < argc - 1) {
                  i++;
                  save_linkfname(argv[i]);
               }
               else {
                  printf("Malformed -o option -- no filename given\n");
                  usage(1);
               }
               break;
            case 'L':
               if (strcmp(&(arg[2]),".") != 0) /* don't add "./" */
                  new_libdir(&(arg[2]));
               break;
            case 'l':
               new_libname(&(arg[2]));
               break;
            case 'm':
               make_map = 1;
               break;
            case 'v':
               verbose = 1;
               break;
            case 'h':
               usage(0);
               break;
            default:
               printf("Unknown option %c\n",arg[1]);
               usage(1);
         }  /* switch (arg[1]) */         
      }/*end else*/
   }/*end for*/
   if (linkfname[0] == '\0') save_linkfname("aout");
}/*end parse_cmdline*/


static void read_linkfile(ALL_TABLES* t, int lix)
   /* read tables from cmdline-specified link file */
{
   FILE* file2in;
      /*open for reading the next link file*/
   if ((file2in = fopen(linkfile[lix], "r")) == NULL) 
       fatal("Can't open %s\n", linkfile[lix]);
      /* read the argument file into *t */
   read_tables(file2in, t);
   fclose(file2in);
}  /* read_linkfile */

static void add_linkfname(ALL_TABLES* tfinal,int lix)
   /* add linkfile[lix] to fnameline of *tfinal */
{
   int n = strlen(tfinal->fnameline);
   strcat(tfinal->fnameline," ");
   strncat(tfinal->fnameline,linkfile[lix],MAX_FNAME-n-2);
   tfinal->fnameline[MAX_FNAME-1] = '\0';
}  /* add linkfname */
   
static void link_another(ALL_TABLES* tfinal, ALL_TABLES* t1,
   ALL_TABLES* t2, int lix)
   /* link another .pob file into the final link file               */
   /* NOTE:  next linkfile is expected to have been read into *t2!! */
{
   extern int last_level0;
   last_level0 = t1->btab[0].last;
   add_linkfname(tfinal,lix);
    /*initially merge code table and copy the remaing
      tables into the final resultant data structure*/
   merge_codetables(tfinal, t1, t2);
   copy_five_tables(tfinal, t1);
   merge_infiles(tfinal, t1, t2);
   merge_symbol_table(tfinal,t2,lix);
    /* now we can fix all loops */
   fix_loops(tfinal);
   update_monfields(tfinal,t2);
    /*Once the tab table names, block table entries, and array table
      entires have been adjusted and the corresponding code in the
      code table has been adjusted, correct the string table and
      adjust the code tabel accordingly*/
   if (t2->stab_size > 0) {
       append_stab(tfinal, t2);
   }/*end if*/
    /*After all of the other tables have been adjusted, set up the
      debug table*/
   append_dbgs(tfinal, t1, t2);
}  /* link_another */

static int unresolved_externs(ALL_TABLES* tfinal) 
   /* return 1 if external symbols still exist */
{
   int i;
   for (i = tfinal->btab[0].last; i; i = tfinal->tab[i].link) {
      if (R[i]->status == UNRESEXT) return 1;
   }
   return 0;
}  /* unresolved_externs */

static int externals_left(ALL_TABLES* tfinal) 
   /* return 1 if any external symbols still exist */
{
   int i;
   for (i = tfinal->btab[0].last; i; i = tfinal->tab[i].link) {
      if ((R[i]->status == UNRESEXT)||(R[i]->status == TESTEDEXT)) return 1;
   }
   return 0;
}  /* externals_left */

static void open_all_archives()
   /* open all valid link libraries */
{
   FNAME_STRING testpath;
   int i, j, n;
   if (last_libname < 0) return;    /* no libraries ! */
   for (i = 0; i <= last_libdir; i++) {
      for (j = 0; j <= last_libname; j++) {
         strncpy(testpath,libdir[i],MAX_FNAME);
         n = strlen(testpath);
         strncat(testpath,libname[j],MAX_FNAME-n);
         testpath[MAX_FNAME-1] = '\0';
         last_ar++;
         if (last_ar >= NUMARCHIVES)
            fatal("Maximum number (%d) of libraries exceeded",NUMARCHIVES);
         open_archive(&(ar[last_ar]),testpath);
         if (ar[last_ar].rf != NO_ARCHIVE) 
            read_archive(&(ar[last_ar]));
         else
            last_ar--;  /* re-use the ar Archive entry */
      }  /* for j */
   }  /* for i */
}  /* open_all_archives */

static void write_linkfile(ALL_TABLES* tfinal)
   /* output linked file with the appropriate suffix */
{
   FILE* finalout;
   extern int mainproc_tix;
   if ((mainproc_tix >= 0)&& !externals_left(tfinal)) 
      strcat(linkfname,".pco");
   else
      strcat(linkfname,".pob");
     /* write the tfinal tables to the link file */
   if ((finalout = fopen(linkfname, "w")) == NULL) 
      fatal("Can't open link file %s\n", linkfname);
   write_tables(finalout, tfinal);
   printf("Output link file stored in %s\n",linkfname);
}  /* write_linkfile */

static void append_arname(Archive* ar, char* membername) 
   /* create a new linkfile name that is the concatenation of */ 
   /* the membername and the archive name from which it comes */ 
   /* return the index in the linkfile array where the name   */ 
   /* was stored                                              */
{
   int n;
   FNAME_STRING tmp;
   strncpy(tmp,membername,MAX_FNAME);
   strcat(tmp,"(");
   n = strlen(tmp);
   strncat(tmp,ar->fname,MAX_FNAME - n - 2);
   strcat(tmp,")");
   tmp[MAX_FNAME-1] = '\0';
   new_linkfile(tmp);
}  /* append_arname */

static void swap_table_ptrs(ALL_TABLES** tfinal, ALL_TABLES** t1,
                            ALL_TABLES** t2)
   /* Swap the pointers to the pcode tables to prepare for linking */
   /* another file.  Release dynamically allocated space.          */
{
   ALL_TABLES* tmp = *t1;     /* swap table pointers */
   *t1 = *tfinal;
   *tfinal = tmp;
   strncpy((*tfinal)->fnameline,(*t1)->fnameline,MAX_FNAME);
   /* free dynamically allocated space.  *t1 must stay! */
   free((*tfinal)->inputfile);
   free((*t2)->inputfile);
}  /* swap_table_ptrs */


/*************************************
 *                                   * 
 *    BACI Linker Main Program       *
 *                                   *
 *************************************/
int main (int argc, char* argv[]) {
   int i, j, not_resolved; 
   int gi[TSIZE];
   int last_gi;
   ALL_TABLES* tmain;
   int first_link = 1;
   Member* m;
   TAB_STRUCT* t;
   sprintf(ldnamestring, "%s%s%s", comp_proot, ldname_tail, date_string); 
   parse_cmdline(argc,argv); 
   /* look in the current directory for link libs */ 
   if (last_linkfile < 0) {
      nonfatal("Must have at least one .pob or .pco file to link");
      usage(1);
   } 
   finaltables = &ta;     /* set initial values of table pointers */ 
   t1 = &tb; 
   t2 = &tc;
    /* read in the first link file */ 
   read_linkfile(t1,0);
   if (verbose) show_globals(t1,0);
   sprintf(finaltables->fnameline,"Linked files: %s",linkfile[0]);
      /* note all symbol refs */ 
   for(i = t1->btab[0].last; i; i = t1->tab[i].link)
      addRef(i,t1->tab[i].obj,0); 
   for(i=1; i <= last_linkfile; i++) {
      read_linkfile(t2,i);
      link_another(finaltables,t1,t2,i); 
      if (verbose)
         show_globals(t2,i);
      first_link = 0;
      if (i < last_linkfile) /* more  linking to be done */
         swap_table_ptrs(&finaltables,&t1,&t2); 
   }/*end for i*/
   last_cmdline_file = last_linkfile;
   if (last_cmdline_file == 0)
      tmain = t1;
   else
      tmain = finaltables;
   if ((last_libname >= 0) && (externals_left(tmain))) { 
      /* libs to check & externs to resolve */
      open_all_archives();
      if (last_ar >= 0) {    /* if any of the archives had valid names */
         while (unresolved_externs(tmain)) {
            last_gi = global_index(tmain,gi,1); /* must use ndx array  */
            for (i = 0; i <= last_gi; i++) {          /* because of ptr swap */
               if (R[gi[i]]->status == UNRESEXT) {
                  t = &(tmain->tab[gi[i]]);
                  /* try to find this symbol in one of the archives */
                  not_resolved = 1;
                  for (j = 0; j <= last_ar; j++) {
                     m = findMemberBySymbol(&(ar[j]),t->name,found_obj[t->obj],
                           t->typ);
                     if (m != (Member*) 0) {
                        if (first_link)      /* shouldn't swap ptrs if */
                           first_link = 0;   /* this is the first link */
                        else {
                           swap_table_ptrs(&finaltables,&t1,&t2);
                           tmain = finaltables;
                        }
                        PositionToRead(&(ar[j]),m);
                        read_tables(ar[j].rf,t2);
                        append_arname(&(ar[j]),m->name);
                        link_another(finaltables,t1,t2,last_linkfile);
                        not_resolved = 0;
                        if (verbose) show_globals(t2,last_linkfile);
                        tmain = finaltables;
                        break;   /* get out of the for j loop */
                     }  /* if m */
                  }  /* for  j */
                  if (not_resolved)    /* symbol wasn't found in any archive */
                     R[gi[i]]->status = TESTEDEXT;
               }  /* if UNRESEXT */
            }  /* for i */
         }  /* while unresolved_externs */
         for (i = 0; i <= last_ar; i++) fclose(ar[i].rf);
      }  /* if there were archives */
   }  /* if libnames were given and there were unresolved externs */
   if (last_linkfile == 0) { 
      /* only one linkfile, no valid libraries were opened */
      fprintf(stdout,"Only one linkfile %s -- no linking done\n",linkfile[0]);
      finaltables = t1; /* only info is in t1, switch pointers */
   }
   write_linkfile(finaltables); 
   if (externals_left(finaltables)) {
      if (verbose)
         show_externals(finaltables);
      else
         fprintf(stdout,"There are unresolved externals in link file %s\n",
            linkfname);
   }
   if (make_map)
     map_tab(finaltables); 
   return 0;

}//end main

/*
 *
 * $Id: ldmain.c,v 2.13 2007/06/01 20:07:44 bynum Exp $
 *
 */
