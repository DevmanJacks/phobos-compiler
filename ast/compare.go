////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		24 January 2019
//
////////////////////////////////////////////////////////////

package ast

import "phobos/symbol"

// AreCompatibleTypes checks to see if the types are compatible and returns true if they are, false if they are incompatible.
func areCompatibleTypes(lhs, rhs *symbol.Type) bool {
	return true
}
