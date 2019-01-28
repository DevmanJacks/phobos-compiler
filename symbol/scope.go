////////////////////////////////////////////////////////////
//
// Phobos Compiler - tool for building mars operating system
//
// Copyright (c) Mark Jackson		24 January 2019
//
////////////////////////////////////////////////////////////

package symbol

// A Scope maintains a collection of symbols and a reference to it's parent scope.  Symbols can be inserted and looked up in the scope by name.
type Scope struct {
	parent  *Scope
	symbols map[string]Symbol // Lazy creation to save memory when scope contains no symbols
}

// NewScope creates a new scope as a child of the specified parent
func NewScope(parent *Scope) *Scope {
	return &Scope{parent: parent}
}

// CurrentScope refers to the scope that is at the top of the scope stack
var CurrentScope *Scope

// Lookup finds the symbol with the specified name in scope.  If it does not exist Lookup returns nil.
func (s *Scope) Lookup(name string) Symbol {
	return s.symbols[name]
}

// Insert attempts to insert the symbol into the scope.
// If a symbol with the same name exists found will be set to true and the existing symbol will be returned.
// If a symbol with the same name does not exist in the scope found will be false and existing will be nil.
func (s *Scope) Insert(symbol Symbol) (found bool, existing Symbol) {
	name := symbol.Name()

	if existing = s.symbols[name]; existing != nil {
		return true, existing
	}

	if s.symbols == nil {
		s.symbols = make(map[string]Symbol)
	}

	s.symbols[name] = symbol
	return false, nil
}
