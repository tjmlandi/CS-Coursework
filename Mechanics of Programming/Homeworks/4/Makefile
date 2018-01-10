#
# Created by gmakemake (Ubuntu Jul 25 2014) on Wed Oct  8 17:32:08 2014
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CC =		gcc
CFLAGS =	-Wall -Wextra -pedantic -ggdb -std=c99

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	mtime.c song.c songtest.c
PS_FILES =	
S_FILES =	
H_FILES =	mtime.h song.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	mtime.o song.o 

#
# Main targets
#

all:	songtest 

songtest:	songtest.o $(OBJFILES)
	$(CC) $(CFLAGS) -o songtest songtest.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

mtime.o:	mtime.h
song.o:	mtime.h song.h
songtest.o:	mtime.h song.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) songtest.o core

realclean:        clean
	-/bin/rm -f songtest 
