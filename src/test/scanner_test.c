// scanner_test.c - Tests for the scanner.

#include <stdlib.h>
#include <string.h>
#include "../scanner.c"
#include "testing.h"

typedef struct element_t {
    char    *lexeme;
    token_t token;
} element_t;

static element_t test[] = {
    // Identifiers
    { "hello", TOKEN_IDENT },
    { "_", TOKEN_IDENT },
    { "_123", TOKEN_IDENT },
    { "test_var_1", TOKEN_IDENT }
};

static void test_scan()
{
    char buffer[128];
    int count = sizeof(test) / sizeof(element_t);

    for (int i = 0; i < count; i++) {
		source_t *source = add_source((uint8_t *)test[i].lexeme, strlen(test[i].lexeme));
        scanner_t *s = make_scanner(source);
        sprintf(buffer, "\"%s\"", test[i].lexeme);

        token_t token = scan(s);

        if (token == test[i].token) {
            if (token == TOKEN_IDENT) {// || token == TOKEN_INT || token == TOKEN_FLOAT || token == TOKEN_CHAR || token == TOKEN_STR || isKeyword(token)) {
                if (strcmp(s->lexeme, test[i].lexeme) == 0) {
                    test_passed(buffer);
                } else {
                    test_failed(buffer, "Bad lexeme, got: %s", s->lexeme);
                }
            } else {
                test_passed(buffer);
            }
        } else
            test_failed(buffer, "Bad token, expected %d, got %d", test[i].token, token);

        free(s);
		free(source);
    }
}
void scanner_test() {
    test_section("Scanner");
    test_scan();
}