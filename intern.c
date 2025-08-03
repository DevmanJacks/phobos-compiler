/*
 * Intern strings to make string comparison quicker.
 */

#define STRING_BUFFER_INITIAL_SIZE 16384
#define STRING_TABLE_NUM_INITIAL_ENTRIES 4096
#define STRING_TABLE_MAX_OCCUPANCY 60

typedef uint32_t interned_string;

typedef struct string_buffer {
    uint32_t capacity;
    uint32_t length;
    char strings[];
} string_buffer;

typedef struct string_table {
    uint32_t capacity;
    uint32_t length;
    interned_string interned_strings[];
} string_table;

string_buffer *interned_strings;
string_table *interned_string_table;

char *interned_string_value(interned_string s) {
    return interned_strings->strings + s;
}

static uint32_t hash_string(const char *str)
{
    uint32_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;            // hash * 33 + c

    return hash;
}

interned_string intern_string(const char *str) {
    uint32_t hash = hash_string(str);
    uint32_t initial_string_table_slot = hash % interned_string_table->capacity;
    uint32_t string_table_slot = initial_string_table_slot;
    interned_string string_table_entry = interned_string_table->interned_strings[string_table_slot];

    // We will always exit this loop if the max occupancy is less than 100%
    while (string_table_entry != 0xFFFFFFFF) {
        // Found same string so return its position
        if (strcmp(str, interned_strings->strings + string_table_entry) == 0)
            return string_table_entry;

        string_table_slot++;

        if (string_table_slot > interned_string_table->capacity)
            string_table_slot = 0;
    }

    // String not found so add it
    if (interned_string_table->length / interned_string_table->capacity * STRING_TABLE_MAX_OCCUPANCY)
    {
        fprintf(stderr, "Not implemented - increase table size.");
        exit(1);
    }

    interned_string_table->interned_strings[string_table_slot] = interned_strings->length;

    strcpy(interned_strings->strings + interned_strings->length, str);
    interned_strings->length += strlen(str) + 1;
    *(interned_strings->strings + interned_strings->length - 1) = 0;

    return interned_string_table->interned_strings[string_table_slot];
}

void string_intern_initialise() {
    interned_strings = malloc(sizeof(string_buffer) + STRING_BUFFER_INITIAL_SIZE);

    if (interned_strings == NULL)
    {
        perror("Unable to allocate memory for interned_strings.");
        exit(EOUT_OF_MEMORY);
    }

    interned_strings->capacity = STRING_BUFFER_INITIAL_SIZE;
    interned_strings->length = 0;

    interned_string_table = malloc(sizeof(string_table) + STRING_TABLE_NUM_INITIAL_ENTRIES * sizeof(interned_string));

    if (interned_string_table == NULL)
    {
        perror("Unable to allocate memory for interned_string_table.");
        exit(EOUT_OF_MEMORY);
    }

    interned_string_table->capacity = STRING_TABLE_NUM_INITIAL_ENTRIES;
    interned_string_table->length = 0;

    memset(interned_string_table->interned_strings, 0xFF, STRING_TABLE_NUM_INITIAL_ENTRIES * sizeof(interned_string));
}
