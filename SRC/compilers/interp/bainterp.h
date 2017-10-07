/*
   global types used by the BACI interpreter

*/


#define  UNDEF_VALUE 0x3f3f   /* '??' as char, val of undef vbl */
#define  STEPMAX     4        /* max steps before process switch */
#define  STEPMAXP1   5

#define  STMAX    6000        /* size of stack */
#define  STKINCR  200         /* stack size for each process */

#define  MAXPRIO  0x7FFF      /* max priority a WAITC can have */

enum  pstat{   run,           /* normal operation */
               fin,           /* normal termination */
               divchk,        /* division error */
               inxchk,        /* array indexing error */
               stkchk,        /* stack addressing error */
               charchk,       /* invalid character */
               inpchk,        /* invalid input type */
               pcodchk,       /* invalid PCODE opcode */
               rdchk,         /* read error */
               wrtchk,        /* write error */
               padrchk,       /* invalid PCODE address */
               semchk,        /* invalid value in a general semaphore */
               bsemchk,       /* invalid value in a binary semaphore */
               uisemchk,      /* unititialized semaphore */
               xmonchk,       /* cross-monitor call */
               redchk,        /* attempt to read past EOF */
               procchk,       /* invalid process ID */
               proctabchk,    /* proctab full */
               deadlock       /* no runnable process */
         };

/* process table type -- information kept for each concurrent process */
typedef struct   {  
            unsigned    pc;         /* program counter */
            unsigned    t,b;        /* top, bottom of AR */
            int         tabix;      /* index into tab array */
            int         stacksize;  /* stack limit */
            int         display[LSIZE];   /* display */
            int         suspend;    /* -1 or stack addr of semaphore */
            int         active;     /* process active? */
            int         monitor;    /* 0 or tab index of monitor */
            int         priority;   /* WAITC priority P&W */
            int         atomic;     /* is process un-interruptable? */
            unsigned    xpc;        /* no. of pcode instrs executed */
            /* the next two fields are used to execute one source line */
            /* by the current process                                  */
            int         first_spc;  /* least pc for this source line */
            int         last_spc;   /* greatest pc for this source line */
   } proctab;

   /* values for the suspend field of the proctab struct */
#define  NOT_SUSPENDED  -1          /* proc is not suspended */
   /* when a normal proc suspends, the suspend field is the addr of */
   /* the semaphore or condition on which the proc is waiting       */
   /* The following constant is used when a proc suspends for a     */
   /* different reason, like proc[0] at a cobegin block or a proc   */
   /* suspended by the user through SUSPEND                         */
   /* Because of the algorithm in chooseproc, this number should be */
   /* positive and an invalid runtime stack address                 */
#define  GLOBAL_SUSPEND  STMAX
   /* This value is used when a monitor proc is suspended because   */
   /* it signals a condition (the Immediate Resumption Requirement) */
   /* Because of the algorithm in chooseproc, this number should be */
   /* positive and an invalid runtime stack address                 */
#define  SIGNAL_SUSPEND  STMAX+1

/*
 *
 * $Id: bainterp.h,v 1.5 2007/06/01 18:58:44 bynum Exp $
 *
 */
