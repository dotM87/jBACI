 /* BenAri Concurrent PCODE disassembler  main program */
 /*   
   disassembles a PCODE file, showing source lines, if possible 
   Bill Bynum
   June 1997
 */

#include <stdio.h>
#include <string.h>

#include "../include/globdata.h"
#include "../include/globtabs.h"
#include "../include/disutils.h"

char dis_line[256];

extern int fprintf(FILE*,const char*,...);
extern int fputs(const char*, FILE*);
extern int fclose(FILE*);

extern FNAME_STRING prog_name;

extern FNAME_STRING   disasm_fname;
extern FILE*          disasm;

extern int source_wanted;

extern void global_init(int argc, char** argv);

extern void show_source(FILE*, int,int*,int*,int*);

extern void disassemble(char *dis_line, int tix, int lc);

int main(int argc, char* argv[])
{
   int nextblock_lc = 0;
   int cur_block = -1;
   int cur_ifile = 0;
   int dbix = 0;
   int showing_source;

   global_init(argc,argv);
   showing_source = source_wanted;
   while  (lc <= last_code) {
      if (dbg[dbix].lc <= lc)
         show_source(disasm,lc,&cur_ifile,&showing_source,&dbix);
      if (lc >= nextblock_lc) {
         cur_block++;
         if (cur_block == last_cbtab)
            nextblock_lc = last_code+1;
         else
            nextblock_lc = cbtab[cur_block + 1].adr;
         if ((!source_wanted)||(!showing_source)||
             (tab[cbtab[cur_block].tix].obj == outerblock))
            fprintf(disasm,"\n%s:\n",tab[cbtab[cur_block].tix].name);
      }
      disassemble(dis_line,cbtab[cur_block].tix,lc);
      fputs(dis_line,disasm);
      lc++;
   }
   fclose(disasm);
   fprintf(stderr,"Disassembly stored in %s\n",disasm_fname);
   return 0;
}  /* main */

/*
 *
 *  $Id: main.c,v 1.9 2007/06/01 18:51:13 bynum Exp $
 *
 */
