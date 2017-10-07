del *.o
copy ..\lib\date.o
flex bapascal.l
move lex.yy.c lex.c
bison -tvd bapascal.y
move bapascal.tab.c gram.c
move bapascal.tab.h ytab.h
gcc -c -I..\include *.c
gcc -o bapas *.o -L..\lib -lba


