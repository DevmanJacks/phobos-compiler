////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		24 January 2019
//
////////////////////////////////////////////////////////////

package parser

import (
	"phobos/scanner"
	"phobos/source"
	"testing"
)

func TestParse(t *testing.T) {
	var tests = []struct {
		code string
		ast  string
	}{
		{"var i: Int32", "(VarDecl ((Ident i)) (Ident Int32) ())"},
		{"var x = 30", "(VarDecl ((Ident x)) () ((LiteralExpr Integer 30)))"},
	}

	for _, test := range tests {
		p := NewParserWithScanner(scanner.NewScanner(source.FromString(test.code)))
		ast := p.Parse()
		astString := ast[0].String()

		if astString != test.ast {
			t.Errorf("Parse(\"%s\") expected: %s, got: %s", test.code, test.ast, astString)
		}
	}
}
