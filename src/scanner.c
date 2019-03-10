/*
 * scanner.c
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "intern.h"
#include "scanner.h"
#include "token.h"

enum { UNICODE_1_BYTE = 128 };

static inline bool is_binary_digit(int c) {
	return c == '0' || c == '1';
}

static inline bool is_octal_digit(int c) {
	return ('0' <= c && c <= '7');
}

static inline bool is_digit(char c) {
	return ('0' <= c && c <= '9');									// TODO: Add unicode support
}

static inline bool is_hex_digit(int c) {
	return ('0' <= c && c <= '9') || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
}

static inline bool is_letter(char c) {
	return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');		// TODO: Add unicode support
}

static inline pos_t current_pos(scanner_t *scanner) {
	return scanner->offset;
}

static void next_char(scanner_t *scanner) {
	scanner->offset = scanner->next_offset;

	if (scanner->offset < scanner->source->length) {
		if (*(scanner->source->code) + scanner->offset < UNICODE_1_BYTE) {
			scanner->curr_char = *(scanner->source->code + scanner->offset);
			scanner->next_offset++;
		} else {
			// TODO: Implement multi-byte unicode characters
			assert(0);
		}
	} else {
		scanner->curr_char = -1;			// EOF
	}
}

// Return the next byte (we don't need to check for unicode) of the src without advancing the scanner.  If we are at end of file returns 0
static uint8_t peek(scanner_t *scanner)
{
	if (scanner->offset < scanner->source->length) {
		return *(scanner->source->code + scanner->next_offset);
	}

	return 0;
}

static bool scan_escape(scanner_t *scanner)
{
	next_char(scanner);

	// TODO: Ensure escape sequence is valid
	return true;
}

static void scan_character(scanner_t *scanner)
{
	size_t start = scanner->offset;
	next_char(scanner);					// Skip past opening quote
	
	unsigned int num = 0;
	bool valid = true;					// Assume that the character literal is valid.  We will set to false if not.

	while (true) {
		if (scanner->curr_char == '\'') {
			next_char(scanner);
			
			if (valid && num != 1) {
				log_error(current_pos(scanner), current_pos(scanner), "Invalid character literal.");
			}

			break;
		}

		if (scanner->curr_char == '\n' || scanner->curr_char < 0) {
			log_error(current_pos(scanner), current_pos(scanner), "Character literal not terminated.");
			break;
		}

		num++;

		if (scanner->curr_char == '\\') {
			next_char(scanner);
			valid = valid && scan_escape(scanner);
		}

		next_char(scanner);
	}

	scanner->token = TOKEN_CHAR;
	scanner->lexeme = intern_string((const char *)(scanner->source->code + start), scanner->offset - start);
}

static void scan_ident(scanner_t *scanner) {
    size_t start = scanner->offset;

    while (is_letter(scanner->curr_char) || is_digit(scanner->curr_char) || scanner->curr_char == '_') {
        next_char(scanner);
    }

    scanner->lexeme = intern_string((const char *)(scanner->source->code + start), scanner->offset - start);
    scanner->token = lookup_token(scanner->lexeme);
}

typedef enum base_t {
	BASE_BINARY,
	BASE_OCTAL,
	BASE_DECIMAL,
	BASE_HEX
} base_t;

static void scan_integer(scanner_t *s, base_t base) {
	switch (base) {
		case BASE_BINARY:
			while (is_binary_digit(s->curr_char) || s->curr_char == '_')
				next_char(s);

			return;

		case BASE_OCTAL:
			while (is_octal_digit(s->curr_char) || s->curr_char == '_')
				next_char(s);

			return;

		case BASE_DECIMAL:
			while (is_digit(s->curr_char) || s->curr_char == '_')
				next_char(s);

			return;

		case BASE_HEX:
			while (is_hex_digit(s->curr_char) || s->curr_char == '_')
				next_char(s);

			return;
	}
}

static void scan_number(scanner_t *scanner) {
	size_t start = scanner->offset;
	scanner->token = TOKEN_INT;

	if (scanner->curr_char == '0') {
		next_char(scanner);
		char base = scanner->curr_char;

		if (base == 'b' || base == 'B') {
			next_char(scanner);

			if (is_binary_digit(scanner->curr_char)) {
				scan_integer(scanner, BASE_BINARY);
				scanner->lexeme = intern_string((const char *)(scanner->source->code + start), scanner->offset - start);
			} else {
				log_error(current_pos(scanner), current_pos(scanner), "Expected binary digit following %c.", base);
			}

			return;
		} else if (is_octal_digit(scanner->curr_char)) {
			next_char(scanner);
			scan_integer(scanner, BASE_OCTAL);
			scanner->lexeme = intern_string((const char *)(scanner->source->code + start), scanner->offset - start);
			return;
		} else if (base== 'x' || base == 'X') {
			next_char(scanner);

			if (is_hex_digit(scanner->curr_char)) {
				scan_integer(scanner, BASE_HEX);
				scanner->lexeme = intern_string((const char *)(scanner->source->code + start), scanner->offset - start);
			} else {
				log_error(current_pos(scanner), current_pos(scanner), "Expected hex digit following %c.", base);
			}

			return;
		}
	}

	// A decimal integer or float
	if (scanner->curr_char != '.')
		scan_integer(scanner, BASE_DECIMAL);
	
	if (scanner->curr_char == '.') {
		next_char(scanner);
		scanner->token = TOKEN_FLOAT;
	}

	if (scanner->curr_char == 'e' || scanner->curr_char == 'E') {
		scanner->token = TOKEN_FLOAT;
		next_char(scanner);

		if (scanner->curr_char == '-' || scanner->curr_char == '+')
			next_char(scanner);

		if (is_digit(scanner->curr_char))
			scan_integer(scanner, BASE_DECIMAL);
		else {
			log_error(current_pos(scanner), current_pos(scanner), "Illegal character in floating point exponent.");
		}
	}

	scanner->lexeme = intern_string((const char *)(scanner->source->code + start), scanner->offset - start);
}

static void scan_string(scanner_t *scanner)
{
	size_t start = scanner->offset;
	next_char(scanner);					// Skip past opening quote

	bool valid = true;					// Assume that the character literal is valid.  We will set to false if not.

	while (true) {
		if (scanner->curr_char == '\"') {
			next_char(scanner);
			break;
		}
			
		if (scanner->curr_char == '\n' || scanner->curr_char < 0) {
			log_error(current_pos(scanner), current_pos(scanner), "String literal not terminated.");
			break;
		}

		if (scanner->curr_char == '\\') {
			valid = valid && scan_escape(scanner);
		}

		next_char(scanner);
	}

	scanner->token = TOKEN_STR;
	scanner->lexeme = intern_string((const char *)(scanner->source->code + start), scanner->offset - start);
}

#define CASE1(c, t) \
	case c: \
		next_char(scanner); \
		scanner->token = t; \
		break;

#define CASE2(c, t, c2, t2)	\
	case c: \
		next_char(scanner); \
		if (scanner->curr_char == c2) { \
			scanner->token = t2; \
			next_char(scanner); \
		} else { \
			scanner->token = t; \
		} \
		break;

#define CASE3(c, t, c2, t2, c3, t3) \
	case c: \
		next_char(scanner); \
		if (scanner->curr_char == c2) { \
			scanner->token = t2; \
			next_char(scanner); \
		} else if (scanner->curr_char == c3) { \
			scanner->token = t3; \
			next_char(scanner); \
		} else { \
			scanner->token = t; \
		} \
		break;

#define CASE4(c, t, c2, t2) \
	case c: \
		if (is_digit(peek(scanner))) { \
			scan_number(scanner); \
		} else { \
			next_char(scanner); \
			if (scanner->curr_char == c2) { \
				next_char(scanner); \
				scanner->token = t2; \
			} else { \
				scanner->token = t; \
			} \
		} \
		break;

#define CASE5(c, t, c2, t2, c3, t3) \
	case c: \
		if (is_digit(peek(scanner))) { \
			scan_number(scanner); \
		} else { \
			next_char(scanner); \
			if (scanner->curr_char == c2) { \
				next_char(scanner); \
				scanner->token = t2; \
			} else if (scanner->curr_char == c3) { \
				next_char(scanner); \
				scanner->token = t3; \
			} else { \
				scanner->token = t; \
			} \
		} \
		break;

#define CASE6(c, t, t2, t3, t4) \
	case c: \
		next_char(scanner); \
		if (scanner->curr_char == '=') { \
			scanner->token = t2; \
			next_char(scanner); \
		} else if (scanner->curr_char == c) { \
			next_char(scanner); \
			if (scanner->curr_char == '=') { \
				scanner->token = t4; \
				next_char(scanner); \
			} else { \
				scanner->token = t3; \
			} \
		} else { \
			scanner->token = t; \
		} \
		break;

token_t scan(scanner_t *scanner) {
top:
    scanner->token_start_pos = scanner->offset;

    switch (scanner->curr_char) {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
		case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
		case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
		case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
		case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case '_':
            scan_ident(scanner);
            break;

		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			scan_number(scanner);
			break;

		case '\'':
			scan_character(scanner);
			break;

		case '\"':
			scan_string(scanner);
			break;

		case '/':
			next_char(scanner);

			if (scanner->curr_char == '/') {
				// Line comment
				next_char(scanner);

				while (scanner->curr_char != '\n' && scanner->curr_char != -1)
					next_char(scanner);

				goto top;
			} else if (scanner->curr_char == '=') {
				scanner->token = TOKEN_DIV_ASSIGN;
				next_char(scanner);
			} else {
				scanner->token = TOKEN_DIV;
			}

			break;
		
		case -1:
			scanner->token = TOKEN_EOF;
			break;

		CASE1('(', TOKEN_LPAREN)
		CASE1(')', TOKEN_RPAREN)
		CASE1('{', TOKEN_LBRACE)
		CASE1('}', TOKEN_RBRACE)
		CASE1('[', TOKEN_LBRACKET)
		CASE1(']', TOKEN_RBRACKET)
		CASE1(',', TOKEN_COMMA)
		CASE1(';', TOKEN_SEMICOLON)
		CASE1('?', TOKEN_QUESTION)

		CASE2(':', TOKEN_COLON, '=', TOKEN_COLON_ASSIGN)
		CASE2('!', TOKEN_NOT, '=', TOKEN_NOT_EQ)
		CASE2('~', TOKEN_BITWISE_NOT, '=', TOKEN_BITWISE_NOT_ASSIGN)
		CASE2('*', TOKEN_MUL, '=', TOKEN_MUL_ASSIGN)
		CASE2('%', TOKEN_MOD, '=', TOKEN_MOD_ASSIGN)
		CASE2('^', TOKEN_XOR, '=', TOKEN_XOR_ASSIGN)
		CASE2('=', TOKEN_ASSIGN, '=', TOKEN_EQ)

		CASE3('&', TOKEN_BITWISE_AND, '=', TOKEN_BITWISE_AND_ASSIGN, '&', TOKEN_AND)
		CASE3('|', TOKEN_BITWISE_OR, '=', TOKEN_BITWISE_OR_ASSIGN, '|', TOKEN_OR)

		CASE4('.', TOKEN_DOT, '.', TOKEN_DOTDOT)
		CASE4('+', TOKEN_ADD, '=', TOKEN_ADD_ASSIGN)

		CASE5('-', TOKEN_SUB, '=', TOKEN_SUB_ASSIGN, '>', TOKEN_RETURNS)

		CASE6('<', TOKEN_LT, TOKEN_LE, TOKEN_LSHIFT, TOKEN_LSHIFT_ASSIGN)
		CASE6('>', TOKEN_GT, TOKEN_GE, TOKEN_RSHIFT, TOKEN_RSHIFT_ASSIGN)

        default:
            break;
    }

	scanner->token_end_pos = scanner->offset;
	return scanner->token;
}

#undef CASE1
#undef CASE2
#undef CASE3
#undef CASE4
#undef CASE5
#undef CASE6

scanner_t *make_scanner(source_t *source) {
	scanner_t *s = malloc(sizeof(scanner_t));

	if (s) {
		s->source = source;

		s->curr_char = 0;
		s->offset = 0;
		s->next_offset = 0;

		s->token = TOKEN_ILLEGAL;

		// Start it!
		next_char(s);

		return s;
    } else {
        perror("Unable to allocate memory for scanner");
        exit(-1);
    }
}