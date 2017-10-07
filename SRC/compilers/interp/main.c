   /*  a concurrent Pascal PCODE INTERPRETER with monitors  */
   /* author : N. Wirth, E.T.H. ch--8092 Zurich, 1/3/76                   */
   /* modified : M. Ben-Ari, Tel Aviv Univ., 1980                         */
   /* William & Mary : CS 444, Fall 1983, directed by R. E. Noonan,       */
   /*   re-created the program from the Implementation Kit in Ben-Ari's   */
   /*   book.                                                             */
   /*   Modifications suggested by Pramanik & Weinberg, SIGPLAN Notices   */
   /*   v.19 #9, pp 30-33 incorporated by W.L. Bynum, December 1984       */
   /* 8/87  WLB   remove VALUE statement to improve portability           */
   /* 9/87  WLB   add procedure/function termination debug                */
   /* 10/87 WLB   expand debug, add pcode dump                            */
   /* 10/87 WLB   add binary sem, runtime sem checks                      */
   /* 10/87 WLB   incorporate monitors into the parser & interpreter      */
   /*  3/88 WLB   add proc name in show_AR                                */
   /*  9/90 WLB   convert to Turbo Pascal 5.0                             */
   /*  9/93 WLB   separate Concurrent Pascal COMPILER & INTERPRETER       */
   /*  9/93 WLB   rewrite INTERPRETER in C                                */
 
#if defined(TCL)
   /* GUI interpreter */
#include "tcl.h"
#include "tk.h"
#include <stdlib.h>
#include <string.h>
#include "../include/bacicnty.h"
#include "bainterp.h"


#if defined(SunOS)
extern int printf(const char*,...);
extern int fprintf(FILE*,const char*,...);
#endif

extern void make_interpname();
extern void  get_command_options(int, char**);


extern   void init_interpret();  
extern   void interpret();   /* This is the old interpret now called once for
                                each pco line   */
extern   void global_init(); /* Old proc, mostly the same except for commmand
                              line arguments  */

extern enum pstat ps;

extern int xpause;      /* used to pause output */
extern int debugging;   /* flag for debugging */
extern int ShowPCODE;   /* flag for debugging */

extern int last_infile; /* ix of last entry of infile[] array */
extern int last_code;   /* ix of last PCODE instr in code[] array */
extern int break_cnt;   /* # of breakpoints set */

extern int runinit(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]);
extern int getInfile(ClientData cd, Tcl_Interp *interp,int argc, char *argv[]);
extern int getBreakpoint(ClientData cd, Tcl_Interp *interp,
   int argc, char *argv[]);
extern int setBreakpoint(ClientData cd, Tcl_Interp *interp,
   int argc, char *argv[]);
extern int unsetBreakpoint(ClientData cd, Tcl_Interp *interp,
   int argc, char *argv[]);
extern int LinenoToPC(ClientData cd, Tcl_Interp *interp,
   int argc, char *argv[]);
extern int LineHasBreak(ClientData cd, Tcl_Interp *interp,
   int argc, char *argv[]);
extern int WriteData(ClientData cd, Tcl_Interp *interp,
   int argc, char *argv[]);
extern int runcmd();
extern int initAR_db, finalAR_db, end_db, pcexec_db;

extern char pcode_fname[];
extern char interp_pname[];

char* CurrentPCOFile;
char* InterpPName;

static char dirname[512];
static char tmpbuf[512];

Tcl_Interp *interp;

Tk_Window MainWin;

#if defined(Tcl7_5)
#define Tcl_Alloc(m) (malloc(m))
#endif

int main(int argc, char **argv)
{
   int retcode;

   /* Create tcl interpreter and window */
   interp = (Tcl_Interp*) Tcl_CreateInterp();
  
	Tcl_FindExecutable(argv[0]);
   if( Tcl_Init(interp) == TCL_ERROR ) {
      fprintf(stderr, "Tcl_Init failed: %s\n", interp->result);
      exit(1);
   }
   if( Tk_Init(interp) == TCL_ERROR ) {
      fprintf(stderr, "Tk_Init failed: %s\n", interp->result);
      exit(1);
   }
   MainWin = Tk_MainWindow(interp);
   if (MainWin == NULL) {
      fprintf(stderr,"Can't create main window: %s\n",interp->result);
      exit(1);
   }
   /* Create commands and common variables */
   Tcl_CreateCommand(interp, "run", (Tcl_CmdProc *) runinit, (ClientData) NULL, 
      (Tcl_CmdDeleteProc *) NULL);
   Tcl_CreateCommand(interp, "getInfile", (Tcl_CmdProc*) getInfile, 
      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
   Tcl_CreateCommand(interp, "getBreakpoint", (Tcl_CmdProc *) getBreakpoint, 
      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
   Tcl_CreateCommand(interp, "setBreakpoint", (Tcl_CmdProc *) setBreakpoint, 
      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
   Tcl_CreateCommand(interp, "unsetBreakpoint", (Tcl_CmdProc *) unsetBreakpoint, 
      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
   Tcl_CreateCommand(interp, "LinenoToPC", (Tcl_CmdProc *) LinenoToPC, 
      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
   Tcl_CreateCommand(interp, "LineHasBreak", (Tcl_CmdProc *) LineHasBreak, 
      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
   Tcl_CreateCommand(interp, "WriteData", (Tcl_CmdProc *) WriteData, 
      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  
   Tcl_LinkVar(interp, "tinitAR_db", (char *) &initAR_db, TCL_LINK_INT);
   Tcl_LinkVar(interp, "tfinalAR_db", (char *) &finalAR_db, TCL_LINK_INT);
   Tcl_LinkVar(interp, "tend_db", (char *) &end_db, TCL_LINK_INT);
   Tcl_LinkVar(interp, "tpcexec_db", (char *) &pcexec_db, TCL_LINK_INT);
   Tcl_LinkVar(interp, "xpause", (char *) &xpause, TCL_LINK_INT);
   Tcl_SetVar( interp, "xpause", "-1", TCL_GLOBAL_ONLY );
   Tcl_LinkVar(interp, "debugging", (char *) &debugging, TCL_LINK_INT);
   Tcl_SetVar( interp, "debugging", "0", TCL_GLOBAL_ONLY );
   Tcl_LinkVar(interp, "ShowPCODE", (char *) &ShowPCODE, TCL_LINK_INT);
   Tcl_SetVar( interp, "ShowPCODE", "0", TCL_GLOBAL_ONLY );

   Tcl_LinkVar(interp, "tlast_infile", (char *) &last_infile, TCL_LINK_INT);
   Tcl_LinkVar(interp, "tlast_code", (char *) &last_code, TCL_LINK_INT);
   Tcl_LinkVar(interp, "tbreak_cnt", (char *) &break_cnt, TCL_LINK_INT);

   make_interpname();
   get_command_options(argc,argv);
   if(pcode_fname[0] != '\0') {
      CurrentPCOFile = Tcl_Alloc(strlen(pcode_fname)+5); /* .pco suffix + \0 */
      if (CurrentPCOFile == NULL) {
         fprintf(stderr,"Can't Tcl_Alloc space for pcode filename\n");
         exit(1);
      }
      strcpy(CurrentPCOFile,pcode_fname);
   } 
   else {
      CurrentPCOFile = Tcl_Alloc(5); 
      if (CurrentPCOFile == NULL) {
         fprintf(stderr,"Can't Tcl_Alloc space for pcode filename\n");
         exit(1);
      }
      strcpy(CurrentPCOFile,"NULL");
   }
   Tcl_LinkVar(interp, "CurrentPCOFile", (char *) &CurrentPCOFile, 
      TCL_LINK_STRING);
   InterpPName = Tcl_Alloc(strlen(interp_pname)+1);
   if (InterpPName == NULL) {
      fprintf(stderr,"Can't Tcl_Alloc space for interpreter name\n");
      exit(1);
   }
   strcpy(InterpPName,interp_pname);
   Tcl_LinkVar(interp, "InterpPName", (char *) &InterpPName, TCL_LINK_STRING);

   /* Open tcl file and evaluate it */
	retcode = Tcl_Eval(interp,"info nameofexecutable");
	if (retcode != TCL_OK) {
		fprintf(stderr,"Can't get name of executable from Tcl interpreter\n");
		fprintf(stderr,"%s\n",interp->result);
		exit(1);
	}
	strcpy(dirname,interp->result);
	sprintf(tmpbuf,"file dirname %s",dirname);
	retcode = Tcl_Eval(interp,tmpbuf);
	if (retcode != TCL_OK) {
		fprintf(stderr,"Can't find directory name from executable name\n");
		fprintf(stderr,"%s\n",interp->result);
		exit(1);
	}
	strcpy(dirname,interp->result);
   strcat(dirname,"/menu.tcl");
   retcode = Tcl_EvalFile( interp, dirname);
   if ( retcode != TCL_OK ) {
      fprintf( stderr, "\nGUI: Initialization ERROR: " );
      fprintf( stderr, "%s\n", interp->result );
      exit(1);
   }
   Tcl_Exit(0);
   return(0);  
}


#else
   /* plain vanilla interpreter */
extern  void interpret(void);
extern  void global_init(int,char**);
extern  void check_unresolved_externs();

int main(int argc, char **argv)
{
   global_init(argc,argv);
   check_unresolved_externs();
   interpret();
   return(0);
}
#endif

/*
 *
 *  $Id: main.c,v 2.4 2007/06/01 18:58:44 bynum Exp $
 *
 */
