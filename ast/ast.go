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

// Node represents a node in the AST
type Node interface {
	String() string
}

// Ident represents an identifier
type Ident struct {
	Pos  source.Pos
	Name string
}

// NewIdent instantiates an identifier node with the specified information
func NewIdent(pos source.Pos, name string) *Ident {
	return &Ident{Pos: pos, Name: name}
}

func (i *Ident) String() string {
	return fmt.Sprintf("{ \"Name\": %s", i.Name)
}
