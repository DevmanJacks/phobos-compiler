/*
 * scanner.c
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "intern.h"
#include "scanner.h"
#include "token.h"

enum { UNICODE_1_BYTE = 128 };

static inline bool is_digit(char c)
{
	return ('0' <= c && c <= '9');
}

static inline bool is_letter(char c)
{
	return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
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

void scan_ident(scanner_t *scanner) {
    size_t start = scanner->offset;

    while (is_letter(scanner->curr_char) || is_digit(scanner->curr_char) || scanner->curr_char == '_') {
        next_char(scanner);
    }

    scanner->lexeme = intern_string((const char *)(scanner->source->code), scanner->offset - start);
    scanner->token = lookup_token(scanner->lexeme);
}

token_t scan(scanner_t *scanner) {
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

        default:
            break;
    }

	return scanner->token;
}

scanner_t *make_scanner(source_t *source)
{
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