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

static void test_identifier_token() {
    char *src = "ident _test i __wibble";
    Scanner *s = create_scanner(src);
    int tests_failed = 0;
    int tests_passed = 0;

    while (s->current_token->kind != TOKEN_EOF) {
        if (s->current_token->kind != TOKEN_IDENTIFIER) {
            test_failed("test_identifier_token()", "Bad token - expected: %s, got: %s", token_kind_string(TOKEN_IDENTIFIER), token_kind_string(s->current_token->kind));
            tests_failed++;
        }
        else
            tests_passed++;

        next_token(s);
    }

    if (tests_passed > 0)
        multiple_tests_passed("test_identifier_token()", tests_passed);
}

static void test_keyword_tokens() {
    char *src = "const defer else enum func for if import interface struct var while";
    int token_type[] = { TOKEN_CONST, TOKEN_DEFER, TOKEN_ELSE, TOKEN_ENUM, TOKEN_FUNC, TOKEN_FOR, TOKEN_IF, TOKEN_IMPORT, TOKEN_INTERFACE,
                         TOKEN_STRUCT, TOKEN_VAR, TOKEN_WHILE };
                        
    int num_tokens = sizeof(token_type) / sizeof(int);

    Scanner *s = create_scanner(src);
    int tests_failed = 0;

    for (int i = 0; i < num_tokens; i++) {
        if (s->current_token->kind != token_type[i]) {
            test_failed("test_keyword_tokens()", "Bad token - expected: %s, got: %s", token_kind_string(token_type[i]), token_kind_string(s->current_token->kind));
            tests_failed++;
        }

        next_token(s);
    }

    if (num_tokens - tests_failed > 0)
        multiple_tests_passed("test_keyword_tokens()", num_tokens - tests_failed);
}

static void test_multi_character_tokens() {
    char *src = ":= += -= *= /= >= <=";
    int token_type[] = {TOKEN_DECLARE_ASSIGN, TOKEN_ADD_ASSIGN, TOKEN_SUB_ASSIGN, TOKEN_MUL_ASSIGN, TOKEN_DIV_ASSIGN, TOKEN_GE, TOKEN_LE};
                        
    int num_tokens = sizeof(token_type) / sizeof(int);

    Scanner *s = create_scanner(src);
    int tests_failed = 0;

    for (int i = 0; i < num_tokens; i++) {
        if (s->current_token->kind != token_type[i]) {
            test_failed("test_multi_character_tokens()", "Bad token - expected: %s, got: %s", token_kind_string(token_type[i]), token_kind_string(s->current_token->kind));
            tests_failed++;
        }

        next_token(s);
    }

    if (num_tokens - tests_failed > 0)
        multiple_tests_passed("test_multi_character_tokens()", num_tokens - tests_failed);
}

static void test_number_tokens() {
    char *src = "0 1";
    int token_types[] = { TOKEN_INTEGER_LITERAL, TOKEN_INTEGER_LITERAL };
    int token_values[] = { 0, 1 };
                        
    int num_tokens = sizeof(token_types) / sizeof(int);

    Scanner *s = create_scanner(src);
    int tests_failed = 0;

    for (int i = 0; i < num_tokens; i++) {
        if (s->current_token->kind != token_types[i]) {
            test_failed("test_number_tokens()", "Bad token - expected: %s, got: %s", token_kind_string(token_types[i]), token_kind_string(s->current_token->kind));
            tests_failed++;
        } else if (s->current_token->kind == TOKEN_INTEGER_LITERAL && s->current_token->integer_literal != token_values[i]) {
            test_failed("test_number_tokens()", "Bad token value - expected: %d, got: %d", token_values[i], s->current_token->integer_literal);
            tests_failed++;
        }

        next_token(s);
    }

    if (num_tokens - tests_failed > 0)
        multiple_tests_passed("test_number_tokens()", num_tokens - tests_failed);
}

static void test_single_character_tokens() {
    char *src = "{}[]():+-*/ =";
    int token_type[] = {TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_LBRACKET, TOKEN_RBRACKET, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_COLON, TOKEN_ADD, TOKEN_SUB, TOKEN_MUL, 
                        TOKEN_DIV, TOKEN_ASSIGN};
                        
    int num_tokens = sizeof(token_type) / sizeof(int);

    Scanner *s = create_scanner(src);
    int tests_failed = 0;

    for (int i = 0; i < num_tokens; i++) {
        if (s->current_token->kind != token_type[i]) {
            test_failed("test_single_character_tokens()", "Bad token - expected: %s, got: %s", token_kind_string(token_type[i]), token_kind_string(s->current_token->kind));
            tests_failed++;
        }

        next_token(s);
    }

    if (num_tokens - tests_failed > 0)
        multiple_tests_passed("test_single_character_tokens()", num_tokens - tests_failed);
}

extern void scanner_test() {
    test_section("Scanner");
    test_create_scanner();
    test_single_character_tokens();
    test_multi_character_tokens();
    test_identifier_token();
    test_keyword_tokens();
    test_number_tokens();
}