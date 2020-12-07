#include "lexer.h"
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

static void skip_separators(parse_context_t* context)
{
    while (*context->text == ' ' || *context->text == '\n' ||
           *context->text == '\t') {
        context->text++;
    }
}

bool parse_value(parse_context_t* context, uint64_t* result, char** word)
{
    char* start = context->text;

    int len = 0;
    // decimal
    while (*context->text != ' ' && *context->text != '\n' &&
           *context->text != '\t' && *context->text) {
        len++;
        context->text++;
    }

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

token_t* get_next_token(parse_context_t* context)
{
    skip_separators(context);

    // parse number
    if ('0' <= *context->text && *context->text <= '9') {
        uint64_t value;
        char* word;
        if (!parse_value(context, &value, &word)) {
            return NULL;
        }

        token_number_t* number_token = calloc(1, sizeof(token_number_t));
        number_token->base.next = NULL;
        number_token->base.type = TK_NUM;
        number_token->uint64 = value;

        return (token_t*)number_token;
    }

    if (*context->text == '+') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.next = NULL;
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_PLUS;

        return (token_t*)opr_token;
    }

    if (*context->text == '-') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.next = NULL;
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_MINUS;

        return (token_t*)opr_token;
    }

    if (*context->text == '*') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.next = NULL;
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_MUL;

        return (token_t*)opr_token;
    }

    if (*context->text == '/') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.next = NULL;
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_DIV;

        return (token_t*)opr_token;
    }

    // reach to end of line
    if (*context->text == 0) {
        token_t* eof_token = calloc(1, sizeof(token_t));
        eof_token->next = NULL;
        eof_token->type = TK_EOF;
        return eof_token;
    }

    NOT_YET_IMPLEMETED;
    return NULL;
}
