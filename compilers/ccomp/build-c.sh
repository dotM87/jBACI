rm *.o
flex bac.l
move lex.yy.c lex.c
bison -tvd bac.y
mv bac.tab.c gram.c
mv bac.tab.h ytab.h
gcc -c -I../include *.c
gcc -o bacc *.o -L../lib -lba
rm *.o gram.c lex.yy.c ytab.h  bac.output
mv bacc ../../bin





