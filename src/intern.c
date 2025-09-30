/*
 * Intern strings to make string comparison quicker.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "intern.h"

#define STRING_BUFFER_INITIAL_SIZE 16384
#define STRING_TABLE_NUM_INITIAL_ENTRIES 4096
#define STRING_TABLE_MAX_OCCUPANCY 60

typedef struct StringBuffer {
    uint32_t capacity;
    uint32_t length;
    char strings[];
} StringBuffer;

typedef struct StringTable {
    uint32_t capacity;
    uint32_t length;
    InternedString interned_strings[];
} StringTable;

StringBuffer *interned_strings;
StringTable *interned_strings_table;

extern char *interned_string_value(InternedString s) {
    return interned_strings->strings + s;
}

static uint32_t hash_string(const char *str) {
    uint32_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;            // hash * 33 + c

    return hash;
}

extern InternedString intern_string(const char *str) {
    uint32_t hash = hash_string(str);
    uint32_t initial_string_table_slot = hash % interned_strings_table->capacity;
    uint32_t string_table_slot = initial_string_table_slot;
    InternedString string_table_entry = interned_strings_table->interned_strings[string_table_slot];

    // We will always exit this loop if the max occupancy is less than 100%
    while (string_table_entry != 0xFFFFFFFF) {
        // Found same string so return its position
        if (strcmp(str, interned_strings->strings + string_table_entry) == 0)
            return string_table_entry;

        string_table_slot++;

        if (string_table_slot > interned_strings_table->capacity)
            string_table_slot = 0;
    }

    // String not found so add it
    if (interned_strings_table->length / interned_strings_table->capacity * STRING_TABLE_MAX_OCCUPANCY)
    {
        fprintf(stderr, "Not implemented - increase table size.");
        exit(1);
    }

    interned_strings_table->interned_strings[string_table_slot] = interned_strings->length;

    strcpy(interned_strings->strings + interned_strings->length, str);
    interned_strings->length += strlen(str) + 1;
    *(interned_strings->strings + interned_strings->length - 1) = 0;

    return interned_strings_table->interned_strings[string_table_slot];
}

extern void string_intern_initialise() {
    interned_strings = malloc(sizeof(StringBuffer) + STRING_BUFFER_INITIAL_SIZE);

    if (interned_strings == NULL)
    {
        perror("Unable to allocate memory for interned_strings.");
        exit(EOUT_OF_MEMORY);
    }

    interned_strings->capacity = STRING_BUFFER_INITIAL_SIZE;
    interned_strings->length = 0;

    interned_strings_table = malloc(sizeof(StringTable) + STRING_TABLE_NUM_INITIAL_ENTRIES * sizeof(InternedString));

    if (interned_strings_table == NULL)
    {
        perror("Unable to allocate memory for interned_strings_table.");
        exit(EOUT_OF_MEMORY);
    }

    interned_strings_table->capacity = STRING_TABLE_NUM_INITIAL_ENTRIES;
    interned_strings_table->length = 0;

    memset(interned_strings_table->interned_strings, 0xFF, STRING_TABLE_NUM_INITIAL_ENTRIES * sizeof(InternedString));
}
