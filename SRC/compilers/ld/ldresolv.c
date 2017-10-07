/*Scott Mitchell */
/*CS710 Linker Project */
/*18 August 2000*/
/*ld_resolution.c source file */
/* file to resolve name conflicts in the tables */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/bacicnty.h"
#include "../include/readtab.h"
#include "../include/writetab.h"
#include "../include/globdata.h"
#include "../include/genutil.h"
#include "../include/pcode.h"
#include "ldutils.h"
#include "ld.h"


extern int block1, block2;
extern int outer1, outer2;

/*block2 is the section of code in the code table that came from the code
 table of the 2nd file, but does not include the outer block code.  With
 some of the opcodes, the y field is dependent on the address of another
 line of code, the index of a partular function or procedure in the
 atab/btab/stab, or the position on the stack.  All of these items
 change for the block of code added from the 2nd program so new offset
 calculations must be made and the y fields updated to reflect the new
 locations so the user program calls the correct param*/

extern int newcode2_start;  /* addr in tfinal->code of start of t2->code */
   /* = block1 = t1->tab[1].adr, the addr of t1's outer block            */
extern int newcode2_stop;   /* 1 + addr in tfinal->code of end of t2->code */
   /* = block1 + block2                                                    */
extern int newouter2_start; /* addr in tfinal->code of start of t2 outer blk */
   /* = block1 + outer1 + block2 - 1 = t1->last_code + t1->tab[1].adr        */
extern int newouter2_stop; /* 1 + addr in tfinal->code of end of t2 outer blk */
   /* = newouter2_start + outer2 = block1 + outer1 + block2 + outer2 -1       */
   /* = t1->last_code + t2->last_code + 1                                     */ 

extern int last_linkfile;
extern char* linkfile[];

void fix_markstack (ALL_TABLES* tfinal, ALL_TABLES* t2, int ix, 
                     int correctmark) {
/* Search lines of t2 code from the second argument program and modify the   */
/* y field of the markstack instruction at the corresponding location of     */
/* the tfinal code table.  Since the t2 table is never modified, this avoids */
/* changing a markstack instruction that has already been fixed.             */
    int i;
    for (i=0; i <= t2->last_code; i++) {
       if ((t2->code[i].f == MARKSTACK) &&
           (t2->code[i].y == ix)) {
          tfinal->code[newcode2_start+i].y = correctmark;
       }/*end if*/
    }/*end for*/
}/*end fix_markstack*/

void fix_loadaddresses(ALL_TABLES* tfinal, ALL_TABLES* t2, int place, int ix) {
/* Search lines of t2 code from the second argument program for load instrs   */
/* and modify the y field of the corresponding load instruction in the tfinal */
/* code array.  Since the t2 table is never modified, this avoids chaning a   */
/* load instr that has already been fixed.                                    */
   int i;
   int old_adr = t2->tab[ix].adr;
   int new_adr = tfinal->tab[place].adr;
   for (i=0; i<= t2->last_code; i++) {
      if (t2->code[i].x == 0) {    /* only change level 0 addrs */
         switch (t2->code[i].f) {
            case LOAD_ADDR:
            case LOAD_VALUE:
            case LOAD_INDIR:
               if (t2->code[i].y == old_adr) {
                 tfinal->code[newcode2_start+i].y = new_adr;
               }/*end if*/
         } /* switch */
      }
   }/*end for*/
}/*end fix_loadaddresses*/

static void equal_idents(ALL_TABLES* tfinal, int place, int ifinal, int lifx, 
                     ALL_TABLES* t2, int ix, int i2, int lix2, int fixnames) {
    /*checks to see if name and type fields are the same for   */
    /*tfinal->tab[ifinal] and t2->tab[i2], parameters for the  */
    /*procs tfinal->tab[place], t2->tab[ix]                    */
    /* fixname flag is used when an extern proc is in tfinal   */
    /* and the defined proc is in t2, and the parm names may   */
    /* need to be changed                                      */ 
   char tfmon[50];
   char t2mon[50];
   int tfm = tfinal->tab[place].mon;
   int t2m = t2->tab[ix].mon;
   if (tfm) 
      sprintf(tfmon,"of monitor %s",tfinal->tab[tfm].name);
   else
      strcpy(tfmon,"");
   if (t2m) 
      sprintf(t2mon,"of monitor %s",t2->tab[t2m].name);
   else
      strcpy(t2mon,"");
   if ((tfinal->tab[ifinal].typ) != (t2->tab[i2].typ)) {
      fatal("Parameter %s of proc %s %s\nin linkfile %s has type %s\nbut parameter %s of proc %s %s\nin linkfile %s has type %s",
         tfinal->tab[ifinal].name, tfinal->tab[place].name,tfmon,
         linkfile[R[place]->head->lfix], typname[tfinal->tab[ifinal].typ],
         t2->tab[i2].name, t2->tab[ix].name,t2mon,
         linkfile[last_infile], typname[t2->tab[i2].typ]);
   }
   if (tfinal->tab[ifinal].normal != t2->tab[i2].normal) {
      fatal("Call-by-reference/call-by-value mismatch between parameter %s of proc %s %s in linkfile %s\nand parm %s of proc %s %s in linkfile %s",
         tfinal->tab[ifinal].name, tfinal->tab[place].name,tfmon,
         linkfile[R[place]->head->lfix],
         t2->tab[i2].name, t2->tab[ix].name,t2mon,
         linkfile[last_linkfile]);
   }
   if (fixnames) {
      if (strncmp(tfinal->tab[ifinal].name,t2->tab[i2].name,ALNG) != 0) {
         strncpy(tfinal->tab[ifinal].name,t2->tab[i2].name,ALNG);
         tfinal->tab[ifinal].name[ALNG-1] = '\0';
      }
   }
}/*end equal_idents*/

#define FIXNAMES     1   /* use when parm names should be replaced    */
                         /* i.e., tfinal ext_proc and t2 proc         */
#define NO_FIXNAMES  0   /* use when parm names shouldn't be replaced */
                         /* i.e., tfinal proc and t2 ext_proc         */

static void check_parameterlist (ALL_TABLES* tfinal, int place, int lixf,
      ALL_TABLES* t2, int ix, int lix2, int fixnames) {
    /*checks to see if all parameters in two functions or procedures with
      the same name are the same
      if the fixnames flag is on, then the names of the tfinal parms will
      be changed to the names of the t2 parms*/
   int ifinal, i2;
   int rfinal, r2;
   int nofinalparms, no2parms;
   rfinal = tfinal->tab[place].ref;
   ifinal = tfinal->btab[rfinal].lastpar;
   r2 = t2->tab[ix].ref;
   i2 = t2->btab[r2].lastpar;
   nofinalparms = (ifinal == place);
   no2parms = (i2 == ix);
   if (nofinalparms && !no2parms) {
      fatal("Proc %s of linkfile %s has no parameters,\nyet\nproc %s of linkfile %s has parameters",
         tfinal->tab[place].name,linkfile[lixf],
         t2->tab[ix].name,linkfile[lix2]);
   }
   else if (no2parms && !nofinalparms) {
      fatal("Proc %s of linkfile %s has no parameters,\nyet\nproc %s of linkfile %s has parameters",
         t2->tab[ix].name,linkfile[lix2],
         tfinal->tab[place].name,linkfile[lixf]);
   } 
   else if (nofinalparms && no2parms)
      return;
   while ((ifinal) && (i2)) {
      equal_idents(tfinal, place, ifinal, lixf, t2, ix, i2, lix2 ,fixnames);
      ifinal = tfinal->tab[ifinal].link;
      i2 = t2->tab[i2].link;
   }/*end while*/
   if ((ifinal) || (i2)) {
      fatal("Proc %s of linkfile %s\nand proc %s of linkfile %s\nhave different numbers of parameters",
         tfinal->tab[place].name, linkfile[lixf],
         t2->tab[ix].name, linkfile[lix2]);
   }/*end if*/
}/*end check_parameter_list*/

static void fix_tab (ALL_TABLES* tfinal, ALL_TABLES* t2, int place, int ix) {
    /*correct tfinal->tab enitries for external functions and procs*/
   tfinal->tab[place].obj = t2->tab[ix].obj;
   tfinal->tab[place].adr = t2->tab[ix].adr + block1;
}/*end fix_tab*/


/* values for the isExternal flag below */
#define IS_EXTERNAL  1
#define NOT_EXTERNAL 0

void get_locals(ALL_TABLES* tfinal, ALL_TABLES* t2, int place, int ix,
                                    int isExternal)
    /*Function to get the local variables from added functions and
      procedure's into the Identifier Table.   */

    /*Set the loop variables to start at the proc/function's last
      entry in the Block table and continue copying, following the
      link field, until either it becomes 0 (end-of-list) or it runs 
      into the lastpar entry, which should already be in the Identifier 
      table, because it was inserted there when the proc/functions were 
      identified as being the same. 
      If isExternal flag is on, then change the 'vsize'*/
{
   int i,j,n,prev;
   int loop1 = t2->btab[t2->tab[ix].ref].last;
   int loop2 = t2->btab[t2->tab[ix].ref].lastpar;
   if (loop1 == loop2) return; /* no locals */
   reset_tixstack();
   for (i=loop1, n = 0; i && (i != loop2); n++) {
      push_tix(i);
      i = t2->tab[i].link;
   }
   prev = tfinal->btab[tfinal->tab[place].ref].lastpar;
   for (i=0; i < n; i++) {
      j = pop_tix();
      prev = add_tab_entry(tfinal,&(t2->tab[j]),prev);
   }
   tfinal->btab[tfinal->tab[place].ref].last = prev;
   if (isExternal) { 
       /* if there were locals, need to make correction to vsize */
       /* because these locals haven't been seen before          */
      tfinal->btab[tfinal->tab[place].ref].vsize = 
                     t2->btab[t2->tab[ix].ref].vsize;
   }  /* if isExternal */
}  /* end get_locals*/

static void check_publicprocs(ALL_TABLES* tmaj, int maj, int lmaj,
   ALL_TABLES* tmin, int min, int lmin, int fixnames)
   /* make sure that every proc or function in tmin->tab[min]  is 
      external and appears in the monitor tmaj->tab[maj].
      lmaj and lmin are the corresponding indices of the linkfiles 
      involved */
{
   int i;
   int p;
   TAB_STRUCT* tmi;
   TAB_STRUCT* tma;
   for( i = tmin->btab[0].last; i; i = tmin->tab[i].link) {
      tmi = &(tmin->tab[i]);
      if (tmi->mon == min) { /* find a monitor proc */
         p = search_tab(tmaj,tmi->name);
         if (p == 0) {
            fatal("%s %s in monitor %s of linkfile %s\n  does not appear in monitor %s of linkfile %s",
               objname[tmi->obj],tmi->name, tmin->tab[min].name, 
               linkfile[lmin], tmaj->tab[maj].name, linkfile[lmaj]);
         }
         tma = &(tmaj->tab[p]);
         switch (tmi->obj) {
            case ext_procedure:
               if (tma->mon != maj) {
                  fatal("%s %s of linkfile %s\nis not a proc of monitor %s in linkfile %s", 
                     objname[tmi->obj],tmi->name, linkfile[lmin],
                     tmaj->tab[maj].name,linkfile[lmaj]);
               }
               else {   /* it's there, and it's in the monitor */
                  if ((tma->obj != procedure)&&(tma->obj != ext_procedure)){
                     fatal("%s %s of monitor %s in linkfile %s\ndoesn't match %s %s of monitor %s in linkfile %s",
                        objname[tmi->obj],tmi->name,
                        tmin->tab[min].name,linkfile[lmin],
                        objname[tma->obj],tma->name,
                        tmaj->tab[maj].name,linkfile[lmaj]);
                  }
                  else
                     check_parameterlist(tmaj,p,lmaj,tmin,i,lmin,fixnames);   
               }
               break;
            case ext_function:
               tma = &(tmaj->tab[p]);
               if (tma->mon != maj) {
                  fatal("%s %s of linkfile %s\nis not a proc of monitor %s in the linkfile %s", 
                     objname[tmi->obj],tmi->name, linkfile[lmin],
                     tmaj->tab[maj].name,linkfile[lmaj]);
               }
               else {   /* it's there, and it's in the monitor */
                  if ((tma->obj != function)&&(tma->obj != ext_function)){
                     fatal("%s %s of monitor %s in linkfile %s\ndoesn't match %s %s of monitor %s in linkfile %s",
                        objname[tmi->obj],tmi->name,
                        tmin->tab[min].name,linkfile[lmin],
                        objname[tma->obj],tma->name,
                        tmaj->tab[maj].name,linkfile[lmaj]);
                  }
                  else
                     check_parameterlist(tmaj,p,lmaj,tmin,i,lmin,fixnames);   
               }
               break;
            case procedure:   /* the prcs of the minor monitor may have alredy */
            case function:    /* been incorporated into the symbol table */ 
               /* nothing */;
               break;
            default:
               if (tma->obj != tmi->obj) 
                  fatal("Duplicate ID: %s %s in monitor %s of linkfile %s\n  clashes with %s %s of %s",
                     tmi->obj,tmi->name,tmin->tab[min].name,linkfile[lmin],
                     tma->obj,tma->name,linkfile[lmaj]);
         } /* switch */
      }  /* if ti->mon == min */
   }  /* for i */
}  /* check_publicprocs */

void update_monfields(ALL_TABLES* tfinal, ALL_TABLES* t2)
/* check tfinal->tab for entries with a negative mon field 
   When found, look for the monitor from t2->tab in 
   tfinal->tab.  If found, fix the negative mon field,
   else die */
{
   int i, t2mix, tfinalmix;
   for(i = tfinal->btab[0].last; i; i = tfinal->tab[i].link) {
      t2mix = tfinal->tab[i].mon;
      if (t2mix < 0) { /* this mon field need fixing !*/
         tfinalmix = search_tab(tfinal,t2->tab[-t2mix].name);
         if (tfinalmix > 0) 
            tfinal->tab[i].mon = tfinalmix;
         else
            fatal("Monitor %s was not found in\n%s",t2->tab[-t2mix].name,
               linkfile[last_linkfile]);
      }  /* if t2mix */
   }  /* for */
}  /* update_monfields */

void check_types(ALL_TABLES* tfinal, ALL_TABLES* t2, int place, int ix)
   /* compare types of tfinal->tab[place] and t2->tab[ix] and die if */
   /* different */
{
   TAB_STRUCT* taf = &(tfinal->tab[place]);
   TAB_STRUCT* ta2 = &(t2->tab[ix]);
   if (taf->typ != ta2->typ) {
      fatal("%s %s in linkfile %s has type %s\nbut %s %s in linkfile %s has type %s",
         objname[taf->obj],taf->name,linkfile[R[place]->head->lfix],
         typname[taf->typ],
         objname[ta2->obj],ta2->name,linkfile[last_linkfile],
         typname[ta2->typ]);
   }
} /* end check_types */

void tab_name_resolution(ALL_TABLES* tfinal, int place, ALL_TABLES* t2, int ix,
   int lix2) 
{
   /*resolves name conflicts and corrects code_table y field */
   /*entries for variables and procedures in the tab table   */
   TAB_STRUCT* taf;
   TAB_STRUCT* ta2;
   int lixf = R[place]->head->lfix;
   taf = &(tfinal->tab[place]);
   ta2 = &(t2->tab[ix]);
   addRef(place,ta2->obj,lix2);  /* note reference to symbol */

   if(taf->obj == ta2->obj) {
        /*allowable cases of same names and same obj fields */
        /*ext_procedure, ext_function, ext_variable or ext_monitor*/
        /*first check for ext_function, ext_variable, or ext_monitor*/
      switch (taf->obj) {
         case ext_function:
            check_types(tfinal,t2,place,ix);      /*ensure types are the same*/
            fix_markstack(tfinal, t2, ix, place); /*correct MARKSTACK instrs */
            break;
         case ext_procedure:
            /* types will both be notyp, no need to check types */
            fix_markstack(tfinal, t2, ix, place); /*correct MARKSTACK instrs */
            break;
         case ext_monitor:
            /* make sure that every proc in t2->tab[ix] 
                         also appears in tfinal->tab[place]*/
            check_publicprocs(tfinal,place,lixf,t2,ix,lix2,NO_FIXNAMES);
            /* make sure that every proc in tfinal->tab[place] 
                         also appears in t2->tab[ix]*/
            check_publicprocs(t2,ix,lix2, tfinal,place,lixf,NO_FIXNAMES);
            fix_markstack(tfinal, t2, ix, place); /*correct MARKSTACK instrs */
            break;
         case ext_variable:
            check_types(tfinal,t2,place,ix);      /*ensure types are the same*/
            if (taf->typ == arrays) 
               check_atab_dimensions(tfinal, t2, place, ix);
            fix_loadaddresses(tfinal, t2, place, ix); /* fix load addr instrs */
            break;
         default: ; 
            /* ignore types and constants -- they don't affect the link */
      } /* switch taf->obj */
   }/*end if same object*/

        /*allowable cases of same name and different obj fields*/
        /*tfinal is ext_fuction and t2 is function             */
        /*tfinal is function and t2 is ext_function            */
        /*tfinal is ext_procedure and t2 is procedure          */
        /*tfinal is procedure and t2 is ext_procedure          */
        /*tfinal is ext_variable and t2 is variable            */
        /*tfinal is variable and t2 is ext_variable            */
        /*tfinal is ext_monitor and t2 is monitor              */
        /*tfinal is monitor and t2 is ext_monitor              */
   
   if ((taf->obj == ext_function) && (ta2->obj == function)) {
            /*case tfinal is ext_function and t2 is function*/
            /* check param list to ensure match*/
       check_types(tfinal,t2,place,ix);      /*ensure types are the same*/
       check_parameterlist(tfinal, place,lixf,t2, ix,lix2,FIXNAMES);
       get_locals(tfinal, t2, place, ix,IS_EXTERNAL);
       fix_markstack(tfinal, t2, ix, place);
                /*fix object and address fields for entries in the
               tab table from file 1 that were externs*/
       fix_tab(tfinal, t2, place, ix);
       return;
   }/*end case tfinal = ext_function t2 = function*/
   else if ((taf->obj == function) && (ta2->obj == ext_function)) {
            /*case tfinal is function and t2 is ext_function*/
            /*check types, check param list, to ensure match*/
      check_types(tfinal,t2,place,ix);      /*ensure types are the same*/
      check_parameterlist(tfinal, place,lixf,t2,ix,lix2,NO_FIXNAMES);
      fix_markstack(tfinal, t2, ix, place);
      return;
   }/*end case tfinal = function t2 = ext_function*/
   else if ((taf->obj == ext_procedure) && (ta2->obj == procedure)) {
           /*case tfinal is ext_procedure and t2 is procedure*/
           /* check param list, to ensure match*/
      check_parameterlist(tfinal, place,lixf,t2,ix,lix2,FIXNAMES);
      taf->obj = ta2->obj;
      get_locals(tfinal, t2, place, ix,IS_EXTERNAL);
          /*fix MARKSTACK y field opcode instructions in final code table*/
      fix_markstack(tfinal, t2, ix, place);
          /*fix object and address fields for entries in the
            tab table from file 1 that were externs*/
      fix_tab(tfinal, t2, place, ix);
      return;
   }/*end case tfinal = ext_procedure t2 = procedure*/
   else if ((taf->obj == procedure) && (ta2->obj == ext_procedure)) {
        /*case tfinal is procedure and t2 is ext_procedure*/
        /*check types, check param list, to ensure match*/
      check_parameterlist(tfinal, place,lixf,t2,ix,lix2,NO_FIXNAMES);
         /*fix MARKSTACK y field opcode instructions in final code table*/
      fix_markstack(tfinal, t2, ix, place);
      return;
   }/*end case tfinal = procedure t2 = ext_procedure*/
   else if ((taf->obj == ext_variable) && (ta2->obj == variable)) {
        /*case tfinal is ext_variable and t2 is variable*/
      check_types(tfinal,t2,place,ix);      /*ensure types are the same*/
      taf->obj = ta2->obj;
         /*fix y field of load address opcode instructions in final
           code table*/
      fix_loadaddresses(tfinal, t2, place, ix);
      if (taf->typ == arrays) {
         check_atab_dimensions(tfinal,t2,place,ix);
            /*fix y field of INDEX instructionsin final code table*/
         fix_atabindex(tfinal, t2, place, ix);
      }
      return;
   }/*end case tfinal = ext_variable t2 = variable*/
   else if ((taf->obj == variable) && (ta2->obj == ext_variable)) {
         /*case tfinal is variable and t2 is ext_variable*/
      check_types(tfinal,t2,place,ix);           /*ensure types are the same*/
      fix_loadaddresses(tfinal, t2, place, ix);  /* fix load addr instrs */
      if (taf->typ == arrays) {
         check_atab_dimensions(tfinal,t2,place,ix);
            /*fix y field of INDEX instructionsin final code table*/
         fix_atabindex(tfinal, t2, place, ix);
      }
      return;
   }/*end case tfinal = variable t2 = ext_variable*/
   else if ((taf->obj == ext_monitor) && (ta2->obj == monitor)) {
        /* make sure that every proc in t2->tab[ix]
                   also appears in tfinal->tab[place] */ 
      check_publicprocs(tfinal,place,lixf,t2,ix,lix2, NO_FIXNAMES);
        /* make sure that every proc in tfinal->tab[place]
                   also appears in t2->tab[ix] */ 
      check_publicprocs(t2,ix,lix2,tfinal,place,lixf,FIXNAMES);
         /*case tfinal is ext_monitor and t2 is monitor*/
      append_monitor(tfinal, t2, place, ix);
          /*correct object and address fields of tab entry*/
      fix_tab(tfinal, t2, place, ix);
      return;
   }/*end case tfinal = ext_monitor t2 = monitor*/
   else if ((taf->obj == monitor) && (ta2->obj == ext_monitor)) {
         /*case tfinal is monitor and t2 is ext_monitor*/
      /* make sure that every proc in t2->tab[ix]
                   also appears in tfinal->tab[place]*/
      check_publicprocs(tfinal,place, lixf, t2,ix,lix2, NO_FIXNAMES);
      /* make sure that every proc in tfinal->tab[place]
                   also appears in t2->tab[ix]*/
      check_publicprocs( t2,ix,lix2, tfinal,place, lixf, NO_FIXNAMES);
         /*case tfinal is ext_monitor and t2 is monitor*/
      return;
   }/*end case tfinal = monitor t2 = ext_monitor*/
}/*end tab_name_resolution*/

void fix_stringcodeaddresses(ALL_TABLES* tfinal, int tfinalstringsize)
/*Correct y field values in finaltable->code for opcodes WRITE_RAWSTRING,*/
/*SEND_RAWSTRING, BROADCAST_RAWSTRING, COPY_RAWSTRING, CONCAT_RAWSTRING,*/
/*and COMPARE_RAWSTRING */
{
   int i;
   int j;
   for (i=newcode2_start; i<newcode2_stop; i++) {
       switch (tfinal->code[i].f) {
          case WRITE_RAWSTRING:
          case SEND_RAWSTRING:
          case BROADCAST_RAWSTRING:
          case COPY_RAWSTRING:
          case CONCAT_RAWSTRING:
            tfinal->code[i].y += tfinalstringsize;
       }/*switch */
   }/*end for*/
   for (j=newouter2_start; j<newouter2_stop; j++) {
       switch (tfinal->code[j].f) {
          case WRITE_RAWSTRING:
          case SEND_RAWSTRING:
          case BROADCAST_RAWSTRING:
          case COPY_RAWSTRING:
          case CONCAT_RAWSTRING:
            tfinal->code[j].y += tfinalstringsize;
       } /*switch */
   } /*end for*/
} /*end fix_stringcodeaddresses*/


/*
 *
 *   $Id: ldresolv.c,v 2.14 2007/06/01 20:07:44 bynum Exp $
 *
 */
