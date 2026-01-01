/*
 * testpc.c - test entry point
 */

#include <stdarg.h>
#include <stdio.h>
#include "intern_test.h"
#include "source_test.h"
#include "token_test.h"
 
int num_tests;
int num_failed_tests;

static void testing_start(const char *name) {
    num_tests = 0;
    num_failed_tests = 0;

    printf("\e[0;38mTesting %s\e[0m\n", name);
}

static void testing_complete() {
    printf("\nTesting complete.  Total number of tests: %d", num_tests);

    if (num_failed_tests) {
        printf(", failed: %d\n", num_failed_tests);
    } else {
        printf(".  All tests passed.\n");
    }
}

void test_section(char *name) {
    printf("\n\e[0;36m%s\e[0m\n", name);
}

void test_failed(const char *test, char *msg, ...) {
	va_list args;
	va_start(args, msg);
    printf("  \e[0;37m%s:\e[0m ", test);
	vprintf(msg, args);
	printf("\e[0;31m - FAILED\e[0m\n");
	va_end(args);

    num_tests++;
    num_failed_tests++;
}

void test_passed(const char *test) {
    printf("  \e[0;37m%s:\e[0;32m - PASSED\e[0m\n", test);
    num_tests++;
}

int main(int argc, char **argv) {
    testing_start("Phobos Compiler");
    intern_test();
    source_test();
    token_test();
    testing_complete();
}