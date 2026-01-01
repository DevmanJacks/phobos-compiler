/*
 * token_test.c
 */

#include "../token.c"
#include "testing.h"

static void test_token_initialise() {
    token_initialise();

    // Ensure all keywords are interned
    InternedString end_of_keywords = intern_string("** END OF KEYWORDS **");
    intern_string("const");
    intern_string("defer");
    intern_string("else");
    intern_string("enum");
    intern_string("func");
    intern_string("for");
    intern_string("if");
    intern_string("import");
    intern_string("interface");
    intern_string("struct");
    intern_string("var");

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
    else if (intern_string("while") > end_of_keywords)
        test_failed("token_initialise()", "Keyword 'while' not interned.");
    else
        test_passed("token_initialise()");
}

extern void token_test() {
    test_section("Tokens");
    test_token_initialise();
}