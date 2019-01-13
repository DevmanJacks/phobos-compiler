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
		{token.Identifier, "hello"},
		{token.Identifier, "_"},
		{token.Identifier, "_123"},
		{token.Identifier, "test_var_1"},

		// Basic literals
		{token.Integer, "0"},
		{token.Integer, "1"},
		{token.Integer, "12_345_678_901_234_567_890"},
		{token.Integer, "-125"},
		{token.Integer, "+4"},
		{token.Integer, "0x01234567"},
		{token.Integer, "0X89ab_cdef"},
		{token.Integer, "0b0"},
		{token.Integer, "0B1"},
		{token.Integer, "0b1111_0000_1111_0000"},
		{token.Float, "3.14159265"},
		{token.Float, "0."},
		{token.Float, ".0"},
		{token.Float, "1e2"},
		{token.Float, "10E+10"},
		{token.Float, "17e-2"},
		{token.Float, "1.41421356237e10"},
		{token.Character, "'a'"},
		{token.Character, "'\\'"},
		{token.String, "\"This is a sttring literal\""},
		{token.String, "\"\\nThis \\\" is on a new line\""},

		{token.LeftBrace, "{"},
		{token.RightBrace, "}"},
		{token.LeftBracket, "["},
		{token.RightBracket, "]"},
		{token.LeftParenthesis, "("},
		{token.RightParenthesis, ")"},
		{token.Colon, ":"},
		{token.Comma, ","},
		{token.Returns, "->"},
		{token.Dot, "."},
		{token.DotDot, ".."},
		{token.Assign, "="},
		{token.DeclareAssign, ":="},

		// Operators
		{token.Asterisk, "*"},
		{token.Divide, "/"},
		{token.Minus, "-"},
		{token.Mod, "%"},
		{token.Plus, "+"},

		{token.BitwiseAnd, "&"},
		{token.BitwiseOr, "|"},
		{token.BitwiseNot, "~"},
		{token.ShiftLeft, "<<"},
		{token.ShiftRight, ">>"},

		{token.DivideEquals, "/="},
		{token.MinusEquals, "-="},
		{token.ModEquals, "%="},
		{token.MultiplyEquals, "*="},
		{token.PlusEquals, "+="},
		{token.BitwiseAndEquals, "&="},
		{token.BitwiseOrEquals, "|="},
		{token.BitwiseNotEquals, "~="},
		{token.ShiftLeftEquals, "<<="},
		{token.ShiftRightEquals, ">>="},

		{token.Equal, "=="},
		{token.NotEqual, "!="},
		{token.LessThan, "<"},
		{token.LessThanOrEqual, "<="},
		{token.GreaterThan, ">"},
		{token.GreaterThanOrEqual, ">="},

		{token.LogicalAnd, "&&"},
		{token.LogicalOr, "||"},
		{token.Not, "!"},

		{token.As, "as"},
		{token.Break, "break"},
		{token.Case, "case"},
		{token.Const, "const"},
		{token.Continue, "continue"},
		{token.Default, "default"},
		{token.Defer, "defer"},
		{token.Else, "else"},
		{token.Enum, "enum"},
		{token.False, "false"},
		{token.For, "for"},
		{token.Func, "func"},
		{token.If, "if"},
		{token.Import, "import"},
		{token.In, "in"},
		{token.Interface, "interface"},
		{token.New, "new"},
		{token.Return, "return"},
		{token.Struct, "struct"},
		{token.Switch, "switch"},
		{token.True, "true"},
		{token.Type, "type"},
		{token.Var, "var"},
		{token.While, "while"},
	}

	for _, tt := range tests {
		s := NewScanner(tt.lexeme)
		_, tok, lexeme := s.Scan()

		if tok != tt.tok {
			t.Errorf("Scan(\"%s\") expected token: %s, got %s", tt.lexeme, tt.tok.String(), tok.String())
		}

		if lexeme != tt.lexeme {
			t.Errorf("Scan(\"%s\") expected lexeme: %s, got %s", tt.lexeme, tt.lexeme, lexeme)
		}

	}
}
