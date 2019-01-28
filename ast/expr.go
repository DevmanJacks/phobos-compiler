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
	"phobos/token"
	"strconv"
	"strings"
)

// Expr represents an expression node in the AST
type Expr interface {
	Node
	exprNode()
	ResolvedTypes() []*symbol.Type
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

// GenerateCode will generate C code for the AST node
func (e *BadExpr) GenerateCode() {
	panic("Cannot generate code for a program with errors")
}

// Pos returns the start position of the expression in the source
func (e *BadExpr) Pos() source.Pos { panic("Pos() not implemented for BadExpr") }

// Resolve does nothing
func (e *BadExpr) Resolve() { panic("Resolve() not implemented for BadExpr") }

// ResolvedTypes does nothing
func (e *BadExpr) ResolvedTypes() []*symbol.Type { panic("ResolvedTypes() not implemented for BadExpr") }

// String gives a human readable form of a BadExpr
func (e *BadExpr) String() string {
	return fmt.Sprintf("(BadDecl %d %d)", int(e.From), int(e.To))
}

// ========== Binary Expression ==========

// BinaryExpr represents a binary expression node in the AST
type BinaryExpr struct {
	Left  Expr
	Op    token.Token
	Right Expr
}

func (e *BinaryExpr) exprNode() {}

// GenerateCode will generate C code for the AST node
func (e *BinaryExpr) GenerateCode() { panic("GenerateCode() not implemented for BinaryExpr") }

// Pos returns the start position of the expression in the source
func (e *BinaryExpr) Pos() source.Pos { panic("Pos() not implemented for BinaryExpr") }

// Resolve will perform type checking
func (e *BinaryExpr) Resolve() { panic("Resolve() not implemented for BadExpr") }

// ResolvedTypes does nothing
func (e *BinaryExpr) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for BinaryExpr")
}

// String gives a human readable form of a BadExpr
func (e *BinaryExpr) String() string {
	return fmt.Sprintf("(BinaryExpr %s %s %s)", e.Left.String(), e.Op.String(), e.Right.String())
}

// ========== Boolean Literal Expression - true & false ==========

// BoolLiteralExpr represents a boolean true or false literal node in the AST
type BoolLiteralExpr struct {
	Op token.Token
}

func (e *BoolLiteralExpr) exprNode() {}

// GenerateCode will generate C code for the AST node
func (e *BoolLiteralExpr) GenerateCode() { panic("GenerateCode() not implemented for BoolLiteralExpr") }

// Pos returns the start position of the expression in the source
func (e *BoolLiteralExpr) Pos() source.Pos { panic("Pos() not implemented for BoolLiteralExpr") }

// Resolve does nothing
func (e *BoolLiteralExpr) Resolve() { panic("Resolve() not implemented for BoolLiteralExpr") }

// ResolvedTypes does nothing
func (e *BoolLiteralExpr) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for BoolLiteralExpr")
}

// String gives a human readable form of a CompositeExpr
func (e *BoolLiteralExpr) String() string {
	if e.Op == token.True {
		return "true"
	} else {
		return "false"
	}
}

// ========== Call Expression ==========

// CallExpr represents an array or struct literal node in the AST
type CallExpr struct {
	Name      Expr
	Arguments []Expr
}

func (e *CallExpr) exprNode() {}

// GenerateCode will generate C code for the AST node
func (e *CallExpr) GenerateCode() { panic("GenerateCode() not implemented for CallExpr") }

// Pos returns the start position of the expression in the source
func (e *CallExpr) Pos() source.Pos { panic("Pos() not implemented for CallExpr") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (e *CallExpr) Resolve() { panic("Resolve() not implemented for CallExpr") }

// ResolvedTypes does nothing
func (e *CallExpr) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for CallExpr")
}

// String gives a human readable form of a CompositeExpr
func (e *CallExpr) String() string {
	return fmt.Sprintf("(CallExpr %s %s)", e.Name.String(), expressionListAsString(e.Arguments))
}

// ========== Composite Expression ==========

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

// GenerateCode will generate C code for the AST node
func (e *CompositeExpr) GenerateCode() { panic("GenerateCode() not implemented for CompositeExpr") }

// Pos returns the start position of the expression in the source
func (e *CompositeExpr) Pos() source.Pos { panic("Pos() not implemented for CompositeExpr") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (e *CompositeExpr) Resolve() { panic("Resolve() not implemented for CompositeExpr") }

// ResolvedTypes does nothing
func (e *CompositeExpr) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for CompositeExpr")
}

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

// ========== Identifier ==========

// Ident represents an identifier
type Ident struct {
	NamePos source.Pos
	Name    string

	// Resolved type
	Type *symbol.Type
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

// GenerateCode will generate C code for the AST node
func (i *Ident) GenerateCode() { panic("GenerateCode() not implemented for Ident") }

// Pos returns the start position of the expression in the source
func (i *Ident) Pos() source.Pos {
	return i.NamePos
}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (i *Ident) Resolve() { panic("Resolve() not implemented for Ident") }

// ResolvedTypes does nothing
func (i *Ident) ResolvedTypes() []*symbol.Type { panic("ResolvedTypes() not implemented for Ident") }

func (i *Ident) String() string {
	return fmt.Sprintf("(Ident %s)", i.Name)
}

// ========== Index Expression ==========

// IndexExpr represents an index node in the AST
type IndexExpr struct {
	Expr  Expr
	Index Expr
}

func (e *IndexExpr) exprNode() {}

// GenerateCode will generate C code for the AST node
func (e *IndexExpr) GenerateCode() { panic("GenerateCode() not implemented for IndexExpr") }

// Pos returns the start position of the expression in the source
func (e *IndexExpr) Pos() source.Pos { panic("Pos() not implemented for IndexExpr") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (e *IndexExpr) Resolve() { panic("Resolve() not implemented for IndexExpr") }

// ResolvedTypes does nothing
func (e *IndexExpr) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for IndexExpr")
}

func (e *IndexExpr) String() string {
	if e.Expr == nil {
		return fmt.Sprintf("(IndexExpr %s)", e.Index.String())
	} else {
		return fmt.Sprintf("(IndexExpr %s %s)", e.Expr.String(), e.Index.String())
	}
}

// ========== Literal Expression ==========

// LiteralExpr represents a character, float, integer or string literal node in the AST
type LiteralExpr struct {
	Kind     token.Token
	ValuePos source.Pos
	Value    string

	// Resolved
	resolvedType *symbol.Type
	intValue     int
}

func (e *LiteralExpr) exprNode() {}

// GenerateCode will generate C code for the AST node
func (e *LiteralExpr) GenerateCode() { panic("GenerateCode() not implemented for LiteralExpr") }

// Pos returns the start position of the expression in the source
func (e *LiteralExpr) Pos() source.Pos {
	return e.ValuePos
}

// Resolve will evaluate the literal and set the type appropriately.
func (e *LiteralExpr) Resolve() {
	switch e.Kind {
	case token.Integer:
		e.intValue, _ = strconv.Atoi(e.Value)
		e.resolvedType = symbol.CurrentScope.Lookup("Int32").(*symbol.Type)

	default:
		panic("Token " + e.Kind.String() + " not implemented in Resolve() for LiteralExpr")
	}
}

// ResolvedTypes does nothing
func (e *LiteralExpr) ResolvedTypes() []*symbol.Type {
	return []*symbol.Type{e.resolvedType}
}

func (e *LiteralExpr) String() string {
	return fmt.Sprintf("(LiteralExpr %s %s)", e.Kind.String(), e.Value)
}

// ========== New Expression ==========

// NewExpr represents a new expression node in the AST
type NewExpr struct {
	Type Expr
}

func (e *NewExpr) exprNode() {}

// GenerateCode will generate C code for the AST node
func (e *NewExpr) GenerateCode() { panic("GenerateCode() not implemented for NewExpr") }

// Pos returns the start position of the expression in the source
func (e *NewExpr) Pos() source.Pos { panic("Pos() not implemented for NewExpr") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (e *NewExpr) Resolve() { panic("Resolve() not implemented for NewExpr") }

// ResolvedTypes does nothing
func (e *NewExpr) ResolvedTypes() []*symbol.Type { panic("ResolvedTypes() not implemented for NewExpr") }

// String gives a human readable form of a CompositeExpr
func (e *NewExpr) String() string {
	return fmt.Sprintf("(NewExpr %s)", e.Type.String())
}

// ========== Selector Expression ==========

// SelectorExpr represents a selector node in the AST
type SelectorExpr struct {
	Expr Expr
	Name *Ident
}

func (e *SelectorExpr) exprNode() {}

// GenerateCode will generate C code for the AST node
func (e *SelectorExpr) GenerateCode() { panic("GenerateCode() not implemented for SelectorExpr") }

// Pos returns the start position of the expression in the source
func (e *SelectorExpr) Pos() source.Pos { panic("Pos() not implemented for SelectorExpr") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (e *SelectorExpr) Resolve() { panic("Resolve() not implemented for SelectorExpr") }

// ResolvedTypes does nothing
func (e *SelectorExpr) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for SelectorExpr")
}

func (e *SelectorExpr) String() string {
	return fmt.Sprintf("(SelectorExpr %s %s)", e.Expr.String(), e.Name.String())
}

// ========== Unary Expr ==========

// UnaryExpr represents a unary expression node in the AST
type UnaryExpr struct {
	Op         token.Token
	Expression Expr
}

func (e *UnaryExpr) exprNode() {}

// GenerateCode will generate C code for the AST node
func (e *UnaryExpr) GenerateCode() { panic("GenerateCode() not implemented for UnaryExpr") }

// Pos returns the start position of the expression in the source
func (e *UnaryExpr) Pos() source.Pos { panic("Pos() not implemented for UnaryExpr") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (e *UnaryExpr) Resolve() { panic("Resolve() not implemented for UnaryExpr") }

// ResolvedTypes does nothing
func (e *UnaryExpr) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for UnaryExpr")
}

// String gives a human readable form of a UnaryExpr
func (e *UnaryExpr) String() string {
	return fmt.Sprintf("(UnaryExpr %s %s)", e.Op.String(), e.Expression.String())
}

// ========== Array Type ==========

// ArrayType represents an array type node in the AST
type ArrayType struct {
	Length   Expr
	BaseType Expr
}

func (t *ArrayType) exprNode() {}

// GenerateCode will generate C code for the AST node
func (t *ArrayType) GenerateCode() { panic("GenerateCode() not implemented for ArrayType") }

// Pos returns the start position of the expression in the source
func (t *ArrayType) Pos() source.Pos { panic("Pos() not implemented for ArrayType") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (t *ArrayType) Resolve() { panic("Resolve() not implemented for ArrayType") }

// ResolvedTypes does nothing
func (t *ArrayType) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for ArrayType")
}

func (t *ArrayType) String() string {
	return fmt.Sprintf("(ArrayType %s %s)", t.Length.String(), t.BaseType.String())
}

// ========== Enum Type ==========

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

// GenerateCode will generate C code for the AST node
func (t *EnumType) GenerateCode() { panic("GenerateCode() not implemented for EnumType") }

// Pos returns the start position of the expression in the source
func (t *EnumType) Pos() source.Pos { panic("Pos() not implemented for EnumType") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (t *EnumType) Resolve() { panic("Resolve() not implemented for EnumType") }

// ResolvedTypes does nothing
func (t *EnumType) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for EnumType")
}

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

// ========== Pointer Type ==========

// PointerType represents a pointer type node in the AST
type PointerType struct {
	BaseType Expr
}

func (t *PointerType) exprNode() {}

// GenerateCode will generate C code for the AST node
func (t *PointerType) GenerateCode() { panic("GenerateCode() not implemented for PointerType") }

// Pos returns the start position of the expression in the source
func (t *PointerType) Pos() source.Pos { panic("Pos() not implemented for PointerType") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (t *PointerType) Resolve() { panic("Resolve() not implemented for PointerType") }

// ResolvedTypes does nothing
func (t *PointerType) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for PointerType")
}

func (t *PointerType) String() string {
	return fmt.Sprintf("(PointerType %s)", t.BaseType.String())
}

// ========== Slice Type ==========

// SliceType represents a slice type node in the AST
type SliceType struct {
	BaseType Expr
}

func (t *SliceType) exprNode() {}

// GenerateCode will generate C code for the AST node
func (t *SliceType) GenerateCode() { panic("GenerateCode() not implemented for SliceType") }

// Pos returns the start position of the expression in the source
func (t *SliceType) Pos() source.Pos { panic("Pos() not implemented for SliceType") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (t *SliceType) Resolve() { panic("Resolve() not implemented for SliceType") }

// ResolvedTypes does nothing
func (t *SliceType) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for SliceType")
}

func (t *SliceType) String() string {
	return fmt.Sprintf("(SliceType %s)", t.BaseType.String())
}

// ========== Struct Type ==========

// StructType represents a struct type node in the AST
type StructType struct {
	Fields []*Field
}

func (t *StructType) exprNode() {}

// GenerateCode will generate C code for the AST node
func (t *StructType) GenerateCode() { panic("GenerateCode() not implemented for StructType") }

// Pos returns the start position of the expression in the source
func (t *StructType) Pos() source.Pos { panic("Pos() not implemented for StructType") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (t *StructType) Resolve() { panic("Resolve() not implemented for StructType") }

// ResolvedTypes does nothing
func (t *StructType) ResolvedTypes() []*symbol.Type {
	panic("ResolvedTypes() not implemented for StructType")
}

func (t *StructType) String() string {
	return fmt.Sprintf("(StructType %s)", fieldListAsString(t.Fields))
}
