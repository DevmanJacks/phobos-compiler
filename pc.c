/*
 * Phobos compiler entry point.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "error.c"
#include "intern.c"
#include "scanner.c"

void main(int argc, char **argv) {
    string_intern_initialise();

    // Intern all of the keywords
    char *keywords[] = { "else", "if" };

    for (int i = 0; i < 2; i++)
        intern_string(keywords[i]);

    scanner *s = create_scanner("This is a test.");

    while (next_token(s)->kind != TOKEN_EOF) {
        printf("%d,%d %d\n", s->current_token->start_pos, s->current_token->len, s->current_token->lexeme);
    }
}