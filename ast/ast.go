////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package ast

import (
	"strings"
)

// Node represents a node in the AST
type Node interface {
	String() string
}

// Pair represents a name and an expression.  This is used in struct and array literals, function parameters, function returns.
// Name will be nil if the literal is un-named.
type Pair struct {
	Name *Ident
	Expr Expr
}

func (p *Pair) String() string {
	s := strings.Builder{}
	s.WriteString("(NameValuePair ")

	if p.Name == nil {
		s.WriteString("()")
	} else {
		s.WriteString(p.Name.String())
	}

	s.WriteString(" ")
	s.WriteString(p.Expr.String())
	s.WriteString("")
	return s.String()
}
