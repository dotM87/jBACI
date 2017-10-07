   /* global data used by the BenAri PCODE compilers & interpreter */

#include "../include/bacicnty.h"    /* bring in constants & types */

FNAME_STRING pcode_fname;

char  comp_proot[]      = {"BACI System: "};

char  pcode_suffix[]    = {".pco"};

char  pobject_suffix[]  = {".pob"};

                  /*         123456789012  */
char  outerblock_name[] = { "++-outer-++" };
   /* used with -c option when there is no main() proc */

TYPENAME typenames[]    = {"notyp","int","bool","char","bsem",
                           "sem","cond","array","","","rawstr","string"};

OBJNAME objnames[]      = {"constant","variable","type","procedure",
                           "function","monitor"};

FNAME_STRING filename_line;


/*
 *
 *  $Id: globdata.c,v 1.12 2007/06/01 17:49:03 bynum Exp $
 *
 */
