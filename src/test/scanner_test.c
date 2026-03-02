/*
 * scanner_test.c
 */

#include "scanner_test.h"
#include "../scanner.c"
#include "testing.h"

static void test_create_scanner() {
    char *src = "+";
    size_t length = 1;

    Scanner *s = create_scanner(src);

    if (s->src = src) {
        if (s->len = strlen(src)) {
            test_passed("create_scanner()");
        } else {
            test_failed("create_scanner()", "Bad length - expected: %ld, got: %ld", length, s->len);
        }
    } else {
        test_failed("create_scanner()", "Bad src");
    }
}

static void test_multi_character_tokens() {
    char *src = ":= += -= *= /= >= <=";
    int token_type[] = {TOKEN_DECLARE_ASSIGN, TOKEN_ADD_ASSIGN, TOKEN_SUB_ASSIGN, TOKEN_MUL_ASSIGN, TOKEN_DIV_ASSIGN, TOKEN_GE, TOKEN_LE};
                        
    int num_tokens = sizeof(token_type) / sizeof(int);

    Scanner *s = create_scanner(src);
    bool tests_passed = true;

    for (int i = 0; i < num_tokens; i++) {
        if (s->current_token->kind != token_type[i]) {
            test_failed("test_multi_character_tokens()", "Bad token - expected: %s, got: %s", token_kind_string(token_type[i]), token_kind_string(s->current_token->kind));
            tests_passed = false;
        }

        next_token(s);
    }

    if (tests_passed)
        test_passed("test_multi_character_tokens()");
}

static void test_single_character_tokens() {
    char *src = "{}[]():+-*/ =";
    int token_type[] = {TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LBRACKET, TOKEN_RBRACKET, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_COLON, TOKEN_ADD, TOKEN_SUB, TOKEN_MUL, 
                        TOKEN_DIV, TOKEN_ASSIGN};
                        
    int num_tokens = sizeof(token_type) / sizeof(int);

    Scanner *s = create_scanner(src);
    bool tests_passed = true;

    for (int i = 0; i < num_tokens; i++) {
        if (s->current_token->kind != token_type[i]) {
            test_failed("test_single_character_tokens()", "Bad token - expected: %s, got: %s", token_kind_string(token_type[i]), token_kind_string(s->current_token->kind));
            tests_passed = false;
        }

        next_token(s);
    }

    if (tests_passed)
        test_passed("test_single_character_tokens()");
}

extern void scanner_test() {
    test_section("Scanner");
    test_create_scanner();
    test_single_character_tokens();
    test_multi_character_tokens();
}