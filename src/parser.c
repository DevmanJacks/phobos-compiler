/*
 * parser.c
 */

#include <assert.h>
#include <stdio.h>
#include "ast.h"
#include "error.h"
#include "parser.h"
#include "scanner.h"

static inline token_t curr_token(parser_t *parser) { return parser->scanner->token; }
static inline const char *token_lexeme(parser_t *parser) { return parser->scanner->lexeme; }
static inline pos_t token_start_pos(parser_t *parser) { return parser->scanner->token_start_pos; }
static inline pos_t token_end_pos(parser_t *parser) { return parser->scanner->token_start_pos; }

static inline void next_token(parser_t *parser) { scan(parser->scanner); }

static inline bool match_token(parser_t *parser, token_t token) {
    if (curr_token(parser) == token) {
        next_token(parser);
        return true;
    }

    return false;
}

// ========== Identifiers ==========

static ident_t *parse_ident(parser_t *parser) {
    pos_t start = token_start_pos(parser);
    pos_t end = token_end_pos(parser);

    if (curr_token(parser) == TOKEN_IDENT) {
        ident_t *ident = make_ident(start, end, token_lexeme(parser));
        next_token(parser);
        return ident;
    }

    assert(0);
    return NULL;
}

// ========== Expression ==========

static expr_t *parse_literal_expr(parser_t *parser) {
    expr_t *expr = make_literal_expr(token_start_pos(parser), token_end_pos(parser), curr_token(parser), token_lexeme(parser));
    next_token(parser);
    return expr;
}

static expr_t *parse_operand(parser_t *parser) {
    expr_t *expr = NULL;

    switch (curr_token(parser)) {
        case TOKEN_CHAR:
        case TOKEN_FLOAT:
        case TOKEN_INT:
        case TOKEN_STR:
            expr = parse_literal_expr(parser);
            break;

        default:
            assert(0);
            break;
    }

    return expr;
}

static expr_t *parse_binary_expr(parser_t *parser, unsigned int precidence_level) {
    expr_t *expr = parse_operand(parser);

    while (is_binary_op(curr_token(parser))) {
        if (operator_precedence(curr_token(parser)) > precidence_level) {
            token_t op = curr_token(parser);
            next_token(parser);
            // expr = newBinaryExpr(expr, op, parseBinaryExpr(p, operatorPrecedence(op)));
        } else {
            break;
        }
    }

    return expr;
}

static expr_t *parse_expr(parser_t *parser) {
    expr_t *expr = parse_binary_expr(parser, 0);
    return expr;
}

// ========== Type ==========

static expr_t *parse_synonym_type(parser_t *parser) {
    expr_t *expr = make_ident_expr(parse_ident(parser));
    next_token(parser);

    if (match_token(parser, TOKEN_DOT)) {
        while (curr_token(parser) == TOKEN_IDENT) {
            expr = make_selector_expr(expr, parse_ident(parser));

            if (!match_token(parser, TOKEN_DOT))
                break;
        }
    }

    return make_synonym_type(expr);
}

static expr_t *parse_type(parser_t *parser) {
    switch (curr_token(parser)) {
        case TOKEN_IDENT:
            return parse_synonym_type(parser);

        default:
            assert(0);
            return NULL;
    }
}

parser_t *make_parser(scanner_t *scanner) {
	parser_t *parser = malloc(sizeof(parser_t));

	if (parser) {
		parser->scanner = scanner;

        // Start it!
        scan(scanner);

		return parser;
    } else {
        perror("Unable to allocate memory for parser");
        exit(-1);
    }
}