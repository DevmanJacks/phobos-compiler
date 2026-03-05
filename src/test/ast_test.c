/*
 * ast_test.c
 */

#include "../ast.c"
#include "ast_test.h"
#include "testing.h"

static void test_create_binary_expr_astnode() {
    AstNode *left = create_identifier_astnode(create_token(TOKEN_IDENTIFIER, 0, 1));
    AstNode *right = create_integer_literal_astnode(create_token(TOKEN_INTEGER_LITERAL, 4, 1));
    Token *op = create_token(TOKEN_ADD, 2, 1);

    AstNode *ast = create_binary_expr_astnode(left, op, right);

    if (!ast) {
        test_failed("create_binary_expr_astnode()", "Unable to create AST node");
        return;
    }

    if (ast->type != ASTNODE_BINARY_EXPR) {
        test_failed("create_binary_expr_astnode()", "Bad AST type - expected: %s, got: %s", astnode_type_string(ASTNODE_BINARY_EXPR), astnode_type_string(ast->type));
        return;
    }

    if (ast->binary_expr.left != left) {
        test_failed("create_binary_expr_astnode()", "Bad binary_expr.left");
        return;
    }

    if (ast->binary_expr.right != right) {
        test_failed("create_binary_expr_astnode()", "Bad binary_expr.right");
        return;
    }

    if (ast->binary_expr.op != op) {
        test_failed("create_binary_expr_astnode()", "Bad binary_expr.op");
        return;
    }

    test_passed("create_binary_expr_node()");
}

extern void ast_test() {
    test_section("Abstract Syntax Tree");
    test_create_binary_expr_astnode();
}