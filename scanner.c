/*
 * Lexical analyser for the phobos programming language.
 */

typedef struct Scanner {
    char *src;
    int len;
    int pos;

    Token *current_token;
} Scanner;

#define BUFFER_SIZE 128

Token *scan_ident(Scanner *s) {
    static char buffer[BUFFER_SIZE];
    int buffer_pos = 0;
    int start = s->pos;

    while (isalnum(*(s->src + s->pos)) || *(s->src + s->pos) == '_') {
        buffer[buffer_pos++] = *(s->src + s->pos++);
    }

    buffer[buffer_pos] = 0;

    interned_string ident = intern_string(buffer);

    Token *t = create_token(ident < TOKEN_IDENTIFIER ? ident : TOKEN_IDENTIFIER, start, buffer_pos);
    t->ident = ident;
    return t;
}

Token *scan_number(Scanner *s) {
    long value = 0;
    int start = s->pos;

    while (isdigit(*(s->src + s->pos))) {
        char digit = *(s->src + s->pos++) - '0';
        value = value * 10 + digit;
    }

    Token *t = create_token(TOKEN_INTEGER_LITERAL, start, s->pos - start);
    t->integer_literal = value;
    return t;
}

void skip_whitespace(Scanner *s) {
    while (*(s->src + s->pos) == ' ' || *(s->src + s->pos) == '\t' || *(s->src + s->pos) == '\n')
        s->pos++;
}

Token *single_char(Scanner *s, TokenKind kind) {
    s->current_token = create_token(kind, s->pos, 1);
    s->pos++;
    return s->current_token;
}

Token *single_or_double_char(Scanner *s, char next_char, TokenKind next_char_token_kind, TokenKind kind) {
    int start = s->pos++;

    if (*(s->src + s->pos) == next_char) {
        s->pos++;
        s->current_token = create_token(next_char_token_kind, start, 2);
    }
    else
        s->current_token = create_token(kind, start, 1);

    return s->current_token;
}

Token *next_token(Scanner *s) {
    skip_whitespace(s);

    if (s->pos == s->len) {
        if (s->current_token->kind == TOKEN_EOF)
            return s->current_token;

        return create_token(TOKEN_EOF, s->pos, 1);
    }

    char c = *(s->src + s->pos);

    switch (c) {
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

        case '{':
            return single_char(s, TOKEN_LBRACE);

        case '}':
            return single_char(s, TOKEN_RBRACE);

        case '[':
            return single_char(s, TOKEN_LBRACKET);

        case ']':
            return single_char(s, TOKEN_RBRACKET);

        case '(':
            return single_char(s, TOKEN_LPAREN);

        case ')':
            return single_char(s, TOKEN_RPAREN);

        case '>':
            return single_or_double_char(s, '=', TOKEN_GE, TOKEN_GT);

        case '<':
            return single_or_double_char(s, '=', TOKEN_LE, TOKEN_LT);

        case '=':
            return single_or_double_char(s, '=', TOKEN_EQ, TOKEN_ASSIGN);

        case '+':
            return single_or_double_char(s, '=', TOKEN_ADD_ASSIGN, TOKEN_ADD);

        case '-':
            return single_or_double_char(s, '=', TOKEN_SUB_ASSIGN, TOKEN_SUB);

        case '*':
            return single_or_double_char(s, '=', TOKEN_MUL_ASSIGN, TOKEN_MUL);

        case '/':
            return single_or_double_char(s, '=', TOKEN_DIV_ASSIGN, TOKEN_DIV);
    }

    fprintf(stderr, "Unexpected character \'%c\' in source file.", c);
    exit(EUNEXPECTED_CHARACTER);
}

Scanner *create_scanner(char *src) {
    Scanner *s = malloc(sizeof(Scanner));

    if (!s) {
        perror("Unable to create scanner.");
        exit(EOUT_OF_MEMORY);
    }

    s->src = src;
    s->len = strlen(src);
    s->pos = 0;

    s->current_token = 0;

    // Prime the scanner
    next_token(s);

    return s;
}
