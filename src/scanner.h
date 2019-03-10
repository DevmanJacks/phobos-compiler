#pragma once

#include <stddef.h>
#include "source.h"
#include "token.h"

typedef struct scanner {
    source_t *source;

    // Current state
    int    curr_char;
    size_t offset;                      // The character offset in the source
    size_t next_offset;                 // The offset of the next character in the source.  Unicode characters can be up to 4 bytes.

    // Token information
    token_t    token;
    pos_t      token_start_pos;
    pos_t      token_end_pos;
    const char *lexeme;
} scanner_t;

// Public functions
extern scanner_t *make_scanner(source_t *source);
extern token_t scan(scanner_t *scanner);
