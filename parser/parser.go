////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		13 January 2019
//
////////////////////////////////////////////////////////////

package parser

import (
	"fmt"
	"os"
	"phobos/ast"
	"phobos/scanner"
	"phobos/source"
	"phobos/token"
	"strings"
)

// Parser represents a parser for the phobos language
type Parser struct {
	scanner *scanner.Scanner

	// Current token
	pos    source.Pos
	tok    token.Token
	lexeme string
}

// NewParser creates a parser to parse the phobos language
func NewParser(filename string) *Parser {
	p := new(Parser)
	p.scanner = scanner.NewScanner(source.FromFile(filename))
	p.nextToken()
	return p
}

// First sets
var firstDecl = map[token.Token]bool{
	token.Const: true,
	token.Func:  true,
	token.Type:  true,
	token.Var:   true,
}

var followExpr = map[token.Token]bool{}

var firstStmt = map[token.Token]bool{
	token.If: true,
}

func error(pos source.Pos, message string) {
	source.Error(pos, message)
}

// Synchronize the parser to one of the tokens in the map
func (p *Parser) synchronize(tokens map[token.Token]bool) {
	for !tokens[p.tok] && p.tok != token.EndOfFile {
		p.nextToken()
	}
}

func (p *Parser) expect(tok token.Token) bool {
	if p.tok == tok {
		p.nextToken()
		return true
	}

	error(p.pos, "Expected '"+tok.String()+"'.")
	return false
}

func (p *Parser) matchToken(validTokens map[token.Token]bool, consumeToken bool) {
	if validTokens[p.tok] {
		if consumeToken {
			p.nextToken()
		}

		return
	}

	message := strings.Builder{}
	first := true

	for tok := range validTokens {
		if first {
			message.WriteString("Expected ")
			first = false
		} else {
			message.WriteString(", ")
		}

		message.WriteRune('\'')
		message.WriteString(tok.String())
		message.WriteRune('\'')
	}

	error(p.pos, message.String())
	p.synchronize(validTokens)
}

func (p *Parser) nextToken() {
	p.pos, p.tok, p.lexeme = p.scanner.Scan()
}

func (p *Parser) parseIdentifier() *ast.Ident {
	ident := &ast.Ident{Pos: p.pos, Name: p.lexeme}
	p.nextToken()
	return ident
}

func (p *Parser) parseEnumItem() ast.EnumItem {
	ident := p.parseIdentifier()

	if p.tok == token.Assign {
		p.nextToken()
		expr := p.parseExpr()
		return ast.EnumItem{Name: ident, Value: expr}
	}

	return ast.EnumItem{Name: ident}
}

// ========== Expressions ==========

func (p *Parser) parseExpr() ast.Expr {
	return nil
}

func (p *Parser) parseEnumType() ast.Expr {
	enumPos := p.pos
	p.nextToken()

	if p.expect(token.LeftBrace) {
		items := make([]ast.EnumItem, 10)

		for p.tok != token.RightBrace && p.tok != token.EndOfFile {
			items = append(items, p.parseEnumItem())
		}

		if p.expect(token.RightBrace) {
			return &ast.EnumType{EnumPos: enumPos, Items: items}
		}
	}

	return &ast.BadExpr{From: enumPos, To: p.pos}
}

func (p *Parser) parseTypeSpec() ast.Expr {
	switch p.tok {
	case token.Enum:
		return p.parseEnumType()

	default:
		p.notImplemented("parseTypeSpec")
		return nil
	}
}

// ========== Declarations ==========

func (p *Parser) parseTypeDecl() ast.Decl {
	pos := p.pos // type keyword Pos
	p.nextToken()

	if p.tok == token.Identifier {
		ident := p.parseIdentifier()
		spec := p.parseTypeSpec()
		return &ast.TypeDecl{Name: ident, Spec: spec}
	}

	p.expect(token.Identifier)

	// Assume that type name has been missed.  If this causes too many errors we will change later
	p.parseTypeSpec()
	return &ast.BadDecl{From: pos, To: p.pos}
}

func (p *Parser) parseDecl() ast.Decl {
	// Synchroonize to a valid declaration keyword
	p.matchToken(firstDecl, false)

	switch p.tok {
	case token.Type:
		return p.parseTypeDecl()

	default:
		p.notImplemented("parseDecl")
		return nil
	}
}

// Parse the source file and build an AST
func (p *Parser) Parse() []ast.Decl {
	var decls []ast.Decl

	for p.tok != token.EndOfFile {
		decls = append(decls, p.parseDecl())
	}

	return decls
}

// ********** TEMP **********

func (p *Parser) notImplemented(fnName string) {
	source.PrintErrors()
	fmt.Printf("\nFATAL ERROR: Token '%s' not implemented in %s().\n\n", p.tok.String(), fnName)
	os.Exit(1)
}
