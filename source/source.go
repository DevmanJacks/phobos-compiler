////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		13 January 2019
//
////////////////////////////////////////////////////////////

package source

import "io/ioutil"

// The list of sources that make up the application
var sources = make(map[string]*Source)
var base = 1

// Source represents a single source file
type Source struct {
	filename string
	code     []uint8

	base   int
	length int

	lines []Pos // the starting Pos of each line in code
}

// Code gets the code from the Source
func (s *Source) Code() []uint8 {
	return s.code
}

// AddLine records that a new line has started in src
func (s *Source) AddLine(pos Pos) {
	s.lines = append(s.lines, pos)
}

// Pos returns the Pos of an offset within the specified Source
// offset will be s.length when we are at end of file
func (s *Source) Pos(offset int) Pos {
	if 0 <= offset && offset <= s.length {
		return Pos(s.base + offset)
	}

	panic("offset out of range for file.")
}

// FromFile creates a Source from the file and adds it to the list of sources
func FromFile(filename string) *Source {
	src, ok := sources[filename]

	if !ok {
		bytes, err := ioutil.ReadFile(filename)

		if err != nil {
			panic(err)
		}

		src = &Source{filename: filename, code: bytes, base: base, length: len(bytes), lines: []Pos{1}}
		base += len(bytes)
	}

	sources[filename] = src
	return src
}

// FromString will create a Source with the code from the specified string.  The Source will be added to the list of sources
func FromString(src string) *Source {
	s := &Source{filename: "<Anonymous>", code: []uint8(src), base: base, length: len(src), lines: []Pos{1}}
	base += len(src)
	filename := ":" + string(base) // Dummy filename that cannot exist but is still unique
	sources[filename] = s
	return s
}
