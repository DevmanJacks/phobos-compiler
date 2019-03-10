// scanner_test.c - Tests for the scanner.

#include <stdlib.h>
#include <string.h>
#include "../scanner.c"
#include "testing.h"

typedef struct element_t {
    char    *lexeme;
    token_t token;
} element_t;

static element_t test[] = {
    // Identifiers
    { "hello", TOKEN_IDENT },
    { "_", TOKEN_IDENT },
    { "_123", TOKEN_IDENT },
    { "test_var_1", TOKEN_IDENT },

    // Basic type literals
    { "0", TOKEN_INT },
    { "1", TOKEN_INT },
    { "12_345_678_901_234_567_890", TOKEN_INT },
    { "01234567", TOKEN_INT },
    { "0123_456", TOKEN_INT },
    { "0x01234567", TOKEN_INT },
    { "0X89ab_cdef", TOKEN_INT },
    { "0b0", TOKEN_INT },
    { "0B1", TOKEN_INT },
    { "0b1111_0000_1111_0000", TOKEN_INT },
    { "3.14159265", TOKEN_FLOAT },
    { "0.", TOKEN_FLOAT },
    { ".0", TOKEN_FLOAT },
    { "1e2", TOKEN_FLOAT },
    { "10E+10", TOKEN_FLOAT },
    { "17e-2", TOKEN_FLOAT },
    { "1.41421356237e10", TOKEN_FLOAT },
    { "'a'", TOKEN_CHAR },
    { "\"This is a sttring literal\"", TOKEN_STR },

    // Operators
    { "(", TOKEN_LPAREN },
	{ ")", TOKEN_RPAREN },
	{ "{", TOKEN_LBRACE },
	{ "}", TOKEN_RBRACE },
	{ "[", TOKEN_LBRACKET },
	{ "]", TOKEN_RBRACKET },

	{ ",", TOKEN_COMMA },
	{ ".", TOKEN_DOT },
	{ "..", TOKEN_DOTDOT },
	{ ";", TOKEN_SEMICOLON },
	{ ":", TOKEN_COLON },
	{ "?", TOKEN_QUESTION },

	{ "!", TOKEN_NOT },
	{ "~", TOKEN_BITWISE_NOT },
	{ "->", TOKEN_RETURNS },

	{ "*", TOKEN_MUL },
	{ "/", TOKEN_DIV },
	{ "%", TOKEN_MOD },
	{ "<<", TOKEN_LSHIFT },
	{ ">>", TOKEN_RSHIFT },
	{ "&", TOKEN_BITWISE_AND },

	{ "+", TOKEN_ADD },
	{ "-", TOKEN_SUB },
	{ "|", TOKEN_BITWISE_OR },
	{ "^", TOKEN_XOR },

	{ "==", TOKEN_EQ },
	{ "!=", TOKEN_NOT_EQ },
	{ "<", TOKEN_LT },
	{ "<=", TOKEN_LE },
	{ ">", TOKEN_GT },
	{ ">=", TOKEN_GE },

	{ "&&", TOKEN_AND },
	{ "||", TOKEN_OR },

	{ "=", TOKEN_ASSIGN },
	{ ":=", TOKEN_COLON_ASSIGN },
	{ "+=", TOKEN_ADD_ASSIGN },
	{ "-=", TOKEN_SUB_ASSIGN },
	{ "*=", TOKEN_MUL_ASSIGN },
	{ "/=", TOKEN_DIV_ASSIGN },
	{ "%=", TOKEN_MOD_ASSIGN },
	{ "<<=", TOKEN_LSHIFT_ASSIGN },
	{ ">>=", TOKEN_RSHIFT_ASSIGN },
	{ "&=", TOKEN_BITWISE_AND_ASSIGN },
    { "|=", TOKEN_BITWISE_OR_ASSIGN },
	{ "~=", TOKEN_BITWISE_NOT_ASSIGN },
    { "^=", TOKEN_XOR_ASSIGN },

    // Keywords
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
    { "while", TOKEN_WHILE },

	// Comments
	{ "//", TOKEN_EOF }
};

static void test_scan(void) {
    char buffer[128];
    int count = sizeof(test) / sizeof(element_t);

    for (int i = 0; i < count; i++) {
		source_t *source = add_source((uint8_t *)test[i].lexeme, strlen(test[i].lexeme));
        scanner_t *s = make_scanner(source);
        sprintf(buffer, "\"%s\"", test[i].lexeme);

        token_t token = scan(s);

        if (token == test[i].token) {
            if (token == TOKEN_IDENT || token == TOKEN_INT) {// token == TOKEN_FLOAT || token == TOKEN_CHAR || token == TOKEN_STR || isKeyword(token)) {
                if (strcmp(s->lexeme, test[i].lexeme) == 0) {
                    test_passed(buffer);
                } else {
                    test_failed(buffer, "Bad lexeme, got: %s", s->lexeme);
                }
            } else {
                test_passed(buffer);
            }
        } else
            test_failed(buffer, "Bad token, expected %d, got %d", test[i].token, token);

        free(s);
		free(source);
    }
}

void scanner_test(void) {
    test_section("Scanner");
    test_scan();
}