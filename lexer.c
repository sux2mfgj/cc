#include "lexer.h"
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "debug.h"

static void skip_separators(parse_context_t* context)
{
    while (*context->text == ' ' || *context->text == '\n' ||
           *context->text == '\t') {
        context->text++;
    }
}

static bool is_number(parse_context_t* ctx)
{
    return '0' <= *ctx->text && *ctx->text <= '9';
}

static bool parse_value(parse_context_t* context, uint64_t* result, char** word)
{
    char* start = context->text;

    int len = 0;
    // decimal
    do {
        len++;
        context->text++;
    } while (is_number(context));

    *word = calloc(1, len);
    memcpy(*word, start, len);

    long value = strtol(*word, NULL, 10);
    if (errno) {
        warn("detect invalid value (cannot parse by strtol)\n");
        ;
        return false;
    }

    *result = value;
    return true;
}

static token_t* next;

static token_t* _get_next_token(parse_context_t* context, bool step)
{
    skip_separators(context);

    token_t* ret = NULL;
    if (next) {
        ret = next;
        goto found;
    }

    // parse number
    if (is_number(context)) {
        uint64_t value;
        char* word;
        if (!parse_value(context, &value, &word)) {
            return NULL;
        }

        token_number_t* number_token = calloc(1, sizeof(token_number_t));
        number_token->base.type = TK_NUM;
        number_token->uint64 = value;

        ret = (token_t*)number_token;
        debug ("token [number] %d\n", value);
        goto found;
    }

    if (*context->text == '+') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_PLUS;

        ret = (token_t*)opr_token;
        debug ("token [op] +\n");
        goto found;
    }

    if (*context->text == '-') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_MINUS;

        ret = (token_t*)opr_token;
        debug ("token [op] -\n");
        goto found;
    }

    if (*context->text == '*') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_MUL;

        ret = (token_t*)opr_token;
        debug ("token [op] *\n");
        goto found;
    }

    if (*context->text == '/') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_DIV;

        ret = (token_t*)opr_token;
        debug ("token [op] /\n");
        goto found;
    }

    if (*context->text == ';') {
        context->text++;
        token_t* token = calloc(1, sizeof(token_t));
        token->type = TK_SEM;

        ret = (token_t*)token;
        debug ("token [op] ;\n");
        goto found;
    }

    // reach to end of line
    if (*context->text == 0) {
        token_t* eof_token = calloc(1, sizeof(token_t));
        eof_token->type = TK_EOF;
        ret = (token_t*)eof_token;
        debug ("token [op] <EOF>\n");
        goto found;
    }

    NOT_YET_IMPLEMETED;
    return NULL;

found:
    next = step ? NULL : ret;

    return ret;
}

token_t* get_next_token(parse_context_t* context)
{
    return _get_next_token(context, true);
}
