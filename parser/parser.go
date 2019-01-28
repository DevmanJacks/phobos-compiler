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

	compositeAllowed bool
}

// NewParser creates a parser to parse the phobos language
func NewParser(filename string) *Parser {
	return NewParserWithScanner(scanner.NewScanner(source.FromFile(filename)))
}

// NewParserWithScanner creates a parser to parse the phobos language
func NewParserWithScanner(s *scanner.Scanner) *Parser {
	p := new(Parser)
	p.scanner = s
	p.compositeAllowed = true

	p.nextToken()
	return p
}

// First sets
var firstDecl = map[token.Token]bool{
	token.Const:  true,
	token.Func:   true,
	token.Import: true,
	token.Type:   true,
	token.Var:    true,
}

var followExpr = map[token.Token]bool{}

var firstStmt = map[token.Token]bool{
	token.If: true,
}

var firstType = map[token.Token]bool{
	token.Const:  true,
	token.Func:   true,
	token.Import: true,
	token.Type:   true,
	token.Var:    true,
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

func (p *Parser) match(tok token.Token) bool {
	if p.tok == tok {
		p.nextToken()
		return true
	}

	return false
}

func (p *Parser) matchOrSynchronize(validTokens map[token.Token]bool, consumeToken bool) {
	if validTokens[p.tok] {
		if consumeToken {
			p.nextToken()
		}

		return
	}

	message := strings.Builder{}
	count := 1

	for tok := range validTokens {
		if count == 1 {
			message.WriteString("Expected ")
		} else {
			if count == len(validTokens) {
				message.WriteString(" or ")
			} else {
				message.WriteString(", ")
			}
		}

		message.WriteRune('\'')
		message.WriteString(tok.String())
		message.WriteRune('\'')
		count++
	}

	message.WriteRune('.')
	error(p.pos, message.String())
	p.synchronize(validTokens)
}

func (p *Parser) nextToken() {
	p.pos, p.tok, p.lexeme = p.scanner.Scan()
}

func (p *Parser) parseEnumItem() ast.EnumItem {
	ident := p.parseIdentifier()

	if p.tok == token.Assign {
		p.nextToken()
		expr := p.parseExpression()
		return ast.EnumItem{Name: ident, Value: expr}
	}

	return ast.EnumItem{Name: ident}
}

// ========== Expressions ==========

func (p *Parser) parseIdentifier() *ast.Ident {
	ident := &ast.Ident{NamePos: p.pos, Name: p.lexeme}
	p.nextToken()
	return ident
}

func (p *Parser) parseIdentifierList() []*ast.Ident {
	idents := []*ast.Ident{}

	for p.tok == token.Identifier {
		idents = append(idents, p.parseIdentifier())

		if p.tok == token.Comma {
			p.nextToken()
		} else {
			break
		}
	}

	return idents
}

func (p *Parser) parseOperand() ast.Expr {
	var expr ast.Expr

	switch p.tok {
	case token.Asterisk:
		expr = p.parsePointerType()

	case token.Minus, token.Plus, token.Not, token.BitwiseAnd, token.BitwiseNot:
		op := p.tok
		p.nextToken()
		expr = &ast.UnaryExpr{Op: op, Expression: p.parseExpression()}

	case token.Character, token.Float, token.Integer, token.String:
		expr = p.parseLiteralExpression()

	case token.Identifier, token.LeftBrace, token.LeftBracket:
		if p.tok != token.LeftBrace {
			expr = p.parseType()
		}

		if p.compositeAllowed && p.match(token.LeftBrace) {
			var elements []*ast.Element

			for !p.match(token.RightBrace) && p.tok != token.EndOfFile {
				var key ast.Expr
				var value ast.Expr

				if p.tok == token.Identifier || p.tok == token.LeftBracket {
					isIndex := p.match(token.LeftBracket)
					key = p.parseExpression()

					if isIndex {
						p.expect(token.RightBracket)
						key = &ast.IndexExpr{Index: key}
					}

					if p.tok == token.Assign {
						p.nextToken()
						value = p.parseExpression()
					} else {
						value = key
						key = nil
					}
				} else {
					value = p.parseExpression()
				}

				elements = append(elements, &ast.Element{Key: key, Value: value})

				if p.tok != token.RightBrace && p.tok != token.EndOfFile {
					p.expect(token.Comma)
				}
			}

			expr = &ast.CompositeExpr{Type: expr, Elements: elements}
		}

	case token.LeftParenthesis:
		p.nextToken()
		expr = p.parseExpression()
		p.expect(token.RightParenthesis)

	case token.New:
		p.nextToken()
		expr = &ast.NewExpr{Type: p.parseType()}

	case token.True, token.False:
		expr = &ast.BoolLiteralExpr{Op: p.tok}
		p.nextToken()

	default:
		p.notImplemented("parseOperand")
		return nil
	}

	for p.tok == token.Dot || p.tok == token.LeftBracket || p.tok == token.LeftParenthesis {
		switch p.tok {
		case token.Dot:
			p.nextToken()
			expr = &ast.SelectorExpr{Expr: expr, Name: p.parseIdentifier()}

		case token.LeftBracket:
			p.nextToken()
			expr = &ast.IndexExpr{Expr: expr, Index: p.parseExpression()}
			p.expect(token.RightBracket)

		case token.LeftParenthesis:
			p.nextToken()

			if p.tok == token.RightParenthesis {
				expr = &ast.CallExpr{Name: expr}
			} else {
				expr = &ast.CallExpr{Name: expr, Arguments: p.parseExpressionList()}
			}

			p.expect(token.RightParenthesis)
		}
	}

	return expr
}

func (p *Parser) parseBinaryExpr(precidenceLevel int) ast.Expr {
	expr := p.parseOperand()

	for p.tok.IsBinaryOp() {
		if p.tok.OperatorPrecedence() > precidenceLevel {
			op := p.tok
			p.nextToken()
			expr = &ast.BinaryExpr{Left: expr, Op: op, Right: p.parseBinaryExpr(p.tok.OperatorPrecedence())}
		} else {
			break
		}
	}

	return expr
}

func (p *Parser) parseLiteralExpression() ast.Expr {
	expr := &ast.LiteralExpr{Kind: p.tok, ValuePos: p.pos, Value: p.lexeme}
	p.nextToken()
	return expr
}

func (p *Parser) parseExpression() ast.Expr {
	return p.parseBinaryExpr(0)
}

func (p *Parser) parseExpressionList() []ast.Expr {
	exprs := []ast.Expr{}

	for {
		exprs = append(exprs, p.parseExpression())

		if !p.match(token.Comma) {
			break
		}
	}

	return exprs
}

// ---------- Types ----------

func (p *Parser) parseArrayType() ast.Expr {
	p.nextToken()
	var length ast.Expr

	if p.tok != token.RightBracket {
		length = p.parseExpression()
	}

	p.expect(token.RightBracket)
	baseType := p.parseType()

	if length == nil {
		return &ast.SliceType{BaseType: baseType}
	}

	return &ast.ArrayType{Length: length, BaseType: baseType}
}

func (p *Parser) parseEnumType() ast.Expr {
	enumPos := p.pos
	p.nextToken()

	if p.expect(token.LeftBrace) {
		var items []ast.EnumItem

		for p.tok != token.RightBrace && p.tok != token.EndOfFile {
			items = append(items, p.parseEnumItem())
		}

		if p.expect(token.RightBrace) {
			return &ast.EnumType{EnumPos: enumPos, Items: items}
		} else {
			error(p.pos, "Unexpected end of file in enum.")
		}
	}

	return &ast.BadExpr{From: enumPos, To: p.pos}
}

func (p *Parser) parsePointerType() ast.Expr {
	p.nextToken()
	return &ast.PointerType{BaseType: p.parseType()}
}

func (p *Parser) parseField() *ast.Field {
	names := p.parseIdentifierList()
	p.expect(token.Colon)
	typ := p.parseType()
	return &ast.Field{Names: names, Type: typ}
}

func (p *Parser) parseStructType() ast.Expr {
	structPos := p.pos
	p.nextToken()

	if p.expect(token.LeftBrace) {
		var fields []*ast.Field

		for p.tok != token.RightBrace && p.tok != token.EndOfFile {
			fields = append(fields, p.parseField())
		}

		if p.expect(token.RightBrace) {
			return &ast.StructType{Fields: fields}
		} else {
			error(p.pos, "Unexpected end of file in struct.")
		}
	}

	return &ast.BadExpr{From: structPos, To: p.pos}
}

func (p *Parser) parseType() ast.Expr {
	switch p.tok {
	case token.Asterisk:
		return p.parsePointerType()

	case token.Enum:
		return p.parseEnumType()

	case token.Identifier:
		return p.parseIdentifier()

	case token.LeftBracket:
		return p.parseArrayType()

	case token.Struct:
		return p.parseStructType()

	default:
		p.notImplemented("parseType")
		return nil
	}
}

// ========== Statements ==========

func (p *Parser) parseAssignmentOrCall() ast.Stmt {
	start := p.pos
	lhs := p.parseExpressionList()

	if p.tok.IsAssignOp() {
		assignPos := p.pos
		assignToken := p.tok
		p.nextToken()
		rhs := p.parseExpressionList()
		return &ast.AssignStmt{LHS: lhs, TokenPos: assignPos, Token: assignToken, RHS: rhs}
	} else {
		if len(lhs) == 1 {
			if call, ok := lhs[0].(*ast.CallExpr); ok {
				return &ast.ExprStmt{Expression: call}
			}
		}

		error(start, "Expected assignment or call.")
	}

	return &ast.BadStmt{From: start, To: p.pos}
}

func (p *Parser) parseBlockStatement() ast.Stmt {
	p.expect(token.LeftBrace)
	var stmts []ast.Stmt

	for p.tok != token.RightBrace && p.tok != token.EndOfFile {
		stmts = append(stmts, p.parseStatement())
	}

	if p.tok == token.EndOfFile {
		error(p.pos, "Unexpected end of file in block.")
	} else {
		p.expect(token.RightBrace)
	}

	return &ast.BlockStmt{Statements: stmts}
}

func (p *Parser) parseDeferStatement() ast.Stmt {
	p.nextToken()
	return &ast.DeferStmt{Statement: p.parseStatement()}
}

func (p *Parser) parseForStatement() ast.Stmt {
	p.nextToken()
	index := p.parseIdentifier()
	var item *ast.Ident

	if p.tok == token.Comma {
		p.nextToken()
		item = p.parseIdentifier()
	}

	p.expect(token.In)
	p.compositeAllowed = false
	collection := p.parseExpression()
	p.compositeAllowed = true
	body := p.parseBlockStatement()
	return &ast.ForStmt{Index: index, Item: item, Collection: collection, Body: body}
}

func (p *Parser) parseIfStatement() ast.Stmt {
	start := p.pos
	p.nextToken()
	p.compositeAllowed = false
	expr := p.parseExpression()
	p.compositeAllowed = true
	block := p.parseBlockStatement()

	if p.tok == token.Else {
		p.nextToken()
		if p.tok == token.If || p.tok == token.LeftBrace {
			return &ast.IfStmt{Expression: expr, Block: block, ElseStmt: p.parseStatement()}
		}

		error(p.pos, "Expected 'if' or '{'.")
		return &ast.BadStmt{From: start, To: p.pos}
	}

	return &ast.IfStmt{Expression: expr, Block: block}
}

func (p *Parser) parseReturnStatement() ast.Stmt {
	p.nextToken()
	return &ast.ReturnStmt{Expressions: p.parseExpressionList()}
}

func (p *Parser) parseCaseClause() *ast.CaseClause {
	p.nextToken()
	exprs := p.parseExpressionList()
	p.expect(token.Colon)

	var stmts []ast.Stmt

	for p.tok != token.Case && p.tok != token.Default && p.tok != token.RightBrace && p.tok != token.EndOfFile {
		stmts = append(stmts, p.parseStatement())
	}

	return &ast.CaseClause{Expressions: exprs, Statements: stmts}
}

func (p *Parser) parseSwitchStatement() ast.Stmt {
	p.nextToken()
	p.compositeAllowed = false
	expr := p.parseExpression()
	p.compositeAllowed = true
	p.expect(token.LeftBrace)

	gotDefault := false
	var defaults []ast.Stmt
	var clauses []*ast.CaseClause

	for p.tok == token.Case || p.tok == token.Default {
		if p.tok == token.Case {
			clauses = append(clauses, p.parseCaseClause())
		} else if p.tok == token.Default {
			if gotDefault {
				error(p.pos, "Default clause already specified.")
			} else {
				p.nextToken()
				p.expect(token.Colon)

				for p.tok != token.Case && p.tok != token.Default && p.tok != token.RightBrace && p.tok != token.EndOfFile {
					defaults = append(defaults, p.parseStatement())
				}

				gotDefault = true
			}
		} else {
			error(p.pos, "Expected 'case' or 'default'.")
		}
	}

	p.expect(token.RightBrace)
	return &ast.SwitchStmt{Expression: expr, CaseClauses: clauses, DefaultStatements: defaults}
}

func (p *Parser) parseWhileStatement() ast.Stmt {
	p.nextToken()
	p.compositeAllowed = false
	expr := p.parseExpression()
	p.compositeAllowed = true
	body := p.parseBlockStatement()
	return &ast.WhileStmt{Expression: expr, Body: body}
}

func (p *Parser) parseStatement() ast.Stmt {
	switch p.tok {
	case token.Const, token.Type, token.Var:
		return &ast.DeclStmt{Declaration: p.parseDecl()}

	case token.Defer:
		return p.parseDeferStatement()

	case token.For:
		return p.parseForStatement()

	case token.Identifier:
		return p.parseAssignmentOrCall()

	case token.If:
		return p.parseIfStatement()

	case token.LeftBrace:
		return p.parseBlockStatement()

	case token.Return:
		return p.parseReturnStatement()

	case token.Switch:
		return p.parseSwitchStatement()

	case token.While:
		return p.parseWhileStatement()

	default:
		p.notImplemented("parseStatement")
		return nil
	}
}

// ========== Declarations ==========

func (p *Parser) parseConstDecl() ast.Decl {
	pos := p.pos // const keyword Pos
	p.nextToken()

	if p.tok == token.Identifier {
		name := p.parseIdentifier()

		var identType ast.Expr

		if p.tok == token.Colon {
			p.nextToken()
			identType = p.parseType()
		} else if p.tok == token.Identifier {
			error(p.pos, "Expected ':' before type.")
			identType = p.parseType()
		}

		p.expect(token.Assign)
		value := p.parseExpression()

		return &ast.ConstDecl{Name: name, Type: identType, Value: value}
	}

	p.expect(token.Identifier)
	return &ast.BadDecl{From: pos, To: p.pos}
}

func (p *Parser) parseParameter(nameRequired bool) *ast.Parameter {
	pos := p.pos
	expr := p.parseExpression()
	var name *ast.Ident
	var typ ast.Expr

	if p.match(token.Colon) {
		typ = p.parseExpression()

		if ident, ok := expr.(*ast.Ident); ok {
			name = ident
		} else {
			error(pos, "Expected a parameter name.")
		}
	} else {
		typ = expr
	}

	return &ast.Parameter{Name: name, Type: typ}
}

func (p *Parser) parseSignature() *ast.Signature {
	p.matchOrSynchronize(map[token.Token]bool{token.LeftParenthesis: true}, true)

	var params []*ast.Parameter
	var returns []*ast.Parameter

	for p.tok != token.RightParenthesis && p.tok != token.EndOfFile {
		params = append(params, p.parseParameter(true))
		p.match(token.Comma) // Optional ,
	}

	if p.tok == token.EndOfFile {
		error(p.pos, "Unexpected end of file in function signature.")
	} else {
		p.expect(token.RightParenthesis)

		if p.match(token.Returns) {
			p.compositeAllowed = false

			if p.match(token.LeftParenthesis) {
				for p.tok != token.RightParenthesis && p.tok != token.EndOfFile {
					returns = append(returns, p.parseParameter(false))
					p.match(token.Comma)
				}

				if p.tok == token.EndOfFile {
					error(p.pos, "Unexpected end of file in function signature.")
				} else {
					p.expect(token.RightParenthesis)
				}
			} else {
				returns = append(returns, p.parseParameter(false))
			}

			p.compositeAllowed = true
		}
	}

	return &ast.Signature{Params: params, Returns: returns}
}

func (p *Parser) parseFuncDecl() ast.Decl {
	pos := p.pos // func keyword Pos
	p.nextToken()

	if p.tok == token.Identifier {
		name := p.parseIdentifier()

		if p.match(token.Dot) {
			if p.tok == token.Identifier {
				methodType := name
				name = p.parseIdentifier()
				signature := p.parseSignature()
				body := p.parseBlockStatement()
				return &ast.FuncDecl{MethodType: methodType, Name: name, Signature: signature, Body: body.(*ast.BlockStmt)}
			}

			error(p.pos, "Expected method name.")

			if p.tok == token.LeftParenthesis { // Just mathod name missing
				p.parseSignature()
				p.parseBlockStatement()
				return &ast.BadDecl{From: pos, To: p.pos}
			}

			// TODO: recover - don't know where to yet
		} else {
			signature := p.parseSignature()
			body := p.parseBlockStatement()
			return &ast.FuncDecl{Name: name, Signature: signature, Body: body.(*ast.BlockStmt)}
		}
	}

	p.expect(token.Identifier)
	// TODO: recover
	return &ast.BadDecl{From: pos, To: p.pos}
}

func (p *Parser) parseImportDecl() ast.Decl {
	p.nextToken()
	return &ast.ImportDecl{Package: p.parseExpression()}
}

func (p *Parser) parseTypeDecl() ast.Decl {
	pos := p.pos // type keyword Pos
	p.nextToken()

	if p.tok == token.Identifier {
		name := p.parseIdentifier()
		spec := p.parseType()
		return &ast.TypeDecl{Name: name, Spec: spec}
	}

	p.expect(token.Identifier)

	// Assume that type name has been missed.  If this causes too many errors we will change later
	p.parseType()
	return &ast.BadDecl{From: pos, To: p.pos}
}

func (p *Parser) parseVarDecl() ast.Decl {
	pos := p.pos // var keyword Pos
	p.nextToken()

	if p.tok == token.Identifier {
		names := p.parseIdentifierList()

		var identType ast.Expr

		if p.tok == token.Colon {
			p.nextToken()
			identType = p.parseType()
		} else if p.tok == token.Identifier {
			error(p.pos, "Expected ':' before type.")
			identType = p.parseType()
		}

		var exprs []ast.Expr

		if p.tok == token.Assign {
			p.nextToken()
			exprs = p.parseExpressionList()
		} else if identType == nil {
			p.expect(token.Assign)
		}

		return &ast.VarDecl{VarPos: pos, Names: names, Type: identType, Values: exprs}
	}

	p.expect(token.Identifier)
	return &ast.BadDecl{From: pos, To: p.pos}
}

func (p *Parser) parseDecl() ast.Decl {
	// Synchronize to a valid declaration keyword
	p.matchOrSynchronize(firstDecl, false)

	switch p.tok {
	case token.Const:
		return p.parseConstDecl()

	case token.Func:
		return p.parseFuncDecl()

	case token.Import:
		return p.parseImportDecl()

	case token.Type:
		return p.parseTypeDecl()

	case token.Var:
		return p.parseVarDecl()

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

// ********** TEMP - will remove when parser is fully implemented **********

func (p *Parser) notImplemented(fnName string) {
	source.PrintErrors()
	fmt.Printf("\n%s: FATAL ERROR: Token '%s' not implemented in %s().\n\n", p.pos.String(), p.tok.String(), fnName)
	os.Exit(1)
}
