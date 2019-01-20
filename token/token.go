////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package token

import "strconv"

// Token is the set of lexical tokens for phobos
type Token int

// The list of tokens
const (
	ILLEGAL = iota

	EndOfFile
	Identifier
	Integer
	Float
	Character
	String

	LeftBrace
	RightBrace
	LeftBracket
	RightBracket
	LeftParenthesis
	RightParenthesis
	Colon
	Comma
	Returns
	Dot
	DotDot

	Asterisk
	Divide
	Mod
	ShiftLeft
	ShiftRight
	BitwiseAnd

	Plus
	Minus
	BitwiseOr

	Equal
	NotEqual
	LessThan
	LessThanOrEqual
	GreaterThan
	GreaterThanOrEqual

	LogicalAnd
	LogicalOr

	BitwiseNot
	Not

	Assign
	DeclareAssign
	DivideEquals
	MinusEquals
	ModEquals
	MultiplyEquals
	PlusEquals
	BitwiseAndEquals
	BitwiseOrEquals
	BitwiseNotEquals
	ShiftLeftEquals
	ShiftRightEquals

	As
	Break
	Case
	Const
	Continue
	Default
	Defer
	Else
	Enum
	False
	For
	Func
	If
	Import
	In
	Interface
	New
	Return
	Struct
	Switch
	True
	Type
	Var
	While
)

// Token strings
var tokens = []string{
	ILLEGAL: "ILLEGAL",

	EndOfFile:  "EndOfFile",
	Identifier: "Identifier",
	Integer:    "Integer",
	Float:      "Float",
	Character:  "Character",
	String:     "String",

	LeftBrace:        "{",
	RightBrace:       "}",
	LeftBracket:      "[",
	RightBracket:     "]",
	LeftParenthesis:  "(",
	RightParenthesis: ")",
	Colon:            ":",
	Comma:            ",",
	Returns:          "->",
	Dot:              ".",
	DotDot:           "..",

	// Precedence 5
	Asterisk:   "*",
	Divide:     "/",
	Mod:        "%",
	ShiftLeft:  "<<",
	ShiftRight: ">>",
	BitwiseAnd: "&",

	// Precedence 4
	Plus:      "+",
	Minus:     "-",
	BitwiseOr: "|",

	Equal:              "==",
	NotEqual:           "!=",
	LessThan:           "<",
	LessThanOrEqual:    "<=",
	GreaterThan:        ">",
	GreaterThanOrEqual: ">=",

	LogicalAnd: "&&",
	LogicalOr:  "||",

	BitwiseNot: "~",
	Not:        "!",

	Assign:           "=",
	DeclareAssign:    ":=",
	DivideEquals:     "/=",
	MinusEquals:      "-=",
	ModEquals:        "%=",
	MultiplyEquals:   "*=",
	PlusEquals:       "+=",
	BitwiseAndEquals: "&=",
	BitwiseOrEquals:  "|=",
	BitwiseNotEquals: "~=",
	ShiftLeftEquals:  "<<=",
	ShiftRightEquals: ">>=",

	As:        "as",
	Break:     "break",
	Case:      "case",
	Const:     "const",
	Continue:  "continue",
	Default:   "default",
	Defer:     "defer",
	Else:      "else",
	Enum:      "enum",
	False:     "false",
	For:       "for",
	Func:      "func",
	If:        "if",
	Import:    "import",
	In:        "in",
	Interface: "interface",
	New:       "new",
	Return:    "return",
	Struct:    "struct",
	Switch:    "switch",
	True:      "true",
	Type:      "type",
	Var:       "var",
	While:     "while",
}

// IsAssignOp determines if the token is a valid assignment operator
func (t Token) IsAssignOp() bool {
	return Assign <= t && t <= ShiftRightEquals
}

// IsBinaryOp determines if the token is a valid binary operator
func (t Token) IsBinaryOp() bool {
	return Asterisk <= t && t <= LogicalOr
}

func (t Token) String() string {
	s := ""

	if 0 <= t && t < Token(len(tokens)) {
		s = tokens[t]
	} else {
		s = "token(" + strconv.Itoa(int(t)) + ")"
	}

	return s
}

// OperatorPrecedence determines the precedence of the operator token.  If the token is not an operator it has precedence 1
func (t Token) OperatorPrecedence() int {
	if Asterisk <= t && t <= BitwiseAnd {
		return 5
	}

	if Plus <= t && t <= BitwiseOr {
		return 4
	}

	if Equal <= t && t <= GreaterThanOrEqual {
		return 3
	}

	return 1
}

var keywords = map[string]Token{
	"as":        As,
	"break":     Break,
	"case":      Case,
	"const":     Const,
	"continue":  Continue,
	"default":   Default,
	"defer":     Defer,
	"else":      Else,
	"enum":      Enum,
	"false":     False,
	"for":       For,
	"func":      Func,
	"if":        If,
	"import":    Import,
	"in":        In,
	"interface": Interface,
	"new":       New,
	"return":    Return,
	"struct":    Struct,
	"switch":    Switch,
	"true":      True,
	"type":      Type,
	"var":       Var,
	"while":     While,
}

// CheckIdentifier maps an identifier to it's keyword token or IDENT if not a keyword
func CheckIdentifier(ident string) Token {
	if tok, found := keywords[ident]; found {
		return tok
	}

	return Identifier
}
