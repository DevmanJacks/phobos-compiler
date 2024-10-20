/*
 * dynamic_array.c
 */

#include "dynamic_array.h"

void *array_init(dynamic_array *array) {
    array->length = 0;
    array->capacity = DYNAMIC_ARRAY_INITIAL_SIZE;
    array->elements = malloc(sizeof(void *) * DYNAMIC_ARRAY_INITIAL_SIZE);
}

