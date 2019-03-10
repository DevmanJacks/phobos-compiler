#pragma once

#include "scanner.h"

typedef struct parser {
    scanner_t *scanner;
} parser_t;

// Public functions
extern parser_t *make_parser(scanner_t *scanner);