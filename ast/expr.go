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
	"phobos/token"
	"strings"
)

// Expr represents an expression node in the AST
type Expr interface {
	Node
	exprNode()
}

// BadExpr represents a bad expression
type BadExpr struct {
	From, To source.Pos
}

func (e *BadExpr) exprNode() {}

// String gives a human readable form of a BadExpr
func (e *BadExpr) String() string {
	return "{ BadDecl }"
}

// BinaryExpr represents a binary expression node in the AST
type BinaryExpr struct {
	left  Expr
	op    token.Token
	right Expr
}

// EnumItem represents a single item in an enum type
type EnumItem struct {
	Name  *Ident
	Value Expr
}

// String gives a human readable form of a EnumItem
func (i *EnumItem) String() string {
	s := "{ \"Name\":" + i.Name.String() + ", \"Value\":"

	if i.Value == nil {
		s += "null }"
	} else {
		s += i.Value.String() + " }"
	}

	return s
}

// EnumType represents an enum type node in the AST
type EnumType struct {
	EnumPos source.Pos // Pos of enum keyword
	Items   []EnumItem
}

func (t *EnumType) exprNode() {}

// String gives a human readable form of a EnumType
func (t *EnumType) String() string {
	s := strings.Builder{}
	s.WriteString("{ \"Items\":[ ")

	for _, item := range t.Items {
		s.WriteString(item.String())
	}

	s.WriteString(" ] }")

	return s.String()
}
