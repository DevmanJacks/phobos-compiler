#ifndef __SOURCE_H
#define __SOURCE_H

#include <stdint.h>

typedef struct source_t {
    char    *filename;
    uint8_t *code;
    size_t  length;

    // Position of the start of each line
    int    *lines;
    size_t lines_capacity;
    size_t lines_length;

    // Linked list of sources.
    struct source_t *next;
} source_t;

// Compact way to store position in a file
typedef int pos_t;

// Public functions
extern source_t *add_source(uint8_t *code, size_t length);
extern void add_line(source_t *source, pos_t pos);

#endif