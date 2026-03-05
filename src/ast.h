#pragma once

#include "ast.h"
#include "error.h"
#include "token.h"

 typedef enum {
    ASTNODE_BINARY_EXPR,
    ASTNODE_IDENTIFIER,
    ASTNODE_NUMERIC_LITERAL
 } AstNodeType;

typedef struct ast_node {
    AstNodeType type;
    union {
        struct {
            struct ast_node *left;
            Token *op;
            struct ast_node *right;
        } binary_expr;
        Token *token;
    };
} AstNode;

// Public functions
extern AstNode *create_binary_expr_astnode(AstNode *left, Token *op, AstNode *right);
extern AstNode *create_identifier_astnode(Token *t);
extern AstNode *create_integer_literal_astnode(Token *t);
extern void print_astnode(FILE *file, AstNode *node);
