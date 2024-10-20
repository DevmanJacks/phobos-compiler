CC = gcc
CFLAGS = -g -O0 -DDEBUG -std=c11
LDFLAGS =

SRCDIR   = src
BUILDDIR = build

EXECUTABLE_NAME = pc

SOURCE_FILES = dynamic_array.c hashtable.c intern.c pc.c
OBJECT_FILES = $(SOURCE_FILES:%.c=$(BUILDDIR)/%.o)

.PHONY: $(EXECUTABLE_NAME) clean

all: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME): $(OBJECT_FILES)
	$(CC) $(LDFLAGS) $(OBJECT_FILES) -o $(BUILDDIR)/$(EXECUTABLE_NAME)

clean:
	rm -r -f $(BUILDDIR)

create_directories: 
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<