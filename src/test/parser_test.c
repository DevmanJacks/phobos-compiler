/*
 * parser_test.c
 */

#include <string.h>
#include "parser_test.h"
#include "../parser.c"
#include "testing.h"

// Helper functions
static Parser *create_test_parser(char *src) {
    Scanner *s = create_scanner(src);
    Parser *p = create_parser(s);

    return p;
}

static bool verify_node(char *fn, AstNode *node, char *expected_output) {
    FILE *tmp = tmpfile();
    print_astnode(tmp, node);

    size_t length = ftell(tmp);
    rewind(tmp);

    char *node_as_string = malloc(length + 1);
    
    if (!node_as_string) {
        perror("verify_node()");
        exit(EOUT_OF_MEMORY);
    }

    fread(node_as_string, length, 1, tmp);
    fclose(tmp);
    
    if (strcmp(node_as_string, expected_output) == 0)
        return true;

    test_failed(fn, "Bad AST node - expected: %s, got: %s", expected_output, node_as_string);
    return false;
}

static void test_create_parser() {
    char *src = "a := 1";
    Scanner *s = create_scanner(src);
    Parser *p = create_parser(s);

    if (p->scanner == s) {
        test_passed("create_parser()");
    } else {
        test_failed("create_parser()", "Bad scanner");
    }
}

// Expressions

static void test_parse_binary_expr() {
    char *src[] = { "a + 1", "val - 2 * 3", "x / y" };
    char *node_strings[] = { "{ \"node\": \"Binary Expr\", \"start\": 0, \"len\": 5, \"left\": { \"node\": \"Identifier\", \"start\": 0, \"len\": 1, \"name\": \"a\" }, \"op\": { \"Token\": \"ADD\", \"start\": 2, \"len\": 1 }, \"right\": { \"node\": \"Numeric Literal\", \"start\": 4, \"len\": 1, \"value\": 1 }}",
                             "{ \"node\": \"Binary Expr\", \"start\": 0, \"len\": 11, \"left\": { \"node\": \"Identifier\", \"start\": 0, \"len\": 3, \"name\": \"val\" }, \"op\": { \"Token\": \"SUBTRACT\", \"start\": 4, \"len\": 1 }, \"right\": { \"node\": \"Binary Expr\", \"start\": 0, \"len\": 11, \"left\": { \"node\": \"Numeric Literal\", \"start\": 6, \"len\": 1, \"value\": 2 }, \"op\": { \"Token\": \"MULTIPLY\", \"start\": 8, \"len\": 1 }, \"right\": { \"node\": \"Numeric Literal\", \"start\": 10, \"len\": 1, \"value\": 3 }}}",
                             "{ \"node\": \"Binary Expr\", \"start\": 0, \"len\": 5, \"left\": { \"node\": \"Identifier\", \"start\": 0, \"len\": 1, \"name\": \"x\" }, \"op\": { \"Token\": \"DIVIDE\", \"start\": 2, \"len\": 1 }, \"right\": { \"node\": \"Identifier\", \"start\": 4, \"len\": 1, \"name\": \"y\" }}" };

    int num_tests = sizeof(src) / sizeof(char *);
    int num_failed = 0;

    for (int i = 0; i < num_tests; i++) {
        Parser *p = create_test_parser(src[i]);
    
        AstNode *node = parse_expr(p);
    
        if (!verify_node("parse_binary_expr()", node, node_strings[i]))
            num_failed++;
    }

    if (num_failed < 5)
        multiple_tests_passed("parse_binary_expr()", num_tests - num_failed);
}

static void test_parse_identifier() {
    char *src[] = { "_", "newIdentifier", "_item", "i", "_var_" };
    char *node_strings[] = { "{ \"node\": \"Identifier\", \"start\": 0, \"len\": 1, \"name\": \"_\" }",
                             "{ \"node\": \"Identifier\", \"start\": 0, \"len\": 13, \"name\": \"newIdentifier\" }",
                             "{ \"node\": \"Identifier\", \"start\": 0, \"len\": 5, \"name\": \"_item\" }",
                             "{ \"node\": \"Identifier\", \"start\": 0, \"len\": 1, \"name\": \"i\" }",
                             "{ \"node\": \"Identifier\", \"start\": 0, \"len\": 5, \"name\": \"_var_\" }" };

    int num_tests = sizeof(src) / sizeof(char *);
    int num_failed = 0;

    for (int i = 0; i < num_tests; i++) {
        Parser *p = create_test_parser(src[i]);
    
        AstNode *node = parse_expr(p);
    
        if (!verify_node("parse_identifier()", node, node_strings[i]))
            num_failed++;
    }

    if (num_failed < 5)
        multiple_tests_passed("parse_identifier()", num_tests - num_failed);
}

static void test_parse_number() {
    char *src[] = { "0", "1", "256" };
    char *node_strings[] = { "{ \"node\": \"Numeric Literal\", \"start\": 0, \"len\": 1, \"value\": 0 }",
                             "{ \"node\": \"Numeric Literal\", \"start\": 0, \"len\": 1, \"value\": 1 }",
                             "{ \"node\": \"Numeric Literal\", \"start\": 0, \"len\": 3, \"value\": 256 }" };

    int num_tests = sizeof(src) / sizeof(char *);
    int num_failed = 0;

    for (int i = 0; i < num_tests; i++) {
        Parser *p = create_test_parser(src[i]);
    
        AstNode *node = parse_expr(p);
    
        if (!verify_node("parse_number()", node, node_strings[i]))
            num_failed++;
    }

    if (num_failed < 5)
        multiple_tests_passed("parse_number()", num_tests - num_failed);
}

extern void parser_test() {
    test_section("Parser");
    test_create_parser();

    test_section("Parser - Expressions");
    test_parse_identifier();
    test_parse_number();
    test_parse_binary_expr();
}