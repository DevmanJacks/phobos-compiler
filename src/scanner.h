#ifndef __SCANNER_H
#define __SCANNER_H

#include <stddef.h>
#include "source.h"
#include "token.h"

typedef struct scanner_t {
    source_t *source;

    // Current state
    int    curr_char;
    size_t offset;                      // The character offset in the source
    size_t next_offset;                 // The offset of the next character in the source.  Unicode characters can be up to 4 bytes.

    // Token information
    token_t    token;
    pos_t      token_start_pos;
    const char *lexeme;
} scanner_t;

// Public functions
extern token_t scan(scanner_t *scanner);

#endif