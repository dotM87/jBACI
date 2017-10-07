#  Makefile for BenAri Concurrent Pascal Compiler 

OBJS = bacutils.o date.o gram.o lex.o ../lib/readtab.o ../lib/globtabs.o
CFLAGS = -g -Wall
#CFLAGS = -g -Wall -DMACOSX
#CC = gcc296
CC = gcc
#CC = cc
YACC=bison -y
BINDIR = $(HOME)/bin/i586
INCDIR = ../include
INCLS =$(INCDIR)/bacicnty.h $(INCDIR)/compdata.h $(INCDIR)/pcode.h \
   $(INCDIR)/incfiles.h
LIBDIR = ../lib

checkOS:
	$(LIBDIR)/checkLastOS '*.o'
	make bapas

bapas: $(INCLS) $(OBJS)
	(cd $(LIBDIR); make)
	$(CC) $(CFLAGS) -o bapas $(OBJS) -L$(LIBDIR) -lba -lba
	cp -f bapas $(BINDIR)
	rm date.o

date.o:
	rm -f date.c      # make sure old date.c is gone
	(cd $(LIBDIR); make makedate)
	$(LIBDIR)/makedate
	$(CC) $(CFLAGS) -c date.c

gram.o ytab.h:  bapascal.y $(INCLS)
	$(YACC) -tvd bapascal.y
	mv -f y.tab.c gram.c
	mv -f y.tab.h ytab.h
	chmod 444 gram.c ytab.h
	$(CC) $(CFLAGS) -c gram.c

lex.o:	bapascal.l $(INCLS) ytab.h
	rm -f lex.c
	flex bapascal.l
	sed -f unistd.sed lex.yy.c > lex.c
	chmod 444 lex.c
	$(CC) $(CFLAGS) -c lex.c

bacutils.o: bacutils.c $(INCLS)
	$(CC) $(CFLAGS) -c bacutils.c

clean:
	rm -f gram.o lex.o bapas bacutils.o gram.c ytab.h lex.c


#
#   $Id: Makefile,v 1.11 2007/05/31 21:13:02 bynum Exp $
#
