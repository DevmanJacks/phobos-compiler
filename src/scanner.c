/*
 * Lexical analyser for the phobos programming language.
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "intern.h"
#include "scanner.h"
#include "token.h"

static inline int8_t curr_char(Scanner *s) {
    return *(s->source_file->code + s->pos);
} 

#define BUFFER_SIZE 128

static Token *scan_ident(Scanner *s) {
    static char buffer[BUFFER_SIZE];
    int buffer_pos = 0;
    int start = s->pos;

    while (isalnum(curr_char(s)) || curr_char(s) == '_') {
        buffer[buffer_pos++] = curr_char(s);
        s->pos++;
    }

    buffer[buffer_pos] = 0;

    InternedString ident = intern_string(buffer);

    Token *t = create_token(ident < TOKEN_IDENTIFIER ? ident : TOKEN_IDENTIFIER, start, buffer_pos);
    t->ident = ident;
    return t;
}

static Token *scan_number(Scanner *s) {
    long value = 0;
    int start = s->pos;

    while (isdigit(curr_char(s))) {
        char digit = curr_char(s) - '0';
        value = value * 10 + digit;
        s->pos++;
    }

    Token *t = create_token(TOKEN_INTEGER_LITERAL, start, s->pos - start);
    t->integer_literal = value;
    return t;
}

static void skip_whitespace(Scanner *s) {
    while (curr_char(s) == ' ' || curr_char(s)== '\t' || curr_char(s) == '\n')
        s->pos++;
}

static Token *single_char(Scanner *s, TokenKind kind) {
    s->current_token = create_token(kind, s->pos, 1);
    s->pos++;
    return s->current_token;
}

static Token *single_or_double_char(Scanner *s, char next_char, TokenKind next_char_token_kind, TokenKind kind) {
    int start = s->pos++;

    if (curr_char(s) == next_char) {
        s->pos++;
        s->current_token = create_token(next_char_token_kind, start, 2);
    }
    else
        s->current_token = create_token(kind, start, 1);

    return s->current_token;
}

static Token *single_or_double_char_2(Scanner *s, char first_next_char, TokenKind first_next_char_token_kind, char second_next_char, TokenKind second_next_char_token_kind, TokenKind kind) {
    int start = s->pos++;

    if (curr_char(s) == first_next_char) {
        s->pos++;
        s->current_token = create_token(first_next_char_token_kind, start, 2);
    }
    else if (curr_char(s) == second_next_char) {
        s->pos++;
        s->current_token = create_token(second_next_char_token_kind, start, 2);
    } else
        s->current_token = create_token(kind, start, 1);

    return s->current_token;
}

Token *next_token(Scanner *s) {
    skip_whitespace(s);

    if (s->pos >= s->source_file->length) {
        if (s->current_token->kind != TOKEN_EOF)
            s->current_token = create_token(TOKEN_EOF, s->pos, 1);

        return s->current_token;
    }

    switch (curr_char(s)) {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
        case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
        case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '_':
            s->current_token = scan_ident(s);
            return s->current_token;

        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            s->current_token = scan_number(s);
            return s->current_token;

        case '{': return single_char(s, TOKEN_LBRACE);
        case '}': return single_char(s, TOKEN_RBRACE);
        case '[': return single_char(s, TOKEN_LBRACKET);
        case ']': return single_char(s, TOKEN_RBRACKET);
        case '(': return single_char(s, TOKEN_LPAREN);
        case ')': return single_char(s, TOKEN_RPAREN);
        case ',': return single_char(s, TOKEN_COMMA);
        case '.': return single_char(s, TOKEN_DOT);

        case ':': return single_or_double_char(s, '=', TOKEN_DECLARE_ASSIGN, TOKEN_COLON);
        case '>': return single_or_double_char(s, '=', TOKEN_GE, TOKEN_GT);
        case '<': return single_or_double_char(s, '=', TOKEN_LE, TOKEN_LT);
        case '=': return single_or_double_char(s, '=', TOKEN_EQ, TOKEN_ASSIGN);
        case '+': return single_or_double_char(s, '=', TOKEN_ADD_ASSIGN, TOKEN_ADD);  
        case '*': return single_or_double_char(s, '=', TOKEN_MUL_ASSIGN, TOKEN_MUL);
        case '/': return single_or_double_char(s, '=', TOKEN_DIV_ASSIGN, TOKEN_DIV);
        case '!': return single_or_double_char(s, '=', TOKEN_NOT_EQ, TOKEN_NOT);

        case '-': return single_or_double_char_2(s, '>', TOKEN_FUNC_RETURN, '=', TOKEN_SUB_ASSIGN, TOKEN_SUB);
    }

    fprintf(stderr, "Unexpected character \'%c\' in source file.", curr_char(s));
    exit(-10);
}

Scanner *create_test_scanner(char *src) {
    Scanner *s = malloc(sizeof(Scanner));

    if (!s) {
        perror("Unable to create scanner.");
        exit(EOUT_OF_MEMORY);
    }

    s->source_file = NULL;
    s->current_token = 0;

    // Initialise the keywords
    token_initialise();
    
    // Prime the scanner
    next_token(s);

    return s;
}

Scanner *create_scanner_for_source_file(SourceFile *source_file) {
    Scanner *s = malloc(sizeof(Scanner));
    
    if (!s) {
        perror("Unable to create scanner.");
        exit(EOUT_OF_MEMORY);
    }
    
    s->source_file = source_file;
    s->current_token = 0;
    
    // Initialise the keywords
    token_initialise();
    
    // Prime the scanner
    next_token(s);

    return s;
}

void scan_source_file(SourceFile *source_file) {
    Scanner *s = create_scanner_for_source_file(source_file);

    while (s->current_token->kind != TOKEN_EOF) {
        print_token(stderr, s->current_token);
        fprintf(stderr, "\n");
        next_token(s);
    }
}
