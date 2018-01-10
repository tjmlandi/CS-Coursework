CXXFLAGS =	
CFLAGS =	-Wall -Wextra -pedantic -std=c99
CLIBFLAGS =	-lpthread -lrt
CCLIBFLAGS =	-lpthread -lrt

stripped:	all
	strip pt-cruisers

