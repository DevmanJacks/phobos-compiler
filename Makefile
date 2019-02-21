# Makefile for phobos compiler

SRCDIR = src
TSTSRCDIR = src/test
OBJDIR = obj

CC = cc
DBGCFLAGS = -g -O0 -DDEBUG
LFLAGS = 

# Object files
OBJECTS = $(OBJDIR)/intern.o $(OBJDIR)/pc.o
TSTOBJS = $(OBJDIR)/intern_test.o $(OBJDIR)/testpc.o

# Default build
all: pc testpc

pc: $(OBJECTS)
	$(CC) -o pc $^ $(LFLAGS)

testpc: $(TSTOBJS)
	$(CC) -o testpc $^ $(LFLAGS)

$(OBJDIR)/intern.o: $(SRCDIR)/intern.c $(SRCDIR)/intern.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/pc.o: $(SRCDIR)/pc.c
	$(CC) -c $(DBGCFLAGS) -o $@ $<

# Tests
$(OBJDIR)/intern_test.o: $(TSTSRCDIR)/intern_test.c $(SRCDIR)/intern.c $(SRCDIR)/intern.h $(TSTSRCDIR)/testing.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/testpc.o: $(TSTSRCDIR)/testpc.c
	$(CC) -c $(DBGCFLAGS) -o $@ $<

clean:
	@-rm -f $(OBJDIR)/*.o
	@-rm -f pc
	@-rm -f testpc