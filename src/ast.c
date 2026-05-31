/*
 * Abstract Syntax Tree functionality for Phobos programming language.
 */

#include "ast.h"

char *astnode_type_string(AstNodeType type) {
    switch (type) {
        case ASTNODE_BINARY_EXPR: return "Binary Expr";
        case ASTNODE_IDENTIFIER: return "Identifier";
        case ASTNODE_NUMERIC_LITERAL: return "Numeric Literal";
        case ASTNODE_VAR_DECL: return "Variable Declaration";

        default: return "UNKNOWN";
    }
}

int astnode_start(AstNode *node) {
    switch (node->type) {
        case ASTNODE_BINARY_EXPR:
            return 0;

        case ASTNODE_VAR_DECL:
            return node->var_decl.var->start;

        default:
            return node->token->start;
    }
}

int astnode_len(AstNode *node) {
    switch (node->type) {
        case ASTNODE_BINARY_EXPR:        
            while (node->binary_expr.right->type == ASTNODE_BINARY_EXPR)
                node = node->binary_expr.right;

            return astnode_start(node->binary_expr.right) + astnode_len(node->binary_expr.right);

        case ASTNODE_VAR_DECL:
            if (node->var_decl.init_expr)
                return astnode_start(node->var_decl.init_expr) - node->var_decl.var->start + astnode_len(node->var_decl.init_expr);

            return astnode_start(node->var_decl.ident) - node->var_decl.var->start + astnode_len(node->var_decl.ident);

        default:
            return node->token->len;
    }
}

AstNode *create_binary_expr_astnode(AstNode *left, Token *op, AstNode *right) {
        AstNode *node = malloc(sizeof(AstNode));

    if (!node) {
        perror("Unable to create ast node for binary expression.");
        exit(EOUT_OF_MEMORY);
    }

    node->type = ASTNODE_BINARY_EXPR;
    node->binary_expr.left = left;
    node->binary_expr.op = op;
    node->binary_expr.right = right;
}

AstNode *create_identifier_astnode(Token *t) {
    AstNode *node = malloc(sizeof(AstNode));

    if (!node) {
        perror("Unable to create ast node for identifier.");
        exit(EOUT_OF_MEMORY);
    }

    node->type = ASTNODE_IDENTIFIER;
    node->token = t;
}

AstNode *create_integer_literal_astnode(Token *t) {
    AstNode *node = malloc(sizeof(AstNode));

    if (!node) {
        perror("Unable to create ast node for integer literal.");
        exit(EOUT_OF_MEMORY);
    }

    node->type = ASTNODE_NUMERIC_LITERAL;
    node->token = t;
}

AstNode *create_var_decl_astnode(Token *var, AstNode *ident, AstNode *init_expr) {
        AstNode *node = malloc(sizeof(AstNode));

    if (!node) {
        perror("Unable to create ast node for variable declaration.");
        exit(EOUT_OF_MEMORY);
    }

    node->type = ASTNODE_VAR_DECL;
    node->var_decl.var = var;
    node->var_decl.ident = ident;
    node->var_decl.init_expr = init_expr;
}

void print_astnode(FILE *file, AstNode *node) {
    fprintf(file, "{ \"node\": \"%s\", \"start\": %d, \"len\": %d", astnode_type_string(node->type), astnode_start(node), astnode_len(node));

    switch (node->type) {
        case ASTNODE_BINARY_EXPR:
            fprintf(file, ", \"left\": ");
            print_astnode(file, node->binary_expr.left);
            fprintf(file, ", \"op\": ");
            print_token(file, node->binary_expr.op);
            fprintf(file, ", \"right\": ");
            print_astnode(file, node->binary_expr.right);
            break;

        case ASTNODE_IDENTIFIER:
            fprintf(file, ", \"name\": \"%s\" ", interned_string_value(node->token->ident));
            break;

        case ASTNODE_NUMERIC_LITERAL:
            fprintf(file, ", \"value\": %d ", node->token->integer_literal);
            break;

        case ASTNODE_VAR_DECL:
            fprintf(file, ", \"ident\": ");
            print_astnode(file, node->var_decl.ident);

            if (node->var_decl.init_expr) {
                fprintf(file, ", \"init_expr\": ");
                print_astnode(file, node->var_decl.init_expr);
            }

            break;

        default:
            fprintf(file, "UNKNOWN (%d)", node->type);
            break;
    }

    fprintf(file, "}");
}
