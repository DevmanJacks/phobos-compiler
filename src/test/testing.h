#ifndef __TESTING_H
#define __TESTING_H

#include <stdarg.h>

// Public functions
void test_section(char *name);
void test_passed(const char *test);
void test_failed(const char *test, char *msg, ...);

#endif