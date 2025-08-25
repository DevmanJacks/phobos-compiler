/*
 * Abstract Syntax Tree functionality for Phobos programming language.
 */

 typedef enum {
    ASTNODE_BINARY_EXPR,
    ASTNODE_IDENTIFIER,
    ASTNODE_NUMERIC_LITERAL
 } AstNodeType;

char *astnode_type_string(AstNodeType type) {
    switch (type) {
        case ASTNODE_BINARY_EXPR: return "Binary Expr";
        case ASTNODE_IDENTIFIER: return "Identifier";
        case ASTNODE_NUMERIC_LITERAL: return "Numeric Literal";

        default: return "UNKNOWN";
    }
}

 typedef struct ast_node {
    AstNodeType type;
    union {
        struct {
            struct ast_node *left;
            Token *op;
            struct ast_node *right;
        } binary_expr;
        Token *token;
    };
 } AstNode;

 int astnode_start(AstNode *node) {
    switch (node->type) {
        case ASTNODE_BINARY_EXPR:
            return 0;

        default:
            return node->token->start;
    }
 }

 int astnode_len(AstNode *node) {
    switch (node->type) {
        case ASTNODE_BINARY_EXPR:        
            while (node->binary_expr.right->type == ASTNODE_BINARY_EXPR)
                node = node->binary_expr.right;

            return astnode_start(node->binary_expr.right) + astnode_len(node->binary_expr.right);

        default:
            return node->token->len;
    }
 }

 AstNode *create_binary_expr_astnode(AstNode *left, Token *op, AstNode *right) {
        AstNode *node = malloc(sizeof(AstNode));

    if (!node) {
        perror("Unable to create ast node for binary expression.");
        exit(EOUT_OF_MEMORY);
    }

    node->type = ASTNODE_BINARY_EXPR;
    node->binary_expr.left = left;
    node->binary_expr.op = op;
    node->binary_expr.right = right;
 }

 AstNode *create_identifier_astnode(Token *t) {
    AstNode *node = malloc(sizeof(AstNode));

    if (!node) {
        perror("Unable to create ast node for identifier.");
        exit(EOUT_OF_MEMORY);
    }

    node->type = ASTNODE_IDENTIFIER;
    node->token = t;
 }

  AstNode *create_integer_literal_astnode(Token *t) {
    AstNode *node = malloc(sizeof(AstNode));

    if (!node) {
        perror("Unable to create ast node for integer literal.");
        exit(EOUT_OF_MEMORY);
    }

    node->type = ASTNODE_NUMERIC_LITERAL;
    node->token = t;
 }

 void print_astnode(FILE *file, AstNode *node) {
    fprintf(file, "{ \"node\": \"%s\", \"start\": %d, \"len\": %d", astnode_type_string(node->type), astnode_start(node), astnode_len(node));

    switch (node->type) {
        case ASTNODE_BINARY_EXPR:
            fprintf(file, ", \"left\": ");
            print_astnode(file, node->binary_expr.left);
            fprintf(file, ", \"op\": ");
            print_token(file, node->binary_expr.op);
            fprintf(file, ", \"right\": ");
            print_astnode(file, node->binary_expr.right);
            break;

        case ASTNODE_IDENTIFIER:
            fprintf(file, ", \"name\": \"%s\" ", interned_string_value(node->token->ident));
            break;

        case ASTNODE_NUMERIC_LITERAL:
            fprintf(file, ", \"value\": %d ", node->token->integer_literal);
            break;

        default:
            fprintf(file, "UNKNOWN (%d)", node->type);
            break;
    }

    fprintf(file, "}");
}