/*
 * Parser for Phobos language.
 */

#include "parser.h"

static Token *curr_token(Parser *p) {
   return p->scanner->current_token;
}

static int current_token_is_kind(Parser *p, TokenKind kind) {
   return p->scanner->current_token->kind == kind;
}

static int expect_token(Parser *p, TokenKind kind, Token *found_token) {
   if (p->scanner->current_token->kind == kind) {
      found_token = p->scanner->current_token;
      next_token(p->scanner);
      return true;
   }

   // TODO: Handle unexpected token
   return false;
}


/* Errors */

static void syntax_error(char *msg, ...) {
   va_list args;
   va_start(args, msg);
   vfprintf(stderr, msg, args);
   va_end(args);
   exit(ESYNTAX_ERROR);
}

/* Expressions */

AstNode *parse_binary_expr(Parser *p, unsigned int precidence_level) {
   AstNode *expr = parse_operand(p);

   while (is_binary_op(curr_token(p))) {
      if (operator_precedence(curr_token(p)) > precidence_level) {
         Token *op = curr_token(p);
         next_token(p->scanner);
         expr = create_binary_expr_astnode(expr, op, parse_binary_expr(p, operator_precedence(op)));
      } else {
         break;
      }
   }

   return expr;
}

static AstNode *parse_identifier(Parser *p) {
   if (current_token_is_kind(p, TOKEN_IDENTIFIER))
      return create_identifier_astnode(p->scanner->current_token);

   syntax_error("Expected identifier.");
}

AstNode *parse_operand(Parser *p) {
   AstNode *node;

   if (current_token_is_kind(p, TOKEN_IDENTIFIER)) {
      node = parse_identifier(p);
   } else if (current_token_is_kind(p, TOKEN_INTEGER_LITERAL)) {
      node = create_integer_literal_astnode(p->scanner->current_token);
   } else {
      syntax_error(stderr, "Unexpected %s token in expression.", token_kind_string(p->scanner->current_token->kind));
   }

   next_token(p->scanner);

   return node;
}


AstNode *parse_expr(Parser *p) {
   return parse_binary_expr(p, 0);
}

/* Declarations */

AstNode *parse_var_decl(Parser *p) {
   Token *var;

   if (!expect_token(p, TOKEN_VAR, &var)) {
      // TODO: Handle incorrect token
   }

   Token *ident_token;

   if (expect_token(p, TOKEN_IDENTIFIER, &ident_token)) {
      AstNode *ident = create_identifier_astnode(ident_token);

      if (current_token_is_kind(p, TOKEN_ASSIGN)) {
         next_token(p->scanner);

         AstNode *init_expr = parse_expr(p);
         return create_var_decl_astnode(var, ident, init_expr);
      }
      else
         return create_var_decl_astnode(var, ident, NULL);
   } else {
      // TODO: Log error and recover
   }

   return NULL;
}

static AstNode *parse_declaration(Parser *p) {
   switch (curr_token(p)->kind) {
      case TOKEN_VAR:
         return parse_var_decl(p);

      default:
         syntax_error("Expected var.");
   }
}


AstNode *parse(Parser *p) {
   return parse_declaration(p);
}

Parser *create_parser(Scanner *s) {
   Parser *p = malloc(sizeof(Parser));

   if (!p) {
      perror("Unable to create parser.");
      exit(EOUT_OF_MEMORY);
   }

   p->scanner = s;
   return p;
}