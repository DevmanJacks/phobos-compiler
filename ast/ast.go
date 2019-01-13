////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package ast

import "phobos/token"

// Node represents a node in the AST
type Node interface {
	Start() token.Pos
	End() token.Pos
}

// Ident represents an identifier
type Ident struct {
	pos  token.Pos
	name string
}

// NewIdent instantiates an identifier node with the specified information
func NewIdent(pos token.Pos, name string) *Ident {
	return &Ident{pos: pos, name: name}
}
