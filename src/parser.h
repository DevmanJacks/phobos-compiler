#pragma once

#include "ast.h"
#include "scanner.h"
#include "token.h"

 typedef struct {
    Scanner *scanner;
 } Parser;

 // Public functions
 extern Parser *create_parser(Scanner *s);
 extern AstNode *parse(Parser *p);