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

static void test_parse_identifier_expr() {
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
    
        if (!verify_node("parse_identifier_expr()", node, node_strings[i]))
            num_failed++;
    }

    if (num_failed < 5)
        multiple_tests_passed("parse_identifier_expr()", num_tests - num_failed);
}

extern void parser_test() {
    test_section("Parser");
    test_create_parser();

    test_section("Parser - Expressions");
    test_parse_identifier_expr();
}