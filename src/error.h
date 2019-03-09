#pragma once

#include <stdarg.h>
#include "source.h"

// Public functions
extern void log_error(pos_t start_pos, pos_t end_pos, char *msg, ...);
extern void print_errors();