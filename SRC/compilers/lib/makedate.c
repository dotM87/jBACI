/* 
 *
 *  makes a date.c file containing the date_string declaration:
 *  
 *   \* date string file (program-generated -- don't tamper!) *\
 *   char date_string[] = { "06:57  30 Jan 1996" };
 *  
 *  created for the BACI system, 1/29/96, Bill Bynum
 * 
 */


#include <time.h>
#include <stdio.h>

extern void exit(int);
extern int printf(const char*,...);
extern int fprintf(FILE*,const char*,...);
extern int fclose(FILE*);


int main(int argc, char** argv)
{
char *ctime();
time_t theTime,time();
char buf[80];
char *t;
FILE* f;
   theTime = time((time_t *) 0);
   t = ctime(&theTime);
   sprintf(buf,"%.5s  %.2s %.3s %.4s",t+11,t+8,t+4,t+20);
	f = fopen("date.c","w");
	if (f == NULL) {
		printf("%s: can't open date.c\n",argv[0]);
		exit(1);
	}
	fprintf(f,"/* date string file (program-generated -- don't tamper!) */\n");
	fprintf(f,"char date_string[] = { \"%s\" };\n",buf); 
	fclose(f);
   return(0);
}

/*
 *
 *  $Id: makedate.c,v 1.4 2007/06/01 17:49:03 bynum Exp $
 *
 */
