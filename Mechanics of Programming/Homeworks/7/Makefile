#
# Created by gmakemake (Ubuntu Jul 25 2014) on Thu Nov 20 15:55:34 2014
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

CXXFLAGS =	-ggdb -Wall
CFLAGS =	-ggdb -Wall -std=c99 -Wextra -pedantic
CLIBFLAGS =	-L/home/course/csci243/pub/projects/Vault -lvault
CCLIBFLAGS =	

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	vault.c
PS_FILES =	
S_FILES =	
H_FILES =	file_tools.h vault.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	vault.o 

#
# Main targets
#

all: manager

manager:    $(OBJFILES)
	$(CC) $(CFLAGS) -o manager     $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

vault.o:	file_tools.h vault.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) core

realclean:        clean
	-/bin/rm -f manager 
