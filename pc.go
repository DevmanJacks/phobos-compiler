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
	"phobos/parser"
	"phobos/source"
)

func main() {
	// s := scanner.NewScanner(source.FromFile("/Users/mark/mars/Phobos/Token/Token.p"))

	// for pos, tok, lexeme := s.Scan(); tok != token.EndOfFile; pos, tok, lexeme = s.Scan() {
	// 	fmt.Printf("%s: %s #%s#\n", pos.String(), tok.String(), lexeme)
	// }

	p := parser.NewParser("/Users/mark/mars/Phobos/Token/Token.p")
	decls := p.Parse()

	if source.ErrorCount > 0 {
		source.PrintErrors()
	}

	for _, decl := range decls {
		fmt.Print(decl.String())
		fmt.Print(" ")
	}

	fmt.Print("\n\n")
}
