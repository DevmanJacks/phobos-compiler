/*
 * Handle errors.
 */

#include "error.h"

// Error messages
static char *messages[] = {
    [E_EXPECTED_TYPE] = "Expected type."
};

#define MAX_ERRORS 10

static Error_old errors[MAX_ERRORS];

static int next_error = 0;

void display_errors() {
    fprintf(stderr, "We found some errors.\n\n");
}

void syntax_error0(Errors error_num,  AstNode *node) {

}