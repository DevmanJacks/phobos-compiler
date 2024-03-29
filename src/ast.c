/*
 * ast.c
 */

#include <assert.h>
#include <stdio.h>
#include "ast.h"

// ========== Identifier ==========

ident_t *make_ident(pos_t start, pos_t end, const char *name) {
    ident_t *ident = malloc(sizeof(ident_t));

    if (!ident) {
        perror("Unable to allocate memory for ident");
        exit(-1);
    }

    ident->start = start;
    ident->end = end;
    ident->name = name;
    return ident;
}

size_t print_ident(char *buf, size_t len, ident_t *ident) {
    return snprintf(buf, len, "(ident %s)", ident->name);
}

// ========== Expression ==========

static expr_t *make_expr(expr_type_t type) {
    expr_t *expr = malloc(sizeof(expr_t));

    if (!expr) {
        perror("Unable to allocate memory for expr");
        exit(-1);
    }

    expr->type = type;
    return expr;
}

size_t print_expr(char *buf, size_t len, expr_t *expr) {
    size_t offset = 0;

    switch (expr->type) {
        case EXPR_IDENT:
            offset = print_ident(buf, len, expr->ident);
            break;

        case EXPR_LITERAL:
            offset = snprintf(buf, len, "(literal ");

            switch (expr->literal.class) {
                case TOKEN_INT:
                    offset += snprintf(buf + offset, len - offset, "INT");
                    break;

                case TOKEN_FLOAT:
                    offset += snprintf(buf + offset, len - offset, "FLOAT");
                    break;
        
                case TOKEN_CHAR:
                    offset += snprintf(buf + offset, len - offset, "CHAR");
                    break;

                case TOKEN_STR:
                    offset += snprintf(buf + offset, len - offset, "STR");
                    break;

                default:
                    assert(0);
            }

            offset += snprintf(buf + offset, len - offset, " %s)", expr->literal.value);
            break;

        default:
            assert(0);
    }

    return offset;
}

// ---------- Literal ----------

expr_t *make_ident_expr(ident_t *ident) {
    expr_t *expr = make_expr(EXPR_IDENT);
    expr->ident = ident;
    return expr;
}

expr_t *make_literal_expr(pos_t start, pos_t end, token_t class, const char *value) {
    assert(class == TOKEN_INT || class == TOKEN_FLOAT || class == TOKEN_CHAR || class == TOKEN_STR);

    expr_t *expr = make_expr(EXPR_LITERAL);
    expr->literal.start = start;
    expr->literal.end = end;
    expr->literal.class = class;
    expr->literal.value = value;
    return expr;
}

expr_t *make_selector_expr(expr_t *expr, ident_t *ident) {
    expr_t *sel_expr = make_expr(EXPR_SELECTOR);
    sel_expr->selector.expr = expr;
    sel_expr->selector.ident = ident;
    return sel_expr;
}

// ========== Type ==========

size_t print_type(char *buf, size_t len, expr_t *type) {
    size_t offset = snprintf(buf, len, "(type ");

    switch (type->type) {
        case EXPR_SYNONYM_TYPE:
            offset += print_expr(buf + offset, len - offset, type->synonym_type);
            offset += snprintf(buf + offset, len - offset, ")");
            break;

        default:
            assert(0);
            break;
    }

    return offset;
}

// ---------- Synonym Type ----------

expr_t *make_synonym_type(expr_t *type) {
    expr_t *expr = make_expr(EXPR_SYNONYM_TYPE);
    expr->synonym_type = type;
    return expr;
}