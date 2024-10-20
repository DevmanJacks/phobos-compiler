#pragma once

#include <stdint.h>

typedef uint32_t interned_string;

// Public functions
extern interned_string intern_string(const char *string);