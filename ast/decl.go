////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package ast

// Decl represents a declaration node in the AST
type Decl interface {
	Node
	declNode()
}

// ConstDecl represents a constant declaration
type ConstDecl struct {
	Name  Ident
	Type  Expr
	Value Expr
}

// TypeDecl represents a type declaration
type TypeDecl struct {
}

// VarDecl represents a variable declaration
type VarDecl struct {
	Names  []Ident
	Type   Expr
	Values []Expr
}

// NewVarDecl instantiates a new VarDecl
func NewVarDecl() {

}

// Decl interface declarations
func (d *ConstDecl) declNode() {}
func (d *TypeDecl) declNode()  {}
func (d *VarDecl) declNode()   {}
