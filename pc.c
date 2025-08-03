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
#include "token.c"
#include "scanner.c"

void main(int argc, char **argv) {
    string_intern_initialise();
    token_initialise();
    
    //Scanner *s = create_scanner("const defer else enum func for if import interface struct var while");
    //Scanner *s = create_scanner("if a > 3 b = true else b = false");
    Scanner *s = create_scanner("a += 1");

    while (next_token(s)->kind != TOKEN_EOF) {
        print_token(stdout, s->current_token, s->src);
        printf("\n");
    }
}