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

extern void scanner_test() {
    test_section("Scanner");
    test_create_scanner();
}