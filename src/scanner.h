#pragma once

#include "token.h"

typedef struct Scanner {
    char *src;
    int len;
    int pos;

    Token *current_token;
} Scanner;

// Public functions
extern Scanner *create_scanner(char *src);
extern Token *next_token(Scanner *s);