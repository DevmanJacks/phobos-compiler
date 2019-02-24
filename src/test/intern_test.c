/*
 * intern_test.c
 */

#include "../intern.c"
#include "testing.h"

void test_intern_string() {
    test(intern_string("hello", 5) != "hello", "Interned string and raw string are same when using ==");
    test(intern_string("hello", 5) == intern_string("hello", 5), "Different interned strings with same text do not match");
    test(intern_string("hello world", 11) != intern_string("hello", 5), "Interned substring matches complete string");
    test(intern_string(intern_string("hello", 5), 5) == intern_string("hello", 5), "Nested interned string does not match interned string");
}

void intern_test() {
    test_section("String Interning");
    test_intern_string();
}