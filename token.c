/*
 * Token details.
 */

typedef enum {
    TOKEN_CONST = 0,
    TOKEN_DEFER = 6,
    TOKEN_ELSE = 12,
    TOKEN_ENUM = 17,
    TOKEN_FUNC = 22,
    TOKEN_FOR = 27,
    TOKEN_IF = 31,
    TOKEN_IMPORT = 34,
    TOKEN_INTERFACE = 41,
    TOKEN_STRUCT = 51,
    TOKEN_VAR = 58,
    TOKEN_WHILE = 62,

    TOKEN_IDENTIFIER,
    TOKEN_INTEGER_LITERAL,

    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,

    // Assignment
    TOKEN_ASSIGN,
    TOKEN_DECLARE_ASSIGN,
    TOKEN_ADD_ASSIGN,
    TOKEN_SUBTRACT_ASSIGN,
    TOKEN_MULTIPLY_ASSIGN,
    TOKEN_DIVIDE_ASSIGN,

    // Arithmetic operators
    TOKEN_ADD,
    TOKEN_SUBTRACT,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,

    // Boolean operators
    TOKEN_EQ,
    TOKEN_GE,
    TOKEN_GT,
    TOKEN_LE,
    TOKEN_LT,

    TOKEN_EOF
} TokenKind;

typedef struct Token {
    TokenKind kind;
    char *start;
    int len;
    union {
        interned_string ident;
        int integer_literal;
    };
} Token;

void token_initialise() {
    // Intern all of the keywords
    intern_string("const");
    intern_string("defer");
    intern_string("else");
    intern_string("enum");
    intern_string("func");
    intern_string("for");
    intern_string("if");
    intern_string("import");
    intern_string("interface");
    intern_string("struct");
    intern_string("var");
    intern_string("while");
}

Token *create_token(TokenKind kind, char *start, int len) {
    Token *new_token = malloc(sizeof(Token));

    if (!new_token) {
        perror("Unable to create token.");
        exit(EOUT_OF_MEMORY);
    }
 
    new_token->kind = kind;
    new_token->start = start;
    new_token->len = len;

    return new_token;
}

char *token_kind_string(TokenKind kind) {
    switch (kind) {
        case TOKEN_CONST: return "CONST";
        case TOKEN_DEFER: return "DEFER";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_ENUM: return "ENUM";
        case TOKEN_FUNC: return "FUNC";
        case TOKEN_FOR: return "FOR";
        case TOKEN_IF: return "IF";
        case TOKEN_IMPORT: return "IMPORT";
        case TOKEN_INTERFACE: return "INTERFACE";
        case TOKEN_STRUCT: return "STRUCT";
        case TOKEN_VAR: return "VAR";
        case TOKEN_WHILE: return "WHILE";

        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER_LITERAL: return "INTEGER_LITERAL";

        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        
        // Assignment
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_DECLARE_ASSIGN: return "DECLARE_ASSIGN";
        case TOKEN_ADD_ASSIGN: return "ADD_ASSIGN";
        case TOKEN_SUBTRACT_ASSIGN: return "SUBTRACT_ASSIGN";
        case TOKEN_MULTIPLY_ASSIGN: return "MULTIPLY_ASSIGN";
        case TOKEN_DIVIDE_ASSIGN: return "DIVIDE_ASSIGN";

        // Arithmetic operators
        case TOKEN_ADD: return "ADD";
        case TOKEN_SUBTRACT: return "SUBTRACT";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";

        // Boolean operators
        case TOKEN_EQ: return "EQUAL";
        case TOKEN_GE: return "GREATER_THAN_OR_EQUAL";
        case TOKEN_GT: return "GREATER_THAN";
        case TOKEN_LE: return "LESS_THAN_OR_EQUAL";
        case TOKEN_LT: return "LESS_THAN";
        
        case TOKEN_EOF: return "EOF";

        default: return "UNKNOWN";
    }
}

void print_token(FILE *file, Token *t, char *src_start) {
    fprintf(file, "{ \"Token\": \"%s\", \"start\": %d, \"len\": %d", token_kind_string(t->kind), t->start - src_start, t->len);

    switch (t->kind) {
        case TOKEN_IDENTIFIER:
            fprintf(file, ", \"identifier\": \"%s\" }", interned_string_value(t->ident));
            break;

        case TOKEN_INTEGER_LITERAL:
            fprintf(file, ", \"value\": %d }", t->integer_literal);
            break;

        default:
            fprintf(file, " }");
            break;
    }
}