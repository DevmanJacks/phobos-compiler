#pragma once

#include <stdlib.h>

#define DYNAMIC_ARRAY_INITIAL_SIZE  4

typedef struct dynamic_array {
    int length;
    int capacity;
    void **elements;
} dynamic_array;

// Public functions
extern void *array_init(dynamic_array *array);