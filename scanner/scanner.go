////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		12 January 2019
//
////////////////////////////////////////////////////////////

package scanner

import (
	"phobos/source"
	"phobos/token"
)

// Scanner represents a scanner for the phobos language.
type Scanner struct {
	source *source.Source
	src    []uint8

	// Scanning state
	ch     int8
	offset int
}

// NewScanner creates a new scanner to tokenise the src.
func NewScanner(src *source.Source) *Scanner {
	s := new(Scanner)
	s.source = src
	s.src = src.Code()
	s.ch = '~'
	s.offset = -1
	s.next()
	return s
}

func (s *Scanner) error(offset int, message string) {
	source.Error(s.source.Pos(offset), message)
}

func (s *Scanner) addLine() {
	s.source.AddLine(s.source.Pos(s.offset))
}

// Read the next ascii character from the source into s.ch (s.ch == -1 means EOF).
// s.offset will point at the character in s.ch
func (s *Scanner) next() {
	if s.offset < len(s.src)-1 {
		s.offset++

		if s.src[s.offset] > 127 {
			s.error(s.offset, "Illegal character '"+string(s.src[s.offset])+"'.")
		}

		s.ch = int8(s.src[s.offset])
	} else {
		s.offset = len(s.src)
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
	offset := s.offset

	if s.ch == '\\' {
		s.next()

		if s.ch == 'n' || s.ch == 'r' || s.ch == 't' || s.ch == '\\' || s.ch == c {
			s.next()
		} else {
			s.error(offset, "Unsupported escape sequence '\\"+string(s.ch)+"'.")
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
		if s.ch == -1 {
			s.error(start, "End of file encountered in character literal.")
		} else {
			s.error(start, "Unexpected character in character literal '"+string(s.ch)+"'.")
			s.next()

			for s.ch != '\'' && s.ch != -1 {
				s.next()
			}

			if s.ch == '\'' {
				s.next()
			} else {
				s.error(start, "End of file encountered in character literal.")
			}
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
	offset := s.offset

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
		s.error(offset, "Illegal character in integer literal '"+string(s.ch)+"'.  Valid prefixes are 0b, 0B, 0x and 0X.")
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
				s.error(start, "Illegal character '"+string(s.ch)+"' in binary integer literal.")
			}

			return string(s.src[start:s.offset]), token.Integer

		case 'x', 'X':
			s.next()

			if isHexDigit(s.ch) {
				s.scanInteger(16)
			} else {
				s.error(start, "Illegal character '"+string(s.ch)+"' in hexadecimal integer literal.")
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
			s.error(start, "Illegal character '"+string(s.ch)+"' in floating point exponent.")
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
		s.error(start, "End of file encountered in string literal.")
	}

	return string(s.src[start:s.offset])
}

func (s *Scanner) skipWhitespace() {
	for s.ch == ' ' || s.ch == '\n' || s.ch == '\t' {
		if s.ch == '\n' {
			s.next()
			s.addLine()
		} else {
			s.next()
		}
	}
}

// Scan scans the src for the next token.
func (s *Scanner) Scan() (pos source.Pos, tok token.Token, lexeme string) {
top:
	s.skipWhitespace()
	pos = s.source.Pos(s.offset)

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
