#ifndef __TOKEN_H
#define __TOKEN_H

typedef enum token_t {
    TOKEN_ILLEGAL = -1,
    TOKEN_EOF = 0,
    TOKEN_IDENT,
    TOKEN_INT,

    TOKEN_VAR
} token_t;

// Public functions
extern token_t lookup_token(const char *ident);

#endif