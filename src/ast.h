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
    // ---------- Expressions -----------
    EXPR_IDENT,
    EXPR_LITERAL,
    EXPR_SELECTOR,

    // ----------- Types ----------
    EXPR_SYNONYM_TYPE
} expr_type_t;

typedef struct expr {
    expr_type_t type;
    union {
        // ---------- Expressions -----------
        ident_t *ident;
        struct {
            pos_t start;
            pos_t end;
            token_t class;
            const char *value;
        } literal;
        struct {
            struct expr *expr;
            ident_t *ident;
        } selector;

        // ---------- Types ----------
        struct expr *synonym_type;
    };
} expr_t;

extern size_t print_expr(char *buf, size_t len, expr_t *expr);

expr_t *make_ident_expr(ident_t *ident);
extern expr_t *make_literal_expr(pos_t start, pos_t end, token_t class, const char *value);
expr_t *make_selector_expr(expr_t *expr, ident_t *ident);

// --------- Types ----------

typedef expr_t type_t;

extern size_t print_type(char *buf, size_t len, expr_t *expr);

extern expr_t *make_synonym_type(expr_t *type);