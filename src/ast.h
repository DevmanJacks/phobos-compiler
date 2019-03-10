#pragma once

#include <stdint.h>
#include "source.h"
#include "token.h"

// ========== Identifier ==========

typedef struct ident {
    pos_t      start;
    pos_t      end;
    const char *name;
} ident_t;

extern ident_t *make_ident(pos_t start, pos_t end, const char *name);
extern size_t print_ident(char *buf, size_t len, ident_t *ident);

// ========== Expression ==========

typedef enum expr_type {
    EXPR_LITERAL
} expr_type_t;

typedef struct expr {
    expr_type_t type;
    union {
        struct {
            pos_t start;
            pos_t end;
            token_t class;
            const char *value;
        } literal;
    };
} expr_t;

extern size_t print_expr(char *buf, size_t len, expr_t *expr);

extern expr_t *make_literal_expr(pos_t start, pos_t end, token_t class, const char *value);
