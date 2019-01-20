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

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *BadDecl) Resolve() {}

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

func (d *ConstDecl) declNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *ConstDecl) Resolve() {}

// String gives a human readable form of a VarDecl
func (d *ConstDecl) String() string {
	s := strings.Builder{}
	s.WriteString("(ConstDecl ")
	s.WriteString(d.Name.String())
	s.WriteRune(' ')

	if d.Type == nil {
		s.WriteString("()")
	} else {
		s.WriteString(d.Type.String())
	}

	s.WriteRune(' ')
	s.WriteString(d.Value.String())
	s.WriteString(")")
	return s.String()
}

// FuncDecl represents a function declaration
type FuncDecl struct {
	MethodType *Ident
	Name       *Ident
	Signature  *Signature
	Body       *BlockStmt
}

func (d *FuncDecl) declNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *FuncDecl) Resolve() {}

// String gives a human readable form of a VarDecl
func (d *FuncDecl) String() string {
	s := strings.Builder{}
	s.WriteString("(FuncDecl ")

	if d.MethodType == nil {
		s.WriteString("()")
	} else {
		s.WriteString(d.MethodType.String())
	}

	s.WriteString(" ")
	s.WriteString(d.Name.String())
	s.WriteString(" ")
	s.WriteString(d.Signature.String())
	s.WriteString(" ")
	s.WriteString(d.Body.String())
	s.WriteString(")")
	return s.String()
}

// ImportDecl represents an import declaration
type ImportDecl struct {
	Package Expr
}

func (d *ImportDecl) declNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *ImportDecl) Resolve() {}

// String gives a human readable form of a TypeDecl
func (d *ImportDecl) String() string {
	return fmt.Sprintf("(ImportDecl %s)", d.Package.String())
}

// TypeDecl represents a type declaration
type TypeDecl struct {
	Name *Ident
	Spec Expr
}

func (d *TypeDecl) declNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *TypeDecl) Resolve() {}

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

func (d *VarDecl) declNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *VarDecl) Resolve() {}

// String gives a human readable form of a VarDecl
func (d *VarDecl) String() string {
	s := strings.Builder{}
	s.WriteString("(VarDecl ")
	s.WriteString(identListAsString(d.Names))
	s.WriteRune(' ')

	if d.Type == nil {
		s.WriteString("()")
	} else {
		s.WriteString(d.Type.String())
	}

	s.WriteRune(' ')
	s.WriteString(expressionListAsString(d.Values))
	s.WriteString(")")
	return s.String()
}
