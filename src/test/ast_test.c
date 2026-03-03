/*
 * ast_test.c
 */

#include "../ast.c"
#include "ast_test.h"
#include "testing.h"

static void test_create_binary_expr_astnode() {

}

extern void ast_test() {
    test_section("Abstract Syntax Tree");
    test_create_binary_expr_astnode();
}