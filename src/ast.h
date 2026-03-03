#pragma once

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
extern void print_astnode(FILE *file, AstNode *node);
