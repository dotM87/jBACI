#  Makefile for the BenAri library,  libba.a

INCDIR = ../include
LIBNAME = libba.a

LIBOBJS = compdata.o globdata.o globtabs.o incfiles.o genutil.o writetab.o\
            computil.o readtab.o disutils.o

CC = gcc
#CC = cc

CFLAGS = -g


all: checkos makedate $(LIBNAME)

# checkLastOS checks to see if the last make was for the current OS
# If so, then no files are deleted and the other two make targest are tested; 
# otherwise, *.o libba.a and makedate are deleted, then testing the other
# two targets remakes everything
#
checkos:
	@./checkLastOS '*.o libba.a makedate'

makedate:  makedate.c
	$(CC) $(CFLAGS) -o makedate makedate.c

$(LIBNAME): $(LIBOBJS)
	ar cr $(LIBNAME) $(LIBOBJS)
	ranlib $(LIBNAME)

compdata.o:  compdata.c $(INCDIR)/bacicnty.h
	$(CC) $(CFLAGS) -c -I$(INCDIR) compdata.c
   
computil.o:  computil.c $(INCDIR)/bacicnty.h $(INCDIR)/globdata.h \
   $(INCDIR)/globtabs.h $(INCDIR)/compdata.h $(INCDIR)/incfiles.h \
   $(INCDIR)/pcode.h
	$(CC) $(CFLAGS) -c -I$(INCDIR) computil.c

disutils.o: disutils.c $(INCDIR)/bacicnty.h
	$(CC) $(CFLAGS) -c -I$(INCDIR) disutils.c

genutil.o:  genutil.c
	$(CC) $(CFLAGS) -c -I$(INCDIR) genutil.c

globdata.o:  globdata.c $(INCDIR)/bacicnty.h
	$(CC) $(CFLAGS) -c -I$(INCDIR) globdata.c
   
globtabs.o:  globtabs.c $(INCDIR)/bacicnty.h
	$(CC) $(CFLAGS) -c -I$(INCDIR) globtabs.c
   
incfiles.o:  incfiles.c  $(INCDIR)/bacicnty.h $(INCDIR)/genutil.h
	$(CC) $(CFLAGS) -c -I$(INCDIR) incfiles.c

readtab.o:  readtab.c $(INCDIR)/genutil.h $(INCDIR)/globdata.h
	$(CC) $(CFLAGS) -c -I$(INCDIR) readtab.c

writetab.o:  writetab.c $(INCDIR)/globdata.h
	$(CC) $(CFLAGS) -c -I$(INCDIR) writetab.c

clean:
	rm -f *.o $(LIBNAME) makedate

# 
#
#  $Id: Makefile,v 1.8 2007/06/01 18:32:33 bynum Exp $
#
#
