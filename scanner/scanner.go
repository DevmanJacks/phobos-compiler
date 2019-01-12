////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package scanner

import (
	"phobos/token"
	"unicode"
	"unicode/utf8"
)

// Scanner represents a scanner for the phobos language.
type Scanner struct {
	src []uint8

	// Scanning state
	ch         rune
	offset     int
	nextOffset int
}

const byteOrderMark = 0xFEFF // Only allowed as first character in a unicode string

// Init prepares the scanner to tokenise the src.
func (s *Scanner) Init(src string) {
	s.src = []uint8(src)

	s.ch = '~'
	s.offset = 0
	s.nextOffset = 0

	s.next()

	if s.ch == byteOrderMark {
		s.next() // Ignore byte order mark if it is the first rune
	}
}

// Read the next unicode character from the source into s.ch
// 		s.ch < 0 		EOF
func (s *Scanner) next() {
	if s.nextOffset < len(s.src) {
		s.offset = s.nextOffset
		r, size := utf8.DecodeRune(s.src[s.offset:])

		if r == utf8.RuneError {
			// Error
		} else if r == byteOrderMark && s.offset != 0 {
			// error
		}

		s.ch = r
		s.nextOffset += size
	} else {
		s.ch = -1
	}
}

func (s *Scanner) scanIdentifier() string {
	start := s.offset

	for isLetter(s.ch) || isDigit(s.ch) {
		s.next()
	}

	return string(s.src[start:s.nextOffset])
}

// Scan scans the src for the next token.
func (s *Scanner) Scan() (pos token.Pos, tok token.Token, lexeme string) {
	switch ch := s.ch; {
	case isLetter(ch):
		lexeme = s.scanIdentifier()
		tok = token.CheckIdentifier(lexeme)
		pos = token.Pos(s.offset)
	}

	return pos, tok, lexeme
}

// ========== Helper functions ==========

func isDigit(ch rune) bool {
	return '0' <= ch && ch <= '9' || ch >= utf8.RuneSelf && unicode.IsDigit(ch)
}

func isLetter(ch rune) bool {
	return 'A' <= ch && ch <= 'Z' || 'a' <= ch && ch <= 'z' || ch == '_' || ch >= utf8.RuneSelf && unicode.IsLetter(ch)
}
