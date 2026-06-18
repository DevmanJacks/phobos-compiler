#pragma once

#include "scanner.h"
#include "source.h"

typedef struct parser
{
    SourceFile *source_file;
    Scanner *scanner;
} Parser;

// Public functions
extern void parse_source_file(SourceFile *source_file);