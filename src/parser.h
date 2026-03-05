#pragma once

#include "ast.h"
#include "scanner.h"
#include "token.h"

 typedef struct {
    Scanner *scanner;
 } Parser;

 // Public functions