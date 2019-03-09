/*
 * error.c
 */

#include <stdio.h>
#include <string.h>
#include "error.h"

typedef struct error_t {
    pos_t start_pos;
    pos_t end_pos;
    char *msg;
} error_t;

#define MAX_ERRORS 1

error_t errors[MAX_ERRORS];
int num_errors = 0;

void log_error(pos_t start_pos, pos_t end_pos, char *msg, ...) {
    static char buffer[1024];

    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 1024, msg, args);
    va_end(args);

    errors[num_errors].start_pos = start_pos;
    errors[num_errors].end_pos = end_pos;
    errors[num_errors].msg = msg;
    num_errors++;

    if (num_errors == MAX_ERRORS) {
        print_errors();
        fprintf(stderr, "\nMaximum number of errors encountered, compilation stopped.\n");
    }
}

void print_errors() {
    for (int i = 0; i < num_errors; i++) {
        fprintf(stderr, "%s\n", errors[i].msg);
    }
}