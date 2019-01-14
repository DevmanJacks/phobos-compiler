////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package main

import (
	"fmt"
	"phobos/scanner"
	"phobos/source"
	"phobos/token"
)

func main() {
	s := scanner.NewScanner(source.FromFile("/Users/mark/mars/Phobos/Token/Token.p"))

	for pos, tok, lexeme := s.Scan(); tok != token.EndOfFile; pos, tok, lexeme = s.Scan() {
		fmt.Printf("%s: %s #%s#\n", pos.String(), tok.String(), lexeme)
	}
}
