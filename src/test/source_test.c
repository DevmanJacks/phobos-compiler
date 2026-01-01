/*
 * source_test.c
 */

#include "../source.c"
#include "testing.h"

static void test_add_line() {
    Source *source = create_source(NULL, (uint8_t *)"var a\nvar b\n", 13);
    add_line(source, 6);
    
    if (source->lines) {
        if (source->lines_length == 2) {
            if (source->lines[0] == 0 && source->lines[1] == 6) {
                test_passed("add_line()");
            } else {
                test_failed("add_line()", "line[0] expected: %d, got: %d, line[1] expected: %d, got: %d", 0, source->lines[0], 6, source->lines[1]);
            }
        } else {
            test_failed("add_line()", "Bad source->lines_length, expected: %ld, got: %ld", 2, source->lines_length);
        }
    } else {
        test_failed("add_line()", "source->lines is NULL");
    }
}

static void test_create_source() {
    char *filename = "test.p";
    uint8_t *code = (uint8_t *)"Hello World!";
    size_t length = 12;

    Source *source = create_source(filename, code, length);

    if (source->filename == filename) {
        if (source->code == code) {
            if (source->length == length) {
                test_passed("create_source()");
            } else {
                test_failed("create_source()", "Bad length - expected: %ld, got: %ld", length, source->length);
            }
        } else {
            test_failed("create_source()", "Bad code");
        }
    } else {
        test_failed("create_source()", "Bad filename - expected: %s, got: %s", filename, source->filename);
    }
}

extern void source_test() {
    test_section("Source");
    test_create_source();
    test_add_line();
}