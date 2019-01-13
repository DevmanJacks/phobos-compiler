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
)

// Scanner represents a scanner for the phobos language.
type Scanner struct {
	src []uint8

	// Scanning state
	ch     int8
	offset int
}

// NewScanner creates a new scanner to tokenise the src.
func NewScanner(src string) *Scanner {
	s := new(Scanner)
	s.src = []uint8(src)
	s.ch = '~'
	s.offset = -1
	s.next()
	return s
}

// Read the next ascii character from the source into s.ch (s.ch == -1 means EOF).
// s.offset will point at the character in s.ch
func (s *Scanner) next() {
	if s.offset < len(s.src)-1 {
		s.offset++

		if s.src[s.offset] > 127 {
			// Error - illegal character
		}

		s.ch = int8(s.src[s.offset])
	} else {
		s.ch = -1
	}
}

func (s *Scanner) peek() int8 {
	if s.offset < len(s.src)-1 {
		return int8(s.src[s.offset+1])
	}

	return -1
}

// c contans ' for a character literal and " for a string literal
func (s *Scanner) scanEscape(c int8) {
	if s.ch == '\\' {
		s.next()

		if s.ch == 'n' || s.ch == 'r' || s.ch == 't' || s.ch == '\\' || s.ch == c {
			s.next()
		} else {
			// Error - unsupported escape sequence
		}
	}
}

func (s *Scanner) scanCharacter() string {
	start := s.offset
	s.next()

	if s.ch == '\\' {
		s.scanEscape('\'')
	} else {
		s.next()
	}

	if s.ch == '\'' {
		s.next()
	} else {
		// Error - unexpected character in character literal
		s.next()

		for s.ch != '\'' && s.ch != -1 {
			s.next()
		}

		if s.ch == '"' {
			s.next()
		} else {
			// Error - end of file in character literal
		}
	}

	return string(s.src[start:s.offset])
}

func (s *Scanner) scanIdentifier() (lexeme string, tok token.Token) {
	start := s.offset

	for isLetter(s.ch) || isDigit(s.ch) {
		s.next()
	}

	lexeme = string(s.src[start:s.offset])
	tok = token.CheckIdentifier(lexeme)

	return lexeme, tok
}

func (s *Scanner) scanInteger(base int) {
	switch base {
	case 2:
		for isBinaryDigit(s.ch) || s.ch == '_' {
			s.next()
		}

	case 10:
		for isDigit(s.ch) || s.ch == '_' {
			s.next()
		}

	case 16:
		for isHexDigit(s.ch) || s.ch == '_' {
			s.next()
		}

	default:
		// Error - unsupported base
	}
}

func (s *Scanner) scanNumber() (lexeme string, tok token.Token) {
	start := s.offset

	// Skip leading + or -
	if s.ch == '+' || s.ch == '-' {
		s.next()
	}

	if s.ch == '0' { // Zero value or start of hex or binary literal
		s.next()

		switch s.ch {
		case 'b', 'B':
			s.next()

			if isBinaryDigit(s.ch) {
				s.scanInteger(2)
			} else {
				// Error - must bave binary digit following 0b
			}

			return string(s.src[start:s.offset]), token.Integer

		case 'x', 'X':
			s.next()

			if isHexDigit(s.ch) {
				s.scanInteger(16)
			} else {
				// Error - must bave hex digit following 0x
			}

			return string(s.src[start:s.offset]), token.Integer
		}
	}

	// Decimal integer
	s.scanInteger(10)
	tok = token.Integer

	if s.ch == '.' {
		if s.peek() != '.' {
			s.next()
			s.scanInteger(10)
			tok = token.Float
		}
	}

	if s.ch == 'e' || s.ch == 'E' {
		s.next()

		if s.ch == '+' || s.ch == '-' {
			s.next()
		}

		if isDigit(s.ch) {
			s.scanInteger(10)
		} else {
			// Error - Illegal character in floating point exponent
		}

		tok = token.Float
	}

	return string(s.src[start:s.offset]), tok
}

func (s *Scanner) scanString() string {
	start := s.offset
	s.next()

	for s.ch != '"' && s.ch != -1 {
		if s.ch == '\\' {
			s.scanEscape('"')
		} else {
			s.next()
		}
	}

	if s.ch == '"' {
		s.next()
	} else {
		// Error - end of file in string
	}

	return string(s.src[start:s.offset])
}

func (s *Scanner) skipWhitespace() {
	for s.ch == ' ' || s.ch == '\n' || s.ch == '\t' {
		s.next()
	}
}

// Scan scans the src for the next token.
func (s *Scanner) Scan() (pos token.Pos, tok token.Token, lexeme string) {
top:
	s.skipWhitespace()

	switch {
	case isLetter(s.ch):
		lexeme, tok = s.scanIdentifier()

	case isDigit(s.ch):
		lexeme, tok = s.scanNumber()

	default:
		switch s.ch {
		case -1:
			lexeme, tok = "", token.EndOfFile
		case '\'':
			lexeme, tok = s.scanCharacter(), token.Character

		case '"':
			lexeme, tok = s.scanString(), token.String

		case '{':
			s.next()
			lexeme, tok = "{", token.LeftBrace

		case '}':
			s.next()
			lexeme, tok = "}", token.RightBrace

		case '[':
			s.next()
			lexeme, tok = "[", token.LeftBracket

		case ']':
			s.next()
			lexeme, tok = "]", token.RightBracket

		case '(':
			s.next()
			lexeme, tok = "(", token.LeftParenthesis

		case ')':
			s.next()
			lexeme, tok = ")", token.RightParenthesis

		case ',':
			s.next()
			lexeme, tok = ",", token.Comma

		case '.':
			if isDigit(s.peek()) {
				lexeme, tok = s.scanNumber()
			} else {
				s.next()

				if s.ch == '.' {
					s.next()
					lexeme, tok = "..", token.DotDot
				} else {
					lexeme, tok = ".", token.Dot
				}
			}

		case ':':
			s.next()

			if s.ch == '=' {
				s.next()
				lexeme, tok = ":=", token.DeclareAssign
			} else {
				lexeme, tok = ":", token.Colon
			}

		case '=':
			s.next()

			if s.ch == '=' {
				s.next()
				lexeme, tok = "==", token.Equal
			} else {
				lexeme, tok = "=", token.Assign
			}

		case '!':
			s.next()

			if s.ch == '=' {
				s.next()
				lexeme, tok = "!=", token.NotEqual
			} else {
				lexeme, tok = "!", token.Not
			}

		case '*':
			s.next()

			if s.ch == '=' {
				s.next()
				lexeme, tok = "*=", token.MultiplyEquals
			} else {
				lexeme, tok = "*", token.Asterisk
			}

		case '/':
			s.next()

			if s.ch == '=' {
				lexeme, tok = "/=", token.DivideEquals
			} else if s.ch == '/' { // Line comment
				for s.ch != '\n' && s.ch != -1 {
					s.next()
				}

				goto top
			} else {
				lexeme, tok = "/", token.Divide
			}

		case '+':
			if isDigit(s.peek()) {
				lexeme, tok = s.scanNumber()
			} else {
				s.next()

				if s.ch == '=' {
					s.next()
					lexeme, tok = "+=", token.PlusEquals
				} else {
					lexeme, tok = "+", token.Plus
				}
			}

		case '-':
			if isDigit(s.peek()) {
				lexeme, tok = s.scanNumber()
			} else {
				s.next()
				if s.ch == '>' {
					s.next()
					lexeme, tok = "->", token.Returns
				} else if s.ch == '=' {
					s.next()
					lexeme, tok = "-=", token.MinusEquals
				} else {
					lexeme, tok = "-", token.Minus
				}
			}

		case '%':
			s.next()

			if s.ch == '=' {
				s.next()
				lexeme, tok = "%=", token.ModEquals
			} else {
				lexeme, tok = "%", token.Mod
			}

		case '<':
			s.next()

			if s.ch == '=' {
				s.next()
				lexeme, tok = "<=", token.LessThanOrEqual
			} else if s.ch == '<' {
				s.next()

				if s.ch == '=' {
					s.next()
					lexeme, tok = "<<=", token.ShiftLeftEquals
				} else {
					lexeme, tok = "<<", token.ShiftLeft
				}
			} else {
				lexeme, tok = "<", token.LessThan
			}

		case '>':
			s.next()

			if s.ch == '=' {
				lexeme, tok = ">=", token.GreaterThanOrEqual
			} else if s.ch == '>' {
				s.next()

				if s.ch == '=' {
					s.next()
					lexeme, tok = ">>=", token.ShiftRightEquals
				} else {
					lexeme, tok = ">>", token.ShiftRight
				}
			} else {
				lexeme, tok = ">", token.GreaterThan
			}

		case '&':
			s.next()

			if s.ch == '&' {
				s.next()
				lexeme, tok = "&&", token.LogicalAnd
			} else if s.ch == '=' {
				s.next()
				lexeme, tok = "&=", token.BitwiseAndEquals
			} else {
				lexeme, tok = "&", token.BitwiseAnd
			}

		case '|':
			s.next()

			if s.ch == '|' {
				s.next()
				lexeme, tok = "||", token.LogicalOr
			} else if s.ch == '=' {
				s.next()
				lexeme, tok = "|=", token.BitwiseOrEquals
			} else {
				lexeme, tok = "|", token.BitwiseOr
			}

		case '~':
			s.next()

			if s.ch == '=' {
				s.next()
				lexeme, tok = "~=", token.BitwiseNotEquals
			} else {
				lexeme, tok = "~", token.BitwiseNot
			}

		default:
			// Error
		}
	}

	return pos, tok, lexeme
}

// ========== Helper functions ==========

func isBinaryDigit(ch int8) bool {
	return ch == '0' || ch == '1'
}

func isDigit(ch int8) bool {
	return '0' <= ch && ch <= '9'
}

func isHexDigit(ch int8) bool {
	return '0' <= ch && ch <= '9' || 'a' <= ch && ch <= 'f' || 'A' <= ch && ch <= 'F'
}

func isLetter(ch int8) bool {
	return 'A' <= ch && ch <= 'Z' || 'a' <= ch && ch <= 'z' || ch == '_'
}
