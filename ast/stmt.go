////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package ast

import (
	"strings"
)

// Stmt represents a statement node in the AST
type Stmt interface {
	Node
	stmtNode()
}

// BlockStmt represents a block statement node in the AST
type BlockStmt struct {
	Statements []Stmt
}

func (s *BlockStmt) stmtNode() {}

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
