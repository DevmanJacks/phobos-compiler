/*
 * Handle all types of compile errors
 */

 #pragma once
 
// Fatal errors
enum fatal_errors {
    EOUT_OF_MEMORY = 1,
    EUNEXPECTED_CHARACTER = 2,
    ESYNTAX_ERROR = 3
};