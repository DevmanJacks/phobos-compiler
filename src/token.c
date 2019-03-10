/*
 * token.c
 */

#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"

static char *tokens[] = {
	[TOKEN_EOF] = "EOF",
	[TOKEN_IDENT] = "identifier",
	[TOKEN_CHAR] = "character",
	[TOKEN_INT] = "integer",
	[TOKEN_FLOAT] = "float",
	[TOKEN_STR] = "string",

	[TOKEN_LPAREN] = "(",
	[TOKEN_RPAREN] = ")",
	[TOKEN_LBRACE] = "{",
	[TOKEN_RBRACE] = "}",
	[TOKEN_LBRACKET] = "[",
	[TOKEN_RBRACKET] = "]",

	[TOKEN_COMMA] = ",",
	[TOKEN_DOT] = ".",
	[TOKEN_DOTDOT] = "..",
	[TOKEN_SEMICOLON] = ";",
	[TOKEN_COLON] = ":",
	[TOKEN_QUESTION] = "?",

	[TOKEN_NOT] = "!",
	[TOKEN_BITWISE_NOT] = "~",
	[TOKEN_RETURNS] = "->",

	[TOKEN_MUL] = "*",
	[TOKEN_DIV] = "/",
	[TOKEN_MOD] = "%",
	[TOKEN_LSHIFT] = "<<",
	[TOKEN_RSHIFT] = ">>",
	[TOKEN_BITWISE_AND] = "&",

	[TOKEN_ADD] = "+",
	[TOKEN_SUB] = "-",
	[TOKEN_BITWISE_OR] = "|",
	[TOKEN_XOR] = "^",

	[TOKEN_EQ] = "==",
	[TOKEN_NOT_EQ] = "!=",
	[TOKEN_LT] = "<",
	[TOKEN_LE] = "<=",
	[TOKEN_GT] = ">",
	[TOKEN_GE] = ">=",

	[TOKEN_AND] = "&&",
	[TOKEN_OR] = "||",

	[TOKEN_ASSIGN] = "=",
	[TOKEN_COLON_ASSIGN] = ":=",
	[TOKEN_ADD_ASSIGN] = "+=",
	[TOKEN_SUB_ASSIGN] = "-=",
	[TOKEN_MUL_ASSIGN] = "*=",
	[TOKEN_DIV_ASSIGN] = "/=",
	[TOKEN_MOD_ASSIGN] = "%=",
	[TOKEN_LSHIFT_ASSIGN] = "<<=",
	[TOKEN_RSHIFT_ASSIGN] = ">>=",
	[TOKEN_BITWISE_AND_ASSIGN] = "&=",
	[TOKEN_BITWISE_OR_ASSIGN] = "|=",
	[TOKEN_BITWISE_NOT_ASSIGN] = "~=",
	[TOKEN_XOR_ASSIGN] = "^=",

	[TOKEN_BREAK] = "break",
	[TOKEN_CASE] = "case",
	[TOKEN_CONST] = "const",
	[TOKEN_CONTINUE] = "continue",
	[TOKEN_DEFAULT] = "default",
	[TOKEN_DEFER] = "defer",
	[TOKEN_ELSE] = "else",
	[TOKEN_ENUM] = "enum",
	[TOKEN_FALSE] = "false",
	[TOKEN_FOR] = "for",
	[TOKEN_FUNC] = "func",
	[TOKEN_IF] = "if",
	[TOKEN_IMPORT] = "import",
	[TOKEN_IN] = "in",
	[TOKEN_INTERFACE] = "interface",
	[TOKEN_NEW] = "new",
	[TOKEN_RETURN] = "return",
	[TOKEN_STRUCT] = "struct",
	[TOKEN_SWITCH] = "switch",
	[TOKEN_TRUE] = "true",
	[TOKEN_TYPE] = "type",
	[TOKEN_VAR] = "var",
	[TOKEN_WHILE] = "while"
};

const char *token_string(token_t token) {
	assert(0 <= token && token <= NUM_TOKENS);
	return tokens[token];
}

unsigned int operator_precedence(token_t token) {
    if (TOKEN_MUL_START <= token && token <= TOKEN_MUL_END)
        return 5;

    if (TOKEN_ADD_START <= token && token <= TOKEN_ADD_END)
        return 4;

    if (TOKEN_CMP_START <= token && token <= TOKEN_CMP_END)
        return 3;

    return 1;
}

typedef struct keyword {
	char *keyword;
	token_t token;
} keyword_t;

// The keywords below must be in alphabetical order as we will be doing a binary search to find entries
static keyword_t keywords[] = {
	{ "break", TOKEN_BREAK },
	{ "case", TOKEN_CASE },
	{ "const", TOKEN_CONST },
	{ "continue", TOKEN_CONTINUE },
	{ "default", TOKEN_DEFAULT },
	{ "defer", TOKEN_DEFER },
	{ "else", TOKEN_ELSE },
	{ "enum", TOKEN_ENUM },
	{ "false", TOKEN_FALSE },
	{ "for", TOKEN_FOR },
	{ "func", TOKEN_FUNC },
	{ "if", TOKEN_IF },
	{ "import", TOKEN_IMPORT },
	{ "in", TOKEN_IN },
	{ "interface", TOKEN_INTERFACE },
	{ "new", TOKEN_NEW },
	{ "return", TOKEN_RETURN },
	{ "struct", TOKEN_STRUCT },
	{ "switch", TOKEN_SWITCH },
	{ "true", TOKEN_TRUE },
	{ "type", TOKEN_TYPE },
	{ "var", TOKEN_VAR },
	{ "while", TOKEN_WHILE }
};

// Find the keyword and return it's token, otherwise return an identifier
token_t lookup_token(const char *ident) {
	int low = 0;
	int high = sizeof(keywords) / sizeof(keyword_t) - 1;

	while (true) {
		int i = low + (high - low) / 2;
		int cmp = strcmp(ident, keywords[i].keyword);

		if (cmp == 0)
			return keywords[i].token;
		else if (cmp < 0)
			high = i - 1;
		else
			low = i + 1;

		if (low > high)
			return TOKEN_IDENT;
	}
}