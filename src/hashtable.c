/*
 * hashtable.c
 */

static unsigned long hash_string(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;            // hash * 33 + c

    return hash;
}