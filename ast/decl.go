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
	"phobos/symbol"
	"strings"
)

// Decl represents a declaration node in the AST
type Decl interface {
	Node
	declNode()
}

// ========== Bad Declaration ==========

// BadDecl represents a bad declaration
type BadDecl struct {
	From, To source.Pos
}

func (d *BadDecl) declNode() {}

// GenerateCode will generate C code for the AST node
func (d *BadDecl) GenerateCode() {
	panic("Cannot generate code for a program with errors")
}

// Pos returns the start position of the expression in the source
func (d *BadDecl) Pos() source.Pos { panic("Pos() not implemented for BadDecl") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *BadDecl) Resolve() { panic("Resolve() not implemented for BadDecl") }

// String gives a human readable form of a TypeDecl
func (d *BadDecl) String() string {
	return fmt.Sprintf("(BadDecl %d %d)", d.From, d.To)
}

// ========== Constant Declaration ==========

// ConstDecl represents a constant declaration
type ConstDecl struct {
	Name  *Ident
	Type  Expr
	Value Expr
}

func (d *ConstDecl) declNode() {}

// GenerateCode will generate C code for the AST node
func (d *ConstDecl) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (d *ConstDecl) Pos() source.Pos { panic("Pos() not implemented for ConstDecl") }

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

// ========== Function Declaration ==========

// FuncDecl represents a function declaration
type FuncDecl struct {
	MethodType *Ident
	Name       *Ident
	Signature  *Signature
	Body       *BlockStmt
}

func (d *FuncDecl) declNode() {}

// GenerateCode will generate C code for the AST node
func (d *FuncDecl) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (d *FuncDecl) Pos() source.Pos { panic("Pos() not implemented for FuncDecl") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *FuncDecl) Resolve() { panic("Resolve() not implemented for FuncDecl") }

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

// ========== Import Declaration ==========

// ImportDecl represents an import declaration
type ImportDecl struct {
	Package Expr
}

func (d *ImportDecl) declNode() {}

// GenerateCode will generate C code for the AST node
func (d *ImportDecl) GenerateCode() {
	panic("ImportDecl nodes should not exist in AST following Resolve step")
}

// Pos returns the start position of the expression in the source
func (d *ImportDecl) Pos() source.Pos { panic("Pos() not implemented for ImportDecl") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *ImportDecl) Resolve() { panic("Resolve() not implemented for ImportDecl") }

// String gives a human readable form of a TypeDecl
func (d *ImportDecl) String() string {
	return fmt.Sprintf("(ImportDecl %s)", d.Package.String())
}

// ========== Type Declaration ==========

// TypeDecl represents a type declaration
type TypeDecl struct {
	Name *Ident
	Spec Expr
}

func (d *TypeDecl) declNode() {}

// GenerateCode will generate C code for the AST node
func (d *TypeDecl) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (d *TypeDecl) Pos() source.Pos { panic("Pos() not implemented for TypeDecl") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (d *TypeDecl) Resolve() { panic("Resolve() not implemented for TypeDecl") }

// String gives a human readable form of a TypeDecl
func (d *TypeDecl) String() string {
	return fmt.Sprintf("(TypeDecl %s %s)", d.Name.String(), d.Spec.String())
}

// ========== Variable Declaration ==========

// VarDecl represents a variable declaration
type VarDecl struct {
	VarPos source.Pos
	Names  []*Ident
	Type   Expr
	Values []Expr
}

func (d *VarDecl) declNode() {}

// GenerateCode will generate C code for the AST node
func (d *VarDecl) GenerateCode() {}

// Pos returns the position of the var keyword in the source
func (d *VarDecl) Pos() source.Pos {
	return d.VarPos
}

// Resolve will infer the type for the variables if not specified and check the type against the initialisation expression
func (d *VarDecl) Resolve() {
	var typ *symbol.Type

	if d.Type != nil {
		d.Type.Resolve()

		if len(d.Type.ResolvedTypes()) > 1 {
			error(d.Type.Pos(), "Cannot have multiple types for variable in declaration")
		} else {
			typ = d.Type.ResolvedTypes()[0]
		}

	}

	if d.Values != nil {
		for _, value := range d.Values {
			value.Resolve()
			if typ != nil {
				for _, resolvedType := range value.ResolvedTypes() {
					if !areCompatibleTypes(resolvedType, typ) {
						error(value.Pos(), "Types are incompatible: "+typ.String()+" and "+resolvedType.String())
					}
				}
			}
		}
	}

	for _, ident := range d.Names {
		found, existing := symbol.CurrentScope.Insert(symbol.NewVariable(ident.Pos(), ident.Name, typ))
		if found {
			errorWithPrevious(ident.Pos(), "Variable %s already declared.  Previous declaration is here:", existing.Pos())
		}

		// Set type based on expression resolved types
	}
}

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

// ========== Helpers ==========

func error(pos source.Pos, message string) {
	source.Error(pos, message)
}

func errorWithPrevious(pos source.Pos, message string, previousPos source.Pos) {
	source.ErrorWithPrevious(pos, message, previousPos)
}
