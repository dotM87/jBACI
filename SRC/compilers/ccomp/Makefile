#  Makefile for BenAri Concurrent C Compiler in C
#    adapted from the makefile for the BenAri Concurrent Pascal compiler in C

OBJS = baccutil.o date.o lex.o gram.o 
CFLAGS = -g -Wall 
#CFLAGS = -g
# pick which one of the following two lines you need
#YACC=yacc
YACC=bison -y
#CC = gcc296
CC = gcc
#CC = cc
BINDIR = $(HOME)/bin/i586
INCDIR = ../include
INCLS =$(INCDIR)/bacicnty.h $(INCDIR)/compdata.h $(INCDIR)/pcode.h \
   $(INCDIR)/incfiles.h

LIBDIR = ../lib

checkOS:
	$(LIBDIR)/checkLastOS '*.o'
	make bacc

bacc: $(INCLS) $(OBJS)
	(cd $(LIBDIR); make)
	$(CC) $(CFLAGS) -o bacc $(OBJS) -L$(LIBDIR) -lba  -lba 
	cp -f bacc $(BINDIR)
	rm date.o

date.o:
	rm -f date.c
	(cd $(LIBDIR); make makedate)
	$(LIBDIR)/makedate
	$(CC) $(CFLAGS) -c date.c

gram.o ytab.h:  bac.y $(INCLS)
	$(YACC) -tvd bac.y
	mv -f y.tab.c gram.c
	mv -f y.tab.h ytab.h
	chmod 444 gram.c ytab.h
	$(CC) $(CFLAGS) -c gram.c

lex.o:	bac.l $(INCLS) ytab.h unistd.sed
	rm -f lex.c
	flex bac.l
	sed -f unistd.sed lex.yy.c > lex.c
	chmod 444 lex.c
	$(CC) $(CFLAGS) -c lex.c

baccutil.o:  baccutil.c $(INCLS)
	$(CC) $(CFLAGS) -c baccutil.c

clean:
	rm -f *.o bacc gram.c lex.c ytab.h



#
#     $Id: Makefile,v 1.9 2007/05/31 21:09:24 bynum Exp $
#
