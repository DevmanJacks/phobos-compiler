#pragma once

#include <stdarg.h>

// Helper macros
#define test(t, ...) if ((t)) test_passed(#t); else test_failed(#t, __VA_ARGS__);

// Public functions
void multiple_tests_passed(const char *test_func, int num_tests_passed);
void test_section(char *name);
void test_passed(const char *test);
void test_failed(const char *test, char *msg, ...);