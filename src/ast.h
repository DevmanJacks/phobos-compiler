#pragma once

#include "token.h"

typedef enum {
    /* Expressions */
    ASTNODE_BINARY_EXPR,
    ASTNODE_IDENTIFIER,
    ASTNODE_NUMERIC_LITERAL,

    /* Declarations */
    ASTNODE_FUNC_DECL,
    ASTNODE_STRUCT_DECL,
    ASTNODE_VAR_DECL,

    /* Statements */
    ASTNODE_DEFER_STMT,
    ASTNODE_IF_STMT,
    ASTNODE_RETURN_STMT
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

        /* Declarations */
        struct {
            Token *func;
            struct ast_node *ident;
            struct ast_node *parameters;
            struct ast_node *return_type;
            struct ast_node *statements;
        } func_decl;
        struct {
            Token *var;
            struct ast_node *ident;
            struct ast_node *declared_type;
            struct ast_node *init_expr;
        } var_decl;
    };

    // Link to next node in list
    struct ast_node *next;
} AstNode;

// Public functions
extern AstNode *create_binary_expr_astnode(AstNode *left, Token *op, AstNode *right);
extern AstNode *create_identifier_astnode(Token *t);
extern AstNode *create_integer_literal_astnode(Token *t);
extern AstNode *create_var_decl_astnode(Token *var, AstNode *ident, AstNode *declared_type, AstNode *init_expr);
extern void print_astnode(FILE *file, AstNode *node);
