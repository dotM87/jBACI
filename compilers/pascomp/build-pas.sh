rm *.o
cp ../lib/date.o .
flex bapascal.l
mv lex.yy.c lex.c
bison -tvd bapascal.y
mv bapascal.tab.c gram.c
mv bapascal.tab.h ytab.h
gcc -c -I../include *.c
gcc -o bapas *.o -L../lib -lba
rm *.o gram.c lex.c ytab.h  bapascal.output
mv bapas ../../bin

