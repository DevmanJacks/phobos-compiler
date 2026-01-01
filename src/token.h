#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "intern.h"
#include "source.h"

typedef enum {
    TOKEN_CONST = 0,
    TOKEN_DEFER = 6,
    TOKEN_ELSE = 12,
    TOKEN_ENUM = 17,
    TOKEN_FUNC = 22,
    TOKEN_FOR = 27,
    TOKEN_IF = 31,
    TOKEN_IMPORT = 34,
    TOKEN_INTERFACE = 41,
    TOKEN_STRUCT = 51,
    TOKEN_VAR = 58,
    TOKEN_WHILE = 62,

    TOKEN_IDENTIFIER,
    TOKEN_INTEGER_LITERAL,

    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,

    // Multiplication precedence
	TOKEN_BINARY_OP_START,
	TOKEN_MUL_START = TOKEN_BINARY_OP_START,
	TOKEN_MUL = TOKEN_MUL_START,
	TOKEN_DIV,
	TOKEN_MOD,
	TOKEN_LSHIFT,
	TOKEN_RSHIFT,
	TOKEN_BITWISE_AND,
	TOKEN_MUL_END = TOKEN_BITWISE_AND,

	// Addition precedence
	TOKEN_ADD_START,
	TOKEN_ADD = TOKEN_ADD_START,
	TOKEN_SUB,
	TOKEN_BITWISE_OR,
	TOKEN_XOR,
	TOKEN_ADD_END = TOKEN_XOR,

	// Comparative precedence
	TOKEN_CMP_START,
	TOKEN_EQ = TOKEN_CMP_START,
	TOKEN_NOT_EQ,
	TOKEN_LT,
	TOKEN_LE,
	TOKEN_GT,
	TOKEN_GE,
	TOKEN_CMP_END = TOKEN_GE,

    TOKEN_AND,
	TOKEN_OR,
	TOKEN_BINARY_OP_END = TOKEN_OR,

    // Assignment operators
	TOKEN_ASSIGN_OP_START,
	TOKEN_ASSIGN = TOKEN_ASSIGN_OP_START,
	TOKEN_DECLARE_ASSIGN,
	TOKEN_ADD_ASSIGN,
	TOKEN_SUB_ASSIGN,
	TOKEN_MUL_ASSIGN,
	TOKEN_DIV_ASSIGN,
	TOKEN_MOD_ASSIGN,
	TOKEN_LSHIFT_ASSIGN,
	TOKEN_RSHIFT_ASSIGN,
	TOKEN_BITWISE_AND_ASSIGN,
    TOKEN_BITWISE_OR_ASSIGN,
	TOKEN_BITWISE_NOT_ASSIGN,
    TOKEN_XOR_ASSIGN,
	TOKEN_ASSIGN_OP_END = TOKEN_XOR_ASSIGN,

    TOKEN_EOF
} TokenKind;

typedef struct Token {
    TokenKind kind;
    int start;
    int len;
    union {
        InternedString ident;
        int integer_literal;
    };
} Token;

// Inline functions
inline bool is_binary_op(Token *token) { return TOKEN_BINARY_OP_START <= token->kind && token->kind <= TOKEN_BINARY_OP_END; }

// Public functions
extern Token *create_token(TokenKind kind, Pos start, int len);
extern void print_token(FILE *file, Token *t);
extern void token_initialise();
extern char *token_kind_string(TokenKind kind);