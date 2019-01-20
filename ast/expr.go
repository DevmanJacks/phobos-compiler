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

func expressionListAsString(exprs []Expr) string {
	s := strings.Builder{}
	s.WriteRune('(')
	first := true

	for _, expr := range exprs {
		if first {
			first = false
		} else {
			s.WriteString(" ")
		}

		s.WriteString(expr.String())
	}

	s.WriteRune(')')
	return s.String()
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
	Left  Expr
	Op    token.Token
	Right Expr
}

func (e *BinaryExpr) exprNode() {}

// String gives a human readable form of a BadExpr
func (e *BinaryExpr) String() string {
	return fmt.Sprintf("(BinaryExpr %s %s %s)", e.Left.String(), e.Op.String(), e.Right.String())
}

// BoolLiteralExpr represents a boolean true or false literal node in the AST
type BoolLiteralExpr struct {
	Op token.Token
}

func (e *BoolLiteralExpr) exprNode() {}

// String gives a human readable form of a CompositeExpr
func (e *BoolLiteralExpr) String() string {
	if e.Op == token.True {
		return "true"
	} else {
		return "false"
	}
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

// CallExpr represents an array or struct literal node in the AST
type CallExpr struct {
	Name      Expr
	Arguments []Expr
}

func (e *CallExpr) exprNode() {}

// String gives a human readable form of a CompositeExpr
func (e *CallExpr) String() string {
	return fmt.Sprintf("(CallExpr %s %s)", e.Name.String(), expressionListAsString(e.Arguments))
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

	if e.Type == nil {
		s.WriteString("()")
	} else {
		s.WriteString(e.Type.String())
	}

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

func identListAsString(idents []*Ident) string {
	s := strings.Builder{}
	s.WriteRune('(')
	first := true

	for _, ident := range idents {
		if first {
			first = false
		} else {
			s.WriteString(" ")
		}

		s.WriteString(ident.String())
	}

	s.WriteRune(')')
	return s.String()
}

func (i *Ident) exprNode() {}

func (i *Ident) String() string {
	return fmt.Sprintf("(Ident %s)", i.Name)
}

// IndexExpr represents an index node in the AST
type IndexExpr struct {
	Expr  Expr
	Index Expr
}

func (e *IndexExpr) exprNode() {}

func (e *IndexExpr) String() string {
	if e.Expr == nil {
		return fmt.Sprintf("(IndexExpr %s)", e.Index.String())
	} else {
		return fmt.Sprintf("(IndexExpr %s %s)", e.Expr.String(), e.Index.String())
	}
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

// NewExpr represents a new expression node in the AST
type NewExpr struct {
	Type Expr
}

func (e *NewExpr) exprNode() {}

// String gives a human readable form of a CompositeExpr
func (e *NewExpr) String() string {
	return fmt.Sprintf("(NewExpr %s)", e.Type.String())
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

// UnaryExpr represents a unary expression node in the AST
type UnaryExpr struct {
	Op         token.Token
	Expression Expr
}

func (e *UnaryExpr) exprNode() {}

// String gives a human readable form of a UnaryExpr
func (e *UnaryExpr) String() string {
	return fmt.Sprintf("(UnaryExpr %s %s)", e.Op.String(), e.Expression.String())
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

// EnumItem represents a single item in an enum type
type EnumItem struct {
	Name  *Ident
	Value Expr
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

// PointerType represents a pointer type node in the AST
type PointerType struct {
	BaseType Expr
}

func (t *PointerType) exprNode() {}

func (t *PointerType) String() string {
	return fmt.Sprintf("(PointerType %s)", t.BaseType.String())
}

// SliceType represents a slice type node in the AST
type SliceType struct {
	BaseType Expr
}

func (t *SliceType) exprNode() {}

func (t *SliceType) String() string {
	return fmt.Sprintf("(SliceType %s)", t.BaseType.String())
}

// StructType represents a struct type node in the AST
type StructType struct {
	Fields []*Field
}

func (t *StructType) exprNode() {}

func (t *StructType) String() string {
	return fmt.Sprintf("(StructType %s)", fieldListAsString(t.Fields))
}
