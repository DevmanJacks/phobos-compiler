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
	"io/ioutil"
	"phobos/scanner"
	"phobos/token"
)

func main() {
	bytes, err := ioutil.ReadFile("/Users/mark/mars/Phobos/Token/Token.p")

	if err != nil {
		fmt.Print(err)
	} else {
		s := scanner.NewScanner(string(bytes))

		for pos, tok, lexeme := s.Scan(); tok != token.EndOfFile; pos, tok, lexeme = s.Scan() {
			fmt.Printf("%d: %s #%s#\n", int(pos), tok.String(), lexeme)
		}
	}
}
