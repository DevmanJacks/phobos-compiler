////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package ast

import (
	"phobos/token"
)

// Expr represents an expression node in the AST.
type Expr interface {
}

// BinaryExpr represents a binary expression node in the AST.
type BinaryExpr struct {
	left  Expr
	op    token.Token
	right Expr
}
