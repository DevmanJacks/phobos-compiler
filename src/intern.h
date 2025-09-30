#pragma once

#include <stdint.h>

typedef uint32_t InternedString;

// Public functions
extern void string_intern_initialise();
extern InternedString intern_string(const char *str);
extern char *interned_string_value(InternedString s);