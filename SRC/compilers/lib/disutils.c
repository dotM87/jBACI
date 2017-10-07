/*
   BACI disassembly utilities
   used by the disassembler and interpreter
*/

#include <stdio.h>
#include <string.h>

#include "../include/globdata.h" /* brings in bacicnty.h */
#include "../include/globtabs.h" /* brings in tables     */
#include "../include/pcode.h"

CODEBLOCK cbtab[BSIZE];
int last_cbtab;

extern int last_tab;

int build_cbtab()
   /* 
      Build the code block table, cbtab.  This table is used to mark
      proc boundaries when there is no source file available
   */
{
   int ix,jx;
   int cbix = 0;
   CODEBLOCK tmp;
   /* we assume that btab is consistently sized (i.e., no size check here) */
   for (ix = 0; ix <= last_tab; ix++) {
         /* save information for the only BACI objects that can have PCODE */
         /* main(), procedure, function, or monitor (init code)            */
         /* The procs internal to the monitor will have their own entries  */
      if ((tab[ix].obj == procedure) || (tab[ix].obj == function) ||
          (tab[ix].obj == monitor) || (tab[ix].obj == outerblock) ||
          (tab[ix].obj == mainproc)) {
         cbtab[cbix].tix = ix;
         cbtab[cbix].bix = tab[ix].ref; 
         cbtab[cbix].adr = tab[ix].adr;
         cbix++;
      }
   }  /* for */
   cbix--;  /* back up to size of cbtab */
   /* bubble sort into increasing code address order */
   for(ix = cbix; ix > 0; ix--) {
      for (jx =0; jx < ix; jx++) {
         if (cbtab[jx].adr > cbtab[jx+1].adr) {
            tmp = cbtab[jx];
            cbtab[jx] = cbtab[jx+1];
            cbtab[jx+1] = tmp;
         }
      }
   }
   return cbix;
}  /* build_cbtab */

int find_var(int tabix, int level, int offset)
   /* look for a variable at 'level' & 'offset' of the proc or func */
   /* whose tab[] index is 'tabix'                                  */
   /* return the tab[] index of the variable, if found, else 0      */
{
   int bix = tab[tabix].ref;
   int ix = btab[bix].last;
   while (ix) {
      if ((tab[ix].lev == level)&&(tab[ix].adr == offset)&&
          ((tab[ix].obj == variable)||(tab[ix].obj == ext_variable)))
         return ix;
      ix = tab[ix].link;
   }
   return ix;
}  /* find_var */

int search_var(int tabix, int level, int offset)
   /* search for a variable at 'level' & 'offset' starting at the */
   /* proc whose tab[] index is 'tabix'.  Continue the search     */
   /* outward through every block in the code block table at a    */
   /* lower level than the tab[tabix] proc                        */
   /* return the tab[] index of the variable, if found, else 0    */
   /* When 'level' == 0, this is the return value of a function.  */
   /* In this case, return the tabix of the function              */
{
   int jx;
   int ix;
   int mon;
   if ((offset == 0)&&(level > 0))
      return tabix;   /* function return value */
   ix = find_var(tabix,level,offset);
   if (ix > 0) return ix;
   mon = tab[tabix].mon;
   if (mon > 0) { // if a mon proc, then look in the monitor next
      ix = find_var(mon,level,offset);
      if (ix > 0) return ix;
   }
   for (jx = 0; jx <= last_cbtab; jx++) {
      if ((tab[cbtab[jx].tix].lev < tab[tabix].lev) ||
          (tab[cbtab[jx].tix].obj == outerblock))  {
         ix = find_var(cbtab[jx].tix,level,offset);
         if (ix > 0) return ix;
      }
   }
   return 0;
}

 /* The disassemble() proc */

char nextline_space[] = {"                     "};

static char locbuf[256];

void disassemble(char *dl, int tix, int lc)
   /* disassemble the instruction at code[lc]     */
   /* 'tix' is the tab index of the current block */
   /* (used for searching the symbol table        */
{
   int ix;
   ORDER ir;
   ir = code[lc];
   sprintf(dl,"%4d %4d %4d %4d  ",lc,ir.f,ir.x,ir.y);
   switch (ir.f) { 
   case LOAD_ADDR: /* 0, load address */
      ix = search_var(tix, ir.x,ir.y);
      sprintf(locbuf,"LOAD_ADDR, push &%s\n",tab[ix].name);
      strcat(dl,locbuf);
      break;
   case LOAD_VALUE: /* 1, load value */
      ix = search_var(tix, ir.x,ir.y);
      sprintf(locbuf,"LOAD_VALUE, push %s\n",tab[ix].name);
      strcat(dl,locbuf);
      break;
   case LOAD_INDIR: /* 2, load indirect */
      ix = search_var(tix, ir.x,ir.y);
      sprintf(locbuf,"LOAD_INDIRECT, push s[%s]\n",tab[ix].name);
      strcat(dl,locbuf);
      break;
   case UPDATE_DISPLAY: /* 3, update display */
      sprintf(locbuf,"UPDATE_DISPLAY from level %d out to level %d\n",
         ir.y,ir.x);
      strcat(dl,locbuf);
      break;
   case COBEGIN: /* 4, cobegin */  
      strcat(dl,"COBEGIN\n");
      break;
   case COEND: /* 5, coend */
      strcat(dl,"COEND\n");
      break;
   case P_WAIT: /* 6, P or WAIT */
      strcat(dl,"P(s[s[t]]), pop(1)\n");
      break;      
   case V_SIGNAL: /* 7, V  or SIGNAL */
      strcat(dl,"V(s[s[t]]), pop(1)\n");
      break; 
   case ENTERMON: /* 8, entermon  P&W */
      sprintf(locbuf,"ENTERMON %s\n",tab[ir.y].name);
      strcat(dl,locbuf);
      break;  /* 8 entermon */
   case EXITMON: /* 9, exitmon  P&W */
      strcat(dl,"EXITMON\n");
      break;  /* 9 exitmon */
   case WAITC: /* 10, waitc  P&W */
      strcat(dl,"WAITC, priority s[t], condition &s[t-1], pop(2)\n");
      break;  /* case 10  -- waitc  P&W */
   case SIGNALC: /* 11, signalc  P&W */
      strcat(dl,"SIGNALC, condition &s[t], pop(1)\n");
      break;  /* case 11  -- signalc P&W */
   case EMPTY: /* 12, empty  P&W */
      strcat(dl,"EMPTY, if queue for &s[t] is empty, s[t] = 1, else s[t] = 0\n");
      break;  /* case 12 -- empty P&W */
   case EOLN_OR_EOF: /* 13, EOLN or EOF, emitted in "factor", was 8 */  
      switch(ir.y) { 
         case SF_EOF: /* 17, EOF */
            strcat(dl,"SF_EOF, push EOF(stdin)\n");
            break;
         case SF_EOLN: /* 18, EOLN */
            strcat(dl,"SF_EOLN, push EOLN(stdin)\n");
            break; /* case 18  eof or eoln  */
         default:
            sprintf(locbuf,"unknown ir.y field %d at %d\n",ir.y,lc);
            strcat(dl,locbuf);
            break;
      } /* switch */
      break;
   case JUMP:   /* 14, jump, used in "ifstmt" was case 10  P&W */
      sprintf(locbuf,"JUMP to %d\n",ir.y);
      strcat(dl,locbuf);
      break; 
   case JZER: /* 15, conditional jump, used in "ifstmt"  was case 11  P&W */
      sprintf(locbuf,"JZER, if (s[t] == 0), jump to %d, pop(1)\n",ir.y);
      strcat(dl,locbuf);
      break;
   case BEGINFOR: /* 16, for1up  was case 14  P&W */
      strcat(dl,
         "BEGINFOR, if (s[t-1] <= s[t]) then s[s[t-2]] = s[t-1]\n");
      sprintf(locbuf,"%s          else {pop(3), JUMP %d}\n", 
         nextline_space,ir.y);
      strcat(dl,locbuf);
      break;
   case ENDFOR: /* 17, for2up  was case 15  P&W */
      strcat(dl,"ENDFOR, s[s[t-2]]++, if (s[t-1] <= s[t]) then\n");
      sprintf(locbuf,"%s        {s[s[t-2]] = s[t-1], JUMP %d} else pop(3)\n",
         nextline_space,ir.y);
      strcat(dl,locbuf);
      break;
   case MARKSTACK: /* 18, markstack */
      sprintf(locbuf,"MARKSTACK %s\n",tab[ir.y].name);
      strcat(dl,locbuf);
      break; 
   case CALL: /* 19, call */
      if (ir.x == -1) strcat(dl,"monitor ");
      sprintf(locbuf,"CALL, psize-1 = %d\n",ir.y);
      strcat(dl,locbuf);
      break;
   case INDEX: /* 21, index */
      sprintf(locbuf,"INDEX atab[%d], pop(1)\n",ir.y);
      strcat(dl,locbuf);
      break; 
   case LOAD_BLOCK: /* 22, load block */
      sprintf(locbuf,"LOAD_BLOCK, pop(1), push %d words at &s[oldt]\n",ir.y);
      strcat(dl,locbuf);
      break;
   case COPY_BLOCK: /* 23, copy block */
      sprintf(locbuf, 
         "COPY_BLOCK, copy %d words from &s[t] to &s[t-1], pop(2)\n", ir.y);
      strcat(dl,locbuf);
      break; 
   case PUSH_LIT: /* 24, literal */
      sprintf(locbuf,"PUSH_LIT %d\n",ir.y);
      strcat(dl,locbuf);
      break;  /* 24 */
   case READ: /* 27, read */
      sprintf(locbuf,"READ %s to &s[t], pop(1)\n",
         ((ir.y == ints)?"int":"char"));
      strcat(dl,locbuf);
      break; /* 27 */
   case WRITE_RAWSTRING: /* 28,write raw string */
      sprintf(locbuf,"WRITE_RAWSTRING stab[%d] to stdout\n",ir.y);
      strcat(dl,locbuf);
      break;  /* 28 */
   case WRITE: /* 29, write1 */
      strcat(dl,"WRITE ");
      switch (ir.y) {
         case ints:     strcat(dl,"(int)"); break;
         case sems:     strcat(dl,"(semaphore)"); break;
         case bsems:    strcat(dl,"(binarysem)"); break;
         case chars:    strcat(dl,"(char)"); break;
         case bools:    strcat(dl,"(boolean)"); break;
         default:       strcat(dl,"(\?\?)"); break;
      }
      strcat(dl," s[t] to stdout, pop(1)\n");
      break;  /* 29 */
  case HALT: /* 31, halt */
      strcat(dl,"HALT\n");
      break; 
  case EXIT_PROC: /* 32, exit procedure */
      strcat(dl,"EXIT_PROC\n");
      break;  /* 32 */
  case EXIT_FCN: /* 33, exit function */
         /* get index of subroutine from AR */
      strcat(dl,"EXIT_FCN\n");
      break;  /* 33 */
  case VALUE_AT: /* 34, replace TOS by the value it points to */
      strcat(dl,"VALUE_AT, s[t] = s[s[t]]\n");
      break;
  case COMPLEMENT: /* 35, complement TOS */
      strcat(dl,"COMPLEMENT s[t]\n");
      break;
  case NEGATE: /* 36, negate TOS */
      strcat(dl,"NEGATE s[t]\n");
      break;
  case STORE_KEEP: /* 37, store & keep value */
      strcat(dl,"STORE_KEEP, s[s[t-1]] = s[t], s[t-1] = s[t], pop(1)\n");
      break;  /* store & keep */
  case STORE: /* 38, store */
      strcat(dl,"STORE, s[s[t-1]] = s[t], pop(2)\n");
      break;  /* store */
  case STORE_SEM: /* 39, check & store for semaphores */
      strcat(dl,"STORE_SEM, s[s[t-1]] = sem_check(s[t]), pop(2)\n");
      break;  /* store_sem */
  case CALL_MONINIT: /* 40, monitor init */
      strcat(dl,"CALL_MONINIT\n");
      break;  /* 40 */
  case RET_MONINIT: /* 41, return from monitor init */
      strcat(dl,"RET_MONINIT\n");
      break;  /* 41 */
  case TEST_EQ_KEEP: /* 44, is TOS == TOS - 1?  leave ans on TOS */
      strcat(dl,"TEST_EQ_KEEP, s[t] = (s[t-1] == s[t])\n");
      break;  /* 44 */
  case TEST_EQ: /* 45, is TOS == TOS - 1?, pop & leave ans on TOS */
      strcat(dl,"TEST_EQ, pop(1), s[t] = (s[oldt-1] == s[oldt])\n");
      break;  /* 45 */
  case TEST_NE: /* 46, is TOS != TOS - 1?, pop & leave ans on TOS */
      strcat(dl,"TEST_NE, pop(1), s[t] = (s[oldt-1] != s[oldt])\n");
      break;  /* 46 */
  case TEST_LT: /* 47, is TOS - 1 < TOS?, pop & leave ans on TOS */
      strcat(dl,"TEST_LT, pop(1), s[t] = (s[oldt-1] < s[oldt])\n");
      break;  /* 47 */
  case TEST_LE: /* 48, is TOS - 1 <= TOS?, pop & leave ans on TOS */
      strcat(dl,"TEST_LE, pop(1), s[t] = (s[oldt-1] <= s[oldt])\n");
      break;  /* 48 */
  case TEST_GT: /* 49, is TOS - 1 > TOS?, pop & leave ans on TOS */
      strcat(dl,"TEST_GT, pop(1), s[t] = (s[oldt-1] > s[oldt])\n");
      break;  /* 49 */
  case TEST_GE: /* 50, is TOS - 1 >= TOS?, pop & leave ans on TOS */
      strcat(dl,"TEST_GE, pop(1), s[t] = (s[oldt-1] >= s[oldt])\n");
      break;  /* 50 */
  case DO_OR: /*  51, OR top two numbers on stack & leave result in TOS */
      strcat(dl,"DO_OR, pop(1), s[t] = (s[oldt-1] | s[oldt])\n");
      break;  /* 51 */
  case DO_ADD: /* 52, add top two numbers on stack & leave result in TOS */
      strcat(dl,"DO_ADD, pop(1), s[t] = (s[oldt-1] + s[oldt])\n");
      break;  /* 52 */
  case DO_SUB: /* 53, [TOS - 1] - [TOS], leave result in TOS */
      strcat(dl,"DO_SUB, pop(1), s[t] = (s[oldt-1] - s[oldt])\n");
      break;  /* 53 */
  case DO_AND: /* 56, AND top two booleans on stack & leave result in TOS */
      strcat(dl,"DO_AND, pop(1), s[t] = (s[oldt-1] & s[oldt])\n");
      break;  /* 56 */
  case DO_MUL: /* 57, multiply top two nos on stack & leave result in TOS */
      strcat(dl,"DO_MUL, pop(1), s[t] = (s[oldt-1] * s[oldt])\n");
      break;  /* 57 */
  case DO_DIV: /* 58, [TOS - 1] / [TOS], leave result in TOS */ 
      strcat(dl,"DO_DIV, pop(1), s[t] = (s[oldt-1] / s[oldt])\n");
      break;  /* 58 */
  case DO_MOD: /* 59, replace TOS by [TOS-1] % (MOD) TOS */
      strcat(dl,"DO_MOD, pop(1), s[t] = (s[oldt-1] \% s[oldt])\n");
      break;  /* 59 */
  case READLN: 
      strcat(dl,"READLN\n");
      break;
  case WRITELN: /* 63, writeln */
      strcat(dl,"WRITELN\n");
      break;  /* 63 */
  case WHICH_PROC:   /* 71, which_proc */
      strcat(dl,"WHICH_PROC, push # current process\n");
      break;
  case SUSPEND:   /* 72, suspend */
      strcat(dl,"SUSPEND current process\n");
      break;
  case REVIVE:    /* 73, revive */
      strcat(dl,"REVIVE process # s[t]\n");
      break;
   case RANDOM:   /* 74, random */
      strcat(dl,"RANDOM, s[t] = random(0..s[oldt] - 1)\n");
      break;
   case SHORTCALL:   /* 80, short (no stack) call */
      sprintf(locbuf,"SHORTCALL to %d, shortcall_reg = pc, pc = %d\n",
         ir.y,ir.y);
      strcat(dl,locbuf);
      break;
   case SHORTRET:    /* 81, short (no stack) return */
      strcat(dl,"SHORTRET, pc = shortcall_reg\n");
      break;
   case SSCANF_OP:   /* 90 BACI sscanf */
      sprintf(locbuf,"SSCANF(&s[t-n],stab[%d],s[t-n-1]..s[t-1]), pop(s[t])\n",
         ir.y);
      strcat(dl,locbuf);
      strcat(dl,nextline_space);
      strcat(dl,"s[t] = # successful scans\n");
      break;
   case SPRINTF_OP:  /* 91 BACI sprintf */
      sprintf(locbuf,"SPRINTF(&s[t-n],stab[%d],s[t-n-1]..s[t-1]),pop(s[t]+1)\n",
         ir.y);
      strcat(dl,locbuf);
      break;
   case SEND_RAWSTRING:    /* 100, send raw string DBACI only */
      sprintf(locbuf,"SEND_RAWSTRING at stab[%d] to pid s[t], pop(1)\n",ir.y);
      strcat(dl,locbuf);
      break;
   case SEND_STRING:       /* 101, send string DBACI only */
      strcat(dl,"SEND_STRING at s[s[t-1]] to pid s[t], pop(2)\n");
      break;
   case RECEIVE_OP:        /* 102, receive string or int DBACI only */
      strcat(dl,
         "RECEIVE_OP, receive string or int message into s[s[t]]\n");
      sprintf(locbuf,
         "%sstore %d at s[t] if msg was an int, %d if msg was a string\n",
         nextline_space,INT_MSG,STRING_MSG);
      strcat(dl,locbuf);
      break;
   case BROADCAST_RAWSTRING:  /* 103, broadcast raw string DBACI only */
      sprintf(locbuf,"BROADCAST_RAWSTRING at stab[%d]\n",
         ir.y);
      strcat(dl,locbuf);
      break;
   case BROADCAST_STRING:  /* 104, broadcast string DBACI only */
      strcat(dl,"BROADCAST_STRING at s[s[t]], pop(1)\n");
      break;
   case RECEIVE_ID:        /* 105, receive string or int msg and an id */
      strcat(dl,"RECEIVE_ID, receive a string or int msg into s[s[t-1]]\n");
      sprintf(locbuf,"%spop(1), store %d at s[t] if msg was an int,",
         nextline_space,INT_MSG);
      strcat(dl,locbuf);
      sprintf(locbuf," %d if msg was a string\n",STRING_MSG);
      strcat(dl,locbuf);
      break;               /* DBACI only */
   case SEND_INT:          /* 106, send an int message DBACI only */
      strcat(dl,
         "SEND_INT stored at s[t-1] to pid s[t], pop(2)\n");
      break;
   case BROADCAST_INT:     /* 107, send an int message DBACI only */
      strcat(dl,"BROADCAST_INT stored at s[t], pop(1)\n");
      break;
   case WRITE_STRING:      /* 110, write string */
      strcat(dl,"WRITE_STRING at s[s[t]] to stdout, pop(1)\n");
      break;
   case COPY_RAWSTRING:    /* 111, copy raw string */
      sprintf(locbuf,"COPY_RAWSTRING from stab[%d] to s[s[t]], pop(1)\n",ir.y);
      strcat(dl,locbuf);
      break;
   case COPY_STRING:       /* 112 copy string */
      strcat(dl,"COPY_STRING from &s[s[t]] to s[s[t-1]], pop(2)\n");
      break;
   case CONCAT_RAWSTRING:  /* 113 concat raw string */
      sprintf(locbuf,
         "CONCAT_RAWSTRING at stab[%d] to string at s[s[t]], pop(1)\n",ir.y);
      strcat(dl,locbuf);
      break;
   case CONCAT_STRING:     /* 114 concat string */
      strcat(dl,"CONCAT_STRING at s[s[t]] to string at s[s[t-1]], pop(2)\n");
      break;
   case COMPARE_RAWSTRING: /* 115 compare raw string */
      sprintf(locbuf,
         "COMPARE_RAWSTRING, s[t] = strcmp(&stab[%d],&s[s[oldt]])\n",ir.y);
      strcat(dl,locbuf);
      break;
   case COMPARE_STRING:    /* 116 compare string */
      strcat(dl,"COMPARE_STRING, pop(1), ");
      strcat(dl,"s[t] = strcmp(&s[s[oldt]],&s[s[oldt-1]])\n");
      break;
   case LENGTH_STRING:     /* 117 length of a string */
      strcat(dl,"LENGTH_STRING, s[t] = strlen(&s[s[t]]\n");
      break; 
   default:
      sprintf(locbuf,"Unknown PCODE opcode %d at lc %d\n",ir.f,lc);
      strcat(dl,locbuf);
   }  /* bottom of big, ugly switch stmt */
}  /* disassemble */

/*
 *
 *  $Id: disutils.c,v 1.17 2007/06/01 17:49:03 bynum Exp $
 *
 */

