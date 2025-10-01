#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct Source {
    char         *filename;
    uint8_t      *code;
    unsigned int length;

    // Position of the start of each line
    int *lines;
    int lines_capacity;
    int lines_length;
} Source;

// Compact way to store position in a file
typedef int Pos;

// Public functions
extern Source *add_source(uint8_t *code, size_t length);
extern void add_line(Source *source, Pos pos);