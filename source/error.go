////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package source

import (
	"fmt"
)

type error struct {
	pos     Pos
	message string
}

// Errors is the list of errors encountered so far
var errors []error

// ErrorCount is the number of errors encountered during compilation
var ErrorCount int

const maxErrorsBeforeTermination = 10

// Error records a lexical, syntax or semantic error
func Error(pos Pos, message string) {
	errors = append(errors, error{pos, message})
	ErrorCount++
}

// PrintErrors prints all errors that have been generated during scanning, parsing and type resolution
func PrintErrors() {
	for _, err := range errors {
		fmt.Printf("%s: %s\n", err.pos.String(), err.message)
	}
}
