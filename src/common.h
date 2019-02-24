#ifndef __COMMON_H
#define __COMMON_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Array list
typedef struct {
    size_t capacity;
    size_t length;
    uint8_t array[0];
} list_header_t;

#define list_header(l) ((list_header_t *)((uint8_t *)(l) - offsetof(list_header_t, array)))
#define list_capacity(l) ((l) ? list_header(l)->capacity : 0)
#define list_length(l) ((l) ? list_header(l)->length : 0)

#define list_has_capacity(l, n) (list_length(l) + (n) <= list_capacity(l))
#define list_ensure_capacity(l, n) (list_has_capacity(1, (n)) ? 0 : ((l) = list_grow(l, n, sizeof(*(l)))))

#define append(l, x) (list_ensure_capacity(l, 1), (l)[list_header(l)->length++] = (x))

enum { INITIAL_LIST_CAPACITY = 16 };

static void *list_grow(void *list, int num_elements, size_t element_size) {
    if (list) {
        size_t new_capacity = list_capacity(list) * 2;
        list = realloc(list, new_capacity * element_size + offsetof(list_header_t, array));

        if (!list) {
            perror("Unable to reallocate memory for list");
            exit(-1);
        }

        return list;
    } else {
        list = malloc(INITIAL_LIST_CAPACITY * element_size + offsetof(list_header_t, array));
        list_header(list)->capacity = INITIAL_LIST_CAPACITY;
        list_header(list)->length = 0;
        return ((list_header_t *)list)->array;
    }
}

#endif