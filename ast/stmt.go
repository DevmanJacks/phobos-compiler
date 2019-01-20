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

// AssignStmt represents an assignment statement node in the AST
type AssignStmt struct {
	LHS      []Expr
	Token    token.Token
	TokenPos source.Pos
	RHS      []Expr
}

func (s *AssignStmt) stmtNode() {}

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

// BadStmt represents a bad statement
type BadStmt struct {
	From, To source.Pos
}

func (s *BadStmt) stmtNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *BadStmt) Resolve() {}

// String gives a human readable form of a TypeDecl
func (s *BadStmt) String() string {
	return fmt.Sprintf("(BadStmt %d %d)", s.From, s.To)
}

// BlockStmt represents a block statement node in the AST
type BlockStmt struct {
	Statements []Stmt
}

func (s *BlockStmt) stmtNode() {}

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

// DeclStmt represents a declaration statement node in the AST
type DeclStmt struct {
	Declaration Decl
}

func (s *DeclStmt) stmtNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *DeclStmt) Resolve() {}

func (s *DeclStmt) String() string {
	return fmt.Sprintf("(DeclStmt %s)", s.Declaration.String())
}

// DeferStmt represents a defer statement node in the AST
type DeferStmt struct {
	Statement Stmt
}

func (s *DeferStmt) stmtNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *DeferStmt) Resolve() {}

func (s *DeferStmt) String() string {
	return fmt.Sprintf("(DeferStmt %s)", s.Statement.String())
}

// ExprStmt represents an expression statement node in the AST
type ExprStmt struct {
	Expression Expr
}

func (s *ExprStmt) stmtNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *ExprStmt) Resolve() {}

func (s *ExprStmt) String() string {
	return fmt.Sprintf("(ExprStmt %s)", s.Expression.String())
}

// ForStmt represents a for statement node in the AST
type ForStmt struct {
	Index      *Ident
	Item       *Ident
	Collection Expr
	Body       Stmt
}

func (s *ForStmt) stmtNode() {}

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

// IfStmt represents an if statement node in the AST
type IfStmt struct {
	Expression Expr
	Block      Stmt
	ElseStmt   Stmt
}

func (s *IfStmt) stmtNode() {}

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

// ReturnStmt represents a return statement node in the AST
type ReturnStmt struct {
	Expressions []Expr
}

func (s *ReturnStmt) stmtNode() {}

// Resolve will infer types for any unspecified identifiers and constants and perform type checking
func (s *ReturnStmt) Resolve() {}

func (s *ReturnStmt) String() string {
	return "(ReturnStmt " + expressionListAsString(s.Expressions) + ")"
}

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

// WhileStmt represents a while statement node in the AST
type WhileStmt struct {
	Expression Expr
	Body       Stmt
}

func (s *WhileStmt) stmtNode() {}

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
