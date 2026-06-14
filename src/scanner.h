#pragma once

#include "source.h"
#include "token.h"

typedef struct Scanner {
    SourceFile *source_file;
    long pos;

    Token *current_token;
} Scanner;

// Public functions
extern Scanner *create_scanner_for_source_file(SourceFile *source_file);
extern void scan_source_file(SourceFile *source_file);



extern Scanner *create_test_scanner(char *src);
extern Token *next_token(Scanner *s);