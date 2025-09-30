/*
 * Phobos compiler entry point.
 */

#include "intern.h"

void main(int argc, char **argv) {
    string_intern_initialise();
    // token_initialise();
    
    // //Scanner *s = create_scanner("const defer else enum func for if import interface struct var while");
    // //Scanner *s = create_scanner("if a > 3 b = true else b = false");
    // Scanner *s = create_scanner("abc * 10 + 3 * 4");
    // Parser *p = create_parser(s);

    // AstNode *expr = parse(p);
    // print_astnode(stderr, expr);
}