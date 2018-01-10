#
# Created by gmakemake (Ubuntu Jul 25 2014) on Sun Feb 19 23:15:21 2017
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

#
# This header.mak file will set up all necessary options for compiling
# and linking C and C++ programs which use OpenGL, GLFW, and/or GLEW on
# the CS Ubuntu systems.
#
# To use it, place it in the same directory as your source code, and
# run the command
#
#	gmakemake > Makefile
#
# To compile and link your program, just run "make".
#

# locations of important directories if the header files
# and library files aren't in the standard places
INCLUDE =
LIBDIRS =

# common linker options
LDLIBS = -lGL -lm -lGLEW -lglfw

# language-specific linker options
CLDLIBS =
CCLDLIBS =

CFLAGS = -std=c99 -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES
CCFLAGS = $(CFLAGS)
CXXFLAGS = -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES

LIBFLAGS = -g $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS) $(CLDLIBS)
CCLIBFLAGS = $(LIBFLAGS) $(CCLDLIBS)

########## End of flags from header.mak


CPP_FILES =	ShaderSetup.cpp helloOpenGL.cpp
C_FILES =	
PS_FILES =	
S_FILES =	
H_FILES =	ShaderSetup.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	ShaderSetup.o 

#
# Main targets
#

all:	helloOpenGL 

helloOpenGL:	helloOpenGL.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o helloOpenGL helloOpenGL.o $(OBJFILES) $(CCLIBFLAGS)

#
# Dependencies
#

ShaderSetup.o:	ShaderSetup.h
helloOpenGL.o:	ShaderSetup.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) helloOpenGL.o core

realclean:        clean
	-/bin/rm -f helloOpenGL 
