/*
 * source_test.c
 */

#include "../source.c"
#include "testing.h"

void test_add_line() {
    source_t *source = make_source(NULL, (uint8_t *)"var a\nvar b\n", 9);
    add_line(source, 5);
    add_line(source, 11);
    
    if (source->lines) {
        if (source->lines_length == 2) {
            if (source->lines[0] == 5 && source->lines[1] == 11) {
                test_passed("add_line()");
            } else {
                test_failed("add_line()", "line[0] expected: %d, got: %d, line[1] expected: %d, got: %d", 5, source->lines[0], 11, source->lines[1]);
            }
        } else {
            test_failed("add_line()", "Bad source->lines_length, expected: %ld, got: %ld", 2, source->lines_length);
        }
    } else {
        test_failed("add_line()", "source->lines is NULL");
    }
}

void test_make_source() {
    char *filename = "test.p";
    uint8_t *code = (uint8_t *)"Hello World!";
    size_t length = 12;

    source_t *source = make_source(filename, code, length);

    if (source->filename == filename) {
        if (source->code == code) {
            if (source->length == length) {
                test_passed("make_source()");
            } else {
                test_failed("make_source()", "Bad length - expected: %ld, got: %ld", length, source->length);
            }
        } else {
            test_failed("make_source()", "Bad code");
        }
    } else {
        test_failed("make_source()", "Bad filename - expected: %s, got: %s", filename, source->filename);
    }
}

void source_test() {
    test_section("Source");
    test_make_source();
    test_add_line();
}