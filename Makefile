
# Makefile for phobos compiler

SRCDIR = src
TSTSRCDIR = src/test
OBJDIR = obj

CC = cc
DBGCFLAGS = -g -O0 -DDEBUG -std=c11
LFLAGS = 

# Object files
OBJECTS = $(OBJDIR)/intern.o $(OBJDIR)/scanner.o $(OBJDIR)/source.o $(OBJDIR)/token.o $(OBJDIR)/pc.o
TSTOBJS = $(OBJDIR)/intern_test.o $(OBJDIR)/scanner_test.o $(OBJDIR)/source_test.o $(OBJDIR)/token_test.o $(OBJDIR)/testpc.o

# Default build
all: pc testpc

pc: $(OBJECTS)
	$(CC) -o pc $^ $(LFLAGS)

testpc: $(TSTOBJS)
	$(CC) -o testpc $^ $(LFLAGS)

$(OBJDIR)/intern.o: $(SRCDIR)/intern.c $(SRCDIR)/intern.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/source.o: $(SRCDIR)/source.c $(SRCDIR)/source.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/token.o: $(SRCDIR)/token.c $(SRCDIR)/intern.h $(SRCDIR)/source.h $(SRCDIR)/token.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/error.o: $(SRCDIR)/error.c $(SRCDIR)/error.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/scanner.o: $(SRCDIR)/scanner.c $(SRCDIR)/error.h $(SRCDIR)/intern.h $(SRCDIR)/scanner.h $(SRCDIR)/token.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

# $(OBJDIR)/ast.o: $(SRCDIR)/ast.c $(SRCDIR)/ast.h $(SRCDIR)/source.h
# 	$(CC) -c $(DBGCFLAGS) -o $@ $<

# $(OBJDIR)/parser.o: $(SRCDIR)/parser.c $(SRCDIR)/ast.h $(SRCDIR)/parser.h $(SRCDIR)/scanner.h $(SRCDIR)/token.h
# 	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/pc.o: $(SRCDIR)/pc.c
	$(CC) -c $(DBGCFLAGS) -o $@ $<

# Tests
$(OBJDIR)/intern_test.o: $(TSTSRCDIR)/intern_test.c $(SRCDIR)/intern.c $(SRCDIR)/intern.h $(TSTSRCDIR)/testing.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/source_test.o: $(TSTSRCDIR)/source_test.c $(SRCDIR)/source.c $(SRCDIR)/source.h $(TSTSRCDIR)/testing.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/token_test.o: $(TSTSRCDIR)/token_test.c $(SRCDIR)/token.c $(SRCDIR)/token.h $(TSTSRCDIR)/testing.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/scanner_test.o: $(TSTSRCDIR)/scanner_test.c  $(SRCDIR)/scanner.c  $(SRCDIR)/error.h $(SRCDIR)/intern.h $(SRCDIR)/scanner.h $(SRCDIR)/token.h $(TSTSRCDIR)/testing.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

# $(OBJDIR)/parser_test.o: $(TSTSRCDIR)/parser_test.c  $(SRCDIR)/parser.c $(SRCDIR)/ast.h $(SRCDIR)/error.h $(SRCDIR)/scanner.h $(SRCDIR)/token.h $(TSTSRCDIR)/testing.h
# 	$(CC) -c $(DBGCFLAGS) -o $@ $<

$(OBJDIR)/testpc.o: $(TSTSRCDIR)/testpc.c $(TSTSRCDIR)/intern_test.h $(TSTSRCDIR)/source_test.h $(TSTSRCDIR)/token_test.h $(TSTSRCDIR)/testing.h
	$(CC) -c $(DBGCFLAGS) -o $@ $<

clean:
	@-rm -f $(OBJDIR)/*.o
	@-rm -f pc
	@-rm -f testpc
