#  
#  Make file for the BACI archiver 
#     Bill Bynum, May 2001
#

CC = gcc
#CC = cc
CCFLAGS = -g -Wall
OBJ = armain.o date.o ../lib/globtabs.o
LIB1OBJ = ardelete.o arextrac.o arrdsyms.o \
   arreplac.o artables.o arutils.o arwrite.o
SRC = armain.c ardelete.c arextrac.c arrdsyms.c arreplac.c \
   artables.c arutils.c arwrite.c
HDR = ardelete.h arextrac.h arrdsyms.h arreplac.h \
   artables.h arutils.h arwrite.h
LIB1DIR = .
LIB1   = libbaar.a
LIB2DIR = ../lib
LIB2   = libba.a
LFLAG = -lbaar -lba -lba
BINDIR = $(HOME)/bin/i586

checkOS:
	$(LIB2DIR)/checkLastOS '*.o'
	make baar

baar: $(OBJ) $(LIB1)
	(cd $(LIB2DIR); make)
	$(CC) $(CCFLAGS) -o baar $(OBJ) -L$(LIB1DIR) -L$(LIB2DIR) $(LFLAG)
	cp -f baar $(BINDIR)
	rm date.o

armain.o: armain.c arutils.h artables.h arextrac.h ardelete.h
	$(CC) $(CCFLAGS) -c armain.c

testextract:  testextract.c $(LIB1)
	$(CC) $(CCFLAGS) -o testextract testextract.c \
      -L$(LIB1DIR) -L$(LIB2DIR) $(LFLAG)
 
date.o:
	rm -f date.c
	(cd $(LIB2DIR); make makedate)
	$(LIB2DIR)/makedate
	$(CC) $(CFLAGS) -c date.c

$(LIB1): $(LIB1OBJ)
	ar crv $(LIB1) $(LIB1OBJ)
	ranlib $(LIB1)

ardelete.o: ardelete.c arutils.h artables.h ardelete.h 
	$(CC) $(CCFLAGS) -c ardelete.c

arextrac.o: arextrac.c arutils.h artables.h arextrac.h
	$(CC) $(CCFLAGS) -c arextrac.c

arrdsyms.o: arrdsyms.c artables.h arutils.h
	$(CC) $(CCFLAGS) -c arrdsyms.c

arreplac.o: arreplac.c arrdsyms.h
	$(CC) $(CCFLAGS) -c arreplac.c

arutils.o: arutils.c arutils.h
	$(CC) $(CCFLAGS) -c arutils.c

artables.o: artables.c artables.h ar.h arutils.h 
	$(CC) $(CCFLAGS) -c artables.c

arwrite.o: arwrite.c artables.h arutils.h ar.h
	$(CC) $(CCFLAGS) -c arwrite.c

ar.zip: $(SRC) $(HDR)
	rm -rf ar.zip
	zip ar.zip ar*.c ar*.h *.dos

clean:
	rm -f *.o baar libbaar.a

#
#  $Id: Makefile,v 1.4 2007/05/31 21:55:14 bynum Exp $
#
