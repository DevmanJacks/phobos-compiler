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

// Parameter represents an imput parameter or a return parameter for a function
type Parameter struct {
	Name *Ident
	Type Expr
}

func (p *Parameter) String() string {
	s := strings.Builder{}
	s.WriteString("(Parameter ")

	if p.Name == nil {
		s.WriteString("()")
	} else {
		s.WriteString(p.Name.String())
	}

	s.WriteRune(' ')
	s.WriteString(p.Type.String())
	s.WriteRune(')')
	return s.String()
}

// Signature represents a functions parameters and returns
type Signature struct {
	Params  []*Parameter
	Returns []*Parameter
}

func (s *Signature) String() string {
	str := strings.Builder{}
	str.WriteString("(Signature (")
	first := true

	for _, param := range s.Params {
		if first {
			first = false
		} else {
			str.WriteRune(' ')
		}

		str.WriteString(param.String())
	}

	str.WriteString(") (")
	first = true

	for _, param := range s.Returns {
		if first {
			first = false
		} else {
			str.WriteRune(' ')
		}

		str.WriteString(param.String())
	}

	str.WriteString("))")

	return str.String()
}
