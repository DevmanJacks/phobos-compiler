////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		24 January 2019
//
////////////////////////////////////////////////////////////

package ast

import (
	"phobos/symbol"
	"phobos/token"
	"testing"
)

func TestVarDeclResolve(t *testing.T) {
	symbol.CurrentScope = symbol.NewScope(nil)

	names := []*Ident{{Name: "x"}, {Name: "y"}}
	values := []Expr{&LiteralExpr{Kind: token.Integer, Value: "3"}, &LiteralExpr{Kind: token.Integer, Value: "5"}}
	decl := VarDecl{Names: names, Values: values}

	decl.Resolve()

	for _, ident := range names {
		if ident.Type == nil {
			t.Error("Expected type to be set after Resolve()")
		} else if ident.Type.Name() != "Int32" {
			t.Errorf("Expected type to be Int32, was %s", ident.Type.Name())
		}
	}
}
