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

// Stmt represents a statement node in the AST
type Stmt interface {
	Node
	stmtNode()
}

// ========== Assignment Statement ==========

// AssignStmt represents an assignment statement node in the AST
type AssignStmt struct {
	LHS      []Expr
	Token    token.Token
	TokenPos source.Pos
	RHS      []Expr
}

func (s *AssignStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *AssignStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *AssignStmt) Pos() source.Pos { panic("Pos() not implemented for AssignStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *AssignStmt) Resolve() {}

func (s *AssignStmt) String() string {
	str := strings.Builder{}
	str.WriteString("(AssignStmt ")
	str.WriteString(expressionListAsString(s.LHS))
	str.WriteRune(' ')
	str.WriteString(s.Token.String())
	str.WriteRune(' ')
	str.WriteString(expressionListAsString(s.RHS))
	str.WriteRune(')')
	return str.String()
}

// ========== Bad Statement ==========

// BadStmt represents a bad statement
type BadStmt struct {
	From, To source.Pos
}

func (s *BadStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *BadStmt) GenerateCode() { panic("Cannot generate code for a program with errors") }

// Pos returns the start position of the expression in the source
func (s *BadStmt) Pos() source.Pos { panic("Pos() not implemented for BadStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *BadStmt) Resolve() {}

// String gives a human readable form of a TypeDecl
func (s *BadStmt) String() string {
	return fmt.Sprintf("(BadStmt %d %d)", s.From, s.To)
}

// ========== Block Statement ==========

// BlockStmt represents a block statement node in the AST
type BlockStmt struct {
	Statements []Stmt
}

func (s *BlockStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *BlockStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *BlockStmt) Pos() source.Pos { panic("Pos() not implemented for BlockStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *BlockStmt) Resolve() {}

func (s *BlockStmt) String() string {
	str := strings.Builder{}
	str.WriteString("(BlockStmt (")
	first := true

	for _, stmt := range s.Statements {
		if first {
			first = false
		} else {
			str.WriteRune(' ')
		}

		str.WriteString(stmt.String())
	}

	str.WriteString("))")
	return str.String()
}

// ========== Declaration Statement ==========

// DeclStmt represents a declaration statement node in the AST
type DeclStmt struct {
	Declaration Decl
}

func (s *DeclStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *DeclStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *DeclStmt) Pos() source.Pos { panic("Pos() not implemented for DeclStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *DeclStmt) Resolve() {}

func (s *DeclStmt) String() string {
	return fmt.Sprintf("(DeclStmt %s)", s.Declaration.String())
}

// ========== Defer Statement ==========

// DeferStmt represents a defer statement node in the AST
type DeferStmt struct {
	Statement Stmt
}

func (s *DeferStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *DeferStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *DeferStmt) Pos() source.Pos { panic("Pos() not implemented for DeferStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *DeferStmt) Resolve() {}

func (s *DeferStmt) String() string {
	return fmt.Sprintf("(DeferStmt %s)", s.Statement.String())
}

// ========== Expression Statement ==========

// ExprStmt represents an expression statement node in the AST
type ExprStmt struct {
	Expression Expr
}

func (s *ExprStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *ExprStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *ExprStmt) Pos() source.Pos { panic("Pos() not implemented for ExprStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *ExprStmt) Resolve() {}

func (s *ExprStmt) String() string {
	return fmt.Sprintf("(ExprStmt %s)", s.Expression.String())
}

// ========== For Statement ==========

// ForStmt represents a for statement node in the AST
type ForStmt struct {
	Index      *Ident
	Item       *Ident
	Collection Expr
	Body       Stmt
}

func (s *ForStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *ForStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *ForStmt) Pos() source.Pos { panic("Pos() not implemented for ForStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *ForStmt) Resolve() {}

func (s *ForStmt) String() string {
	str := strings.Builder{}
	str.WriteString("(ForStmt ")
	str.WriteString(s.Index.String())
	str.WriteRune(' ')

	if s.Item == nil {
		str.WriteString("()")
	} else {
		str.WriteString(s.Item.String())
	}

	str.WriteRune(' ')
	str.WriteString(s.Collection.String())
	str.WriteRune(' ')
	str.WriteString(s.Body.String())
	str.WriteRune(')')
	return str.String()
}

// ========== If Statement ==========

// IfStmt represents an if statement node in the AST
type IfStmt struct {
	Expression Expr
	Block      Stmt
	ElseStmt   Stmt
}

func (s *IfStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *IfStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *IfStmt) Pos() source.Pos { panic("Pos() not implemented for IfStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *IfStmt) Resolve() {}

func (s *IfStmt) String() string {
	str := strings.Builder{}
	str.WriteString("(IfStmt ")
	str.WriteString(s.Expression.String())
	str.WriteString(s.Block.String())

	if s.ElseStmt != nil {
		str.WriteRune(' ')
		str.WriteString(s.ElseStmt.String())
	}

	str.WriteRune(')')
	return str.String()
}

// ========== Return Statement ==========

// ReturnStmt represents a return statement node in the AST
type ReturnStmt struct {
	Expressions []Expr
}

func (s *ReturnStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *ReturnStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *ReturnStmt) Pos() source.Pos { panic("Pos() not implemented for ReturnStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *ReturnStmt) Resolve() {}

func (s *ReturnStmt) String() string {
	return "(ReturnStmt " + expressionListAsString(s.Expressions) + ")"
}

// ========== Switch Statement ==========

// CaseClause represents a case clause node in the AST
type CaseClause struct {
	Expressions []Expr
	Statements  []Stmt
}

func (c *CaseClause) String() string {
	s := strings.Builder{}
	s.WriteString("(Case ")
	s.WriteString(expressionListAsString(c.Expressions))
	s.WriteString(" (")
	first := true

	for _, stmt := range c.Statements {
		if first {
			first = false
		} else {
			s.WriteRune(' ')
		}

		s.WriteString(stmt.String())
	}

	s.WriteString("))")
	return s.String()
}

// SwitchStmt represents a switch statement node in the AST
type SwitchStmt struct {
	Expression        Expr
	CaseClauses       []*CaseClause
	DefaultStatements []Stmt
}

func (s *SwitchStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *SwitchStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *SwitchStmt) Pos() source.Pos { panic("Pos() not implemented for SwitchStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *SwitchStmt) Resolve() {}

func (s *SwitchStmt) String() string {
	str := strings.Builder{}
	str.WriteString("(SwitchStmt ")
	str.WriteString(s.Expression.String())
	str.WriteRune(' ')
	first := true

	for _, clause := range s.CaseClauses {
		if first {
			first = false
		} else {
			str.WriteRune(' ')
		}

		str.WriteString(clause.String())
	}

	if s.DefaultStatements != nil {
		first = true
		str.WriteString("(")

		for _, stmt := range s.DefaultStatements {
			if first {
				first = false
			} else {
				str.WriteRune(' ')
			}

			str.WriteString(stmt.String())
		}
	}

	str.WriteString("))")
	return str.String()
}

// ========== While Statement =========

// WhileStmt represents a while statement node in the AST
type WhileStmt struct {
	Expression Expr
	Body       Stmt
}

func (s *WhileStmt) stmtNode() {}

// GenerateCode will generate C code for the AST node
func (s *WhileStmt) GenerateCode() {}

// Pos returns the start position of the expression in the source
func (s *WhileStmt) Pos() source.Pos { panic("Pos() not implemented for WhileStmt") }

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *WhileStmt) Resolve() {}

func (s *WhileStmt) String() string {
	str := strings.Builder{}
	str.WriteString("(WhileStmt ")
	str.WriteString(s.Expression.String())
	str.WriteString(s.Body.String())
	str.WriteRune(')')
	return str.String()
}
