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

	IDENT
	INT

	BREAK
	WHILE
)

// Token strings
var tokens = []string{
	ILLEGAL: "ILLEGAL",

	IDENT: "IDENT",
	INT:   "INT",

	BREAK: "break",
	WHILE: "while",
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

var keywords = map[string]Token{
	"break": BREAK,
	"while": WHILE,
}

// CheckIdentifier maps an identifier to it's keyword token or IDENT if not a keyword
func CheckIdentifier(ident string) Token {
	if tok, found := keywords[ident]; found {
		return tok
	}

	return IDENT
}
