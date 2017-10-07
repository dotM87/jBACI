# makefile for BenAri Concurrent Pascal PCODE disassembler in C
#
OBJS=badautil.o main.o date.o ../lib/globtabs.o
INCDIR = ../include
INCLS=$(INCDIR)/globdata.h $(INCDIR)/bacicnty.h \
	   $(INCDIR)/genutil.h $(INCDIR)/readtab.h $(INCDIR)/disutils.h

CC=gcc
#CC=cc
CFLAGS=-g -Wall
LIBDIR= ../lib
BINDIR = $(HOME)/bin/i586

checkOS:
	$(LIBDIR)/checkLastOS '*.o'
	make badis

badis: $(OBJS) $(INCLS)
	(cd $(LIBDIR); make)
	$(CC) $(CFLAGS) -o badis $(OBJS) -L$(LIBDIR) -lba -lba
	cp -f badis $(BINDIR)
	rm date.o

badautil.o:  badautil.c $(INCLS)
	$(CC) -c $(CFLAGS) badautil.c

main.o:  main.c $(INCDIR)/globdata.h $(INCDIR)/pcode.h $(INCDIR)/disutils.h
	$(CC) -c $(CFLAGS) main.c

date.o:
	rm -f date.c
	(cd $(LIBDIR); make makedate)
	../lib/makedate
	$(CC) -c $(CFLAGS) date.c

clean:
	rm -f $(OBJS) badis

# 
#   $Id: Makefile,v 1.4 2007/05/31 21:14:54 bynum Exp $
#
