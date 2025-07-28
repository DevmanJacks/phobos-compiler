/*
 * Lexical analyser for the phobos programming language.
 */

enum token_kind {
    TOKEN_EOF = 1,
    TOKEN_IDENTIFIER
};

typedef struct token {
    enum token_kind kind;
    int start_pos;
    int len;
    interned_string lexeme;
} token;

typedef struct scanner {
    char *src;
    int len;
    int pos;

    token *current_token;
} scanner;

#define BUFFER_SIZE 128

token *scan_ident(scanner *s) {
    static char buffer[BUFFER_SIZE];
    int buffer_pos = 0;

    while (isalnum(*(s->src + s->pos)) || *(s->src + s->pos) == '_') {
        buffer[buffer_pos++] = *(s->src + s->pos++);
    }

    buffer[buffer_pos] = 0;

    interned_string ident = intern_string(buffer);

    token *new_token = malloc(sizeof(token));

    if (!new_token) {
        perror("Unable to create token.");
        exit(EOUT_OF_MEMORY);
    }

    new_token->kind = TOKEN_IDENTIFIER;
    new_token->lexeme = ident;

    return new_token;
}

void skip_whitespace(scanner *s) {
    while (*(s->src + s->pos) == ' ' || *(s->src + s->pos) == '\t' || *(s->src + s->pos) == '\n')
        s->pos++;
}

token *next_token(scanner *s) {
    skip_whitespace(s);

    if (s->pos == s->len) {
        if (s->current_token->kind == TOKEN_EOF)
            return s->current_token;

        token *new_token = malloc(sizeof(token));

        if (!new_token) {
            perror("Unable to create token.");
            exit(EOUT_OF_MEMORY);
        }

        new_token->kind = TOKEN_EOF;
        new_token->start_pos = s->pos;
        new_token->len = 1;

        return new_token;
    }

    if (isalpha(*(s->src + s->pos)) || *(s->src + s->pos) == '_') {
        s->current_token = scan_ident(s);
        return s->current_token;
    }

    fprintf(stderr, "Unexpected character \'%c\' in source file.", *(s->src + s->pos));
    exit(EUNEXPECTED_CHARACTER);
}

scanner *create_scanner(char *src) {
    scanner *s = malloc(sizeof(scanner));

    if (!s) {
        perror("Unable to create scanner.");
        exit(EOUT_OF_MEMORY);
    }

    s->src = src;
    s->len = strlen(src);
    s->pos = 0;

    s->current_token = 0;

    return s;
}