/*
 * Phobos compiler entry point.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "error.c"
#include "intern.c"

void main(int argc, char **argv) {
    string_intern_initialise();

    printf("Empty string table: %d\n", intern_string("test"));
    printf("Duplicate entry:    %d\n", intern_string("test1"));
}