/*
 * Handle all types of compile errors
 */

 #pragma once

 #include "ast.h"
 
// Fatal errors
enum fatal_errors {
    EOUT_OF_MEMORY = 1,
    EMAX_ERRORS_EXCEEDED
};

enum error_number {
    E_EXPECTED_TYPE
};

typedef struct error_old {
    enum error_number error_number;
    char *msg;
    AstNode *node;
} Error_old;

typedef struct errors {
    Error_old error;
} Errors;

// Public functions
extern void display_errors();
extern void syntax_error0(Errors error_num, AstNode *node);