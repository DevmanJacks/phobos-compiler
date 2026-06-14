#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "ast.h"

// Compact way to store position in a file
typedef size_t Pos;

typedef struct source_file {
    char    *filename;
    uint8_t *code;
    size_t  length;

    // List of AST nodes after parsing
    AstNode *decls;

    // Link to the next source file in the list
    struct source_file *next;
} SourceFile;

typedef struct Source {
    SourceFile *first_source_file;
    Pos        next_pos;


    char         *filename;
    uint8_t      *code;
    unsigned int length;

    // Position of the start of each line
    int *lines;
    int lines_capacity;
    int lines_length;
} Source;

// Public functions
extern SourceFile *create_source_file(char *filename);

extern void initialise_source();
extern Source *add_source(uint8_t *code, size_t length);
extern void add_line(Source *source, Pos pos);