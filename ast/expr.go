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
	return fmt.Sprintf("(BadDecl %d %d)", int(e.From), int(e.To))
}

// BinaryExpr represents a binary expression node in the AST
type BinaryExpr struct {
	left  Expr
	op    token.Token
	right Expr
}

// Element represents a key and value in a composite literal.  Key can be nil
type Element struct {
	Key   Expr
	Value Expr
}

func (e *Element) String() string {
	s := strings.Builder{}
	s.WriteString("(Element ")

	if e.Key == nil {
		s.WriteString("()")
	} else {
		s.WriteString(e.Key.String())
	}

	s.WriteString(" ")
	s.WriteString(e.Value.String())
	s.WriteString("")
	return s.String()
}

// CompositeExpr represents an array or struct literal node in the AST
type CompositeExpr struct {
	Type     Expr
	Elements []*Element
}

func (e *CompositeExpr) exprNode() {}

// String gives a human readable form of a CompositeExpr
func (e *CompositeExpr) String() string {
	s := strings.Builder{}
	s.WriteString("(CompositeExpr ")
	s.WriteString(e.Type.String())
	s.WriteString(" (")
	first := true

	for _, pair := range e.Elements {
		if first {
			first = false
		} else {
			s.WriteRune(' ')
		}

		s.WriteString(pair.String())
	}

	s.WriteString("))")
	return s.String()
}

// Ident represents an identifier
type Ident struct {
	Pos  source.Pos
	Name string
}

func (i *Ident) exprNode() {}

func (i *Ident) String() string {
	return fmt.Sprintf("(Ident %s)", i.Name)
}

// IndexExpr represents an index node in the AST
type IndexExpr struct {
	Index Expr
}

func (e *IndexExpr) exprNode() {}

func (e *IndexExpr) String() string {
	return fmt.Sprintf("(IndexExpr %s)", e.Index.String())
}

// LiteralExpr represents a character, float, integer or string literal node in the AST
type LiteralExpr struct {
	Kind  token.Token
	Pos   source.Pos
	Value string
}

func (e *LiteralExpr) exprNode() {}

func (e *LiteralExpr) String() string {
	return fmt.Sprintf("(LiteralExpr %s %s)", e.Kind.String(), e.Value)
}

// SelectorExpr represents a selector node in the AST
type SelectorExpr struct {
	Expr Expr
	Name *Ident
}

func (e *SelectorExpr) exprNode() {}

func (e *SelectorExpr) String() string {
	return fmt.Sprintf("(SelectorExpr %s %s)", e.Expr.String(), e.Name.String())
}

// EnumItem represents a single item in an enum type
type EnumItem struct {
	Name  *Ident
	Value Expr
}

// ArrayType represents an array type node in the AST
type ArrayType struct {
	Length   Expr
	BaseType Expr
}

func (t *ArrayType) exprNode() {}

func (t *ArrayType) String() string {
	return fmt.Sprintf("(ArrayType %s %s)", t.Length.String(), t.BaseType.String())
}

// String gives a human readable form of a EnumItem
func (i *EnumItem) String() string {
	s := "(EnumItem " + i.Name.String()

	if i.Value != nil {
		s += i.Value.String()
	}

	s += ")"
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
	s.WriteString("(EnumType (")
	first := true

	for _, item := range t.Items {
		if first {
			first = false
		} else {
			s.WriteRune(' ')
		}
		s.WriteString(item.String())
	}

	s.WriteString("))")

	return s.String()
}

// SliceType represents a slice type node in the AST
type SliceType struct {
	BaseType Expr
}

func (t *SliceType) exprNode() {}

func (t *SliceType) String() string {
	return fmt.Sprintf("(SliceType %s)", t.BaseType.String())
}
