////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package ast

import (
	"fmt"
	"phobos/source"
	"strings"
)

// Decl represents a declaration node in the AST
type Decl interface {
	Node
	declNode()
}

// BadDecl represents a bad declaration
type BadDecl struct {
	From, To source.Pos
}

func (d *BadDecl) declNode() {}

// String gives a human readable form of a TypeDecl
func (d *BadDecl) String() string {
	return fmt.Sprintf("(BadDecl %d %d)", d.From, d.To)
}

// ConstDecl represents a constant declaration
type ConstDecl struct {
	Name  *Ident
	Type  Expr
	Value Expr
}

// TypeDecl represents a type declaration
type TypeDecl struct {
	Name *Ident
	Spec Expr
}

// String gives a human readable form of a TypeDecl
func (d *TypeDecl) String() string {
	return fmt.Sprintf("(TypeDecl %s %s)", d.Name.String(), d.Spec.String())
}

// VarDecl represents a variable declaration
type VarDecl struct {
	Names  []*Ident
	Type   Expr
	Values []Expr
}

// String gives a human readable form of a VarDecl
func (d *VarDecl) String() string {
	s := strings.Builder{}
	s.WriteString("(VarDecl (")

	first := true

	for _, name := range d.Names {
		if first {
			first = false
		} else {
			s.WriteString(", ")
		}

		s.WriteString(name.String())
	}

	s.WriteString(") ")

	if d.Type == nil {
		s.WriteString("()")
	} else {
		s.WriteString(d.Type.String())
	}

	s.WriteString(" (")
	first = true

	for _, value := range d.Values {
		if first {
			first = false
		} else {
			s.WriteString(", ")
		}

		s.WriteString(value.String())
	}

	s.WriteString("))")
	return s.String()
}

// Decl interface declarations
func (d *ConstDecl) declNode() {}
func (d *TypeDecl) declNode()  {}
func (d *VarDecl) declNode()   {}
