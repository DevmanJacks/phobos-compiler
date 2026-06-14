/*
 * Source code file handling for the Phobos compiler
 */

#include <stdio.h>
#include <sys/stat.h>
#include "source.h"

void fatal_error(char *msg);

enum { SOURCE_INITIAL_LINES_CAPACITY = 128, SOURCE_INCREASE_LINES_CAPACITY = 128 };

// Source is a singleton.
Source *the_source;

void fatal_error(char *msg) {
    perror(msg);
    exit(-1);
}

extern SourceFile *add_source_file(char *filename) {
    return NULL;
}

extern void add_line(Source *source, Pos pos) {
    if (source->lines_length == source->lines_capacity) {
        source->lines = realloc(source->lines, source->lines_capacity + SOURCE_INCREASE_LINES_CAPACITY);

        if (!source->lines) {
            perror("Unable to reallocate memory for lines");
            exit(-1);
        }
    }

    source->lines[source->lines_length++] = pos;
}

extern void initialise_source() {
    
    the_source = malloc(sizeof(Source));

    if (!the_source)
        fatal_error("Unable to allocate memory for Source.");
}

extern SourceFile *create_source_file(char *filename) {
    struct stat statbuf;

    if (stat(filename, &statbuf))
        fatal_error(filename);
    
    // Load the whole file into memory.  We will split the file into lines during scanning.
    FILE *file = fopen(filename, "r");
    
    if (!file)
        fatal_error(filename);

    SourceFile *source_file = malloc(sizeof(SourceFile));

    if (!source_file)
        fatal_error("Unable to allocate memory for source file.");

    int8_t *code = malloc(statbuf.st_size);
    
    if (!code)
        fatal_error("Unable to allocate memory for source code.");
    
    if (fread(code, 1, statbuf.st_size, file) != statbuf.st_size && ferror(file))
        fatal_error("Unable to read entire source file.");
    
    *(code + statbuf.st_size) = 0;
    
    source_file->filename = filename;
    source_file->code = code;
    source_file->length = statbuf.st_size;
    return source_file;
}

extern Source *create_source(char *filename, char *src, int length) {
    // Load the whole file into memory.  We will split the file into lines during scanning.
    FILE *file = fopen(filename, "r");
    
    if (!file)
        fatal_error("File does not exist or you do not have permissions.");

    the_source->filename = filename;
    fseek(file, 0, SEEK_END);
    the_source->length = ftell(file);
    fseek(file, 0, SEEK_SET);
    the_source->code = malloc(the_source->length + 1);

    if (!the_source->code)
        fatal_error("Unalbe to allocate memory for file.");

    if (fread(the_source->code, 1, the_source->length, file) != the_source->length)
        fatal_error("Unable to read entire source file.");

    *(the_source->code + the_source->length + 1) = 0;
    the_source->lines = malloc(SOURCE_INITIAL_LINES_CAPACITY * sizeof(int));

    if (!the_source->lines) {
        perror("Unable to allocate memory for lines");
        exit(-1);
    }

    the_source->lines_capacity = SOURCE_INITIAL_LINES_CAPACITY;
    
    // The first line of source code is always from the start of the code
    the_source->lines[0] = 0;
    the_source->lines_length = 1;

    return the_source;
}