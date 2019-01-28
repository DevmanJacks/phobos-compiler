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
	"phobos/symbol"
	"strings"
)

const width = 132

func prettyPrint(s string, indent int) {
	if len(s) < width {
		fmt.Print(s)
	} else {
		// Line too long so split it on sensible boundary
		lparenPos := strings.Index(s[1:], "(") + 1
		indent += lparenPos
		fmt.Print(s[:lparenPos])

		parenCount := 0
		completeExpression := false
		start := 0
		sub := s[lparenPos:]

		for i, c := range sub {
			if c == '(' && !completeExpression {
				parenCount++
			} else if c == ')' {
				parenCount--

				if parenCount == 0 {
					completeExpression = true
				}
			} else if completeExpression && c != ' ' && c != ')' {
				prettyPrint(sub[start:i], indent)
				fmt.Print("\n")

				// Indent next line
				for i := 0; i < indent; i++ {
					fmt.Print(" ")
				}

				start = i
				completeExpression = false

				if c == '(' {
					parenCount++
				}
			}
		}

		prettyPrint(sub[start:], indent)
		fmt.Print("\n")
	}
}

func main() {
	// TEMP: Remove these when we get proper initialisation sorted
	symbol.CurrentScope = symbol.NewScope(nil)
	symbol.CurrentScope.Insert(symbol.NewSynonymType("Int32"))

	p := parser.NewParser("/Users/mark/mars/Phobos/pc.p")
	decls := p.Parse()

	if source.ErrorCount > 0 {
		source.PrintErrors()
	}

	for _, decl := range decls {
		decl.Resolve()

		s := decl.String()
		prettyPrint(s, 0)
		fmt.Print("\n")
	}

	fmt.Print("\n\n")
}
