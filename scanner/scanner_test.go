////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package scanner

import (
	"phobos/token"
	"testing"
)

func TestScan(t *testing.T) {
	var tests = []struct {
		tok    token.Token
		lexeme string
	}{
		{token.IDENT, "hello"},
		{token.IDENT, "_"},
		{token.IDENT, "_123"},
		{token.IDENT, "test_var_1"},

		{token.BREAK, "break"},
		{token.WHILE, "while"},
	}

	for _, tt := range tests {
		s := new(Scanner)
		s.Init(tt.lexeme)
		_, tok, lexeme := s.Scan()

		if tok != tt.tok {
			t.Errorf("Scan(\"%s\") expected token: %s, got %s", tt.lexeme, tt.tok.String(), tok.String())
		}

		if lexeme != tt.lexeme {
			t.Errorf("Scan(\"%s\") expected lexeme: %s, got %s", tt.lexeme, tt.lexeme, lexeme)
		}

	}
}
