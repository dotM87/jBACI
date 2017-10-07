del *.o
copy ..\lib\date.o
flex bac.l
move lex.yy.c lex.c
bison -tvd bac.y
move bac.tab.c gram.c
move bac.tab.h ytab.h
gcc -c -I..\include *.c
gcc -o bacc *.o -L..\lib -lba


