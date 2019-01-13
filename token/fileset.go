////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		13 January 2019
//
////////////////////////////////////////////////////////////

package token

// Fileset represents all of the source files that we will be parsing
type Fileset struct {
	filename string
	src      []uint8
	length   int
}

// AddFile adds a file, given the filename, to a Fileset.
func (f *Fileset) AddFile(filename string) {

}
