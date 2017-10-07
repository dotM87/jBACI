   /* 
      BACI disassembly utilities
      used by the disassembler and interpreter
   */

extern CODEBLOCK cbtab[];
   /* code block table, used to mark proc boundaries */

extern int last_cbtab;
   /* size of the code block table */

extern int build_cbtab();
   /* Build the code block table   */

extern int search_var(int tabix, int level, int offset);
   /* search for a variable at 'level' & 'offset' starting at the */
   /* proc whose tab[] index is 'tabix'.  Continue the search     */
   /* outward through every block in the code block table at a    */
   /* lower level than the tab[tabix] proc                        */
   /* return the tab[] index of the variable, if found, else 0    */
   /* When 'level' == 0, this is the return value of a function.  */
   /* In this case, return the tabix of the function              */

extern void disassemble(char *dis_line, int tix, int lc);
   /* disassemble the instruction at code[lc] into the buffer 'dis_line */
   /* 'tix' is the tab index of the current block                       */
   /* used by search_var for searching the symbol table                 */


/*
 *
 * $Id: disutils.h,v 1.4 2007/06/01 17:41:16 bynum Exp $
 *
 */
