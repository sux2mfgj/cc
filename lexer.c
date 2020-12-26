#include "lexer.h"
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
    switch(t->type)
    {
        case TK_OPR:
        {
            token_opr_t* op = (token_opr_t*)t;
            char c;
            switch (op->type){
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
            }
            debug("token [op]   %c\n", c);
            break;
        }
        case TK_NUM:
        {
            token_number_t* number_token = (token_number_t*)t;
            debug("token [num]  %d\n", number_token->uint64);
            break;
        }
        case TK_SEM:
        {
            debug("token [;]\n");
            break;
        }
        case TK_L_PAR:
        {
            debug("token [{]\n");
            break;
        }
        case TK_R_PAR:
        {
            debug("token [}]\n");
            break;
        }
        case TK_ASSIGN:
        {
            debug("token [=]\n");
            break;
        }
        case TK_ID:
        {
            debug("token [id] TODO\n");
            break;
        }
        case TK_EOF:
        {
            debug("token [eof]\n");
            break;
        }
        case TK_TYPE:
        {
            token_ctype_t* ct = (token_ctype_t*)t;
            char* text;
            switch(ct->type)
            {
                case TYPE_UINT64:
                {
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

static void skip_separators(parse_context_t* context)
{
    while (is_skip_char(*context->text))
    {
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
        return false;
    }

    *result = value;
    return true;
}

static token_t* try_to_parse_reserved(parse_context_t* ctx)
{
    if (!strncmp(
            ctx->text, "uint64_t",
            strlen(
                "uint64_t"))) {  // && *(ctx->text + sizeof "uint64_t") == ' '
                                 // // e.g. uint64_t12 is valid and not type.
        ctx->text += sizeof "uint64_t";

        token_ctype_t* token = calloc(1, sizeof(token_ctype_t));
        token->base.type = TK_TYPE;
        token->type = TYPE_UINT64;

        return (token_t*)token;
    }

    // TODO add other reserved words

    return NULL;
}

static int id_strlen(const char* text)
{
    int len;
    for (len = 0; !is_skip_char(*text) && *text != ';'; text++, len++)
        ;
    return len;
}

static token_t* try_to_parse_id(parse_context_t* ctx)
{
    int len = id_strlen(ctx->text);
    if (len == 0) {
        return NULL;
    }

    token_id_t* id = calloc(1, sizeof(token_id_t));
    id->base.type = TK_ID;
    id->id = calloc(1, len + 1);  // 1 is for null.
    strncpy(id->id, ctx->text, len);

    ctx->text += len;

    return (token_t*)id;
}

static token_t* front = NULL;

static token_t* _next_token(parse_context_t* context, bool step)
{
    skip_separators(context);

    token_t* ret = NULL;
    if (front) {
        ret = front;

        if (step) {
            front = NULL;
        }

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
        goto found;
    }

    if (*context->text == '+') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_PLUS;

        ret = (token_t*)opr_token;
        debug("token [op] +\n");
        goto found;
    }

    if (*context->text == '-') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_MINUS;

        ret = (token_t*)opr_token;
        debug("token [op] -\n");
        goto found;
    }

    if (*context->text == '*') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_MUL;

        ret = (token_t*)opr_token;
        goto found;
    }

    if (*context->text == '/') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_DIV;

        ret = (token_t*)opr_token;
        goto found;
    }

    if (*context->text == ';') {
        context->text++;
        token_t* token = calloc(1, sizeof(token_t));
        token->type = TK_SEM;

        ret = (token_t*)token;
        goto found;
    }

    if (*context->text == '{') {
        context->text++;
        token_t* token = calloc(1, sizeof(token_t));
        token->type = TK_L_PAR;

        ret = (token_t*)token;
        goto found;
    }

    if (*context->text == '}') {
        context->text++;
        token_t* token = calloc(1, sizeof(token_t));
        token->type = TK_R_PAR;

        ret = (token_t*)token;
        goto found;
    }

    // reach to end of line
    if (*context->text == 0) {
        token_t* eof_token = calloc(1, sizeof(token_t));
        eof_token->type = TK_EOF;
        ret = (token_t*)eof_token;
        goto found;
    }

    ret = try_to_parse_reserved(context);
    if (ret) {
        goto found;
    }

    ret = try_to_parse_id(context);
    if (ret) {
        goto found;
    }

    NOT_YET_IMPLEMETED;
    return NULL;

found:
    if (!step) {
        front = ret;
    }

    debug_print_token(ret);

    return ret;
}

token_t* get_next_token(parse_context_t* ctx)
{
    return _next_token(ctx, true);
}

token_t* get_front_token(parse_context_t* ctx)
{
    return _next_token(ctx, false);
}
