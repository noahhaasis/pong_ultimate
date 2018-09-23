# compiler to use
CC = gcc

# flags to pass compiler
CFLAGS = -ggdb3 -std=c11 -Wall -I/usr/include/sdl

# name for executable
EXE = pong_ultimate

# space-separated list of header files
HDRS =

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS = -lSDL2 -l SDL2main

# space-separated list of source files
SRCS = pong_ultimate.c

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)

# default target
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) *.o
