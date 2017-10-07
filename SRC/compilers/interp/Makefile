# makefile for BenAri Concurrent Pascal PCODE Interpreter in C
#
##  Particularize to your Tcl, Tk installation
##  For Suse
LTCL = -ltcl8.4
LTK = -ltk8.4
TCLINCDIR = $(HOME)/tcl8.4.10/generic 
TKINCDIR  = $(HOME)/tk8.4.10/generic
## For RedHat workstation 3
#LTCL = -ltcl8.3
#LTK = -ltk8.3
#TCLINCDIR = /usr/local/tcl/generic
#TKINCDIR  = /usr/local/tk/generic
## For Cygwin
#LTCL = -ltcl8.4
#LTK = -ltk8.4
#TCLINCDIR = /usr/include/
#TKINCDIR  = /usr/include
#
#
OBJS=main.o bainterp.o baiutils.o random.o date.o ../lib/globtabs.o
INCDIR = ../include
INCLS=$(INCDIR)/globdata.h $(INCDIR)/bacicnty.h \
	   $(INCDIR)/disutils.h $(INCDIR)/genutil.h $(INCDIR)/readtab.h random.h 
CC=gcc
CFLAGS=-Wall -g $(DEFINES) -I$(INCDIR)
#CC=cc
#CFLAGS= -Wall -g
#CFLAGS=-g $(DEFINES) -I$(INCDIR)
LIBDIR= ../lib
BINDIR= $(HOME)/bin/i586
UNAMECMD=`uname -s | sed -e 's/CYGWIN_.*/CYGWIN/'`

which:
	@echo "What do you want to make for `uname -s`?"
	@echo "     gui             or"
	@echo "     std"
	@echo ""

gui:
	make cleanobj
	make "OS=$(UNAMECMD)" whichgui

whichgui:
	$(LIBDIR)/checkLastOS '*.o'
	make $(OS)gui

CYGWINgui:
	make "DEFINES=-DLinux -DTCL" "LLIBS=$(LTCL) $(LTK) -lX11 -lm" \
      "INCDIRS= -I$(INCDIR) -I$(TCLINCDIR) -I$(TKINCDIR)" \
      "LDFLAGS= -L/usr/lib -L/usr/local/lib -L/usr/X11R6/lib" \
      guibainterp

Darwingui:
	make "DEFINES=-DMACOSX -DTCL" "LLIBS=$(LTCL) $(LTK) -lX11 -ldl -lm" \
      "INCDIRS= -I$(INCDIR) -I$(TCLINCDIR) -I$(TKINCDIR)" \
      "LDFLAGS= -L/usr/lib -L/usr/local/lib -L/usr/X11R6/lib" \
      guibainterp

Linuxgui:
	make "DEFINES=-DLinux -DTCL" "LLIBS=$(LTCL) $(LTK) -lX11 -lm" \
      "INCDIRS= -I$(INCDIR) -I$(TCLINCDIR) -I$(TKINCDIR)" \
      "LDFLAGS= -L/usr/lib -L/usr/local/lib -L/usr/X11R6/lib" \
      guibainterp

guibainterp: $(OBJS) $(INCLS)
	(cd $(LIBDIR); make)
	$(CC) $(CFLAGS) -o bagui $(OBJS) -L$(LIBDIR) $(LDFLAGS) -lba -lba $(LLIBS)
	cp -f bagui $(BINDIR)
	rm date.o

# to track down link problems change the above CC line to 
#  $(CC) $(CFLAGS) -t -o guibainterp $(OBJS) $(LDFLAGS) -L$(LIBDIR) -lba $(LLIBS)

std:
	make cleanobj
	make "OS=`uname -s`" whichstd

whichstd:
	$(LIBDIR)/checkLastOS '*.o'
	make "DEFINES=-D$(OS)" "INCDIRS=-I../$(INCDIR)" stdbainterp

stdbainterp:  $(OBJS) $(INCLS)
	(cd $(LIBDIR); make)
	$(CC) $(CFLAGS) $(INCDIRS) -o bainterp $(OBJS) -L$(LIBDIR) -lba -lba
	cp -f bainterp $(BINDIR)
	rm date.o

bainterp.o:  bainterp.c $(INCLS)
	$(CC) -c $(CFLAGS) $(INCDIRS) bainterp.c

baiutils.o:  baiutils.c $(INCLS)
	$(CC) -c $(CFLAGS) $(INCDIRS) baiutils.c

date.o:
	rm -f date.c
	(cd $(LIBDIR); make)
	$(LIBDIR)/makedate
	$(CC) -c $(CFLAGS) date.c

main.o:  main.c $(INCLS)
	$(CC) -c $(CFLAGS) $(INCDIRS) main.c

random.o: random.c
	$(CC) -c $(CFLAGS) random.c

clean:
	rm -f main.o bainterp.o baiutils.o random.o bainterp ../lib/makedate

cleanobj:
	rm -f main.o bainterp.o baiutils.o random.o ../lib/makedate 

# 
#   $Id: Makefile,v 2.2 2007/06/02 14:06:17 bynum Exp $
#


