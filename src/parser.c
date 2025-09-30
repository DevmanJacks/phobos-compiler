/*
 * Parser for Phobos language.
 */

 typedef struct {
    Scanner *scanner;
 } Parser;

 int current_token_is_kind(Parser *p, TokenKind kind) {
    return p->scanner->current_token->kind == kind;
 }

 Token *curr_token(Parser *p) {
   return p->scanner->current_token;
 }

 AstNode *parse_operand(Parser *p) {
    AstNode *node;

    if (current_token_is_kind(p, TOKEN_IDENTIFIER)) {
        node = create_identifier_astnode(p->scanner->current_token);
    } else if (current_token_is_kind(p, TOKEN_INTEGER_LITERAL)) {
        node = create_integer_literal_astnode(p->scanner->current_token);
    } else {
        fprintf(stderr, "Unexpected %s token in expression.", token_kind_string(p->scanner->current_token->kind));
        exit(ESYNTAX_ERROR);
    }

    next_token(p->scanner);

    return node;
 }

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

AstNode *parse_expr(Parser *p) {
   return parse_binary_expr(p, 0);
}

AstNode *parse(Parser *p) {
   return parse_expr(p);
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