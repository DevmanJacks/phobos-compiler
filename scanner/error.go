////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package scanner

import (
	"fmt"
	"phobos/source"
)

type error struct {
	pos     source.Pos
	message string
}

// Errors is the list of errors encountered so far
var errors []error

const maxErrorsBeforeTermination = 10

// Error records a lexical, syntax or semantic error
func Error(pos source.Pos, message string) {
	errors = append(errors, error{pos, message})

}

// PrintErrors prints all errors that have been generated during scanning, parsing and type resolution
func PrintErrors() {
	for _, err := range errors {
		fmt.Printf("%s: %s", err.pos.String(), err.message)
	}
}
