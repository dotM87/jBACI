#  Makefile for the BACI system linker
#  Scott Mitchell
#  CS 710 Project
#  3 August  2000

CC = gcc
#CC = cc
CCFLAGS = -g -Wall
EXENAME = bald

HFILES =  ld.h ldutils.h ldresolv.h
CFILES = ldutils.c ldresolv.c ldmain.c
OBJS = ldmain.o ldutils.o ldresolv.o date.o ../lib/globtabs.o
BINDIR = $(HOME)/bin/i586

$(EXENAME): $(OBJS)
	$(CC) $(CCFLAGS) -o $(EXENAME) $(OBJS) -L../lib -lba -lba -L../ar \
		-lbaar -lbaar
	cp -f $(EXENAME) $(BINDIR)
	rm -f date.o

ldmain.o: ldmain.c $(HFILES)
	$(CC) -c $(CCFLAGS) ldmain.c

ldutils.o: ldutils.c ldresolv.h ld.h
	$(CC) -c $(CCFLAGS) ldutils.c

ldresolv.o: ldresolv.c  ldutils.h ld.h
	$(CC) -c $(CCFLAGS) ldresolv.c

date.o:
	rm -rf date.c date.o
	../lib/makedate
	$(CC) $(FLAGS) -c date.c

tar: $(HFILES) $(CFILES) Makefile
	tar fcv ld_all.tar ld*.c ld*.h Makefile
	gzip -f ld_all.tar
	uuencode ld_all.tar.gz ld_all.tar.gz > ld_all.uue

clean:
	rm -f $(EXENAME) $(OBJS)

#
#     $Id: Makefile,v 1.2 2007/05/31 22:02:10 bynum Exp $
#
