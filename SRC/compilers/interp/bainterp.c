/* BenAri Concurrent Pascal PCODE Intepreter -- INTERPRETER BODY */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "../include/bacicnty.h"        /* for PMAX, PMAXP1 */ 
#include "../include/globdata.h"
#include "../include/globtabs.h"
#include "../include/pcode.h"
#include "../include/genutil.h"
#include "../include/disutils.h"

#include "random.h"
#include "bainterp.h"

#if defined(SunOS)
extern   int printf(const char*,...);
extern   int fprintf(FILE*,const char*,...);
extern   int fputc(int,FILE*);
extern   int fputs(const char*,FILE*);
extern   int fseek(FILE*,long,int);
extern   int sscanf(const char*,const char*,...);
#endif

   /* interpreter global variables */
extern char interp_pname[];
extern char interp_pnroot[];
extern char date_string[];

extern char xpcode_suffix[];

extern FNAME_STRING xpcode_fname;
extern FILE  *xpcode;

extern InputFile* inputfile;

   /* debug flags */
extern int   end_db, initAR_db, finalAR_db, pcexec_db, debugging;


int   charl, charh;        /* lowest char ord = ord' ', highest= ord '~'*/
ORDER ir;                  /* holds current instruction */
enum pstat ps;             /* current interpreter state */
int   s[STMAX];            /* the stack */
proctab  ptab[PMAXP1];     /* the process table */
int npr;                   /* number of concurrent processes */
int curpr;                 /* current process executing */
int stepcount;             /* number of steps before switch */
int pflag;                 /* 1 ==> in COBEGIN, 0 at COEND */

int shortcall_reg;         /* to hold return address from a shortcall */

int singlestep;            /* 1 if debugger is singlestepping, else 0 */ 
int continuing;            /* 1 if debugger is in a continue, else 0 */ 

int in_shortcall;          /* 1 if shortcall is underway, else 0 */

int curr_file;             /* inputfile[] ix of currently executing src file */

   /* We have to manipulate the choice of a proc to run inside a cobegin 
      block so each thread is started by a MARKSTACK/CALL/UPDATE_DISPLAY
      sequence, executed by the thread (!), but any other instructions
      in the block are executed by the main proc (like the instructions
      of a for loop).  The main proc suspends at the coend stmt, to allow 
      the other threads to run. 

      The starting_thread boolean is flipped on in the when the markstack
      instruction is encountered when pflag is on and is turned off by
      the update_display instruction.  As long as both pflag and 
      starting_thread are 1, the most current process, curpr, will 
      turn out to be npr, the most recently activated thread */

int starting_thread;    /* set to 0 in initialize_interp() */

#define BUFFERSIZE 256
char  readbuf[BUFFERSIZE];
char  tempbuf[BUFFERSIZE];
char  disasmbuf[BUFFERSIZE];

#define STRBUFSIZE 2048
char strbuf[STRBUFSIZE];

#if defined(TCL)
extern char tcl_buffer[];
extern int xpause;
extern int pause;
extern int lastline;
void tcl_printf(char* s,int wn); /* send message s to window wn and window 0 */
void tcl_1printf(char* s,int wn); /* send message s only to window wn */
void tcl_putchar(char s,int wn);  /* write s to window wn */
void tcl_putnum(int s,int wn);  /* write s to window wn */
void tcl_showsrcline(int srcfno,int ln);  /* highlight ln in srcwin srcfno */
void tcl_dialog(char* s);        /* Tk dialog window with message s and 'OK' */
void tcl_gets(char* s,int wn);   /* Gets a string from tcl   */
void createwindow(int n);        /* creates a window for process n */
void break_alert(char*, int );   /* in its own window announces a breakpt */
extern int   ShowPCODE;
#endif

#define  PCOWIN   411      /* window number of pcode window */
#define  DATAWIN  505      /* window number of data window */

extern int chooseproc(int, enum pstat*,int*);
   /* choose a runnable process, if possible */

extern void show_AR(int b,int t,int pr);
   /* show the AR s[t] down to s[b] for the process that owns it */

extern void show_display(int pr);
   /* show the display of the process number 'pr' */

void stop_interp(int curpr, enum pstat ps);
   /* stop the interpreter and dump debug information */

extern int saddr_check(int curpr, int addr);
   /* check a stack address */

extern int paddr_check(int curpr, int addr);
   /* check a pcode address */

extern void local_strcpy(char* d, char* s);
   /* When the stack words were initialized to 0x00003f3f, the C-Lib */
   /* strcpy issued a SEGV when it hit the 3f's.  The bare-bones     */
   /* local version is too dumb to care.                             */

extern int at_breakpoint(int pc);
   /* if 'pc' is at a breakpoint, then return its index in breakpoint[], */
   /* else return -1                                                     */

extern int current_codeblock(int pc);
   /* return the cbtab index of the codeblock corresponding to 'pc' */

extern int pc_to_dbgix(int pc);
   /* return the dbg index corresponding to 'pc' */

extern void debugger(int curpr, int* curr_file, int* singstep, int* continuing);
   /* enter the interactive debugger */

extern int xsscanf(int curpr, int scan_strix, int six, int parm_cnt);
   /* s[scan_strix] has the address of the string to be scanned */
   /* stab[six] is the beginning of the format string */
   /* parm_cnt is the number of parms (not counting addr of scan string) */

extern void xsprintf(int curpr, int scan_strix, int six, int parm_cnt);
   /* s[scan_strix] has the address of the string to be built */
   /* stab[six] is the beginning of the format string */
   /* parm_cnt is the number of parms (not counting addr of scan string) */

extern void init_proc_displays();
   /* initialize the displays for the concurrent threads to point to    */
   /* the stack frames of the outer, global scope and the scope of the  */
   /* main block                                                        */

void interpret()
{
      /* local variables */
   int   h1,h2,h3,h4,h5,h6,minprior;  
   int   signalproc, entryproc;  /* used in EXITMON */
   int   tix,cbix,brix;    /* indices into tables */
   int   dbix;
   int   pc;               /* current pc */
   char  last_char;        /* last char read */
   char *p;                /* string pointers */
   char *q;

#if !defined(TCL)
   /* TCL main proc calls interpret to single-step */
   do {  /* bottom of loop is a long ways down */
#endif
      curpr = chooseproc(curpr,&ps,&stepcount);
#if defined(TCL)
      if (ps != run) return;
#else
      if (ps != run) break;
#endif
      pc = ptab[curpr].pc;
      ir = code[pc];             /* get next instruction       */
      ptab[curpr].pc++;          /* update pc to look ahead    */
      /* if a context sw is due next time, and if the next instr is an */
      /* exit proc or exit fcn, then delay switch for one more instr   */
      if (stepcount == 0)
         if ((ir.f == EXIT_PROC) || (ir.f == EXIT_FCN)) stepcount = 1; 
      dbix = pc_to_dbgix(pc);
      if (curr_file != dbg[dbix].fix) curr_file = dbg[dbix].fix;
#if defined(TCL)
      if (ShowPCODE || debugging || pcexec_db) {
#else
      if (debugging || pcexec_db) {
#endif
         cbix = current_codeblock(pc);
         sprintf(disasmbuf,"p%02d ",curpr);
         disassemble(&disasmbuf[4],cbtab[cbix].tix,pc);
#if defined(TCL)
         if (ShowPCODE) {
            tcl_1printf(disasmbuf,PCOWIN);
         }
#endif
         if (debugging) {
            if (ptab[curpr].first_spc >= 0) { /* executing one source line */
               /* if we're out of range of the current source line, stop */
               /* we have to check a range of pc values because of jumps */
               if ((!in_shortcall) &&((pc < ptab[curpr].first_spc) || 
                   (pc > ptab[curpr].last_spc)))  {
                  sprintf(tempbuf,
                     "Process %d at line %d of %s (pc = %d)\n",
                     curpr, dbg[dbix].flno, inputfile[dbg[dbix].fix].fname,pc);
#if defined(TCL)
                  break_alert(tempbuf,0);
                  xpause = -1;
#else
                  fputs(tempbuf,stdout);
                  singlestep = 1;
#endif
                  ptab[curpr].first_spc = ptab[curpr].last_spc = -1;
               }  /* if pc out of range of current source line */
            }  /* if first_spc >= 0 */
            else { /* not source line singlestepping */
               brix = at_breakpoint(pc);
               if (brix >= 0) {
                  tix = cbtab[brix].tix;
                  sprintf(tempbuf,
"At breakpoint %d   Process #%d: %s,\n   loc %d, line %d of %s\n",
                     brix,curpr,tab[ptab[curpr].tabix].name,
                     pc,dbg[dbix].flno, inputfile[dbg[dbix].fix].fname);
#if defined(TCL)
                  break_alert(tempbuf,1);
                  xpause = -1;
               }  /* if brix >= 0 */
            }  /* else not source line singlestepping */
         }  /* if debugging */
#else
                  fputs(tempbuf,stdout);
                  singlestep = 1;
               }  /* if brix >= 0 */ 
            }  /* else no source line singlestepping */
            if (singlestep) fputs(disasmbuf,stdout);
            debugger(curpr,&curr_file,&singlestep,&continuing);
            if (continuing) singlestep = 0;
         }  /* if debugging */
#endif
         if (pcexec_db) 
            fputs(disasmbuf,xpcode);
      }  /* if (debugging || pcexec_db) */
#if defined(TCL)
      if (xpause == -1) {
         tcl_showsrcline(dbg[dbix].fix,dbg[dbix].flno);
      }
#endif
      if (pflag){
         /* COBEGIN flips pflag on, COEND flips it off and suspends the    */
         /* main proc.  In between, go to a new thread each time a         */
         /* MARKSTACK is seen.  This causes the pattern                    */
         /*          MARKSTACK/pushparms/CALL                              */
         /* to occur until all threads in the COBEGIN have been activated. */
         if (ir.f == MARKSTACK) {
            npr++;
            if (npr > PMAX) {
               stop_interp(curpr,proctabchk);
               exit(1);
            }
            starting_thread = 1;
         }
         if (starting_thread) curpr = npr;
      }
         
      switch (ir.f) { 
      case LOAD_ADDR: /* 0, load address */
         ptab[curpr].t = saddr_check(curpr,ptab[curpr].t + 1);;
         if (ptab[curpr].t > ptab[curpr].stacksize)
            ps = stkchk;
         else
            s[ptab[curpr].t] = saddr_check(curpr, 
               ptab[curpr].display[ir.x] + ir.y);
         break;
      case LOAD_VALUE: /* 1, load value */
         ptab[curpr].t = saddr_check(curpr,ptab[curpr].t + 1);;
         if (ptab[curpr].t > ptab[curpr].stacksize)
            ps = stkchk;
         else
            s[ptab[curpr].t] = s[saddr_check(curpr,
               ptab[curpr].display[ir.x]+ir.y)];
         break;
      case LOAD_INDIR: /* 2, load indirect */
         ptab[curpr].t = saddr_check(curpr,ptab[curpr].t + 1);;
         if (ptab[curpr].t > ptab[curpr].stacksize)
            ps = stkchk;
         else {
            h1 = saddr_check(curpr,ptab[curpr].display[ir.x] + ir.y);
            s[ptab[curpr].t] = s[ saddr_check(curpr,s[h1]) ];
         }
         break;
      case UPDATE_DISPLAY: /* 3, update display */
         /* assert(ir.x < ir.y);                                  */
         /* display[ir.y] is changed to the base of the current   */
         /* AR.  For each k such that ir.x < k < ir.y, display[k] */
         /* is changed to static link of display[k+1].            */
         /* display[ir.x] is unchanged.                           */
         h1 = ir.y;
         h2 = ir.x;
         h3 = ptab[curpr].b;
         do {
            ptab[curpr].display[h1] = h3;
            h1--;
            h3 = s[saddr_check(curpr,h3+2)]; /* static link of display[h3] */
         } while (h1 != h2);
         if (starting_thread) starting_thread = 0;
         break;
      case COBEGIN: /* 4, cobegin */  
         pflag = 1; 
         init_proc_displays();
         break;
      case COEND: /* 5, coend */
         pflag = 0;
         ptab[0].active = 0;
         ptab[0].suspend = GLOBAL_SUSPEND;
         stepcount = 0;  /* force a context switch */
         break; 
      case P_WAIT: /* 6, P or WAIT */
         h1 = saddr_check(curpr,s[ptab[curpr].t]);
         if (s[h1] == UNDEF_VALUE)
            ps = uisemchk;
         else {
            ptab[curpr].t--;
            if (s[h1] > 0)  /* if semaphore is pos, decrement it */
               s[h1]--;
            else { /* otherwise suspend this process on semaphore h1 */
               ptab[curpr].suspend = h1;
               ptab[curpr].active = 0;
               stepcount = 0;
            } /* else sem was 0 */
         } /* else sem was defined */ 
         break;      
      case V_SIGNAL: /* 7, V  or SIGNAL */
         h1 = saddr_check(curpr,s[ptab[curpr].t]);
         if (s[h1] == UNDEF_VALUE)
            ps = uisemchk;
         else {
            ptab[curpr].t--;
            h2 = PMAXP1;  /* code like chooseproc */
            h3 = __random(h2);
            h4 = 1 + __random(PMAX);
            while (( h2 >= 0 ) && (ptab[h3].suspend != h1 )){
               h3 = (h3+h4) % (PMAXP1);
               h2--;
            }
            if (h2 < 0){ /* if nobody's waiting .. */
               s[h1]++; /* increment semaphore */
               if ((ir.y) && ( s[h1] > 1))
                  ps = bsemchk;
            }
            else { /* wake someone waiting on semaphore */
               ptab[h3].suspend = NOT_SUSPENDED;
               ptab[h3].active = 1;
            }
         }  /* else sem was defined */
         break; 
      case ENTERMON: /* 8, entermon  P&W */
         h6 = s[saddr_check(curpr,ptab[curpr].b+4)]; /* tab entry this proc */ 
         h5 = s[saddr_check(curpr,s[ptab[curpr].b+3]+4)]; 
            /* caller's (= DL) tab entry */
         if (tab[h5].mon != 0){ 
            /* if this call is coming from monitor init code, then */
            /* make sure it's the right monitor                    */
            if (tab[h5].obj == monitor) {
               if (tab[h6].mon != h5) ps = xmonchk;
            }
            else if (tab[h6].mon != tab[h5].mon) ps = xmonchk;
            /* cross monitor call, tsk, tsk */
            /* same monitor call is OK, but don't change mon semaphore  */
         }
         else {  /* caller is NOT in a monitor */
            h4 = tab[ir.y].mon; /* get stack addr of monitor from instr */
            ptab[curpr].monitor = h4;
            if (s[h4] == UNDEF_VALUE)  
               ps = uisemchk;
            else if (s[h4] == 0) {
               stepcount = 0;
               ptab[curpr].active = 0;    /* inactive */
               ptab[curpr].suspend = h4;  /* suspended on monitor semaphore */
            }  /* if s[h4] == 0 */
            else /* decrement monitor semaphore */
               s[h4]--;
         }  /* caller not in a monitor */
         break;  /* 8 entermon */
      case EXITMON: /* 9, exitmon  P&W */
         h6 = s[saddr_check(curpr,ptab[curpr].b+4)]; /* tab entry this proc */
         h5 = s[saddr_check(curpr,s[ptab[curpr].b+3]+4)]; 
            /* caller's (= DL) tab entry */
         if (tab[h5].mon != 0) {
            /* if this call is coming from monitor init code, then */
            /* make sure it's the right monitor                    */
            if (tab[h5].obj == monitor) {
               if (tab[h6].mon != h5) ps = xmonchk;
            }
            else if (tab[h6].mon != tab[h5].mon) ps = xmonchk;
            /* cross monitor call, tsk, tsk */
            /* same monitor call is OK, but don't change mon semaphore */
         }
         else {  /* caller was NOT in a monitor  */
            h4 = saddr_check(curpr,ptab[curpr].monitor); /* monitor to exit */
            if (s[h4] == UNDEF_VALUE) 
               ps = uisemchk;
            else { /* semaphore was initialized */
               ptab[curpr].monitor = 0;
               h2 = PMAXP1;
               h3 = __random(h2);
               h5 = 1 + __random(PMAX);
               /* look for 
                  1. a monitor process that has signalled a condition
                  2. someone waiting to enter the monitor
                  in that order */
               signalproc = -1;
               entryproc = -1;
               while (h2 >= 0) {
                  if (ptab[h3].monitor == h4) {
                     if ((signalproc < 0)&& 
                        (ptab[h3].suspend == SIGNAL_SUSPEND))
                        signalproc = h3;
                     if ((entryproc < 0)&&(ptab[h3].suspend == h4))
                        entryproc = h3;
                  }
                  if ((signalproc >= 0) && (entryproc >= 0))
                     break;      /* no need to look further */
                  h3 = (h3 + h5) % (PMAXP1);
                  h2--;
               }  /* while */
               if ((signalproc < 0) && (entryproc < 0))
                 /* nobody's waiting, so increment mon sem */
                  s[h4]++;
               else {   /* otherwise wake up one of the ones we found */
                  if (signalproc >= 0)
                     h3 = signalproc;
                  else if (entryproc >= 0)
                     h3 = entryproc;
                  ptab[h3].active = 1;
                  ptab[h3].suspend = NOT_SUSPENDED;
               }
            }  /* else semaphore was initialized */
         }  /*else caller was NOT a monitor proc */
         break;       
      case WAITC: /* 10, waitc  P&W */
         h4 = saddr_check(curpr,ptab[curpr].monitor);
         if (s[h4] == UNDEF_VALUE)  
            ps = uisemchk;
         else { /* sem was defined */
            ptab[curpr].suspend  = s[ptab[curpr].t-1];
            ptab[curpr].priority = s[ptab[curpr].t];
            ptab[curpr].t -= 2;
            ptab[curpr].active = 0;
            stepcount = 0;
            h2 = PMAXP1;
            h3 = __random(h2);
            h5 = 1 + __random(PMAX);
            /* look for someone waiting to enter the monitor */
            while ((h2 >= 0) && 
               ((ptab[h3].monitor != h4) || (ptab[h3].suspend != h4 ))){
               h3 = (h3 + h5) % (PMAXP1);
               h2--;
            }  /* while */
            if (h2 < 0) /* if no one's waiting to enter mon, incr mon sem */
                s[h4]++;
            else {   /* otherwise wake up the proc wanting to enter mon */
               ptab[h3].active = 1;
               ptab[h3].suspend = NOT_SUSPENDED;
            }
         }  /* else sem was inited */
         break;  /* case 10  -- waitc  P&W */
      case SIGNALC: /* 11, signalc  P&W */
         h4 = saddr_check(curpr,ptab[curpr].monitor);
         if (s[h4] == UNDEF_VALUE) /* should never happen! */
             ps = uisemchk;
         else { /* monitor sem was OK */
            h1 = s[ptab[curpr].t];
            ptab[curpr].t--;
            h3 = __random(PMAXP1);
            h6 = 1 + __random(PMAX);
            h5 = 0;
            minprior = MAXPRIO;
            for (h2 = 0; h2 <= PMAX; h2++) {
               if ((ptab[h3].suspend == h1) && (ptab[h3].monitor == h4))
                  if (ptab[h3].priority < minprior) {
                     minprior = ptab[h3].priority;
                     h5 = h3;
                  }  /* if ptab[h3].priority < minprior */
               h3 = (h3 + h6) % (PMAXP1);
            }  /* for h2 = 0 to pmax */
            if (h5 != 0) {
               /* immediate resumption req'mt ==> suspend the signaller */
               ptab[curpr].active = 0;
               ptab[curpr].suspend = SIGNAL_SUSPEND;
               stepcount = 0;
               ptab[h5].active  = 1;
               ptab[h5].suspend = NOT_SUSPENDED;
            }  /* if h5 != 0 */
         }  /* else sem was ok */
         break;  /* case 11  -- signalc P&W */
      case EMPTY: /* 12, empty  P&W */
         h1 = s[ptab[curpr].t];
         h3 = 0;
         for (h2 = 1; h2 <= PMAX; h2++)
            if (ptab[h2].suspend == h1)
               h3 = 1;
         s[ptab[curpr].t] = (h3 == 0 );
         break;  /* case 12 -- empty P&W */
      case EOLN_OR_EOF: /* 13, EOLN or EOF, emitted in "factor", was 8 */  
         switch(ir.y) { 
            case SF_EOF: /* 17, EOF */
               ptab[curpr].t = saddr_check(curpr,ptab[curpr].t + 1);;
               if (ptab[curpr].t > ptab[curpr].stacksize)
                  ps = stkchk;
               else
                  s[ptab[curpr].t] = feof(stdin);
               break;
            case SF_EOLN: /* 18, EOLN */
               ptab[curpr].t = saddr_check(curpr,ptab[curpr].t + 1);;
               if (ptab[curpr].t > ptab[curpr].stacksize)
                  ps = stkchk;
               else  /* EOLN */
                     /* last ch read was \n */
                  s[ptab[curpr].t] = (last_char == '\n'); 
               break; /* case 18  eof or eoln  */
            default:
               ps = pcodchk;
               break;
            } /* switch */
            break;
      case JUMP:   /* 14, jump, used in "ifstmt" was case 10  P&W */
         ptab[curpr].pc = paddr_check(curpr,ir.y);
         break; 
      case JZER: /* 15, conditional jump, used in "ifstmt"  was case 11  P&W */
         if (s[ptab[curpr].t] == 0) ptab[curpr].pc = paddr_check(curpr,ir.y);
         ptab[curpr].t--;
         break;
      case BEGINFOR: /* 16, for1up  was case 14  P&W */
         h1 = s[saddr_check(curpr,ptab[curpr].t-1)];
         if (h1 <= s[ptab[curpr].t]) 
             s[saddr_check(curpr,s[saddr_check(curpr,ptab[curpr].t-2)])] = h1;
         else {
            ptab[curpr].t -= 3;
            ptab[curpr].pc = paddr_check(curpr,ir.y);
         }
         break;
      case ENDFOR: /* 17, for2up  was case 15  P&W */
         h2 = s[saddr_check(curpr,ptab[curpr].t-2)];
         h1 = s[h2] + 1;
         if (h1 <= s[ptab[curpr].t]){
            s[h2] = h1;
            ptab[curpr].pc = paddr_check(curpr,ir.y);
         }
         else
            ptab[curpr].t = saddr_check(curpr,ptab[curpr].t - 3);
         break; 
      case MARKSTACK: /* 18, markstack */
         h1 = btab[ tab[ir.y].ref ].vsize;
         if ((ptab[curpr].t+h1) > ptab[curpr].stacksize) 
             ps = stkchk;
         else {
            /* make the space for the standard 5-word base of the AR */
            ptab[curpr].t += 5;
            /* store vsize - 1 at AR[3]            */
            s[ptab[curpr].t-1] = h1 - 1;
            /* store callee's tab index at AR[4]   */
            s[ptab[curpr].t] = ir.y;
         }
         break; 
      case CALL: /* 19, call */
         ptab[curpr].active = 1;
         h1 = ptab[curpr].t - ir.y;  /* h1 is  callee's AR[0] */
         h2 = s[saddr_check(curpr,h1+4)]; /* AR[4] = h2 is callee's tab index*/
         h3 = tab[h2].lev;           /* h3 is level of callee's name */ 
         /* callee's code runs 1 level higher than its name */
         ptab[curpr].display[h3+1] = h1; 
         /* callee's vsize-1 is left at AR[3] by MARKSTACK */
         /* callee's t is h4 = b + vsize - 1               */
         h4 = saddr_check(curpr,s[h1+3] + h1);
         /* save return addr at AR[1] */
         s[h1+1] = ptab[curpr].pc;
         if (ir.x == -1) {  /* a monitor proc call */
            h5 = tab[tab[h2].mon].mon;    /* get ptr to monitor's AR */
            ptab[curpr].display[h3] = h5; /* make it our outer scope */
            s[h1 + 2]   = h5;             /* AR[2] = static link = h5 */
         }         
         else {  /* non-monitor proc call */
            s[h1+2] = ptab[curpr].display[h3]; /* AR[2] = static link */
         }
         if (pflag) {
            /* if we are starting threads now, then make the dynamic   */
            /* link the main process.  AR[3] = AR[main] = dynamic link */
            s[h1+3] = ptab[0].b;
            /* save the tab index for this process, in case it's needed */
            ptab[curpr].tabix = h2;
#if defined(TCL)
            if (curpr != 0) createwindow(curpr);
#endif
         }
         else  /* non-concurrent proc call */
            /* AR[3] = dynamic link = caller's AR[0] */
            s[h1+3] = ptab[curpr].b;
         /*  initialize callee's local variables  */
         for (h3 = ptab[curpr].t+1; h3 <= h4; h3++)
            s[h3] = 0;        
         /*  initialize callee's frame pointers */
         ptab[curpr].b = h1;
         ptab[curpr].t = h4;
         /* get callee's entry point from the tab array */
         ptab[curpr].pc = paddr_check(curpr,tab[h2].adr);
         /* atomic procs can be nested, so if this thread */
         /* is atomic, then make it more atomic           */
         if (tab[h2].atomic) ptab[curpr].atomic++;
         if (initAR_db) {
#if defined(TCL)
            sprintf(tcl_buffer,"ENTRY to %s\n",tab[h2].name);
            tcl_1printf(tcl_buffer,DATAWIN);
#else
            printf("ENTRY to %s\n",tab[h2].name);
#endif
            show_display( curpr ); 
            show_AR(ptab[curpr].b,ptab[curpr].t,curpr); 
         }
         break;
      case INDEX: /* 21, index */
         h1 = ir.y;  /* h1 points to atab */
         h2 = atab[h1].low;
         h3 = s[ptab[curpr].t];
         if (h3 < h2) 
            ps = inxchk;
         else if (h3 > atab[h1].high)
            ps = inxchk;
         else {
            ptab[curpr].t--;
            s[ptab[curpr].t] = s[ptab[curpr].t] + ((h3-h2) * atab[h1].elsize);
         }
         break; 
      case LOAD_BLOCK: /* 22, load block */
         h1 = s[ptab[curpr].t];
         ptab[curpr].t--;
         h2 = ir.y + ptab[curpr].t;
         if (h2 > ptab[curpr].stacksize)
            ps = stkchk;
         else
            while (ptab[curpr].t < h2) {
               ptab[curpr].t = saddr_check(curpr,ptab[curpr].t + 1);;
               s[ptab[curpr].t] = s[h1];
               h1++;
            }  /* while */
         break;
      case COPY_BLOCK: /* 23, copy block */
         h1 = s[saddr_check(curpr,ptab[curpr].t-1)];
         h2 = s[ptab[curpr].t];
         h3 = h1 + ir.y;
         while (h1 < h3)
            s[saddr_check(curpr,h1++)] = s[saddr_check(curpr,h2++)];
         ptab[curpr].t -= 2;
         break; 
      case PUSH_LIT: /* 24, literal */
         ptab[curpr].t = saddr_check(curpr,ptab[curpr].t + 1);;
         if (ptab[curpr].t > ptab[curpr].stacksize)
            ps = stkchk;
         else
            s[ptab[curpr].t] = ir.y;
         break;  /* 24 */
      case READ: /* 27, read */
         if (feof(stdin))
            ps = redchk;
         else
            switch (ir.y){
               case ints: 
#if defined(TCL)
                  tcl_gets(readbuf,curpr);
#else
                  fgets(readbuf,BUFFERSIZE,stdin);
#endif
                  h1 = sscanf(readbuf,"%d",&h2);
                  if ((h1 == 0)||(h1 == EOF))
                     ps = inpchk;
                  else
                     s[saddr_check(curpr,s[ptab[curpr].t])] = h2;
                  break;
               case chars: 
#if defined(TCL)
                  tcl_gets(readbuf,curpr);
#else
                  fgets(readbuf,BUFFERSIZE,stdin);
#endif
                  s[saddr_check(curpr,s[ptab[curpr].t])] = last_char= readbuf[0]; 
                  break;
               default: ps = rdchk;
            }  /* switch */
            ptab[curpr].t--;
         break; /* 27 */
      case WRITE_RAWSTRING: /* 28,write raw string */
         h1 = ir.y;
         while (stab[h1]) {
#if defined(TCL)
            tcl_putchar(stab[h1],curpr);
#else
            fputc(stab[h1],stdout);
#endif
            h1++;
         }
         break;  /* 28 */
      case WRITE: /* 29, write1 */
         switch (ir.y){
            case ints:
            case sems: 
            case bsems:
#if defined(TCL)
               tcl_putnum(s[ptab[curpr].t],curpr);
#else
               printf("%d",s[ptab[curpr].t]); 
#endif
               break;
            case bools: 
               if (s[ptab[curpr].t] == 0)
#if defined(TCL)
                  tcl_printf("FALSE",curpr);
               else
                  tcl_printf("TRUE",curpr);
#else
                  printf("FALSE"); 
               else
                  printf("TRUE");
#endif
               break;
            case chars:
               if ((s[ptab[curpr].t] != '\n') &&
                   ((s[ptab[curpr].t] < charl) || (s[ptab[curpr].t] > charh)))
                  ps = charchk;
               else
#if defined(TCL)
                  tcl_putchar((char)s[ptab[curpr].t],curpr);
#else
                  fputc(s[ptab[curpr].t],stdout);
#endif
               break;
            default: ps = wrtchk;
         } /* switch */
         ptab[curpr].t--;
         break;  /* 29 */
     case HALT: /* 31, halt */
         ps = fin; 
         break; 
     case EXIT_PROC: /* 32, exit procedure */
         /* tab index of proc is at AR[4] */
         h1 = s[saddr_check(curpr,ptab[curpr].b+4)]; 
         if (end_db){   
            /*  announce proc termination */
#if defined(TCL)
            sprintf(tcl_buffer,"process %d %s,  procedure %s ended\n" ,
               curpr,tab[ptab[curpr].tabix].name,tab[h1].name);
            tcl_1printf(tcl_buffer,DATAWIN);
#else
            printf("process %d %s,  procedure %s ended\n" ,
               curpr,tab[ptab[curpr].tabix].name,tab[h1].name);
#endif
         }
         /* if proc is atomic, then make the calling thread less atomic */
         if (tab[h1].atomic) ptab[curpr].atomic--;
         if (finalAR_db){
#if defined(TCL)
            sprintf(tcl_buffer,"EXIT from %s\n",tab[h1].name);
            tcl_1printf(tcl_buffer,DATAWIN);
#else
            printf("EXIT from %s\n",tab[h1].name);
#endif
            show_display(curpr); 
            show_AR(ptab[curpr].b,ptab[curpr].t,curpr); 
         }
         /* pop this proc's AR by going to the next AR on the stack */
         ptab[curpr].t = saddr_check(curpr,ptab[curpr].b - 1);
         /* retrieve the return address from this proc's AR[1] */
         ptab[curpr].pc = paddr_check(curpr,s[ptab[curpr].b + 1]);
         /* the return addr at AR[1] of a concurrent thread is set to */
         /* zero at interpreter startup.  It's highly unlikely that   */
         /* ANY proc can have a return address of 0, since it would   */
         /* have to be called by a MARKSTACK/CALL pair.               */
         if (ptab[curpr].pc != 0){
            /* retrieve dynamic link from AR[3] */
            h2 = s[ saddr_check(curpr,ptab[curpr].b + 3)];
            /* h2 now points to AR[caller] */
            assert(s[h2+1] != -1);  /* does this ever happen?? */
            if (s[h2+1] == -1) /* return from mon proc call  */
               h2 = saddr_check(curpr,s[h2+3]);   /* go down one dynamic link */
            ptab[curpr].b = h2;
         }
         else {
            /* a thread is terminating */
            npr--;
            ptab[curpr].active = 0;
            ptab[curpr].suspend = NOT_SUSPENDED;
            /* cause a context switch next time */
            stepcount = 0;
            /* re-activate the main proc when the number of concurrent */
            /* threads reaches zero                                    */
            if (npr == 0) {
               ptab[0].active = 1;
               ptab[0].suspend = NOT_SUSPENDED;
            }
         }
         break;  /* 32 */
     case EXIT_FCN: /* 33, exit function */
            /* get tab index of subroutine from AR[4] */
         h1 = s[saddr_check(curpr,ptab[curpr].b+4)]; 
         if (end_db){  /*  announce function termination */
#if defined(TCL)
            sprintf(tcl_buffer,"process %d %s,  procedure %s ended\n" ,
               curpr,tab[ptab[curpr].tabix].name,tab[h1].name);
            tcl_1printf(tcl_buffer,DATAWIN);
#else
            printf("process %d %s,  procedure %s ended\n" ,
               curpr,tab[ptab[curpr].tabix].name,tab[h1].name);
#endif
         }
         /* if the proc is atomic, then make the calling thread less atomic */
         if (tab[h1].atomic) ptab[curpr].atomic--;
         if (finalAR_db){
#if defined(TCL)
            sprintf(tcl_buffer,"EXIT from %s\n",tab[h1].name);
            tcl_1printf(tcl_buffer,DATAWIN);
#else
            printf("EXIT from %s\n",tab[h1].name);
#endif
            show_display(curpr); 
            show_AR(ptab[curpr].b,ptab[curpr].t,curpr); 
         }
         /* leave the funtion value on the top of the stack */
         ptab[curpr].t = ptab[curpr].b;
         /* retrieve return address */
         ptab[curpr].pc = paddr_check(curpr,s[ptab[curpr].b + 1]);
            /* check for a monitor call in the return address of the */
            /* caller's AR, AR[1]                                    */
         h2 = s[ptab[curpr].b+3];
         assert(s[h2+1] != -1);
         if (s[h2 + 1] == -1)   /* return from mon proc call  */
            h2 = s[h2+3];           /* slide down one dynamic link */
         ptab[curpr].b = h2;
         break; /* 33 */
     case VALUE_AT: /* 34, replace TOS by the value it points to */
         s[ptab[curpr].t] = s[saddr_check(curpr,s[ptab[curpr].t])]; 
         break;
     case COMPLEMENT: /* 35, complement TOS */
         s[ptab[curpr].t] = ((s[ptab[curpr].t] == 1) ? 0 : 1); 
         break;
     case NEGATE: /* 36, negate TOS */
         s[ptab[curpr].t] = -s[ptab[curpr].t];  
         break;
     case STORE_KEEP: /* 37, store & keep value */
         s[s[ptab[curpr].t - 1 ]] = s[ptab[curpr].t];
         s[ptab[curpr].t - 1] = s[ptab[curpr].t];
         ptab[curpr].t -= 1;
         break;  /* store & keep */
     case STORE: /* 38, store */
         s[saddr_check(curpr,s[ptab[curpr].t - 1 ])] = s[ptab[curpr].t];
         ptab[curpr].t -= 2;
         break;  /* store */
     case STORE_SEM: /* 39, check & store for semaphores */
         if (s[ptab[curpr].t] < 0)
            ps = semchk;
         else if ((ir.y) && (s[ptab[curpr].t] > 1))
            ps = bsemchk;
         /* go ahead with the store so user will see what went wrong*/
         s[saddr_check(curpr,s[ptab[curpr].t - 1])] = s[ptab[curpr].t];  
         ptab[curpr].t -= 2;          
         break; /* check & store */
     case CALL_MONINIT: /* 40, monitor init */
         assert(curpr == 0);        /* has to occur in main() */
         h1 = saddr_check(curpr,ptab[0].t-4); /* t -= 5 in markstack, h1=BOAR */
         h2 = s[h1+4];      /* get tab index */
         h3 = tab[h2].lev;  /* level of monitor, 0 (in C), 0 or 1 in Pasc */
         h4 = s[h1+3] + h1;       /*vsize-1 stored in AR+3 by markstack */
         s[h1] = 1;                 /* init monitor semaphore */
         s[h1+1] = ptab[0].pc;            /* ret addr */
         s[h1+2] = ptab[0].display[1];    /* static link to AR of main */
         s[h1+3] = ptab[0].display[1];  /* make dynamic link the same */
         ptab[curpr].display[h3+1] = h1;   /* set display for init code */
         for (h3 = ptab[curpr].t + 1; h3 <= h4; h3++)   
            s[h3] = 0;              /* initialize local vbls */
         tab[h2].mon = h1;          /* point to this AR in tab.mon */
         ptab[curpr].t = h4;        /* set top & bottom of init stack frame */
         ptab[curpr].b = h1;        /* set main pgm's TOS */
         if (initAR_db){
#if defined(TCL)
            sprintf(tcl_buffer,"MONINIT ENTRY %s\n",tab[h2].name);
            tcl_1printf(tcl_buffer,DATAWIN);
#else
            printf("MONINIT ENTRY %s\n",tab[h2].name);
#endif
            show_display(curpr); 
            show_AR(ptab[curpr].b,ptab[curpr].t,0); 
         }
         ptab[curpr].pc = paddr_check(curpr,tab[h2].adr);  /* branch to init code */
         break;  /* 40 */
     case RET_MONINIT: /* 41, return from monitor init */
            /* get return addr */
         ptab[curpr].pc = paddr_check(curpr,
            s[saddr_check(curpr,ptab[curpr].b+1)]);  
         s[ptab[curpr].b+1] = -1;     /* set ret adr to mark as monitor AR */
            /* crawl down static link  to AR of main */
         ptab[curpr].b = s[saddr_check(curpr,ptab[curpr].b+2)]; 
         ptab[curpr].display[1] = ptab[curpr].b; /* reset display to main AR */
         if (finalAR_db){
#if defined(TCL)
            tcl_1printf("MONINIT EXIT\n",DATAWIN);
#else
            printf("MONINIT EXIT\n");
#endif
            show_display(curpr); 
            show_AR(ptab[curpr].b,ptab[curpr].t,curpr); 
         }
         break;  /* 41 */
     case TEST_EQ_KEEP: /* 44, is TOS == TOS - 1?  leave ans on TOS */
         s[ptab[curpr].t] = (s[ptab[curpr].t] == s[ptab[curpr].t-1]);
         break;  /* 44 */
     case TEST_EQ: /* 45, is TOS == TOS - 1?, pop & leave ans on TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = (s[ptab[curpr].t] == s[ptab[curpr].t+1]);
         break;  /* 45 */
     case TEST_NE: /* 46, is TOS != TOS - 1?, pop & leave ans on TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = (s[ptab[curpr].t] != s[ptab[curpr].t+1]);
         break;  /* 46 */
     case TEST_LT: /* 47, is TOS - 1 < TOS?, pop & leave ans on TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = (s[ptab[curpr].t] < s[ptab[curpr].t+1]);
         break;  /* 47 */
     case TEST_LE: /* 48, is TOS - 1 <= TOS?, pop & leave ans on TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = (s[ptab[curpr].t] <= s[ptab[curpr].t+1]);
         break;  /* 48 */
     case TEST_GT: /* 49, is TOS - 1 > TOS?, pop & leave ans on TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = (s[ptab[curpr].t] > s[ptab[curpr].t+1]);
         break;  /* 49 */
     case TEST_GE: /* 50, is TOS - 1 >= TOS?, pop & leave ans on TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = (s[ptab[curpr].t] >= s[ptab[curpr].t+1]);
         break;  /* 50 */
     case DO_OR: /*  51, OR top two numbers on stack & leave result in TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = (s[ptab[curpr].t] | s[ptab[curpr].t + 1]);
         break;  /* 51 */
     case DO_ADD: /* 52, add top two numbers on stack & leave result in TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = s[ptab[curpr].t] + s[ptab[curpr].t+1];
         break;  /* 52 */
     case DO_SUB: /* 53, [TOS - 1] - [TOS], leave result in TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = s[ptab[curpr].t] - s[ptab[curpr].t+1];
         break;  /* 53 */
     case DO_AND: /* 56, AND top two booleans on stack & leave result in TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = (s[ptab[curpr].t] & s[ptab[curpr].t+1]);
         break;  /* 56 */
     case DO_MUL: /* 57, multiply top two nos on stack & leave result in TOS */
         ptab[curpr].t--;
         s[ptab[curpr].t] = s[ptab[curpr].t] * s[ptab[curpr].t+1];
         break;  /* 57 */
     case DO_DIV: /* 58, [TOS - 1] / [TOS], leave result in TOS */ 
         ptab[curpr].t--;
         if (s[ptab[curpr].t+1] == 0)
            ps = divchk;
         else
            s[ptab[curpr].t] = s[ptab[curpr].t] / s[ptab[curpr].t+1];
         break;  /* 58 */
     case DO_MOD: /* 59, replace TOS by [TOS-1] % (MOD) TOS */
         ptab[curpr].t--;
         if (s[ptab[curpr].t+1] == 0)
            ps = divchk;
         else
            s[ptab[curpr].t] = s[ptab[curpr].t] % s[ptab[curpr].t+1];
         break;  /* 59 */
     case READLN: 
         if (feof(stdin)) /* 62, readln */
            ps = redchk;
         break;
     case WRITELN: /* 63, writeln */
#if defined(TCL)
         tcl_printf("\n",curpr);
#else
         printf("\n");
#endif
         break;  /* 63 */
     case WHICH_PROC:   /* 71, which_proc */
         ptab[curpr].t = saddr_check(curpr,ptab[curpr].t + 1);;
         if (ptab[curpr].t > ptab[curpr].stacksize)
            ps = stkchk;
         else
            s[ptab[curpr].t] = curpr;
         break;
     case SUSPEND:   /* 72, suspend */
         if (ptab[curpr].atomic) 
            ps = deadlock; /* atomic ==> non-preemptible */
         else {
            ptab[curpr].active = 0;
            ptab[curpr].suspend = GLOBAL_SUSPEND;
            stepcount = 0;    /* force context switch */
         }
         break;
     case REVIVE:    /* 73, revive */
         h1 = s[ptab[curpr].t];
         ptab[curpr].t--;
         if ((h1 >= PMAX)||(h1 < 0))
            ps = procchk;
         else if (ptab[h1].tabix < 0)
            ps = procchk;
         ptab[h1].active = 1;
         ptab[h1].suspend = NOT_SUSPENDED;
         break;
      case RANDOM:   /* 74, random */
         h1 = s[ptab[curpr].t];
         s[ptab[curpr].t] = __urandom(h1);
         break;
      case SHORTCALL:   /* 80, short (no stackframe) call */
         in_shortcall = 1;
         shortcall_reg = ptab[curpr].pc;
         ptab[curpr].pc = paddr_check(curpr,ir.y);
         break;
      case SHORTRET:    /* 81, short (no stackframe) return */
         in_shortcall = 0;
         h1 = paddr_check(curpr,shortcall_reg);
         ptab[curpr].pc = h1;
         break;
      case SSCANF_OP:   /* 90, sscanf */
         h1 = s[ptab[curpr].t];  /* # parm addrs on the stack to scan into */
         h2 = saddr_check(curpr,ptab[curpr].t-h1-1); /* adr of string to scan */
         h4 = xsscanf(curpr,h2,ir.y,h1);
         ptab[curpr].t -= (h1 + 1);    /* pop addrs of scan str & parms */
         s[ptab[curpr].t] = h4;
         break;               
      /* case S_WRITE:   91, schedule write  REPLACE WITH */
      case SPRINTF_OP:
         h1 = s[ptab[curpr].t];  /* # parm addrs on the stack to print */
         h2 = saddr_check(curpr,ptab[curpr].t - h1 -1); /* addr of string to build */
         xsprintf(curpr,h2,ir.y,h1);
         ptab[curpr].t -= (h1 + 2);    /* pop parm_ct, string  and parm addrs */
         break;   
      case WRITE_STRING:
         /* 110 write the string at s[s[t]] to stdout */
         p = (char*) &s[saddr_check(curpr,s[ptab[curpr].t])];
         while (*p) {
#if defined(TCL)
            tcl_putchar(*p,curpr);
#else
            fputc(*p,stdout);
#endif
            p++;
         }
         ptab[curpr].t--;
         break;
      case COPY_RAWSTRING: 
         /* 111 copy a string from stab[ir.y] to s[s[t]], pop(1) */
         h1 = saddr_check(curpr,s[ptab[curpr].t]);
         local_strcpy((char *) &s[h1],&stab[ir.y]);
         ptab[curpr].t--;
         break;
      case COPY_STRING:
         /* 112 copy the string at s[s[t]] to s[s[t-1]], pop(2) */
         h1 = saddr_check(curpr,s[ptab[curpr].t-1]);
         h2 = saddr_check(curpr,s[ptab[curpr].t]);
         /* we cannot trust a user not to overlap the source & dest */
         local_strcpy(strbuf,(char *) &s[h2]);
         local_strcpy((char *) &s[h1],strbuf);
         ptab[curpr].t -= 2;
         break;
      case CONCAT_RAWSTRING:
         /* 113 concatenate the raw string at stab[ir.y] to the string */
         /* at s[s[t]], pop(1)                                         */
         p = &stab[ir.y];
         h1 = saddr_check(curpr,s[ptab[curpr].t]);
         q = (char *) &s[h1];
         /* find the end of the destination string */
         while (*q) q++;
         /* now do the copy */
         local_strcpy(q,p);
         ptab[curpr].t--;
         break;
      case CONCAT_STRING:
         /* 114 concatenate the string at s[s[t]] to the string at */
         /* s[s[t-1]], pop(2)                                      */
         h1 = saddr_check(curpr,s[ptab[curpr].t-1]);
         h2 = saddr_check(curpr,s[ptab[curpr].t]);
         q = (char *) &s[h1];
         p = (char *) &s[h2];
         /* take care of overlapping strings */
         local_strcpy(strbuf,p);
         /* find the end of the destination string */
         while (*q) q++;
         /* now do the copy */
         local_strcpy(q,strbuf);
         ptab[curpr].t -= 2;
         break;
      case COMPARE_RAWSTRING:
         /* 115 compare the string at s[s[t]] with the raw string   */
         /* at stab[ir.y] with strcmp and return the result at s[t] */
         h1 = saddr_check(curpr,s[ptab[curpr].t]);
         h2 = strcmp((char *) &s[h1], &stab[ir.y]);
         s[ptab[curpr].t] = h2;
         break;
      case COMPARE_STRING:
         /* 116 compare the string at s[s[t-1]] with the string at s[s[t]] */
         /* using strcmp, pop(1),  and return the strcmp result at s[t]    */
         h1 = saddr_check(curpr,s[ptab[curpr].t-1]);
         h2 = saddr_check(curpr,s[ptab[curpr].t]);
         ptab[curpr].t--;
         s[ptab[curpr].t] = strcmp((char *) &s[h1], (char *) &s[h2]);
         break;
      case LENGTH_STRING:
         /* 117 return at s[t] the length of the string at s[s[t]] */
         h1 = saddr_check(curpr,s[ptab[curpr].t]);
         s[ptab[curpr].t] = strlen((char *)&s[h1]);
         break; 
      default:
         ps = pcodchk;
     }  /* bottom of big, ugly switch stmt */
#if !defined(TCL)
   } while (ps == run);
   stop_interp(curpr, ps); 
#endif
}  /* interpret */

/*
 *
 *  $Id: bainterp.c,v 2.18 2007/06/01 18:58:14 bynum Exp $
 *
 */
