////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package token

// Pos is a compact way to store the position in a source file.
type Pos int

// NoPos represents no position.  This is mainly used when we manually add nodes into the AST.
const NoPos = 0
