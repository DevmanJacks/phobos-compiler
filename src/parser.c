/*
 * Parser for Phobos language.
 */

#include "parser.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

#include "error.h"
#include "scanner.h"
#include "source.h"

static AstNode *parse_operand(Parser *p);
static AstNode *parse_type(Parser *p);

/* Errors */

typedef enum error_num
{
    ERR_EXPECTED_DECL,
    ERR_EXPECTED_IDENT,
    ERR_EXPECTED_OPERAND,
    ERR_EXPECTED_TYPE
} ErrorNum;

typedef struct error_def
{
    char *msg;
    int num_args;
} ErrorDef;

static ErrorDef error_defs[] = {
    [ERR_EXPECTED_DECL] = {"Expected func, interface or var.", 0},
    [ERR_EXPECTED_TYPE] = {"Expected identifier.", 0},
    [ERR_EXPECTED_IDENT] = {"Expected identifier or integer literal.", 0},
    [ERR_EXPECTED_TYPE] = {"Expected identifier.", 0}};

typedef struct error
{
    ErrorNum err;
    unsigned int start;
    unsigned int length;
} Error;

#define MAX_ERRORS 100

static Error errors[MAX_ERRORS];
static int num_errors = 0;

static void syntax_error(ErrorNum error_num, Token *token)
{
    if (num_errors >= MAX_ERRORS)
    {
        fprintf(stderr, "Maximun number of errors in a source file exceeded.");
        exit(EMAX_ERRORS_EXCEEDED);
    }

    errors[num_errors].err = error_num;
    errors[num_errors].start = token->start;
    errors[num_errors].length = token->len;
    num_errors++;
}

static inline Token *curr_token(Parser *p) { return p->scanner->current_token; }

static inline bool current_token_is_kind(Parser *p, TokenKind kind)
{
    return p->scanner->current_token->kind == kind;
}

static bool expect_token(Parser *p, TokenKind kind, Token **found_token)
{
    if (p->scanner->current_token->kind == kind)
    {
        *found_token = p->scanner->current_token;
        next_token(p->scanner);
        return true;
    }

    // TODO: Handle unexpected token
    return false;
}

static inline void skip_token(Parser *p) { next_token(p->scanner); }

/* Expressions */

AstNode *parse_binary_expr(Parser *p, unsigned int precidence_level)
{
    AstNode *expr = parse_operand(p);

    while (is_binary_op(curr_token(p)))
    {
        if (operator_precedence(curr_token(p)) > precidence_level)
        {
            Token *op = curr_token(p);
            next_token(p->scanner);
            expr = create_binary_expr_astnode(
                expr, op, parse_binary_expr(p, operator_precedence(op)));
        }
        else
        {
            break;
        }
    }

    return expr;
}

static AstNode *parse_identifier(Parser *p)
{
    if (current_token_is_kind(p, TOKEN_IDENTIFIER))
        return create_identifier_astnode(p->scanner->current_token);

    syntax_error(ERR_EXPECTED_IDENT, curr_token(p));
}

static AstNode *parse_operand(Parser *p)
{
    AstNode *node;

    if (current_token_is_kind(p, TOKEN_IDENTIFIER))
    {
        node = parse_identifier(p);
    }
    else if (current_token_is_kind(p, TOKEN_INTEGER_LITERAL))
    {
        node = create_integer_literal_astnode(p->scanner->current_token);
    }
    else
    {
        syntax_error(ERR_EXPECTED_OPERAND, curr_token(p));
    }

    next_token(p->scanner);

    return node;
}

AstNode *parse_expr(Parser *p) { return parse_binary_expr(p, 0); }

/* Declarations */

AstNode *parse_var_decl(Parser *p)
{
    Token *var;

    if (!expect_token(p, TOKEN_VAR, &var))
    {
        // This should never happen
        // TODO: Handle this better
        exit(-5);
    }

    Token *ident_token;

    if (expect_token(p, TOKEN_IDENTIFIER, &ident_token))
    {
        AstNode *ident = create_identifier_astnode(ident_token);
        AstNode *declared_type = NULL;

        if (current_token_is_kind(p, TOKEN_COLON))
        {
            declared_type = parse_type(p);
        }

        if (current_token_is_kind(p, TOKEN_ASSIGN))
        {
            next_token(p->scanner);

            AstNode *init_expr = parse_expr(p);
            return create_var_decl_astnode(var, ident, declared_type,
                                           init_expr);
        }
        else
            return create_var_decl_astnode(var, ident, declared_type, NULL);
    }
    else
    {
        // TODO: Log error and recover
    }

    return NULL;
}

static AstNode *parse_declaration(Parser *p)
{
    switch (curr_token(p)->kind)
    {
    case TOKEN_VAR:
        return parse_var_decl(p);

    default:
        syntax_error(ERR_EXPECTED_DECL, curr_token(p));
    }
}

/* Types */

AstNode *parse_type(Parser *p)
{
    if (current_token_is_kind(p, TOKEN_COLON))
        skip_token(p);

    switch (curr_token(p)->kind)
    {
    case TOKEN_IDENTIFIER:
        return parse_identifier(p);

    default:
        syntax_error(ERR_EXPECTED_IDENT, curr_token(p));
    }
}

static void parse(Parser *p)
{
    AstNode *decl = NULL;

    while (curr_token(p)->kind != TOKEN_EOF)
    {
        decl = parse_declaration(p);
        decl->next = p->source_file->decls;
        p->source_file->decls = decl;
    }
}

void parse_source_file(SourceFile *source_file)
{
    Parser *p = malloc(sizeof(Parser));

    if (!p)
    {
        perror("Unable to create parser.");
        exit(EOUT_OF_MEMORY);
    }

    p->source_file = source_file;
    p->scanner = create_scanner_for_source_file(source_file);

    parse(p);
}
