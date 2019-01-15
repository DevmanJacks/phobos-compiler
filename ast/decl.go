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
	return fmt.Sprintf("{ \"From\":%d, \"To\":%d", d.From, d.To)
}

// ConstDecl represents a constant declaration
type ConstDecl struct {
	Name  *Ident
	Type  Expr
	Value Expr
}

// TypeDecl represents a type declaration
type TypeDecl struct {
	Pos  source.Pos
	Name *Ident
	Spec Expr
}

// String gives a human readable form of a TypeDecl
func (d *TypeDecl) String() string {
	return fmt.Sprintf("{ \"Name\": %s, \"Spec\": %s", d.Name.String(), d.Spec.String())
}

// VarDecl represents a variable declaration
type VarDecl struct {
	Names  []*Ident
	Type   Expr
	Values []Expr
}

// Decl interface declarations
func (d *ConstDecl) declNode() {}
func (d *TypeDecl) declNode()  {}
func (d *VarDecl) declNode()   {}
