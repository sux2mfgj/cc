#include "lexer.h"
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "util.h"

#ifndef NDEBUG
static void debug_print_token(token_t* t)
{
    switch (t->type) {
        case TK_OPR: {
            token_opr_t* op = (token_opr_t*)t;
            char c;
            switch (op->type) {
                case OP_PLUS:
                    c = '+';
                    break;
                case OP_MINUS:
                    c = '-';
                    break;
                case OP_MUL:
                    c = '+';
                    break;
                case OP_DIV:
                    c = '/';
                    break;
                case OP_EQ:
                    NOT_YET_IMPLEMETED;
                    break;
                case OP_NEQ:
                    NOT_YET_IMPLEMETED;
                    break;
            }
            debug("token [op]   %c\n", c);
            break;
        }
        case TK_NUM: {
            token_number_t* number_token = (token_number_t*)t;
            debug("token [num]  %d\n", number_token->uint64);
            break;
        }
        case TK_SEM: {
            debug("token [;]\n");
            break;
        }
        case TK_L_PAR: {
            debug("token [{]\n");
            break;
        }
        case TK_R_PAR: {
            debug("token [}]\n");
            break;
        }
        case TK_ASSIGN: {
            debug("token [=]\n");
            break;
        }
        case TK_ID: {
            debug("token [id] TODO\n");
            break;
        }
        case TK_EOF: {
            debug("token [eof]\n");
            break;
        }
        case TK_TYPE: {
            token_ctype_t* ct = (token_ctype_t*)t;
            char* text;
            switch (ct->type) {
                case TYPE_UINT64: {
                    text = "uint64_t";
                    break;
                }
            }

            debug("token [type] %s\n", text);
            break;
        }
        default:
            NOT_YET_IMPLEMETED;
    }
}
#endif

static bool is_skip_char(char c)
{
    return c == ' ' || c == '\n' || c == '\t';
}

static bool is_number(char c)
{
    return '0' <= c && c <= '9';
}

static token_t* parse_value(context_t* ctx)
{
    char* start = ctx->buffer;
    int len = 0;
    while (is_number(*ctx->buffer)) {
        len++;
        ctx->buffer++;
    }

    char buffer[81];
    memcpy(buffer, start, len);
    buffer[len] = '\0';
    long value = strtol(buffer, NULL, 10);
    if (errno) {
        warn("detect invalue value (cannot parse by strtol)");
        return NULL;
    }

    token_number_t* t = calloc(1, sizeof(token_number_t));
    t->base.type = TK_NUM;
    t->uint64 = value;

    return (token_t*)t;
}

static token_t* _next_token(context_t* ctx)
{
    token_t* t;

    bool done = false;
    while (!done) {
        if (*ctx->buffer) {
            while (is_skip_char(*ctx->buffer)) {
                ctx->buffer++;
            }
            done = true;
        }

        if (!*ctx->buffer) {
            if (!ctx->read_line(ctx)) {
                t = calloc(1, sizeof *t);
                t->type = TK_EOF;
                goto found;
            }
            done = false;
        }
    }

    if (is_number(*ctx->buffer)) {
        t = parse_value(ctx);
        goto found;
    }

    switch (*ctx->buffer) {
        case '=': {
            ctx->buffer++;
            if (*ctx->buffer == '=') {
                t = calloc(1, sizeof(operator_type_t));
                ctx->buffer++;
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_EQ;
                goto found;
            }
            else {
                t = calloc(1, sizeof(token_t));
                t->type = TK_ASSIGN;
                goto found;
            }
        }
        case '!': {
            ctx->buffer++;
            if (*ctx->buffer == '=') {
                t = calloc(1, sizeof(operator_type_t));
                ctx->buffer++;
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_NEQ;
                goto found;
            }
            else {
                t = calloc(1, sizeof(token_t));
                t->type = TK_NOT;
                goto found;
            }
        }
        case '+': {
            ctx->buffer++;
            t = calloc(1, sizeof(operator_type_t));
            if (*ctx->buffer == '=') {
                ctx->buffer++;
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_ADDEQ;
                goto found;
            }
            else {
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_PLUS;
                goto found;
            }
        }
        case ';': {
            ctx->buffer++;
            t = calloc(1, sizeof(token_t));
            t->type = TK_SEM;
            goto found;
        }
        case '{': {
            ctx->buffer++;
            t = calloc(1, sizeof(token_t));
            t->type = TK_L_PAR;
            goto found;
        }
        case '}': {
            ctx->buffer++;
            t = calloc(1, sizeof(token_t));
            t->type = TK_R_PAR;
            goto found;
        }
    }

    err(EXIT_FAILURE, "cannot parse a input that is %s\n", ctx->buffer);
found:
    return t;
}

/*
static lex_err_t _next_token(context_t* ctx, token_t** token)
{
    char t[81];
    char* word = t;
fill_buffer:
    while (!*ctx->buffer) {
        if (!ctx->read_line(ctx->buffer)) {
            return LEX_EOI;
        }
    }

    while (is_skip_char(*ctx->buffer)) {
        ctx->buffer++;
    }

    if (!*ctx->buffer) {
        goto fill_buffer;
    }

    if (is_reserved(*ctx->buffer)) {
        switch (*ctx->buffer) {
            case '=':
            case '!':
            case '<':
            case '>': {
                *word++ = *ctx->buffer++;
                if (*ctx->buffer == '=') {
                    *word++ = *ctx->buffer++;
                }
                goto found;
            }
            case '|': {
                *word++ = *ctx->buffer++;
                if (*ctx->buffer == '|') {
                    *word++ = *ctx->buffer++;
                }
                goto found;
            }
            case '&': {
                *word++ = *ctx->buffer++;
                if (*ctx->buffer == '&') {
                    *word++ = *ctx->buffer++;
                }
                goto found;
            }
            case '+': {
                *word++ = *ctx->buffer++;
                if (*ctx->buffer == '+') {
                    *word++ = *ctx->buffer++;
                }
                goto found;
            }
            case '-': {
                *word++ = *ctx->buffer++;
                if (*ctx->buffer == '-') {
                    *word++ = *ctx->buffer++;
                }
                else if (*ctx->buffer == '>') {
                    *word++ = *ctx->buffer++;
                }
                goto found;
            }
            default: {
                *word++ = *ctx->buffer++;
                goto found;
            }
        }
    }

    if (*ctx->buffer == '"') {
        while (true) {
            if (*ctx->buffer == '\\') {
                *word++ = *ctx->buffer++;
            }
            *word++ = *ctx->buffer++;
            if (*ctx->buffer == '"') {
                *word++ = *ctx->buffer++;
                goto found;
            }
        }
    }

    if (*ctx->buffer == '\'') {
        while (true) {
            if (*ctx->buffer == '\\') {
                *word++ = *ctx->buffer++;
            }
            *word++ = *ctx->buffer++;
            if (*ctx->buffer == '\'') {
                *word++ = *ctx->buffer++;
                goto found;
            }
        }
    }

    while (!is_skip_char(*ctx->buffer) || !*ctx->buffer) {
        if (is_reserved(*ctx->buffer)) {
            goto found;
        }
        *word++ = *ctx->buffer++;
    }

found:
    *word = '\0';

    return LEX_OK;
}

token_t* get_next_token(context_t* ctx)
{
    token_t *token;
    lex_err_t result = _next_token(ctx, &token);
    switch (result) {
        case LEX_OK:
        {
            return token;
        }
        case LEX_EOI:
        {
            token = calloc(1, sizeof *token);
            token->type = TK_EOF;
            return token;
        }
    }

    assert("wtf");
}
token_t* get_front_token(context_t* ctx);
*/

static token_t* front = NULL;
token_t* get_next_token(context_t* ctx)
{
    if (front)
    {
        token_t* ret = front;
        front = NULL;
        return ret;
    }

    return _next_token(ctx);

}

token_t* get_front_token(context_t* ctx)
{
    if (!front)
    {
        front = _next_token(ctx);
    }

    return front;
}
