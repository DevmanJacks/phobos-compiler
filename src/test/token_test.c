/*
 * token_test.c
 */

#include "../token.c"
#include "token_test.h"
#include "testing.h"

static void test_operator_precedence() {
    Token *add_token = create_token(TOKEN_ADD, 0, 1);

    if (operator_precedence(add_token) != 4) {
        test_failed("operator_precedence()", "Precedence of + should be 4.");
    } else {
        test_passed("operator_precedence()");
    }
}

static void test_token_initialise() {
    // Intern the keywords
    token_initialise();
    InternedString end_of_keywords = intern_string("** END OF KEYWORDS **");

    // All keywords should have an InternedString value less than end_of_keywords
    if (intern_string("const") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'const' not interned.");
    else if (intern_string("defer") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'defer' not interned.");
    else if (intern_string("else") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'else' not interned.");
    else if (intern_string("enum") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'enum' not interned.");
    else if (intern_string("func") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'func' not interned.");
    else if (intern_string("for") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'for' not interned.");
    else if (intern_string("if") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'if' not interned.");
    else if (intern_string("import") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'import' not interned.");
    else if (intern_string("interface") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'interface' not interned.");
    else if (intern_string("struct") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'struct' not interned.");
    else if (intern_string("var") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'var' not interned.");
    else if (intern_string("while") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'while' not interned.");
    else
        test_passed("token_initialise()");
}

extern void token_test() {
    test_section("Tokens");
    test_token_initialise();
    test_operator_precedence();
}