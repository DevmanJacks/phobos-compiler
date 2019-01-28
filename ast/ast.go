////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package ast

import (
	"phobos/source"
	"strings"
)

// Node represents a node in the AST
type Node interface {
	GenerateCode()
	Pos() source.Pos
	Resolve()
	String() string
}

// Field represents a field within a structure type
type Field struct {
	Names []*Ident
	Type  Expr
}

func fieldListAsString(fields []*Field) string {
	s := strings.Builder{}
	s.WriteRune('(')
	first := true

	for _, field := range fields {
		if first {
			first = false
		} else {
			s.WriteString(" ")
		}

		s.WriteString(field.String())
	}

	s.WriteRune(')')
	return s.String()
}

func (f *Field) exprNode() {}

func (f *Field) String() string {
	s := strings.Builder{}
	s.WriteString("(Field ")
	s.WriteString(identListAsString(f.Names))
	s.WriteRune(' ')
	s.WriteString(f.Type.String())
	s.WriteRune(')')
	return s.String()
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
