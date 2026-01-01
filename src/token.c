/*
 * Everything tokens.
 */

#include <stdio.h>
#include "error.h"
#include "token.h"

unsigned int operator_precedence(Token *op_token) {
    TokenKind operator = op_token->kind;

    if (TOKEN_MUL_START <= operator && operator <= TOKEN_MUL_END)
        return 5;

    if (TOKEN_ADD_START <= operator && operator <= TOKEN_ADD_END)
        return 4;

    if (TOKEN_CMP_START <= operator && operator <= TOKEN_CMP_END)
        return 3;

    return 1;
}

extern void token_initialise() {
    // Intern all of the keywords
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
    intern_string("while");
}

extern Token *create_token(TokenKind kind, int start, int len) {
    Token *new_token = malloc(sizeof(Token));

    if (!new_token) {
        perror("Unable to create token.");
        exit(EOUT_OF_MEMORY);
    }
 
    new_token->kind = kind;
    new_token->start = start;
    new_token->len = len;

    return new_token;
}

extern char *token_kind_string(TokenKind kind) {
    switch (kind) {
        case TOKEN_CONST: return "CONST";
        case TOKEN_DEFER: return "DEFER";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_ENUM: return "ENUM";
        case TOKEN_FUNC: return "FUNC";
        case TOKEN_FOR: return "FOR";
        case TOKEN_IF: return "IF";
        case TOKEN_IMPORT: return "IMPORT";
        case TOKEN_INTERFACE: return "INTERFACE";
        case TOKEN_STRUCT: return "STRUCT";
        case TOKEN_VAR: return "VAR";
        case TOKEN_WHILE: return "WHILE";

        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER_LITERAL: return "INTEGER_LITERAL";

        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        
        // Assignment
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_DECLARE_ASSIGN: return "DECLARE_ASSIGN";
        case TOKEN_ADD_ASSIGN: return "ADD_ASSIGN";
        case TOKEN_SUB_ASSIGN: return "SUBTRACT_ASSIGN";
        case TOKEN_MUL_ASSIGN: return "MULTIPLY_ASSIGN";
        case TOKEN_DIV_ASSIGN: return "DIVIDE_ASSIGN";

        // Arithmetic operators
        case TOKEN_ADD: return "ADD";
        case TOKEN_SUB: return "SUBTRACT";
        case TOKEN_MUL: return "MULTIPLY";
        case TOKEN_DIV: return "DIVIDE";

        // Boolean operators
        case TOKEN_EQ: return "EQUAL";
        case TOKEN_GE: return "GREATER_THAN_OR_EQUAL";
        case TOKEN_GT: return "GREATER_THAN";
        case TOKEN_LE: return "LESS_THAN_OR_EQUAL";
        case TOKEN_LT: return "LESS_THAN";
        
        case TOKEN_EOF: return "EOF";

        default: return "UNKNOWN";
    }
}

extern void print_token(FILE *file, Token *t) {
    fprintf(file, "{ \"Token\": \"%s\", \"start\": %d, \"len\": %d", token_kind_string(t->kind), t->start, t->len);

    switch (t->kind) {
        case TOKEN_IDENTIFIER:
            fprintf(file, ", \"identifier\": \"%s\" }", interned_string_value(t->ident));
            break;

        case TOKEN_INTEGER_LITERAL:
            fprintf(file, ", \"value\": %d }", t->integer_literal);
            break;

        default:
            fprintf(file, " }");
            break;
    }
}