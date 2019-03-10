// parser_test.c - Tests for the parser.

#include <stdint.h>
#include <string.h>
#include "../parser.c"
#include "parser_test.h"
#include "../scanner.h"
#include "testing.h"

// Helpers to create (and free) a parser from a string
parser_t *create_parser(char *src)
{
    source_t *source = add_source((uint8_t *)src, strlen(src));
    scanner_t *s = make_scanner(source);
    parser_t  *p = make_parser(s);

    // suppressPrintingSyntaxErrors(p);

    return p;
}

static void release_parser(parser_t *p) {
	free(p->scanner->source);
    free(p->scanner);
    free(p);
}

// Buffer for parse tree results
#define BUFLEN 1024

static char buf[BUFLEN];
parser_t *p;

#define test_parse(what, src, expected) \
    { \
        parser_t *p = create_parser(src); \
        what##_t *what = parse_##what(p); \
        print_##what(buf, BUFLEN, what); \
        if (strcmp(expected, buf) == 0) { \
            test_passed("parse_" #what "(" #src ")"); \
        } else { \
            test_failed("parse_" #what "(" #src ")", "Expected: %s, got: %s", expected, buf); \
        } \
        release_parser(p); \
    }

static void test_parse_expr(void) {
    test_parse(expr, "1", "(literal INT 1)")
}

static void test_parse_ident(void) {
    test_parse(ident, "pi", "(ident pi)")
}

void parser_test(void) {
    test_section("Parser");
    test_parse_ident();
    test_parse_expr();
}