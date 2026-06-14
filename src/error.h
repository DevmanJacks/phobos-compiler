/*
 * Handle all types of compile errors
 */

 #pragma once

 #include "ast.h"
 
// Fatal errors
enum fatal_errors {
    EOUT_OF_MEMORY = 1,
    EUNEXPECTED_CHARACTER = 2,
    EINVALID_CMDLINE_ARGS = 3
};

enum error_number {
    E_EXPECTED_TYPE
};

typedef struct error {
    enum error_number error_number;
    char *msg;
    AstNode *node;
} Error;

typedef struct errors {
    Error error;
} Errors;

// Public functions
extern void display_errors();
extern void syntax_error0(Errors error_num, AstNode *node);