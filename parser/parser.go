////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		13 January 2019
//
////////////////////////////////////////////////////////////

package parser

import (
	"phobos/ast"
	"phobos/scanner"
	"phobos/token"
)

// Parser represents a parser for the phobos language
type Parser struct {
	scanner *scanner.Scanner

	// Current token
	pos    token.Pos
	tok    token.Token
	lexeme string
}

// NewParser creates a parser to parse the phobos language
func NewParser(file token.File) *Parser {
	p := new(Parser)
	return p
}

func (p *Parser) nextToken() {
	p.pos, p.tok, p.lexeme = p.scanner.Scan()
}

func (p *Parser) parseIdentifier() ast.Ident {
	return ast.NewIdent(p.pos, p.lexeme)
}

func (p *Parser) parseDecl() ast.Decl {
	pos, tok, lexeme := p.s.Scan()
	return ast.Decl{}
}

// Parse the source file and build an AST
func (p *Parser) Parse() []ast.Decl {
	var decls []ast.Decl
	return decls
}
