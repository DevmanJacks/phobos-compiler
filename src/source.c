/*
 * Source code file handling for the Phobos compiler
 */

#include <stdio.h>
#include "source.h"

enum { SOURCE_INITIAL_LINES_CAPACITY = 128, SOURCE_INCREASE_LINES_CAPACITY = 128 };

extern Source *add_source(uint8_t *code, size_t length) {
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

static Source *create_source(char *filename, uint8_t *code, size_t length) {
    Source *source = malloc(sizeof(Source));

    if (!source) {
        perror("Unable to allocate memory for Source");
        exit(-1);
    }

    source->filename = filename;
    source->code = code;
    source->length = length;

    source->lines = malloc(SOURCE_INITIAL_LINES_CAPACITY * sizeof(int));

    if (!source->lines) {
        perror("Unable to allocate memory for lines");
        exit(-1);
    }

    source->lines_capacity = SOURCE_INITIAL_LINES_CAPACITY;
    
    // The first line of source code is always from the start of the code
    source->lines[0] = 0;
    source->lines_length = 1;

    return source;
}