del *.o
del libba.a
gcc -c -I..\include *.c
ar cr libba.a *.o
ranlib libba.a
gcc -o makedate makedate.c
makedate
gcc -c date.c


