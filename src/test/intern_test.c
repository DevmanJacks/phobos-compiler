/*
 * intern_test.c
 */

#include "../intern.c"
#include "testing.h"

void test_intern_string() {
    test(interned_string_value(intern_string("hello")) != "hello", "Interned string and raw string are same when using ==");
    test(intern_string("hello") == intern_string("hello"), "Different interned strings with same text do not match");
    test(intern_string("hello world") != intern_string("hello"), "Interned substring matches complete string");
}

void intern_test() {
    test_section("String Interning");
    string_intern_initialise();
    test_intern_string();
}