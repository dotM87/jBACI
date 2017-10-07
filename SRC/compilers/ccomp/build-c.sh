rm *.o
cp ../lib/date.o
flex bac.l
mv lex.yy.c lex.c
bison -tvd bac.y
mv bac.tab.c gram.c
mv bac.tab.h ytab.h
gcc -c -I../include *.c
gcc -o bacc *.o -L../lib -lba


