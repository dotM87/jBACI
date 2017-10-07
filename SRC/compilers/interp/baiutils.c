 /* BenAri Concurrent PCODE Interpreter Utilities */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/bacicnty.h"
#include "../include/globdata.h"
#include "../include/globtabs.h"
#include "../include/readtab.h"
#include "../include/genutil.h"
#include "random.h"
#include "bainterp.h"

#define TEMPBUFSIZE 1024
static char tempbuf[TEMPBUFSIZE];

extern int last_code;      /* lc of last code byte */

extern int in_shortcall;   /* 1 if shortcall is being executed */

extern int last_infile;    /* ix of last entry of infile[] array */

#if   defined(TCL)
char  tcl_buffer[256];
int   lastline;
int   lastpr;
#define abs(x) ((x) < 0 ? -x : x)

/* bring in the Tcl/Tk definitions */
#include <tcl.h>
#include <tk.h>

void tcl_printf(char* s,int wn);  /* send message s to window wn  & window 0 */
void tcl_1printf(char* s,int wn); /* send message s to window wn */
void tcl_dialog(char* s);        /* Tk dialog window with message s and 'OK' */
void tcl_message(char* s);       /* Tk message window with message s */
void tcl_gets(char* s,int wn);   /* Gets a string from tcl   */
void colorwindows(int cpr);
void createwindow(int n);          /* creates a window for process n */
   /* NOTE: window for process 0 is created by the bacirun proc in menu.tcl */
void makedatawin();

int ShowPCODE;       /* 1 if PCODE window is open */
#endif


#if defined(SunOS)
extern int fprintf(FILE*,const char*,...);
extern int fputc(int ,FILE*);
extern int fclose(FILE*);
extern int fputs(const char*,FILE*);
extern int printf(const char*,...);
extern int sscanf(const char*, const char*,...);
#else
#if !defined(DOS)
extern int atoi(const char*);
#endif
#endif

extern int build_cbtab();
FNAME_STRING interp_pname;

char interp_pnroot[]  = {"BACI Concurrent PCODE Interpreter in C, " };
   
extern char date_string[];

char xpcode_suffix[] = { ".xpc" };

FNAME_STRING pcode_dir;       /* directory containing the pcode file */

FNAME_STRING xpcode_fname;
FILE  *xpcode;

   /* debug flags */
int   end_db=0, initAR_db=0, finalAR_db=0, pcexec_db=0,
      debugging = 0;

FNAME_STRING   buf;  /* input buffer for reading tables */

extern int charl, charh;   /* lowest char ord = ord' ', highest= ord '~'*/
extern ORDER ir;           /* holds current instruction */
extern enum pstat ps;      /* current interpreter state */
extern int s[];            /* the stack */
extern proctab ptab[];     /* the process table */
extern CODEBLOCK cbtab[];  /* code block table */
extern int last_cbtab;     /* index of last element cbtab */

int main_dbix;             /* dbg index of the first byte of the main proc */
int main_b;                /* bottom of stackframe of main proc */
int main_tix;              /* tab index of main proc */

extern InputFile *inputfile;

extern int singlestep;

#define MAXBREAK 100          /* This many breaks active at once */
/* array of breakpoint information */
struct {
   int adr;          /* lc of the break */
   int tix;          /* tab index of the code block owning the break*/ 
   int uix;          /* index of the break that user refers to */
   int dbgix;        /* index into the dbg array of this breakpoint */
} breakpoint[MAXBREAK];
int break_cnt;       /* # active entries in breakpoint array */
int next_uix = 0;    /* next index of user break index to be assigned */


#define DATAWIN   505   /* "process" number for data window */
char out_buf[256];      /* used for data display */
char tmp_buf[256];      /* used for data display */

void  show_help(char **argv)
   /* display help information to stderr */
{
   fprintf(stderr,"%s\n",interp_pname);
   fprintf(stderr,"Usage:   %s [optional_flags]  pcode_filename\n",argv[0]);
   fprintf(stderr,"Optional flags:\n");
   fprintf(stderr,
      "   -d  enter the debugger, single step, set breakpoints\n");
   fprintf(stderr,
      "   -e  show the activation record (AR) on entry to each process\n");
   fprintf(stderr, "   -x  show the AR on exit from each process\n");
   fprintf(stderr, "   -t  announce process termination\n");
   fprintf(stderr, "   -h  show this help\n");
   fprintf(stderr, "   -p  show PCODE instructions as they are executed\n");
   fprintf(stderr, "The name of the PCODE file is required.  If missing, you\n");
   fprintf(stderr, "will be prompted for it.  The file suffix \"%s\" will\n",
         pcode_suffix);
   fprintf(stderr, "be appended to the filename you give.\n");
}  /* show_help */

void  get_command_options(int argc, char** argv)
   /* process the options from the command line */
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
#if defined(TCL)
            case 'd': debugging = 1; ShowPCODE = 1; break;
#else
            case 'd': debugging = 1; break;
#endif
#if defined(TCL)
            case 'h': break;     /* ignore -h option in gui version */
#else
            case 'h': show_help(argv); break;
#endif
            case 'x': finalAR_db = 1; break;
            case 'e': initAR_db = 1; break;
            case 't': end_db = 1; break;
            case 'p': pcexec_db = 1; break;
            default:
#if defined(TCL)
               ; /* nothing - let invalid options go by in gui version */
#else
               fprintf(stderr,"Invalid option '%s'\n",argv[aix]);
#endif
         }  /* switch */
      } /* for processing options */
      if (aix < argc) 
         strcpy(pcode_fname,argv[aix]);
   }  /* else process options */
}  /* get_command_options */

void make_interpname()
   /* create the timestamped name of the interpreter */
{
   strcat(interp_pname,interp_pnroot);
   strcat(interp_pname,date_string);
} /* make_interpname */

void get_pcodefname(char* pco_fname)
   /* get the name of the pcode file */
{
   int tlen;
   if (pco_fname[0] == '\0'){
      fprintf(stderr,"Enter name of PCODE file (%s suffix assumed): ",
         pcode_suffix);
      fgets(pco_fname,MAX_FNAME,stdin);
      tlen = strlen(pco_fname);
      pco_fname[tlen-1]='\0'; /* get rid of \n */
      if (tlen == 1) {
         fprintf(stderr,"Name of the PCODE file is required!\n");
         fprintf(stderr,"%s\n",interp_pname);
         exit(1);
      }
   }
}  /* get_pcodefname */

int fix_pcodefname(char* pco_fname)
   /* add the pcode_suffix, if necessary; determine pcode_dir string */
{
   char* x;
   char* q;
   char* y;
   extern char* getenv(const char*);
   if (pco_fname[0] == '~') { /* expand the tilde */
      x = getenv("HOME");
      if (x != NULL) {
         strcpy(buf,x);
         strcat(buf,&pco_fname[1]);
         strcpy(pco_fname,buf);
      }  /* if there is a value for HOME */
   }  /* if the user wants ~ expanded */
   strcpy(pcode_dir,pco_fname);
   x = strrchr(pco_fname,'.');  /* look for suffix */
   y = strrchr(pco_fname,'/');  /* dir name comes after server net addr */
   if ((x == NULL)||(x < y)) {  /* .'s can occur in in server net addr */
      strcat(pco_fname,pcode_suffix);
   }
   else { /* pco_fname has a suffix -- is it the one we want? */
      if (strcmp(x,pcode_suffix) != 0) { /* if not the right suffix, then die */
         fprintf(stderr,"Invalid file suffix \"%s\" -- %s expected\n",
            x,pcode_suffix);
         return -1;
      }
   }  /* else pco_fname has a suffix */
   /* find the directory part of the file name */
   q = strrchr(pcode_dir,(char) '/');
   if (q == NULL)
      strcpy(pcode_dir,"./");
   else {
      q++; *q = '\0';
   }
   return 0;
}  /* fix_pcodefname */

FILE* open_pcodefile(char* pco_fname)
   /* open the pcode file */
{
   FILE* p = NULL;
   if (pco_fname[0] == '\0') return NULL;
   if ((p = fopen(pco_fname,"r")) == NULL) {
      fprintf(stderr,"Cannot open %s\n",pco_fname);
   }
   return p;
}  /* open_pcodefile */

void init_interpreter();
void init_xpcodefile(char*);

extern char filename_line[];

void load_and_run(FILE* p)
{
   read_entire_pcode_file(p);
   last_cbtab = build_cbtab();
   if (pcexec_db) init_xpcodefile(pcode_fname);
   init_interpreter();
   break_cnt = 0;
   singlestep = 1;
   fprintf(stdout,"%s",filename_line);
   fprintf(stdout,"Executing PCODE ...\n");
}  /* load_and_run */

void global_init(int argc, char **argv)
   /* Initialize the program variables for the cmdline version */
{
   FILE* p;
   make_interpname();
   get_command_options(argc,argv);
   get_pcodefname(pcode_fname);
   if (fix_pcodefname(pcode_fname) < 0)
      exit(1);
   p = open_pcodefile(pcode_fname);
   if (p == NULL) {
      exit(1);
   }
   else 
      load_and_run(p);
}  /* global_init */

#if defined(TCL)
static void index_error(char *proc, int expected, int found)
{
   fprintf(stderr,"\n%s:  index mismatch  %d expected   %d found\n",
      proc,expected,found);
   exit(1);
}  /* index_error */

/* 
      In the "standard" interpreter, the readtabs.c routine
   read_file_info allocates the space for the infile array
   dynamically.  This OK because the array is used only once.
      However, in the GUI, the infile array can be re-used
   several times, depending on user interaction, so dynamic
   allocation with no deallocation causes a small memory leak.
      The simplest choice in the GUI is to use static allocation.
   Unfortunately, this imposes a fixed limit on the number of 
   included files, but most users will never reach it.
*/

#define MAXINFILES 30
static FNAME_STRING infn[MAXINFILES];
static InputFile inf[MAXINFILES];

int TCLread_file_info(FILE* f, InputFile** inputfile)
   /*  reads source filenames and nesting information */
{
   int ix, dummy, first_infile, last_infile,infile_size;
   fgets(buf,MAX_FNAME,f); 
   sscanf(buf," %d %d",&first_infile,&last_infile);
   if (first_infile != 0) index_error("read_file_info",0,first_infile);
   infile_size = last_infile + 1;
   if (infile_size >= MAXINFILES) {
      fprintf(stderr,"More than %d input files\n",MAXINFILES);
      exit(1);
   }
   *inputfile = &inf[0];
   fgets(buf,MAX_FNAME,f); /* header line */
   for (ix = 0; ix < infile_size; ix++) {
      fgets(buf,MAX_FNAME,f);
      sscanf(buf," %d %d %s ", &dummy, &inf[ix].parent,infn[ix]);
      inf[ix].fname = infn[ix];
      if (dummy != ix) index_error("read_file_info",ix,dummy);
      inf[ix].line_no = 0;
      inf[ix].f = NULL;
   }
   return last_infile;
}  /* TCLread_file_info */



void global_tcl_init(char* pco_fname)
   /* Initialize the program variables for the Tcl/Tk version */
{
   char lname[256];
   FILE* p;
   strcpy(lname,pco_fname);   /* routines below modify the pco_fname string */
   last_infile = -1;
   break_cnt = 0;
   singlestep = 1;
   lastpr = 0;
   if (strcmp(lname,"NULL")) {
      if (fix_pcodefname(lname) < 0) exit(1);
      p = open_pcodefile(lname);
      /* read_entire_pcode_file code implanted below,  to be able to call */
      /* TCLread_file_info routine instead of read_file_info              */
      check_header(p);
      fgets(filename_line,MAX_FNAME,p);
      last_code = read_code_table(p,code);
      last_tab = read_identifier_table(p,tab);
      last_btab = read_block_table(p,btab);
      last_atab = read_array_table(p,atab);
      stab_size = read_string_table(p,stab);
      last_infile = TCLread_file_info(p,&inputfile);
      last_dbg = read_debug_info(p,dbg);
      fclose(p);
      last_cbtab = build_cbtab();
      if (pcexec_db) init_xpcodefile(lname);
      init_interpreter();
      tcl_1printf(filename_line,0);
      tcl_1printf("Executing PCODE ...\n",0);
   }
}  /* global_tcl_init */
#endif   

int chooseproc(int cpr, enum pstat* pst, int* stepcnt)
/* 
   From a random starting point, look through each entry of ptab
   for a process that is active and not suspended.  If no such
   process is found, declare deadlock and blame it on the most
   recently seen suspended process, if there is one, otherwise,
   blame it on the most recently seen active process 
   returns # of newly chosen process
   returns in *pst the process status
   returns in *stepcnt the number of steps in the process's quantum
*/
{
   int active, suspended, look, add, d;
   if (ptab[0].active)  /* proc0 becomes inactive after CODEND stmt */
      cpr = 0;          /* until that happens, proc0 is king        */
   else if (ptab[cpr].atomic) {  
     if (ptab[cpr].suspend >= 0) {  // suspended + atomic ==> deadlock
       *pst = deadlock;
#if defined(TCL)
       colorwindows(cpr);
#endif
      }  /* if cpr is atomic and suspended */
      // if cpr is atomic and not suspended, then let it run
      // cpr will be returned below
   }
   else if (*stepcnt == 0){  /* time quantum expired */
      d = PMAXP1;             /* must switch contexts */
      suspended = -1;
      look = __random(PMAXP1);
      add = 1 + __random(PMAXM1); /* 1 <= add < PMAXP1 */
      while ( ((!ptab[look].active) || (ptab[look].suspend>=0) ||
               (ptab[look].tabix < 0)) && (d >= 0) ){
         /* keep looking until a called process that is active and not
            suspended is found */
         d--;
         if (ptab[look].active > 0) active = look;
         if (ptab[look].suspend >= 0) suspended = look;
         /* VERY IMPORTANT that equation below has full period of PMAXP1 */
         look = (look + add) % PMAXP1;
      } /* while */
      if (d < 0) {  /* no runnable process was found */
         /* choose most recently seen suspended proc, else m.r.seen active */
         cpr = ((suspended >= 0) ? suspended : active );
         *pst = deadlock;
#if defined(TCL)
         colorwindows(cpr);
#endif
      }
      else {   /* we found someone to run */
         *stepcnt = __random(STEPMAXP1);
         cpr = look; 
#if defined(TCL)
         colorwindows(cpr);
#endif
      }  /* else a runnable proc was found */
   }  /* *stepcnt was 0, time for a context switch */
   else /* no context switch -- count down current proc's quantum */
      (*stepcnt)--;
   return cpr;
}  /* chooseproc */


int saddr_check(int curpr, int addr)
   /* Check a stack address */
{
   void stop_interp(int curpr, enum pstat);
   if ((addr >= ptab[curpr].stacksize)||(addr < 0)){
#if defined(TCL)
      sprintf(tcl_buffer,"\nBad stack address:  %d  at loc %d in process %d\n",
         addr,ptab[curpr].pc,curpr);
      tcl_1printf(tcl_buffer,0);
#else
      printf("\nBad stack address:  %d  at loc %d in process %d\n",
         addr,ptab[curpr].pc,curpr);
#endif
      stop_interp(curpr,stkchk);
      exit(1);
   }
   return addr;
}  /* saddr_check */

int paddr_check(int curpr, int addr)
   /* Check a pcode address */
{
   void stop_interp(int curpr, enum pstat);
   if ((addr >= CMAX)||(addr < 0)) {
#if defined(TCL)
      sprintf(tcl_buffer,
         "\nBad pcode address:  %d    at loc  %d of current process:  %d\n",
         addr,ptab[curpr].pc,curpr);
      tcl_1printf(tcl_buffer,0);
#else
      printf("\nBad pcode address:  %d    at loc  %d of current process:  %d\n",
         addr,ptab[curpr].pc,curpr);
#endif
      stop_interp(curpr,padrchk);
      exit(1);
   }
   return addr;
}  /* paddr_check */

void show_display(int pr)
   /* Show the display for the process 'pr' */
{
int   ix;
   if ((pr >= 0) && (pr <= PMAX)) {
      sprintf(out_buf,"Display for process: %s\n",
         ((ptab[pr].tabix > 0) ? tab[ptab[pr].tabix].name : "???"));
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
      for (ix = LMAX; ix >= 0; ix--) {
         sprintf(out_buf,"%3d %4d\n",ix,ptab[pr].display[ix]);
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
      }
   } /* if */
}  /* show_display */
         
char* show_stack_loc(char *buf, TYPES t, int sta)
   /* Write the contents of the stack location s[sta] having  */
   /* type t into the caller's buffer buf                     */
{
   int tmp;
   char* p = buf;
   switch (t) {
      case  bsems:
      case  sems:
      case  bools:
      case  ints: sprintf(buf,"  %6d",s[sta]); break;
      case  chars:   
         tmp = s[sta] % 256;
         if ((tmp < charl) || (tmp > charh)) 
            sprintf(buf,"  0x%02X",tmp);
         else
            sprintf(buf,"  %c",tmp);
         break;
      case  strings:
         sprintf(buf,"  %s",(char *) &s[sta]);
      default: ;
   }  /* switch */
   while (*p) p++;   /* find the end */
   return p;
}  /* show_stack_loc */

int get_stack_addr( int prno, int tix, int vix)
   /* Calculate the base address on the stack for a pgm variable */
   /* tix   tab index of the block containing the variable       */
   /* vix   tab index of the variable                            */
   /* pno   process number of the process owning the variable    */
{
   /* global int main_b; */
   int tmp;
   if (tab[vix].lev == 0)
   /* if a global variable, then global base is 0, so return var's addr */
      tmp = tab[vix].adr;
   else if (tab[tix].obj == outerblock)
   /* base of outer block is at stack loc 0 */
      tmp = tab[vix].adr;
   else if (tab[tix].obj == mainproc)
      /* base addr of main block is main_b */
      tmp = main_b+tab[vix].adr;
   else if (tab[tix].obj == monitor)
      tmp = tab[tix].mon + tab[vix].adr;
   else  /* use current process */
      if (prno == 0)
         tmp = ptab[0].b + tab[vix].adr;
      else { /* for a process we have to calculate it's frame base */
         tmp = (ptab[prno-1].stacksize + 1) + tab[vix].adr;
      }
   return tmp;
}  /* get_stack_addr */

char* show_simple_var(char *buf, int prno, int tix, int vix)
   /* Write to the buffer buf the value of a simple (scalar) variable    */
   /* that has tab index vix and belongs to process pno and to the block */
   /* with tab index tix                                                 */
{
   int tmp;
   char *p = buf;
   tmp = get_stack_addr(prno,tix,vix);
   if (tab[vix].typ == strings) {
      sprintf(buf,"string[%d] %-12s @s[%d]",4*tab[vix].ref-1,
         tab[vix].name, tmp);
   }
   else {
      sprintf(buf,"%-6s %-12s @s[%d]", typenames[tab[vix].typ],
         tab[vix].name, tmp);
   }
   while (*p) p++;
   p = show_stack_loc(p,tab[vix].typ,tmp);
   return p;
}  /* show_simple_var */

#define LINEWIDTH 60

void show_row(int vix, int stkadr, int top, int aix[], int inx[])
   /* Show the last (rightmost) dimension of a matrix with */
   /* tab index vix and base addr stkadr                   */
{
   int i;
   int low = atab[aix[top]].low;
   int high = atab[aix[top]].high;
   TYPES t = atab[aix[top]].eltyp;
   int base = stkadr;
   char* p;
   sprintf(out_buf,"%-6s %s", typenames[t], tab[vix].name);
   for (i = 0; i < top; i++) {
      sprintf(tmp_buf,"%s [%d]",out_buf,inx[i]); 
      strcpy(out_buf,tmp_buf);
   }
   sprintf(tmp_buf,"%s [%d..%d]  @s[%d]\n",out_buf,low,high, stkadr);
   strcpy(out_buf,tmp_buf);
#if defined(TCL)
   tcl_1printf(out_buf,DATAWIN);
#else
   fputs(out_buf,stdout);
#endif
   p = out_buf;
   for (i = low; i <= high; i++) { 
      p = show_stack_loc(p,atab[aix[top]].eltyp,base);
      base++;
      if (((int) p - (int) out_buf) > LINEWIDTH) {
         strcat(p,"\n");
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
         p = out_buf;
      }
   }
   if (p != out_buf) {
      strcat(out_buf,"\n"); 
#if defined(TCL)
      tcl_1printf(out_buf,DATAWIN);
#else
      fputs(out_buf,stdout);
#endif
   }
}  /* show_row */

int make_slice(int vix, int level, int stkadr, int top, int aix[], int inx[])
   /* recurse to the rightmost (outer) dimension of an array, keeping   */
   /* track in inx of the current array index of each dimension.        */
   /* On reaching the rightmost dimension, show the "row"               */
/* Perhaps an example will help
   Assume the following declaration in the outer block of a Pascal pgm.
      c : array[1..3, 4..5, 6..8] of integer;
   Suppose that the tab index of the main block is 2, the tab index of
   the array c is 5, and the base address of the array c on the stack is
   14.  Suppose that the atab indices of the dimensions of c (left to right)
   are 2, 3, and 4, resp. 
   make_aix_array(5,*top,aix) will result in top == 2, aix == 2 3 4

   The make_slice(5,0,14,2,aix,inx); call in show_array will recurse
   through the dimensions of c showing the last dimension for
   each combination of values for the lower dimensions.  That is,
   the following make_slice and show_row calls result:
               vix level stkadr top  aix     inx
   make_slice  5     0     14    2   2 3 4   1
   make_slice  5     1     14    2   2 3 4   1 4
   make_slice  5     2     14    2   2 3 4   1 4
   show_row    5           14    2   2 3 4   1 4 shows c[1][4][6..8]
   make_slice  5     2     17    2   2 3 4   1 5
   show_row    5           17    2   2 3 4   1 5 shows c[1][5][6..8]
   make_slice  5     1     20    2   2 3 4   2 5
   make_slice  5     2     20    2   2 3 4   2 4
   show_row    5           20    2   2 3 4   2 4 shows c[2][4][6..8]
   make_slice  5     2     23    2   2 3 4   2 5
   show_row    5           23    2   2 3 4   2 5 shows c[2][5][6..8]
   make_slice  5     1     26    2   2 3 4   3 5
   make_slice  5     2     26    2   2 3 4   3 4
   show_row    5           26    2   2 3 4   3 4 shows c[3][4][6..8]
   make_slice  5     2     29    2   2 3 4   3 5
   show_row    5           29    2   2 3 4   3 5 shows c[3][5][6..8]
*/       
{
   int i;
   
   if (level < top) {
      for (i = atab[aix[level]].low; i <= atab[aix[level]].high; i++) {
         inx[level] = i;
         stkadr = make_slice(vix,level+1, stkadr, top,aix,inx);
      }
   }
   else {
      show_row(vix,stkadr,top,aix,inx);
      stkadr += atab[aix[top]].size;
   }
   return stkadr;
}  /* make_slice */

void make_aix_array(int vtix, int* top, int aix[])
   /* Create an array, aix, of indices into the atab array        */
   /* for the array having tab index vtix.                        */
   /* aix[0] is the atab index of the first (leftmost) dimension  */
   /* of the array,  aix[1] is the atab index of the next         */
   /* dimension of the array and so on                            */
   /* At termination, aix[*top] will be the atab index of the     */
   /* last (rightmost) dimension of the array                     */
{
   int nexta;
   nexta = tab[vtix].ref;
   *top = -1;
   do {
      (*top)++;
      if (*top >= AMAX)
         fatal("Have more than %d dimensions in array %s\n", AMAX,tab[vtix]);
      aix[*top] = nexta;
      nexta = atab[nexta].elref;
   } while (nexta);
}  /* make_aix_array */
  
void show_array(int prno, int tix, int vix)
   /* Show the array with tab index vix            */
   /* in process prno having tab index tix         */
   /* See the above example for clarification      */
{
   int aix[AMAX];
   int inx[AMAX];
   int top;
   int base;
   int i;
   /* get the base address of the array */
   base = get_stack_addr(prno, tix, vix);
   /* create the array of atab indices */
   make_aix_array(vix,&top,aix);
   if (top > 0) {
      sprintf(out_buf,"%-6s %s", typenames[atab[aix[top]].eltyp],
         tab[vix].name);
      for(i = 0; i <= top; i++)  {
         sprintf(tmp_buf,"%s [%d..%d]",out_buf,atab[aix[i]].low,
            atab[aix[i]].high);
         strcpy(out_buf,tmp_buf);
      }
      strcat(out_buf,"\n"); 
#if defined(TCL)
      tcl_1printf(out_buf,DATAWIN);
#else
      fputs(out_buf,stdout);
#endif
   }
   base = make_slice(vix, 0, base, top, aix, inx);
}  /* show_array */

void show_vars(int prno,int tix)
   /* show the variables in the block with tab index tix  */
{
   char* p;
   int ix;
   int bix = tab[tix].ref;
      /* if psize == vsize, then this block has no variables */
   ix = btab[bix].last; 
   while (ix) {
      if (tab[ix].obj == variable) {
         switch (tab[ix].typ) {
            case notyp:
            case ints:
            case bools:
            case chars:
            case sems:
            case bsems:
            case conds:
            case strings:
               p = show_simple_var(out_buf,prno,tix,ix);
               strcat(p,"\n");
#if defined(TCL)
               tcl_1printf(out_buf,DATAWIN);
#else
               fputs(out_buf,stdout);
#endif
               break;
            case arrays:
               show_array(prno,tix, ix); break;
            default: ;
         } /* inner if */
      }  /* outer if */
      ix = tab[ix].link;
   }  /* while */
}  /* show_vars */

void write_header(char* hdr, int curpr)
   /* write the header 'hdr' with a time, date & location stamp */
{
   int pc, dbgix;
   time_t x;
   int lno;
   int pc_to_dbgix(int);
#if defined(SunOS)
   extern time_t time(time_t*);
#endif

   pc = ptab[curpr].pc;
   dbgix = pc_to_dbgix(pc);
   time(&x);
   lno = dbg[dbgix].flno;
   if (lno < 0) lno = -lno;
   sprintf(out_buf,"\n%s\n Process %d, loc %d, line %d of %s\n %s",hdr,curpr,
      pc,lno,inputfile[dbg[dbgix].fix].fname,ctime(&x));
   /* ctime inserts a newline '\n' */
#if defined(TCL)
   tcl_1printf(out_buf,DATAWIN);
#else
   fputs(out_buf,stdout);
#endif
}  /* write_header */

void write_process_table(int curpr)
   /* write the process table */
{
   int   x;
   write_header("Process Table",curpr);
   sprintf(out_buf,"\n Process        Active  Suspend  PC    xpc   atomic\n");
#if defined(TCL)
   tcl_1printf(out_buf,DATAWIN);
#else
   fputs(out_buf,stdout);
#endif
   for (x = 0; x <= PMAX; x++)
      if (ptab[x].active >= 0) {
         if (ptab[x].tabix > 0)
            sprintf(out_buf,"%2d %-12s %5d  %5d  %5d  %5d  %5d\n",
               x,tab[ptab[x].tabix].name,ptab[x].active,
               ptab[x].suspend,ptab[x].pc,ptab[x].xpc,ptab[x].atomic);
         else
            sprintf(out_buf,"%2d %-12s %5d  %5d  %5d  %5d  %5d\n", x, " ",
               ptab[x].active,ptab[x].suspend,ptab[x].pc,
               ptab[x].xpc,ptab[x].atomic);
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
      }
}  /* write_process_table */

void write_global_variables(int curpr)
   /* write the global and main proc variables */
{
   write_header("Global and Mainproc Variables",curpr);
   sprintf(out_buf,
      "\nGlobal Variables \ntype  name         stack loc  value\n");
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
   /*show_vars(0,0);*/
   show_vars(0,1);
   sprintf(out_buf,
      "\nMainproc Variables \ntype  name         stack loc  value\n");
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
   show_vars(0,ptab[0].tabix);
}  /* write_global_variables */

void write_monitor_variables(int curpr)
   /* write all monitor global variables */
{
   extern int last_tab;
   int x;

   write_header("Monitor Variables",curpr);
   sprintf(out_buf, "type  name         stack loc  value\n");
#if defined(TCL)
   tcl_1printf(out_buf,DATAWIN);
#else
   fputs(out_buf,stdout);
#endif
   for (x = 0; x <= last_tab; x++)
      if (tab[x].obj == monitor) {
         sprintf(out_buf,"%s:\n",tab[x].name);
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
         show_vars(0,x);
      }
}  /* write_monitor_variables */

void write_currproc_variables(int curpr)
   /* write the variables of the current procedure or function */
{
   char tbuf[80];
   int tix;
   int tix_addr = ptab[curpr].b + 4;
   if ((tix_addr < 0)||(tix_addr >= ptab[curpr].stacksize)) {
      fprintf(stdout,"\n***Invalid stackframe address %d for process %d\n",
         tix_addr,curpr);
      fprintf(stdout,"*** Cannot write the variables for process %d\n",curpr);
      return;
   }
   /* procedure's tab index is at s + 4 */
   tix = s[tix_addr];
   if (tix >= TMAX) {
      fprintf(stdout,"write_currproc_variables: bad tab index %d > %d\n",
         tix,TMAX);
      exit(1);
   }
   sprintf(tbuf,"Variables of current procedure or function %s",tab[tix].name);
   write_header(tbuf,curpr);
   show_vars(curpr,tix);
}  /* write_currproc_variables */

void write_process_variables(int curpr)
   /* write the variables of the current process */
{
   int x;

   write_header("Process Variables",curpr);
   sprintf(out_buf,"type  name         stack loc  value\n");
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
   for (x = 1; x <= PMAX; x++)
      if (ptab[x].active >= 0) {
         sprintf(out_buf,"Process #%d  %s\n",x,tab[ptab[x].tabix].name);
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
         show_vars(x,ptab[x].tabix);
      }
}  /* write_process_variables */

#define ITEMSPERLINE 10

void dump_stack(int b, int t)
   /* write the stack from s[t] down to s[b] */
{
   int ix, count;
   out_buf[0] = '\0';
   for (ix = t, count = 0; ix >= b; ix--){
      if (s[ix] == UNDEF_VALUE)
         strcat(out_buf,"----  ");
      else {
         sprintf(tmp_buf,"%s%4d  ",out_buf,s[ix]); 
         strcpy(out_buf,tmp_buf);
      }
      if (++count == ITEMSPERLINE){
         strcat(out_buf,"\n");
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
         out_buf[0] = '\0';
         count = 0;
      }
   }  /* for */
   if (count > 0) {
      strcat(out_buf,"\n");
#if defined(TCL)
      tcl_1printf(out_buf,DATAWIN);
#else
      fputs(out_buf,stdout);
#endif
   }
}  /* dump_stack */

void show_AR(int b,int t)
   /* show the AR s[t] down to s[b] */
{
   int localb = ((b < 0) ? 0 : b);
   sprintf(out_buf,"Activation Record  t = %3d down to b = %3d\n",t,b);
#if defined(TCL)
   tcl_1printf(out_buf,DATAWIN);
#else
   fputs(out_buf,stdout);
#endif
   sprintf(out_buf,"  for program unit: %s\n",tab[s[b+4]].name);
#if defined(TCL)
   tcl_1printf(out_buf,DATAWIN);
#else
   fputs(out_buf,stdout);
#endif
   dump_stack(localb,t);
}  /* show_AR */

void write_all_stacks(int curpr)
   /* dump the stacks of the main proc and concurrent procs */
{
   int i;
   write_header("Runtime Stacks",curpr);

   sprintf(out_buf,"Stack for main proc from %d down to 0\n", ptab[0].t);
#if defined(TCL)
   tcl_1printf(out_buf,DATAWIN);
#else
   fputs(out_buf,stdout);
#endif
   dump_stack(0,ptab[0].t);
   sprintf(out_buf,"Stacks of concurrent processes\n");
#if defined(TCL)
   tcl_1printf(out_buf,DATAWIN);
#else
   fputs(out_buf,stdout);
#endif
   for (i = 1; i <= PMAX; i++)
      if (ptab[i].active != -1) {
         sprintf(out_buf,"Process # %d:  %s stack from %d down to %d\n",
            i, tab[ptab[i].tabix].name, ptab[i].t, ptab[i].b);
#if defined(TCL)
         tcl_1printf(out_buf,DATAWIN);
#else
         fputs(out_buf,stdout);
#endif
         dump_stack(ptab[i].b,ptab[i].t);
      }
}  /* write_all_stacks */

void stop_interp(int curpr,enum pstat ps)
   /* stop the interpreter and dump debug information */
{
   void write_process_table(int);
   void write_monitor_variables(int);
   void write_process_variables(int);
   void write_currproc_variables(int);
   void write_all_stacks(int);

#if defined(TCL)
   void makedatawin();
#endif
   if (ps != fin) { 
#if defined(TCL)
      makedatawin();
#endif
      sprintf(out_buf,"\n*** Halt at %5d in process %4d because ",
         ptab[curpr].pc,curpr);
      switch (ps){
         case deadlock: strcat(out_buf,"of deadlock\n"); break;
         case divchk  : strcat(out_buf,"of division by 0\n"); break;
         case inxchk  : strcat(out_buf,"of invalid array index\n"); break;
         case stkchk  : strcat(out_buf,"of invalid stack address\n"); break;
         case padrchk : strcat(out_buf,"of invalid code address\n"); break;
         case charchk : strcat(out_buf,"of invalid character\n"); break;
         case inpchk  : strcat(out_buf,"of invalid input\n"); break;
         case redchk  : strcat(out_buf,"of reading past end of file\n"); break;
         case semchk  : strcat(out_buf,"of a negative semaphore initial value\n"); 
                        break;
         case bsemchk : strcat(out_buf,"of a binary semaphore not 0 or 1\n");
                        break;
         case uisemchk: strcat(out_buf,"of an un-initialized semaphore\n"); 
                        break;
         case xmonchk : strcat(out_buf,"of a cross monitor call\n"); break;
         case pcodchk : strcat(out_buf,"of invalid PCODE\n"); break;
         case rdchk   : strcat(out_buf,"this variable type cannot be read\n"); 
                        break;
         case wrtchk  : strcat(out_buf,"this variable type cannot be written\n"); 
                        break;
         case procchk : strcat(out_buf,"of invalid process number\n"); break;
         case proctabchk: strcat(out_buf,"process table is exhausted\n"); break;
         default: ;
      }  /* switch */
#if defined(TCL)
      tcl_1printf(out_buf,DATAWIN);
#else
      fputs(out_buf,stdout);
#endif
      write_process_table(curpr);
      write_global_variables(curpr);
      write_monitor_variables(curpr);
      write_process_variables(curpr);
      write_currproc_variables(curpr);
      write_all_stacks(curpr);
   }
   else { /* ps == fin */
      if (finalAR_db) {
         sprintf(out_buf,"PROGRAM EXIT\n");
#if defined(TCL)
            tcl_1printf(out_buf,DATAWIN);
#else
            fputs(out_buf,stdout);
#endif
         show_display(0);
         sprintf(out_buf,"Stack for main proc from %d down to 0\n",
            ptab[0].t);
#if defined(TCL)
            tcl_1printf(out_buf,DATAWIN);
#else
            fputs(out_buf,stdout);
#endif
         dump_stack(0,ptab[curpr].t);
      }
   } /* else ps == fin  */
   if (pcexec_db) {
      fclose(xpcode);
      sprintf(out_buf,"PCODE execution trace stored in %s\n",xpcode_fname);
#if defined(TCL)
      tcl_1printf(out_buf,0);
#else
      fputs(out_buf,stdout);
#endif
   }
}  /* stop_interp */


void local_strcpy(char* d, char* s)
   /* When the stack words were initialized to 0x00003f3f, the C-Lib */
   /* strcpy issued a SEGV when it hit the 3f's.  The bare-bones     */
   /* local version below is too dumb to care.                       */
{
   while (*s) {
      *d = *s;
      d++;s++;
   }
   *d = '\0';
}  /* local_strcpy */

void init_proc_displays()
   /* initialize the displays for the concurrent threads to point to    */
   /* the stack frames of the outer, global scope and the scope of the  */
   /* main block                                                        */
{
   int ix, jx;
   int main_b = btab[0].vsize;   /* size of the global frame, base of main */
   for (ix = 1; ix <= PMAX; ix++) {
      ptab[ix].display[0] = 0;      /* base of the global frame */
      ptab[ix].display[1] = main_b; /* base of the main frame */
      for (jx = 2; jx <= LMAX; jx++) ptab[ix].display[jx] = 0;
   }
}  /* init_proc_displays */

void init_xpcodefile(char* pcode_fname)
   /* open and initialize the xpcode file */
{      
   char* p;
   strcpy(xpcode_fname,pcode_fname);
   p = strrchr(xpcode_fname,'.');
   if (p == NULL)
      strcat(xpcode_fname,xpcode_suffix);
   else
      strcpy(p,xpcode_suffix);
   if ((xpcode = fopen(xpcode_fname,"w+")) == NULL){
      fprintf(stderr,"Can't open file for executed PCODE!\n");
      exit(1);
   }
   fprintf(xpcode,"Executing PCODE: %s\n",filename_line);
   fprintf(xpcode,"proc  lc    f    x    y     disassembled instruction\n");
}  /* init_xpcodefile */

void init_interpreter( )
   /* initialize the interpreter's data structures */
{
   extern int curpr, npr, pflag, stepcount, starting_thread;
   int ix;           /* loop counter */
   int block0_vsize; /* size of outer block */
      /* max outer block size is the total stack size minus the space */
      /* needed for the stackframes for the PMAXP1 possible threads   */
   int MAX_BLOCK0_VSIZE = STMAX - PMAXP1*STKINCR; 

   charl = ' ';    /* lowest char ord */ 
   charh = '~';    /* highest character ordinal */

   /* initialize the stack */
   for (ix = 0; ix < STMAX; ix++) s[ix] = 0;

   /* initialize space for variables in the outer block */
   block0_vsize = btab[0].vsize;
   if (block0_vsize > MAX_BLOCK0_VSIZE) {
      fprintf(stderr,
         "init_interpreter: outer block size (%u)\n   larger than allowable space %d\n",
            btab[0].vsize,MAX_BLOCK0_VSIZE);
      exit(1);
   }
   /* establish stack frame for main pgm */ 
   ptab[0].b = main_b = block0_vsize;
   s[main_b] = 0;                 /* main proc result */
   s[main_b+1] = 0;               /* main proc return address */
   s[main_b+2] = 0;               /* main proc static link */
   s[main_b+3] = -1;              /* main proc dynamic link */
   s[main_b+4] = btab[0].lastpar; /* tab index of main proc temp stored here */
   /* set tab index of main proc */
   main_tix = ptab[0].tabix = s[main_b+4]; 
   /* initialize display of main proc */
   for (ix = 0; ix <= LMAX; ix++) ptab[0].display[ix] = 0;
   ptab[0].display[0] = 0;                   /* outer block */
   ptab[0].display[1] = main_b;              /* main proc */
   /* set top of stack to sum of sizes of stackframes of outer block */
   /* and main proc                                                  */
   ptab[0].t = block0_vsize + btab[tab[main_tix].ref].vsize - 1;
   if (ptab[0].t >= STMAX) {
      fprintf(stderr,
         "init_interpeter: Process 0 Stack Top (ptab[0].t) (%u)\n   greater than stack size %d\n",
         ptab[0].t,STMAX);
      exit(1);
   }
   ptab[0].pc = tab[main_tix].adr;  /* set entry point */
   ptab[0].active = 1;              /* main proc is active */
   ptab[0].suspend = NOT_SUSPENDED; /* main proc is not suspended */
   ptab[0].monitor = 0;             /* not in a monitor call */
   ptab[0].priority = 0;            /* at this priority level */ 
   ptab[0].xpc = 0;                 /* # PCODE instructions executed */
   ptab[0].first_spc = ptab[0].last_spc = -1;
   /* main proc gets STKINCR, plus what's left over after allocating */
   /* the stack to the other procs and the global space              */
   ptab[0].stacksize = ptab[0].b + STKINCR + MAX_BLOCK0_VSIZE - block0_vsize;
   /* show main proc startup information, if necessary */
   if (initAR_db) {
#if defined(TCL)
      tcl_1printf("STARTUP ",DATAWIN);
#else
      printf("STARTUP ");
#endif
      show_display(0); 
      show_AR(ptab[0].b,ptab[0].t);
   }

   for (ix = 0; ix <= last_dbg; ix++)
      if (dbg[ix].lc == tab[main_tix].adr) {
         main_dbix = ix;
         break;
      }


   /* initialize proc table entries for all other processes */
   for (ix = 1; ix <= PMAX; ix++) {
      ptab[ix].active = -1; /* for process in use, active = 0 or 1 */
      ptab[ix].pc = 0;
      ptab[ix].monitor = 0;
      ptab[ix].priority = ptab[ix].xpc = 0;
      ptab[ix].tabix = -1;
      ptab[ix].suspend = NOT_SUSPENDED;
      ptab[ix].atomic = 0;
      ptab[ix].b = ptab[ix - 1].stacksize + 1;
      /* make sure that return addr stored in AR of each proc is zero, */
      /* because EXIT_PROC keys on it                                  */
      s[ptab[ix].b+1] = 0;
      ptab[ix].stacksize = ptab[ix].b + STKINCR - 1;
      ptab[ix].t = ptab[ix].b - 1;
      ptab[ix].first_spc = ptab[ix].last_spc = -1;
   }
   init_proc_displays();
   /* initialize the biggies */
   npr = curpr = 0;
   starting_thread = pflag = 0;
   __randomize();   /* wake up the random number generator */
   stepcount = 0; 
   ps = run;
}  /* init_interpreter */

int find_words(char* s,char* word[])
   /* splits the string 's' into words on the tabs & blanks */
   /* returns the number of words found                     */
{
   char* tmp;
   int i = 0;
   tmp = strtok(s,"\t \n");
   while (tmp) {
      word[i] =  tmp;
      i++;
      tmp = strtok(NULL,"\t \n");
   } 
   return i;
}  /* find_words */

int pc_to_dbgix(int pc)
   /* convert a pcode address to the index of the corresponding entry */
   /* in the dbg array                                                */
   /* The in_shortcall flag keeps the cursor from bouncing around     */
   /* during the initialization of the outer scope                    */
{
   int ix;
   if (in_shortcall) return main_dbix;
   for(ix = 0; ix < last_dbg; ix++)
      if ((pc >= dbg[ix].lc)&&(pc < dbg[ix+1].lc)) {
         return ix;
      }
   return last_dbg;
}  /* pc_to_dbgix */

int lno_to_pc(int curr_file, int lno)
   /* convert from an index curr_file in the input_file array */
   /* and a line number lno in that file into a pcode addr    */
{
   int tlno;
   int ix;
   for(ix = 0; ix < last_dbg; ix++)
      if (curr_file == dbg[ix].fix) {
         tlno = dbg[ix].flno;
         if (tlno < 0) tlno = -tlno;
         if (lno <= tlno) return dbg[ix].lc;
      }
   return -1;
}  /* lno_to_pc */
         
int current_codeblock(int pc)
   /* return the cbtab index of the codeblock corresponding to 'pc' */
{
   int ix;
   for(ix = 0; ix < last_cbtab; ix++)
      if ((pc >= cbtab[ix].adr)&&(pc < cbtab[ix+1].adr))
         return ix;
   return last_cbtab;
}  /* current_codeblock */

int at_breakpoint(int pc)
   /* if 'pc' is at a breakpoint, then return its index in breakpoint[], */
   /* else return -1                                                     */
{
   int ix;
   for (ix = 0; ix < break_cnt; ix++)
      if (pc == breakpoint[ix].adr)
         return breakpoint[ix].uix;
   return -1;
}  /* at_breakpoint */

int set_break(int pc, int* lno)
   /* Set a breakpoint at pcode address pc                      */
   /* Return through the function value the input_file index    */
   /* of the file that contains the pcode address               */
   /* Return in *lno the line number of that file that contains */
   /* the pcode address                                         */
   /* This information is used by Tcl/Tk routines to mark the   */
   /* appropriate line in the right source window               */
{
   int ix;
   char out_buf[256];
   char tmp_buf[256];
   if (break_cnt == MAXBREAK - 1) {
      sprintf(out_buf,"Can't set another breakpoint.\n");  
      sprintf(tmp_buf,
         "At most %d breakpoints can be active simultaneously\n",
         MAXBREAK);
      strcat(out_buf,tmp_buf);
#if defined(TCL)
      tcl_message(out_buf);
#else
      fputs(out_buf,stdout);
#endif
      return -1;
   }
   if (pc > last_code) {
      sprintf(out_buf,"%d is larger than the largest PCODE addr %d\n",
         pc,last_code);
#if defined(TCL)
         tcl_message(out_buf);
#else
         fputs(out_buf,stdout);
#endif
      return -2;
   }
   for(ix = 0; ix < break_cnt; ix++) 
      if (breakpoint[ix].adr == pc) {
         sprintf(out_buf,"Breakpoint # %d already set at location %d\n",
            breakpoint[ix].uix,pc);
#if defined(TCL)
         tcl_message(out_buf);
#else
         fputs(out_buf,stdout);
#endif
         *lno = dbg[breakpoint[ix].dbgix].flno;
         return dbg[breakpoint[ix].dbgix].fix;
      }
   breakpoint[break_cnt].adr = pc;
   breakpoint[break_cnt].tix = cbtab[current_codeblock(pc)].tix;
   breakpoint[break_cnt].uix = next_uix;
   breakpoint[break_cnt].dbgix = ix = pc_to_dbgix(pc);
   next_uix++;
   break_cnt++;
   *lno = dbg[ix].flno;
   return dbg[ix].fix;
}  /* set_break */

int unset_break(int break_uix,int* lno)
   /* Unset the breakpoint with user index break_uix            */
   /* Return through the function value the input_file index    */
   /* of the file that contains the pcode address               */
   /* Return in *lno the line number of that file that contains */
   /* the pcode address                                         */
   /* This information is used by Tcl/Tk routines to mark the   */
   /* appropriate line in the right source window               */
{
   int ix, jx;
   int fno;
   char out_buf[256];

   *lno = 0;
   if (break_uix < 0) {
      sprintf(out_buf,"Invalid breakpoint index %d\n",break_uix);
#if defined(TCL)
      tcl_1printf(out_buf,0);
#else
      fputs(out_buf,stdout);
#endif
      return -1;
   }
   jx = -1;
   for (ix = 0; ix < break_cnt; ix++)
      if (breakpoint[ix].uix == break_uix) {
         jx = ix;
         break;
   }
   if (jx < 0) {
      sprintf(out_buf,"Breakpoint %d is not set\n",break_uix);
#if defined(TCL)
      tcl_1printf(out_buf,0);
#else
      fputs(out_buf,stdout);
#endif
      return -2;
   }
   ix = breakpoint[jx].dbgix;
   fno = dbg[ix].fix;
   *lno = dbg[ix].flno;
   for (ix = jx; ix < break_cnt; ix++){
      breakpoint[ix].adr = breakpoint[ix+1].adr;
      breakpoint[ix].tix = breakpoint[ix+1].tix;
      breakpoint[ix].uix = breakpoint[ix+1].uix;
      breakpoint[ix].dbgix = breakpoint[ix+1].dbgix;
   }
   breakpoint[break_cnt].adr = breakpoint[break_cnt].tix =
      breakpoint[break_cnt].uix = breakpoint[break_cnt].dbgix = 0;
   break_cnt--;
   return fno;
}  /* unset_break */

void list_breakpoint(int ix, char* buf)
   /* store information for breakpoint[ix] in buf (including \n) */
{
   int jx;
   if ((ix < 0)||(ix > break_cnt)) {
      sprintf(out_buf,"Invalid breakpoint index %d\n",ix);
#if defined(TCL)
      tcl_message(out_buf);
#else
      fputs(out_buf,stdout);
#endif
      return;
   }
   sprintf(buf,"%3d  %3d  %-12s",breakpoint[ix].uix,breakpoint[ix].adr,
      tab[breakpoint[ix].tix].name);
   jx = breakpoint[ix].dbgix;
   if (jx >= 0) {
      sprintf(tmp_buf,"%s  %3d  %s",buf,dbg[jx].flno,
         inputfile[dbg[jx].fix].fname);
      strcpy(buf,tmp_buf);
   }
}  /* list_breakpoint */

int get_breakindex(int ix)
   /* returns user index breakpoint[ix].uix */
{
   if ((ix < 0)||(ix > break_cnt)) {
      sprintf(tmp_buf,"Invalid breakpoint index %d\n",ix);
#if defined(TCL)
      tcl_message(tmp_buf);
#else
      fputs(tmp_buf,stdout);
#endif
      return -1;
   }
   return breakpoint[ix].uix;
}  /* get_breakindex */


void start_src_singlestep(int curr_file,int curpr, int pc)
   /* start single-stepping of 1 src instr */
{
   int dbgix, lno, pc2;
   dbgix = pc_to_dbgix(pc);
   ptab[curpr].first_spc = dbg[dbgix].lc;
   lno = dbg[dbgix].flno;
   pc2 = lno_to_pc(curr_file, lno+1);
   ptab[curpr].last_spc = pc2 - 1;
}  /* start_src_singlestep */

#if !defined(TCL)
void show_breakpoints()
   /* display the current breakpoints to stdout */
{
   int ix;
   char tbuf[256];

   if (break_cnt > 0) {
      printf("Current breakpoints\n");
      printf("index loc  block        line#  filename\n");
      for (ix = 0; ix < break_cnt; ix++) {
         list_breakpoint(ix,tbuf);
         fputs(tbuf,stdout);
      }
   }
   else {
      printf("No breakpoints are currently set\n");
   }
}  /* show_breakpoints */


void debugger_help()
   /* show the list of debugger command on stdout */
{
   printf("Debugger Commands:\n");
   printf("   b  lc     -- set a break at location 'lc'\n");
   printf("   B  lno    -- set a break at line number 'lno' of current file'\n");
   printf("   c         -- continue to the next breakpoint\n");
   printf("   d         -- dump the stack of the current process\n");
   printf("   d  t      -- dump 10 stack words from s[t] down to s[t-10]\n");
   printf("   d  t b    -- dump stack words from s[t] down to s[b]\n");
   printf("   F         -- list indices and names of source files\n");
   printf("   F  n      -- make source file 'n' the current file\n");
   printf("   h         -- show this help\n");
   printf("   l  lno    -- display lines 'lno'...'lno'+9 of current file\n");
   printf("   i         -- show current breakpoints\n");
   printf("   m         -- show monitor variables\n");
   printf("   n         -- show lc limits and line # of current source line\n");
   printf("   o  fname  -- open PCODE file 'fname.pco'\n");
   printf("   p         -- show process table\n");
   printf("   q         -- terminate execution\n");
   printf("   r         -- reload PCODE file\n");
   printf("   s         -- execute one PCODE instruction\n");
   printf("   S         -- execute one source line by current process\n");
   printf("   RETURN    -- repeat singlestep or continue\n");
   printf("   u  i      -- unset breakpoint[i]\n");
   printf("   w         -- show where current execution is\n"); 
   printf("   wc        -- write variables of current procedure or function\n");
   printf("   wg        -- write global variables\n");
   printf("   wm        -- write monitor variables\n");
   printf("   wp        -- write process variables\n");
   printf("   ws        -- write all stacks\n");
   printf("   x         -- disassemble the next 10 instructions\n");
   printf("   x  loc    -- dissassemble 10 instructions starting at 'loc'\n");
}  /* debugger_help */
   
extern void disassemble(char*,int,int);

void show_lines(int curr_file, int lno)
   /* show the disassembled PCODE instructions corresponding the line */
   /* numbered lno of the file with input_file index curr_file        */
{
   FNAME_STRING s;
   FILE* f;
   int flno;
   sprintf(s,"%s%s",pcode_dir,inputfile[curr_file].fname);
   f = fopen(s,"r");
   if (f == NULL) {
      printf("The source file \"%s\" is not available\n",s);
      return;
   }
   flno = 0;
   if (lno <= 0) lno = 1;
   while ((!feof(f)) && (flno < lno)) {
      flno++;
      fgets(tempbuf,TEMPBUFSIZE,f);
   }
   printf("file: %s\n",s);
   while((!feof(f)) && (flno < lno+10)) {
      printf("%3d ",flno);
      fputs(tempbuf,stdout);
      flno++;
      fgets(tempbuf,TEMPBUFSIZE,f);
   }
   fclose(f);
}  /* show_lines */

static char xbuf[MAX_FNAME];
static char ybuf[MAX_FNAME];

void debugger(int curpr, int* curr_file, int* singlestep,int *continuing)
   /* implementation ofthe functionality of the pcode debugger */
{
   void check_unresolved_externs();
   FILE* p;
   int reading_cmds = 1;
   char* word[50];
   int word_cnt;
   char* prompt = "(h = help)>";
   int i, dbgix, b, t, pc, pc2, oldpc;
   int cbix;
   int tlno;
   int count;
   int dummy;
   char prev_cmd, cmd;
   static int lno = 1;

   prev_cmd = 's';
   while (reading_cmds && *singlestep) {
      printf("%s ", prompt);
      fgets(xbuf,MAX_FNAME,stdin);
      word_cnt = find_words(xbuf,word);
      if (word_cnt == 0) {
         *singlestep = 1;
         break;
      }
      cmd = word[0][0];
      if (cmd == '\n') 
         cmd = prev_cmd;
      else
         prev_cmd = cmd;
      switch(cmd) {
         case 'b':
            pc = atoi(word[1]);
            set_break(pc,&dummy);
            break;
         case 'B':
            lno = atoi(word[1]);
            pc = lno_to_pc(*curr_file, lno);
            set_break(pc,&dummy);
            break;
         case 'c':
            *singlestep = 0;
            *continuing = 1;
            reading_cmds = 0;
            break;
         case 'd':
            if (word_cnt == 1) {
               t = ptab[curpr].t;
               b = ptab[curpr].b;
            } else if (word_cnt == 2) {
               t = atoi(word[1]);
               if (t >= STMAX) t = STMAX - 1;
               b = t - 10;
               if (b < 0) b = 0;
            } 
            else {
               t = atoi(word[1]);
               if (t >= STMAX) t = STMAX - 1;
               b = atoi(word[2]);
               if (b < 0) b = 0;
            }
            if (t >= b ) {
               printf("Stack for Process #%d: %s from %d down to %d\n",
                  curpr,tab[ptab[curpr].tabix].name,t,b);
               dump_stack(b,t);
            }
            break;
         case 'F':
            printf("Source files (* ==> current file):\n");
            for (i = 0; i <= last_infile; i++) {
               if (i == *curr_file) 
                  printf("%3d*  %-20s\n",i,inputfile[i].fname);
               else
                  printf("%3d   %-20s\n",i,inputfile[i].fname);
            }
            if (word_cnt > 1) {
               t = atoi(word[1]);
               if ((t > last_infile)|| (t < 0)) 
                  printf("%d is not a valid source file index.  \
Current file unchanged\n",t);
               else {
                  *curr_file = t;
                  printf("Current file is now %d  %-20s\n",
                     t,inputfile[t].fname);
                  lno = 1;
               }
            }
            break;
         case 'h':
            debugger_help();
            break;
         case 'i':
            printf("Current process # %d: %s  pc = %d  b = %d  t = %d\n",
               curpr,tab[ptab[curpr].tabix].name, ptab[curpr].pc,
               ptab[curpr].b, ptab[curpr].t);
            show_display(curpr);
            show_breakpoints();
            break;
         case 'l':
            if (word_cnt == 2) lno = atoi(word[1]);
            show_lines(*curr_file,lno);
            lno += 9;
            break;
         case 'n':
            pc = ptab[curpr].pc - 1 ; /* has just been incremented */
            dbgix = pc_to_dbgix(pc);
            lno = dbg[dbgix].flno;
            pc2 = lno_to_pc(*curr_file, lno+1);
            pc2--;
            printf("At lc %d, lc limits for line %d of file %s:  %d to %d\n",
               pc, lno, inputfile[dbg[dbgix].fix].fname, dbg[dbgix].lc, pc2);
            break;
         case 'o':
            if (word[1][0] == '\0') 
               fprintf(stdout,"no filename given\n");
            else {
               strcpy(ybuf,word[1]);
               if (fix_pcodefname(ybuf) >= 0) {
                  p = open_pcodefile(ybuf);
                  if (p != NULL) {
                     strcpy(pcode_fname,ybuf);
                     load_and_run(p);
                     check_unresolved_externs();
                     pc = ptab[curpr].pc;
                     cbix = current_codeblock(pc);
                     sprintf(tempbuf,"p%02d ",curpr);
                     disassemble(&tempbuf[4],cbtab[cbix].tix,pc);
                     fputs(tempbuf,stdout);
                     lno = 1;
                     *singlestep = 1;
                     *continuing = 0;
                  }   /* file opened */
               }  /* pcode file name was ok */
            }
            break;
         case 'p':
            write_process_table(curpr);
            break;
         case 'q':
            exit(0);
            break;
         case 'r':
            p = open_pcodefile(pcode_fname);
            load_and_run(p);
            check_unresolved_externs();
            pc = ptab[curpr].pc;
            cbix = current_codeblock(pc);
            sprintf(tempbuf,"p%02d ",curpr);
            disassemble(&tempbuf[4],cbtab[cbix].tix,pc);
            fputs(tempbuf,stdout);
            lno = 1;
            *singlestep = 1;
            *continuing = 0;
            break;
         case 's':
            reading_cmds = 0;
            *singlestep = 1;
            *continuing = 0;
            break;
         case 'S':
            start_src_singlestep(*curr_file,curpr,ptab[curpr].pc-1);
            *continuing = 1;
            *singlestep = 0;
            break;
         case 'u':
            pc = atoi(word[1]);
            unset_break(pc,&dummy);
            break;
         case 'w':
            switch (word[0][1]) {
               case '\0':
               case '\n':
                  pc = ptab[curpr].pc;
                  printf("Current process # %d: %s  pc = %d  b = %d  t = %d\n",
                     curpr,tab[ptab[curpr].tabix].name, pc,
                     ptab[curpr].b, ptab[curpr].t);
                  dbgix = pc_to_dbgix(pc);
                  tlno = dbg[dbgix].flno;
                  if (tlno < 0) tlno = - tlno;
                  printf("   line %d of %s\n",tlno,
                     inputfile[dbg[dbgix].fix].fname);
                  break;
               case 'c':
                  write_currproc_variables(curpr);
                  break;
               case 'g':
                  write_global_variables(curpr);
                  break;
               case 'm':
                  write_monitor_variables(curpr);
                  break;
               case 'p':
                  write_process_variables(curpr);
                  break;
               case 's':
                  write_all_stacks(curpr);
                  break;
               default:
                  printf("Unknown command: '%s'\n",word[0]);
                  break;
            }
            break;
         case 'x':
            count = 10;
            if (word_cnt == 1) 
               pc = ptab[curpr].pc;
            else {
               pc = atoi(word[1]);
               if (pc < 0) pc = 0;
            }
            t = ptab[curpr].tabix;
            for (oldpc = pc; pc - oldpc < count; pc++){
               if (pc > last_code) break;
               disassemble(tempbuf,t,pc);
               fputs(tempbuf,stdout);
            }
            break;
         case '\n':
         default:
            printf("Unknown command: '%s'\n",word[0]);
            break;
      }  /* switch */
   }  /* while */
}  /* debugger */
#endif

void check_unresolved_externs()
   /* check to see if there were any unresolved external references */
   /* in the .pco file                                              */
{
   extern int last_tab;
   int ix;
   int count = 0;
   char name[30];
   char what[30];
   name[0] = '\0';
   for(ix = 0; ix < last_tab; ix++) {
      switch (tab[ix].obj) {
         case ext_procedure:
            strcpy(name,tab[ix].name);
            strcpy(what,"void function");
            break;
         case ext_function:
            strcpy(name,tab[ix].name);
            strcpy(what,"function");
            break;
         case ext_variable:
            strcpy(name,tab[ix].name);
            strcpy(what,"variable");
            break;
         case ext_monitor:
            strcpy(name,tab[ix].name);
            strcpy(what,"monitor");
            break;
         default:  /* nothing */ ;
      }
      if (name[0]) {
         fprintf(stderr,"Source file contains unresolved external %s: %s\n",
            what,name);
         name[0] = '\0';
         count++;
      }
      if (count) 
         fatal("Cannot execute source file because of %d unresolved externals",
               count);
   }     
}  /* check_unresolved_externs */
            

int next_word(char* buf, char** scanstr)
   /* copy into 'buf' the next whitespace substring in the  */
   /* string pointed to by *scanstr and update *scanstr     */
{
   char* p = *scanstr;
   while ((*p)&&((*p == ' ')||(*p == '\t'))) p++;
   if (*p == '\0') return -1;
   while ((*p)&&(*p != ' ')&&(*p != '\t')) {
      *buf = *p;
      buf++; p++;
   }
   *buf = '\0';
   *scanstr = p;
   return 0;
}  /* next_word */

int next_string(char* buf, char** scanstr)
   /* copy into 'buf' the next "-delimited substring in the  */
   /* string pointed to by *scanstr and update *scanstr      */
   /* returns -1 on error, 0 if OK                           */
{
   char* p = *scanstr;

   while ((*p)&&((*p == ' ')||(*p == '\t'))) p++;
   if (*p != '"') return -1;
   p++; 
   while ((*p)&&(*p != '"')){
      *buf = *p;
      buf++; p++;
   }
   *buf = '\0';
   if (*p == '"') p++;
   *scanstr = p;
   return 0;
}  /* next_string */


int xsscanf(int curpr, int scan_strix, int six, int parm_cnt)
   /* s[scan_strix] has the address of the string to be scanned */
   /* stab[six] is the beginning of the format string */
   /* parm_cnt is the number of parms (not counting addr of scan string) */
{
   char *sp;
   char *fp;
   int i;
   int parm_offset;
   int op_addr;

   sp = (char* ) &s[saddr_check(curpr,s[scan_strix])]; 
   fp = (char* ) &stab[six];
   i = 0;
   parm_offset = scan_strix + 1;
   while (*fp) {
      if ((*fp == ' ')||(*fp == '\t')) {
         fp++; continue;
      }
      if (*fp == '%') {
         fp++;
         if (i == parm_cnt) return i;
         op_addr = saddr_check(curpr,s[parm_offset]);
         parm_offset++;
         tempbuf[0] = '\0';
         switch (*fp) {
            case 'd': 
               if (next_word(tempbuf,&sp)< 0) return i;
               sscanf(tempbuf,"%d",(int *) &s[op_addr]);
               i++;
               break;
            case 'x': 
               if (next_word(tempbuf,&sp)< 0) return i;
               sscanf(tempbuf,"%x",(int *) &s[op_addr]);
               i++;
               break;
            case 's': 
               if (next_word(tempbuf,&sp)< 0) return i;
               strcpy((char*) &s[op_addr], tempbuf);
               i++;
               break;
            case 'q':
               if (next_string(tempbuf,&sp) < 0) return i;
               strcpy((char*) &s[op_addr], tempbuf);
               i++;
               break;
            default:  
               return i;
         }
      }
      fp++;
   }
   return i;
}  /* xscanf */
      
static char tfmt[1024];

char cpfmt(char* tfmt, char** fp)
   /* copies the format string pointed to by *fp into the tfmt buffer */
   /* returns what is probably the format specifier character         */
{
   char* p = tfmt;
   char* q = *fp;
   char t = *q;
   while ((*q != ' ')&&(*q != '\t')&&(*q)) {
      *p = t = *q; 
      p++; q++;
      if ((t == 'd')||(t == 'i')||(t == 'u')||(t == 'o')||
         (t == 'x')||(t == 'X')||(t == 'c')||(t == 's')) break;
   }
   *p = '\0';
   *fp = q;
   return t;
}  /* cpfmt */

void xsprintf(int curpr, int scan_strix, int six, int parm_cnt)
   /* s[scan_strix] has the address of the string to be built */
   /* stab[six] is the beginning of the format string */
   /* parm_cnt is the number of parms (not counting addr of scan string) */
{
   int i;
   char* fp;
   char* bp;
   char* buf;
   char  fspec;
   int parm_offset;
   int op_addr;

   bp =  tempbuf;
   buf = (char* ) &s[saddr_check(curpr,s[scan_strix])];
   fp = (char* ) &stab[six];
   i = 0;
   parm_offset = scan_strix + 1;
   while(*fp) {
      while(*fp) {
         /* copy non-format stuff directly into output buffer */
         if (*fp == '\\') fp++;  // copy escaped char directly
         else if (*fp == '%') break;
         *bp = *fp;
         fp++; bp++;
      }
      /* if there's format left, use it */
      if (*fp == '%') {
         if (i > parm_cnt) break;
         /* must check for %q -- it's a BACI special */
         if (*(fp+1) == 'q') {
            if (i <= parm_cnt)  {
               op_addr = saddr_check(curpr, s[parm_offset]);
               parm_offset++;
               sprintf(bp,"\"%s\"",(char *) &s[op_addr]);
            }
            else
               sprintf(bp,"\"\"");
            fp += 2;
         }
         else {
            fspec = cpfmt(tfmt,&fp);
            if (fspec == 's') {
               if (i <= parm_cnt) {
                  op_addr = saddr_check(curpr, 
                     s[saddr_check(curpr,parm_offset)]);
                  sprintf(bp,tfmt,&s[op_addr]);
               }
               else
                  sprintf(bp," (null) ");
            }
            else
               sprintf(bp,tfmt,(i <= parm_cnt ? 
                  s[saddr_check(curpr, parm_offset)] : 0));
            parm_offset++;
         }
         /* move up the buffer pointer to the inserted NULL */
         while (*bp) bp++;
         i++;
      }
   }
   *bp = '\0';
   strcpy(buf,tempbuf);
}  /* xprintf */

#if defined(TCL)
extern Tcl_Interp                      *interp;

extern enum pstat ps;

int xpause;  /* used to pause output */

char imessage[] = "\"Interperter Message\" ";
char showln[] = "showln ";


/********************************************************************
 *                                                                  *
 * These are C functions that provide access for the other C procs  *
 * to the Tcl procs known by the Tcl/Tk interpreter, defined in     *
 * menu.tcl                                                         *
 *                                                                  *
 ********************************************************************/

void colorwindows(int cpr)
   /* run Tcl/Tk colorwindows proc:                                      */
   /* colorwindows  cpr  ptab[cpr].suspend  lastpr  ptab[lastpr].suspend */
   /* to take care of color changes in the process windows               */
{
   char buffer[80];
   if(lastpr!=cpr) {
      sprintf(buffer,"colorwindows %d %d %d %d",
         cpr,ptab[cpr].suspend,lastpr,ptab[lastpr].suspend); 
         /* must turn int into char */
      if (Tcl_Eval(interp,buffer) == TCL_ERROR)
         fprintf(stderr,"colorwindows: %s\n",interp->result);
      lastpr = cpr; 
   }
}  /* colorwindows */

void createwindow(int n)
   /* run Tcl/Tk proc:        makeprocesswindow n */
{
   extern Tcl_Interp *interp;
   char buf[80];
   sprintf(buf,"makeprocesswindow %d",n);
   if (Tcl_Eval(interp,buf) == TCL_ERROR)
      fprintf(stderr,"createwindow: %s\n",interp->result);
}  /* createwindow */

void makedatawin()
   /* run Tcl/Tk proc:        makeDATAwin          */ 
{
   extern Tcl_Interp *interp;
   char buf[80];
   strcpy(buf,"makeDATAwin");
   if (Tcl_Eval(interp,buf) == TCL_ERROR)
      fprintf(stderr,"makedatawin: %s\n",interp->result);
}  /* makedatawin */

void break_alert(char *m, int show_brkwin)
   /* run Tcl/Tk proc:        break_alert  m  show_brkwin */ 
{
   extern Tcl_Interp *interp;
   char buf[256];
   sprintf(buf, "break_alert {%s} %d",m, show_brkwin);
   if (Tcl_Eval(interp,buf) == TCL_ERROR)
      fprintf(stderr,"break_alert: %s\n",interp->result);
}  /* break_alert */

void remove_alert() 
   /* run Tcl/Tk proc:        remove_alert               */ 
{
   extern Tcl_Interp *interp;
   char buf[80];
   strcpy(buf,"remove_alert");
   if (Tcl_Eval(interp,buf) == TCL_ERROR)
      fprintf(stderr,"remove_alert: %s\n",interp->result);

}  /* remove_alert */

void tcl_showsrcline(int srcfno, int ln)
   /* run Tcl/Tk proc:        showsrcln  srcfno  ln      */ 
{
   extern Tcl_Interp *interp;
   char buffer[80];
   sprintf(buffer,"showsrcln %d %d",srcfno,ln); 
   if (Tcl_Eval(interp,buffer) == TCL_ERROR) {
      printf("tcl_showsrcline: %s\n",interp->result);
   }
}  /* tcl_showsrcline */

void tcl_1printf(char* s,int wn)
   /* run Tcl/Tk proc:        onewinprint  s  wn      */ 
{
   char tbuf[256];
   sprintf(tbuf,"onewinprint {%s} %d",s,wn);
   if (Tcl_Eval(interp,tbuf) == TCL_ERROR)  {
      printf("tcl_1printf: %s\n",interp->result);
   }
}  /* tcl_1printf */

void tcl_printf(char* s,int wn)
   /* run Tcl/Tk proc:        windowprint  s  wn      */ 
{
   char tbuf[256];
   sprintf(tbuf,"windowprint {%s} %d",s,wn);
   if (Tcl_Eval(interp,tbuf) == TCL_ERROR) {
      printf("tcl_printf: %s\n",interp->result);
   }
}  /* tcl_printf */

void tcl_gets(char* s,int wn)
   /* run Tcl/Tk proc:        getstr  wn      */ 
{
   char buffer[256];

   sprintf(buffer,"getstr %d",wn); /* must turn int into char */
   if (Tcl_Eval(interp,buffer) == TCL_ERROR) {
      fprintf(stderr,"tcl_gets: %s\n",interp->result);
   }
   strcpy(s,interp->result);
}  /* tcl_gets */

void tcl_putchar(char s,int wn)
   /* run Tcl/Tk proc:        windowprint "s[0]"  wn    */ 
{
   char tbuf[256];
   sprintf(tbuf,"windowprint {%c} %d\n",s,wn);
   if (Tcl_Eval(interp,tbuf) == TCL_ERROR) {
      printf("tcl_putchar: %s\n",interp->result);
   }
}  /* tcl_putchar */

void tcl_putnum(int s,int wn)
   /* run Tcl/Tk proc:        windowprint {s}  wn    */ 
{
   char tbuf[80];
   sprintf(tbuf,"windowprint {%d} %d",s,wn);
   if (Tcl_Eval(interp,tbuf) == TCL_ERROR) {
     printf("tcl_putnum: %s\n",interp->result);
   }
}  /* tcl_putnum */

void tcl_dialog(char* s)
   /* run Tcl/Tk proc:        OKdialog "BACI" "s"  */
{
   char tbuf[256];
   sprintf(tbuf,"OKdialog \"BACI\" \"%s\"",s);
   if (Tcl_Eval(interp,tbuf) == TCL_ERROR) {
     printf("tcl_dialog: %s\n",interp->result);
   }
}  /* tcl_dialog */

void tcl_message(char* s)
   /* run Tcl/Tk proc:        Message "s"  */
{
   char tbuf[256];
   sprintf(tbuf,"Message \"%s\"",s);
   if (Tcl_Eval(interp,tbuf) == TCL_ERROR) {
     printf("tcl_message: %s\n",interp->result);
   }
}  /* tcl_message */

/********************************************************************
 *                                                                  *
 * These are C functions that provide access for the Tcl procs      *
 * to the C code in this file.  Each function below must be         *
 * registered with the Tcl/Tk interepreter in main.c                *
 *                                                                  *
 ********************************************************************/

int runinit(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]) 
   /* Contains the main interpreter loop        */
   /* Mapped to Tcl/Tk command 'run' in main.c  */
{
   char buf[256];
   int runlevel;
   extern int curpr;
   extern int curr_file;
   extern void interpret();
   if(argc!=3) {
      interp->result = "runinit: wrong # of args";
      return TCL_ERROR;
   }
   if(strlen(argv[1])==0) {
      tcl_message("Nothing to run!");
      return TCL_ERROR;
   }
   runlevel = atoi(argv[2]); 
   if (runlevel == 0) {
      global_tcl_init(argv[1]);
      if (last_infile >= 0) {
         strcpy(buf,"loadAllFiles");
         if (Tcl_Eval(interp,buf) == TCL_ERROR) {
            fprintf(stderr,"runinit:  %s\n",interp->result);
         }
         while (Tk_DoOneEvent(TK_DONT_WAIT)) ;
      }
   } 
   else {   /* runlevel > 0 ==> re-running an existing pco file */
      singlestep = 1;
      lastpr = 0;
      init_interpreter();
      tcl_1printf(filename_line,0);
      tcl_1printf("Executing PCODE ...\n",0);
   }
      /*enable buttons */
   strcpy(buf,"enablerunbuttons");
   if (Tcl_Eval(interp,buf) == TCL_ERROR){
      fprintf(stderr,"runinit:  %s\n",interp->result);
   }
   xpause = -1; /* set xpause to pause */
   do {
      switch(xpause) {
         case -1: /* Paused */
            while (Tk_DoOneEvent(TK_DONT_WAIT));
            break;
         case 0:  /* Continue */
            remove_alert(); /* remove the breakpoint alert, if needed */
            interpret();
            while(Tk_DoOneEvent(TK_DONT_WAIT));
            break;
         case 1: /* pco */
            remove_alert(); /* remove the breakpoint alert, if needed */
            xpause = -1;
            interpret();
            while(Tk_DoOneEvent(TK_DONT_WAIT));
         break;
         case 2: /* src */
            remove_alert(); /* remove the breakpoint alert, if needed */
            xpause = 0;     /* keep running until the source line is done */
            debugging = 1;
            start_src_singlestep(curr_file,curpr, ptab[curpr].pc);
            interpret();
            while(Tk_DoOneEvent(TK_DONT_WAIT));
         break;
         case 999: /* stop event loop */
            ps = fin;
         default:
            while(Tk_DoOneEvent(TK_DONT_WAIT));
      }
   } while (ps == run);
   stop_interp(curpr,ps); 
   if (xpause != 999) { /* not shutting down */
      strcpy(buf,"disablerunbuttons");
      if (Tcl_Eval(interp,buf) == TCL_ERROR) {
         fprintf(stderr,"runinit:  %s\n",interp->result);
         exit(1);
      }
      strcpy(buf,"endcolorProcWindows");
      if (Tcl_Eval(interp,buf) == TCL_ERROR) {
         fprintf(stderr,"runinit:  %s\n",interp->result);
         exit(1);
      }
   }
   while (Tk_DoOneEvent(TK_DONT_WAIT));
   /* xpause = 111;   put the event loop in neutral */
   return TCL_OK;
}  /* runinit */

int getInfile(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]) {
   /* For obtaining elements of the array of source filenames     */
   /* Mapped to Tcl/Tk command 'getInfile' in main.c              */
   /*    getInfile index  returns the string infile[index].fname  */
   int ix;
   if(argc!=2) {
      interp->result = "getInfile: wrong # of args";
      return TCL_ERROR;
   }
   ix = atoi(argv[1]);
   if ((ix > last_infile)||(ix < 0)){
      sprintf(interp->result,"getInfile: InputFile index %d is invalid",ix);
      return TCL_ERROR;
   }      
   Tcl_SetResult(interp,inputfile[ix].fname,TCL_STATIC);
   return TCL_OK;
}  /* getInfile */
   

int getBreakpoint(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]) {
   /* For obtaining elements of the breakpoint array                       */
   /* Mapped to Tcl/Tk command 'getBreakpoint' in main.c                   */
   /*    getBreakpoint index  returns the fields of the                    */
   /*       breakpoint[index] struct, formatted into a string for output   */
   int ix;
   if(argc!=2) {
      interp->result = "getBreakpoint: wrong # of args";
      return TCL_ERROR;
   }
   ix = atoi(argv[1]);
   if ((ix > break_cnt)||(ix < 0)){
      sprintf(interp->result,"getBreakpoint: breakpoint index %d is invalid",ix);
      return TCL_ERROR;
   }      
   list_breakpoint(ix,interp->result);
   return TCL_OK;
}  /* getBreakpoint */
   
int setBreakpoint(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]) {
   /* For setting a breakpoint at an address                               */
   /* Mapped to Tcl/Tk command 'setBreakpoint' in main.c                   */
   /*    setBreakpoint addr   sets a breakpoint at the address $addr       */
   /* Returns the Tcl list "srcfileindex srcfilelinenumber"                */
   int addr;
   int fno;
   int lno;
   if(argc!=2) {
      interp->result = "setBreakpoint: wrong # of args";
      return TCL_ERROR;
   }
   addr = atoi(argv[1]);
   fno = set_break(addr,&lno);
   if (fno < 0) {
      sprintf(interp->result,"setBreakpoint:  error %d setting breakpoint",fno);
      return TCL_ERROR;
   }
   else {
      sprintf(interp->result,"%d %d",fno,lno);
      return TCL_OK;
   }
}  /* setBreakpoint */
   
int unsetBreakpoint(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]) {
   /* For unsetting a breakpoint by user index                             */
   /* Mapped to Tcl/Tk command 'unsetBreakpoint' in main.c                 */
   /*    unsetBreakpoint userindex                                         */
   /* Unsets the breakpoint having user index $userindex                   */
   /* If successful, returns "fileindex lineno" of breakpoint unset        */
   /* Throws an error if there was a problem                               */
   int ix,bix;
   int fno,lno;
   if(argc!=2) {
      interp->result = "unsetBreakpoint: wrong # of args";
      return TCL_ERROR;
   }
   ix = atoi(argv[1]);
   bix = get_breakindex(ix);
   if (bix < 0) {
      sprintf(interp->result,"unsetBreakpoint: invalid break index %d",ix);
      return TCL_ERROR;
   }
   fno = unset_break(bix,&lno);
   if (fno < 0) {
      interp->result = "unsetBreakpoint: error unsetting breakpoint";
      return TCL_ERROR;
   }
   else {
      sprintf(interp->result,"%d %d",fno,lno);
      return TCL_OK;
   }
}  /* unsetBreakpoint */
   
int LinenoToPC(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]) {
   /* For converting a line number to a pcode address                      */
   /* Mapped to Tcl/Tk command 'LinenoToPC' in main.c                      */
   /*    LinenoToPC [list fno lno]                                         */
   /* Returns the pcode address of the beginning of the line               */
   /* Throws an error if there was a problem                               */
   int lno;
   int curr_file;
   int pc;
   if(argc!=3) {
      interp->result = "LinenoToPC: wrong # of args";
      return TCL_ERROR;
   }
   curr_file = atoi(argv[1]);
   lno = atoi(argv[2]);
   pc = lno_to_pc(curr_file,lno);
   sprintf(interp->result,"%d",pc);
   return TCL_OK;
}  /* LinenoToPC */
   
int LineHasBreak(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]) {
   /* Boolean function to see if any breakpoints remain for a given line   */
   /* of a given file                                                      */
   /* Mapped to Tcl/Tk command 'LineHasBreak' in main.c                    */
   /*    LineHasBreak [list fno lno]                                       */
   /* Returns number of breakpoints set on line number $lno of file        */
   /* with index $fno                                                      */
   /* otherwise returns 0                                                  */
   int lno;
   int fno;
   int ret;
   int ix,jx;
   if(argc!=3) {
      interp->result = "LineHasBreak: wrong # of args";
      return TCL_ERROR;
   }

   fno = atoi(argv[1]);
   lno = atoi(argv[2]);
   ret = 0;
   for (ix = 0; ix < break_cnt; ix++) {
      jx = breakpoint[ix].dbgix;
      if ((fno == dbg[jx].fix)&&(lno == dbg[jx].flno)) {
         ret++;
      }
   }
   sprintf(interp->result,"%d",ret);
   return TCL_OK;
}  /* LineHasBreak */

int WriteData(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]) {
   /* For writing different data to the DATA window                       */
   /* Mapped to Tcl/Tk command 'WriteData' in main.c                      */
   /*       WriteData  <option>                                           */
   /* where <option> is one of                                            */
   /*    process_table     write the process table                        */
   /*    global_vars       write all global variables                     */
   /*    monitor_vars      write all monitor variables                    */
   /*    process_vars      write all process variables                    */
   /*    stacks            write all process stacks                       */
   /* The DATA window is expected to exist (ie, the makeDATAwin call has  */
   /* already occurred */
   extern int curpr;
   if (argc != 2) {
                     /* 12345678901234567891123*/
      interp->result = "usage: WriteData <item>\
where <item> == process_table  global_vars  monitor_vars  process_vars  stacks";
/*6789312345678941234567895123456789612345678971234567890123456789012345678901*/
      return TCL_ERROR;
   }
   if (strcmp(argv[1],"process_table") == 0) {
      write_process_table(curpr);
      interp->result = "";
      return TCL_OK;
   }
   if (strcmp(argv[1],"global_vars") == 0) {
      write_global_variables(curpr);
      interp->result = "";
      return TCL_OK;
   }
   if (strcmp(argv[1],"monitor_vars") == 0) {
      write_monitor_variables(curpr);
      interp->result = "";
      return TCL_OK;
   }
   if (strcmp(argv[1],"process_vars") == 0) {
      write_process_variables(curpr);
      interp->result = "";
      return TCL_OK;
   }
   if (strcmp(argv[1],"currproc_vars") == 0) {
      write_currproc_variables(curpr);
      interp->result = "";
      return TCL_OK;
   }
   if (strcmp(argv[1],"stacks") == 0) {
      write_all_stacks(curpr);
      interp->result = "";
      return TCL_OK;
   }
   sprintf(interp->result,"WriteData:  invalid option \"%s\"",argv[1]);
   return TCL_ERROR;
}  /* WriteData */

#endif


/*
 *  $Id: baiutils.c,v 2.27 2007/11/25 20:00:47 bynum Exp $
 */
