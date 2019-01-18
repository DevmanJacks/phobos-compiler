////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		14 January 2019
//
////////////////////////////////////////////////////////////

package source

import "fmt"

// Pos is a compact way to store a position in a source file
type Pos int

// NoPos represents no position.  This is mainly used when we manually add nodes into the AST
const NoPos = 0

// String will return a human readable representation of a Pos
func (p Pos) String() string {
	for _, s := range sources {
		if s.base <= int(p) && int(p) < s.base+s.length {
			start := 0
			end := len(s.lines) - 1
			line := 0

			for end >= start {
				index := (end-start)/2 + start

				if p == s.lines[index] {
					line = index
					break
				} else if p < s.lines[index] {
					end = index - 1
					line = end
				} else {
					line = start
					start = index + 1
				}
			}

			if line >= len(s.lines) {
				line = len(s.lines) - 1
			}

			column := p - s.lines[line] + 1
			return fmt.Sprintf("%s:%d:%d", s.filename, line+1, column)
		}
	}

	panic("pos is out of range for all sources.")
}
