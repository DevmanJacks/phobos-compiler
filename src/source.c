/*
 * source.c
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "source.h"

static source_t *sources = NULL;

enum { INITIAL_LINES_CAPACITY = 128, INCREASE_LINES_CAPACITY = 128 };

static source_t *make_source(char *filename, uint8_t *code, size_t length) {
    source_t *source = malloc(sizeof(source_t));

    if (!source) {
        perror("Unable to allocate memory for source");
        exit(-1);
    }

    source->next = NULL;
    source->filename = filename;
    source->code = code;
    source->length = length;

    source->lines = malloc(INITIAL_LINES_CAPACITY * sizeof(int));

    if (!source->lines) {
        perror("Unable to allocate memory for lines");
        exit(-1);
    }

    source->lines_capacity = INITIAL_LINES_CAPACITY;
    source->lines_length = 0;

    return source;
}

source_t *add_source(uint8_t *code, size_t length) {
    source_t *source = make_source(NULL, code, length);
    source->next = sources;
    sources = source;
	return source;
}

void add_line(source_t *source, pos_t pos) {
    if (source->lines_length == source->lines_capacity) {
        source->lines = realloc(source->lines, source->lines_capacity + INCREASE_LINES_CAPACITY);

        if (!source->lines) {
            perror("Unable to reallocate memory for lines");
            exit(-1);
        }
    }

    source->lines[source->lines_length++] = pos;
}