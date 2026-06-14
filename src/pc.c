/*
 * Phobos compiler entry point.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "error.h"
#include "parser.h"
#include "source.h"

// Linked list of source files that we will include in the compilation
static SourceFile *source_files = NULL;

static void fatal_error(char *msg) {
    perror(msg);
    exit(-1);
}

static void display_usage() {
    fprintf(stderr, "Usage: pc [-scanonly] <input file>\n");
}

void main(int argc, char **argv) {
    if (argc < 2) {
        display_usage();
        exit(EINVALID_CMDLINE_ARGS);
    }

    // Command line parameters
    bool scan_only = false;
    char *filename = NULL;

    for (int i = 1; i < argc; i++) {
        char *arg = *(argv + i);

        if (*arg == '-') {
            if (strcmp(arg, "-scanonly") == 0)
                scan_only = true;
            else
                display_usage();
        } else if (!filename)
            filename = arg;
        else
            display_usage();
    }

    SourceFile *source_file = create_source_file(filename);

    if (scan_only)
        scan_source_file(source_file);
    else {
        source_file->next = source_files;
        source_files = source_file;
        parse_source_file(source_file);
    }
}