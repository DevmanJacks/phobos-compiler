#pragma once

#include <stdarg.h>

// Helper macros
#define test(t, ...) if ((t)) test_passed(#t); else test_failed(#t, __VA_ARGS__);

// Public functions
void test_section(char *name);
void test_passed(const char *test);
void test_failed(const char *test, char *msg, ...);