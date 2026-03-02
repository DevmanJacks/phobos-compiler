/*
 * token_test.c
 */

#include "../token.c"
#include "token_test.h"
#include "testing.h"

static void test_operator_precedence() {
    int token_kinds[] = { TOKEN_ADD, TOKEN_SUB, TOKEN_MUL, TOKEN_DIV, TOKEN_EQ, TOKEN_NOT_EQ, TOKEN_LT, TOKEN_LE, TOKEN_GT, TOKEN_GE };
    int precidences[] = { 4, 4 , 5, 5, 3, 3, 3, 3, 3, 3 };
    int num_operators = sizeof(token_kinds) / sizeof(int);
    int num_tests_passed = 0;

    for (int i = 0; i < num_operators; i++) {
        Token *token = create_token(token_kinds[i], 0, 1);

        if (operator_precedence(token) == precidences[i])
            num_tests_passed++;
        else
            test_failed("operator_precedence()", "Precedence of %s should be %d.", token_kind_string(token_kinds[i]), precidences[i]);

        free(token);
    }

    if (num_tests_passed > 0)
        multiple_tests_passed("operator_precedence()", num_tests_passed);
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