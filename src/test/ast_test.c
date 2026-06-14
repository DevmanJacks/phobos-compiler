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

static void test_create_identifier_astnode() {
    AstNode *node = create_identifier_astnode(create_token(TOKEN_IDENTIFIER, 0, 1));

    if (!node) {
        test_failed("create_identifier_astnode()", "Unable to create AST node");
        return;
    }

    if (node->type != ASTNODE_IDENTIFIER) {
        test_failed("create_identifier_astnode()", "Bad AST type - expected: %s, got: %s", astnode_type_string(ASTNODE_IDENTIFIER), astnode_type_string(node->type));
        return;
    }

    test_passed("create_identifier_astnode()");
}

static void test_create_integer_literal_astnode() {
    AstNode *node = create_integer_literal_astnode(create_token(TOKEN_INTEGER_LITERAL, 4, 1));

    if (!node) {
        test_failed("create_integer_literal_astnode()", "Unable to create AST node");
        return;
    }

    if (node->type != ASTNODE_NUMERIC_LITERAL) {
        test_failed("create_integer_literal_astnode()", "Bad AST type - expected: %s, got: %s", astnode_type_string(ASTNODE_NUMERIC_LITERAL), astnode_type_string(node->type));
        return;
    }

    test_passed("create_integer_literal_astnode()");
}

static void test_create_var_decl_astnode() {
    Token *var = create_token(TOKEN_VAR, 0, 3);
    AstNode *ident = create_identifier_astnode(create_token(TOKEN_IDENTIFIER, 4, 1));
    AstNode *declared_type = create_identifier_astnode(create_token(TOKEN_IDENTIFIER, 7, 3));
    AstNode *init_expr = create_integer_literal_astnode(create_token(TOKEN_INTEGER_LITERAL, 13, 2));

    AstNode *decl = create_var_decl_astnode(var, ident, declared_type, init_expr);

    if (!decl) {
        test_failed("create_var_decl_astnode()", "Unable to create AST node");
        return;
    }

    if (decl->type != ASTNODE_VAR_DECL) {
        test_failed("create_var_decl_astnode()", "Bad AST type - expected: %s, got: %s", astnode_type_string(ASTNODE_NUMERIC_LITERAL), astnode_type_string(decl->type));
    }

    if (decl->var_decl.var != var) {
        test_failed("create_var_decl_astnode()", "Start of node not set or set incorrectly.");
        return;
    }

    if (decl->var_decl.ident != ident) {
        test_failed("create_var_decl_astnode()", "Identifier not set or set incorrectly.");
        return;
    }

    if (decl->var_decl.declared_type != declared_type) {
        test_failed("create_var_decl_astnode()", "Declared type not set or set incorrectly.");
        return;
    }

    if (decl->var_decl.init_expr != init_expr) {
        test_failed("create_var_decl_astnode()", "Initial expression not set or set incorrectly.");
        return;
    }

    return test_passed("create_var_decl_astnode()");
}

extern void ast_test() {
    test_section("Abstract Syntax Tree");
    test_create_identifier_astnode();
    test_create_integer_literal_astnode();
    test_create_binary_expr_astnode();
    test_create_var_decl_astnode();
}