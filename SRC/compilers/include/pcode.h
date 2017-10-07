/*  BenAri Concurrent Pascal Compiler & Interpreter PCODE tables */
/*  PCODE is stored in the ORDER struct with fields f, x, y.     */
/*  The f field is the opcode and x and y are "utility" fields   */

#define LOAD_ADDR        0    /* push  display[x] + y */
#define LOAD_VALUE       1    /* push s[display[x]+y] */
#define LOAD_INDIR       2    /* push s[s[display[x]+y]]  */
#define UPDATE_DISPLAY   3    
   /* update the display from level y out to level x by following the */
   /* static links                                                    */
#define COBEGIN          4    /* pflag = TRUE, create concurrent threads */
#define COEND            5    /* pflag = FALSE, let concurrent threads run */
#define P_WAIT           6    /* P(s[t]) = WAIT(s[t]) */
#define V_SIGNAL         7    /* V(s[t]) = SIGNAL(s[t]) */
#define ENTERMON         8    /* enter monitor tab[y].mon */
#define EXITMON          9    /* exit current monitor */
#define WAITC           10    /* wait on condition s[t-1] w/ prio s[t] */ 
#define SIGNALC         11    /* signal condition s[t] */
#define EMPTY           12    /* s[t]=TRUE if no procs suspended on s[t] */
#define EOLN_OR_EOF     13    /* y=17 ==> s[t] = EOF, y=18 ==> s[t] = EOLN */
#define JUMP            14    /* pc = y */
#define JZER            15    /* if (s[t] == 0) pc = y */

/* FOR i := e1 TO e2 DO stmt is implemented by pushing */
/* s[t] = e2  s[t-1] = e1  s[t-2] = addr(i), then ...  */
#define BEGINFOR        16    /* aka for1up */
   /* if (e1 <= e2) i = e1 else {pop(3); pc = y} */
#define ENDFOR          17    /* aka for2up */
   /* e1++; if (e1 <= e2){i = e1; pc = y} else pop(3); */

#define MARKSTACK       18
   /* allocate space for the 5 word fixed part of stack frame        */
   /* (ptab[curpr].t += 5)                                           */
   /*    s[t-4] = s[b]   (after call):  function result              */
   /*    s[t-3] = s[b+1] (after call):  return address               */
   /*    s[t-2] = s[b+2] (after call):  static link                  */
   /*    s[t-1] = s[b+3] (after call):  dynamic link                 */
   /* 'til call = btab[tab[y].ref].vsize-1, the total size of the    */
   /*             stack frame (5 + parms + local vars)               */
   /*    s[t]   = s[b+4] (after call):  ir.y, the tab index of proc  */
#define CALL            19
   /* 1.  save dynamic link oldb = b                                 */
   /* 2.  initialize the local variable area                         */
   /* 2.  set bottom of frame, b = t - ir.y                          */  
   /* 3.  retrieve tab index of proc from s[b+4]                     */
   /* 4.  retrieve level of proc from tab[index].lev                 */
   /* 5a. static link, regular proc: set s.l. @ s[b+2] = disp[level] */ 
   /* 5b. static link, monitor proc: retrieve stack addr of          */ 
   /*     monitor's AR (h5 = tab[tab[index].mon].mon)                */ 
   /*     set s[b+2] = disp[level] = h5                              */
   /* 6.  set dynamic link at s[b+3] to oldb, saved in #1            */
   /* 7.  set display[level+1] to b (proc executes 1 level up)       */
   /* 8.  set s[b+1] to pc, the return address of the call           */
   /* 9.  jump to proc by setting pc to tab[index].adr               */

#define INDEX           21
   /* pop(1), check that the index s[oldt] is between atab[y].low and       */
   /* atab[y].high, leave the addr of array element with this index in s[t] */
   /* i.e., s[t] = s[oldt-1] + (s[oldt] - atab[y].low) * atab[y].elsize     */
#define LOAD_BLOCK      22 /* push y words from address s[t] onto stack */
#define COPY_BLOCK      23 /* copy y words from addr s[t] to addr s[t-1] */
#define PUSH_LIT        24 /* push literal y onto stack */
#define READ            27 
   /* read a value of type y (ints or chars) into vbl with addr s[t] */
#define WRITE_RAWSTRING 28 
   /* write to stdout string of length s[t] starting at stab[y] */
#define WRITE           29
   /* write to stdout s[t] for ints, bools, chars, sems, bsems */
#define HALT            31 /* halt execution */
#define EXIT_PROC       32 /* exit procedure */
#define EXIT_FCN        33 /* exit function */
#define VALUE_AT        34 /* replace s[t] by the value at addr s[t] */
#define COMPLEMENT      35 /* complement s[t], s[t] = ((s[t] == 1)?0:1) */ 
#define NEGATE          36 /* s[t] = -s[t] */
#define STORE_KEEP      37 /* store s[t] at the addr s[t-1], move s[t] to */
                           /* s[t-1] & pop(1)                             */
#define STORE           38 /* store s[t] at the addr s[t-1] & pop(2) */
#define STORE_SEM       39 
   /* check s[t] as bsems or sems value, store at s[t-1] & pop(2) */
#define CALL_MONINIT    40 /* execute init block for a monitor */
#define RET_MONINIT     41 /* return from monitor init block */
   /* test for equality & keep 'first' operand on stack below test */
#define TEST_EQ_KEEP    44 /* s[t] = (s[t-1] == s[t]) */
   /* TEST_OP means pop(1) and s[t] = (s[oldt-1] OP s[oldt]) */
#define TEST_EQ         45 /* pop(1), s[t] = (s[oldt-1] == s[oldt]) */
#define TEST_NE         46 /* pop(1), s[t] = (s[oldt-1] != s[oldt]) */
#define TEST_LT         47 /* pop(1), s[t] = (s[oldt-1] < s[oldt]) */
#define TEST_LE         48 /* pop(1), s[t] = (s[oldt-1] <= s[oldt]) */
#define TEST_GT         49 /* pop(1), s[t] = (s[oldt-1] > s[oldt]) */
#define TEST_GE         50 /* pop(1), s[t] = (s[oldt-1] >= s[oldt]) */
   /* binary operations have the same syntax */
#define DO_OR           51 /* pop(1), s[t] = (s[oldt-1] | s[oldt]) */
#define DO_ADD          52 /* pop(1), s[t] = (s[oldt-1] + s[oldt]) */
#define DO_SUB          53 /* pop(1), s[t] = (s[oldt-1] - s[oldt]) */
#define DO_AND          56 /* pop(1), s[t] = (s[oldt-1] & s[oldt]) */
#define DO_MUL          57 /* pop(1), s[t] = (s[oldt-1] * s[oldt]) */
#define DO_DIV          58 /* pop(1), s[t] = (s[oldt-1] / s[oldt]) */
#define DO_MOD          59 /* pop(1), s[t] = (s[oldt-1] % s[oldt]) */

#define READLN          62 /* fscanf(stdin,"%[^\n]\n") = READLN; */
#define WRITELN         63 /* printf("\n") = WRITELN; */

#define EXIT            70 /* terminate the program */
#define WHICH_PROC      71 /* return proc no of current thread */
#define SUSPEND         72 /* suspend calling thread */
#define REVIVE          73 /* revive proc by proc no */
#define RANDOM          74 /* return randomly chosen int in 0..range-1 */

#define SHORTCALL       80 /* push pc, pc = y   (no activation record) */
#define SHORTRET        81 /* pc = s[t], pop(1) (shortcall companion)  */

#define SSCANF_OP       90
   /* BACI sscanf.  y = stab index of format string                     */
   /* s[t] = # parms on stack, not incl. string address to be scanned,  */
   /* plus the addresses of the operand into which scan results are to  */
   /* be placed.  Last parm is not NULL, as in std C                    */
   /* scan parms are popped, # of successful scans is left in s[t]      */

#define SPRINTF_OP      91
   /* BACI sprintf.  y = stab index of format string                       */
   /* s[t] = # parms on stack, not incl. address of string buffer to be    */
   /* built, plus the values (ints) or addresses (strings) of the parms    */
   /* to be inserted into the string being constructed.  Last parm is      */
   /* not NULL, as in Std C.  all sprintf parms are popped, (void return)  */

/* Steve Burdette -- distributed BACI opcodes and constants */
#define SEND_RAWSTRING        100
   /* sends the raw string stored at stab[y] to the process ID s[t], pops(1) */
#define SEND_STRING           101 
   /* sends the string stored at s[s[t-1]] to the process ID s[t], pops(2) */
#define RECEIVE_OP            102
   /* receives a string or int message into the space at s[s[t]]           */
   /* stores INT_MSG at s[t] if message was an int, else stores STRING_MSG */
   /* no pop                                                               */
#define BROADCAST_RAWSTRING   103
   /* broadcast the raw string stored at stab[y]  */
#define BROADCAST_STRING      104
   /* broadcast the string stored at s[s[t]], pop(1)  */
#define RECEIVE_ID            105
   /* receive a string or int message into the space at s[s[t-1]], stores */
   /* ID of sender at s[s[t]], pops(1), stores INT_MSG at s[t] if msg was */
   /* an int, else stores STRING_MSG                                      */
#define SEND_INT              106
   /* send the int stored at s[t-1] to the process with id s[t], pop(2) */
#define BROADCAST_INT         107
   /* broadcast the int stored at s[t], pop(1) */

   /* One of these two constants is left on the stack by a receive */
#define INT_MSG      1
#define STRING_MSG   2
/* end Steve */


   /* WRITE_RAWSTRING is defined to be 28 above */
#define WRITE_STRING       110
   /* writes the string stored at s[s[t]] to stdout, pops(1)       */
#define COPY_RAWSTRING     111
   /* copies the string stored at stab[y] to s[s[t]], pops(1)   */
#define COPY_STRING        112
   /* copies string stored at s[s[t-1]] to s[s[t]], pops(2) */
#define CONCAT_RAWSTRING   113
   /* concatenates the string stored at stab[y] to string at s[s[t]], pops(1) */
#define CONCAT_STRING      114
   /* concatenates string stored at s[s[t]] to string at s[s[t-1]], pops(2) */
#define COMPARE_RAWSTRING  115
   /* compares the string stored at s[s[t]] to the string at stab[y]  */
   /* using strcmp, pops(1), returns the result of the strcmp at s[t] */
#define COMPARE_STRING     116
   /* compares the string stored at s[s[t-1]] to the string at s[s[t]] */
   /* using strcmp, pops(1), returns the result of the strcmp at s[t]  */
#define LENGTH_STRING      117
   /* finds length of string stored at s[s[t]] using strlen, returns   */
   /* result at s[t]                                                   */

/*  Constants used for standard procs and functions */
#define SP_READ          1
#define SP_READLN        2
#define SP_WRITE         3
#define SP_WRITELN       4
#define SP_INITSEM       5
#define SP_PWAIT         6
#define SP_VSIGNAL       7
#define SP_WAITC         8
#define SP_SIGNALC       9
#define SP_EXIT         10
#define SP_SUSPEND      11
#define SP_REVIVE       12

#define SF_EMPTY        16    /* was 2 in original BenAri */
#define SF_EOF          17    /* switch(y) in pcode 13 */
#define SF_EOLN         18    /* switch(y) in pcode 13 */
#define SF_WHICHPROC    19
#define SF_RANDOM       20

/*
 *
 *  $Id: pcode.h,v 1.10 2007/06/01 17:41:16 bynum Exp $
 *
 */
