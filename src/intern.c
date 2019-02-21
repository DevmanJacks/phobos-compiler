/*
 * intern.c - intern all strings
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intern.h"

#define STRINGS_DATA_SIZE   1024 - sizeof(size_t) - sizeof(void *)

typedef struct strings_t {
    char data[STRINGS_DATA_SIZE];
    size_t end;
    struct strings_t *next;
} strings_t;

strings_t *interned_strings = NULL;

char *add_interned_string(const char *start, size_t length) {
    strings_t *bucket = interned_strings;
    size_t required_length = length + 1;
    assert(required_length <= STRINGS_DATA_SIZE);

    // Find a bucket with room for the string
    while (bucket && STRINGS_DATA_SIZE - bucket->end < required_length)
        bucket = bucket->next;

    if (!bucket) {
        bucket = malloc(sizeof(strings_t));

        if (bucket) {
            bucket->end = 0;
            bucket->next = interned_strings;
            interned_strings = bucket;

        } else {
            perror("ERROR: Unable to allocate memory for interned strings - ");
            exit(-1);
        }
    }

    char *ptr = bucket->data + bucket->end;
    memcpy(ptr, start, length);
    *(ptr + length) = 0;
    bucket->end += required_length;
    return ptr;
}

// Used for iterating over the interned strings when resizing
strings_t *iterate_bucket = NULL;
size_t iterate_offset = 0;

static char *first_interned_string() {
    if (!interned_strings)
        return NULL;

    iterate_bucket = interned_strings;
    iterate_offset = strlen(interned_strings->data) + 1;
    return interned_strings->data;
}

static char *next_interned_string() {
    if (iterate_offset == iterate_bucket->end) {
        iterate_bucket = iterate_bucket->next;
        iterate_offset = 0;

        if (!iterate_bucket || iterate_offset == iterate_bucket->end)
            return NULL;
    }

    char *str = iterate_bucket->data + iterate_offset;
    iterate_offset += strlen(str) + 1;
    return str;
}

#define INITIAL_TABLE_CAPACITY  1021        // Allow hash_table_t to fit in a 4K memory block.  Capacity should be a prime number.

typedef struct hash_table_t {
    unsigned int capacity;
    unsigned int occupancy;
    char *table[];
} hash_table_t;

hash_table_t *string_table = NULL;

// A simple hash function that works well on strings
static unsigned long hash(const char *str, size_t length) {
    unsigned long hash = 5381;

    for (int i = 0; i < length; i++)
        hash = ((hash << 5) + hash) + *(str + i);            // hash * 33 + c

    return hash;
}

// This is the same hash function as above but works on C strings.
// THEY MUST BE KEPT IN SYNC!!!!
static unsigned long hash_string(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;            // hash * 33 + c

    return hash;
}

static void resize_string_table() {
    unsigned int new_capacity = INITIAL_TABLE_CAPACITY;

    if (!string_table)
        free(string_table);

    string_table = calloc(sizeof(hash_table_t) + sizeof(char *) * new_capacity, 1);

    if (string_table) {
        string_table->capacity = new_capacity;
    } else {
        perror("ERROR: Unable to allocate memory for string table - ");
        exit(-1);
    }

    // Add all of the currently interned strings to the new table
    char *str = first_interned_string();

    while (str) {
        unsigned int index = hash_string(str) % string_table->capacity;

        while (string_table->table[index])
            index = (index + 1) % string_table->capacity;

        string_table->table[index] = str;
        str = next_interned_string();
    }
}

// Intern the string
const char *intern_string(const char *start, size_t length) {
    unsigned long hash_value = hash(start, length);
    unsigned int index;

    if (string_table) {
        unsigned int hashed_index = hash_value % string_table->capacity;
        index = hashed_index;

        while (string_table->table[index]) {
            if (memcmp(start, string_table->table[index], length) == 0 && string_table->table[index + 1] == 0)
                return string_table->table[index];

            index = (index + 1) % string_table->capacity;

            // Hash table full?  This should NEVER happen as we will resize when we start to get full.
            if (index == hashed_index)
                break;
        } 
    }

    // The string is not in the hash table.
    // Check to see if we have capacity and insert if so, resize and insert if not.
    if (!string_table || (float)string_table->occupancy / (float)string_table->capacity > 0.8) {
        resize_string_table();
        index = hash_value % string_table->capacity;

        while (string_table->table[index]) {
            index = (index + 1) % string_table->capacity;
        } 
    }

    char *internedString = add_interned_string(start, length);
    string_table->table[index] = internedString;
    return internedString;
}
