////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		24 January 2019
//
////////////////////////////////////////////////////////////

package symbol

import (
	"phobos/source"
)

// A Symbol defines
type Symbol interface {
	Name() string
	Pos() source.Pos
}

// States of symbol resolution
const (
	unresolved = iota
	resolving
	resolved
)

// A Package represents a package with declarations in it's scope
type Package struct {
	name  string
	scope *Scope
}

// Name returns the Symbol name
func (p *Package) Name() string {
	return p.name
}

// A Type represents a declared type
type Type struct {
	pos  source.Pos
	name string
}

// Name returns the Symbol name
func (t *Type) Name() string {
	return t.name
}

// Pos returns the Pos where the Symbol is declared in the source
func (t *Type) Pos() source.Pos {
	return t.pos
}

// String returns a human readable representation of a Type Symbol
func (t *Type) String() string {
	return ""
}

// NewSynonymType creates a new Type
func NewSynonymType(name string) *Type {
	return &Type{name: name}
}

// A Variable represents a declared variable, function parameter, function named return or struct field
type Variable struct {
	pos  source.Pos
	name string
	typ  Symbol
}

// NewVariable creates a new variable symbol with the specified name
func NewVariable(pos source.Pos, name string, typ *Type) *Variable {
	return &Variable{pos: pos, name: name, typ: typ}
}

// Name returns the Symbol name
func (v *Variable) Name() string {
	return v.name
}

// Pos returns the Symbol Pos
func (v *Variable) Pos() source.Pos {
	return v.pos
}
