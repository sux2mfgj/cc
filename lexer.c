#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "util.h"

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
    t->base.line = ctx->line_number;
    t->uint64 = value;

    return (token_t*)t;
}

static bool check_str_without_terminator(char* str, const char* ideal)
{
    return !strncmp(str, ideal, strlen(ideal));
}

static token_t* try_to_parse_reserved(context_t* ctx)
{
    if (check_str_without_terminator(ctx->buffer, "uint64_t")) {
        token_ctype_t* t = calloc(1, sizeof(token_ctype_t));
        t->base.type = TK_TYPE;
        t->base.line = ctx->line_number;
        t->type = TYPE_UINT64;

        ctx->buffer += strlen("uint64_t");

        return (token_t*)t;
    }

    if (check_str_without_terminator(ctx->buffer, "int")) {

        token_ctype_t* t = calloc(1, sizeof(token_ctype_t));
        t->base.type = TK_TYPE;
        t->base.line = ctx->line_number;
        t->type = TYPE_INT;

        ctx->buffer += strlen("int");

        return (token_t*)t;
    }

    if (check_str_without_terminator(ctx->buffer, "void")) {
        token_ctype_t* t = calloc(1, sizeof(token_ctype_t));
        t->base.type = TK_TYPE;
        t->base.line = ctx->line_number;
        t->type = TYPE_VOID;

        ctx->buffer += strlen("void");

        return (token_t*)t;
    }

    if (check_str_without_terminator(ctx->buffer, "if")) {
        token_t* t = calloc(1, sizeof(token_t));
        t->type = TK_IF;
        t->line = ctx->line_number;

        ctx->buffer += strlen("if");

        return (token_t*)t;
    }

    if (check_str_without_terminator(ctx->buffer, "else")) {
        token_t* t = calloc(1, sizeof(token_t));
        t->type = TK_ELSE;
        t->line = ctx->line_number;

        ctx->buffer += strlen("else");

        return (token_t*)t;
    }

    if (check_str_without_terminator(ctx->buffer, "return")) {
        token_t* t = calloc(1, sizeof(token_t));
        t->type = TK_RET;
        t->line = ctx->line_number;

        ctx->buffer += strlen("return");

        return t;
    }
    // TODO add other words

    return NULL;
}

static bool is_reserved(context_t* ctx)
{
    switch (*ctx->buffer) {
        case '=':
        case '!':
        case '+':
        case ';':
        case '{':
        case '}':
        case '(':
        case ')':
        case '<':
        case '>':
            // TODO
            return true;
    }

    return false;
}

static token_t* parse_id(context_t* ctx)
{
    char* start = ctx->buffer;
    int len = 0;
    while (!is_skip_char(*ctx->buffer) && *ctx->buffer && !is_reserved(ctx)) {
        ctx->buffer++;
        len++;
    }

    token_id_t* t = calloc(1, sizeof(token_id_t));
    t->base.type = TK_ID;
    t->base.line = ctx->line_number;
    t->id = calloc(len + 1, sizeof(char));
    memcpy(t->id, start, len);

    return (token_t*)t;
}

static token_t* _next_token(context_t* ctx);

typedef struct _tnode_t {
    struct _tnode_t* next;
    token_t* token;
} tnode_t;

static tnode_t* tnode_head = NULL;

static void push_tnode(token_t* token)
{
    tnode_t* tnode = calloc(1, sizeof(tnode_t));
    tnode->next = NULL;
    tnode->token = token;
    if (!tnode_head) {
        tnode_head = tnode;
        return;
    }

    tnode_t* cur = tnode_head;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = tnode;
}

static token_t* pop_tnode(context_t* ctx)
{
    if (!tnode_head) {
        return _next_token(ctx);
    }

    tnode_t* ret = tnode_head;
    tnode_head = tnode_head->next;

    token_t* token = ret->token;
    free(ret);

    warn("pop_tnode: %u", token->type);
    return token;
}

static void parse_include(context_t* ctx)
{
    while (is_skip_char(*ctx->buffer)) {
        ctx->buffer++;
    }

    token_t* inc = parse_id(ctx);
    inc->line = ctx->line_number;
    assert(inc);
    push_tnode(inc);

    while (is_skip_char(*ctx->buffer)) {
        ctx->buffer++;
    }

    char end;
    token_t* close_token;

    if (*ctx->buffer == '<') {
        ctx->buffer++;
        token_opr_t* t = calloc(1, sizeof(token_opr_t));
        t->base.type = TK_OPR;
        t->base.line = ctx->line_number;
        t->type = OP_LT;

        end = '>';

        token_opr_t* t2 = calloc(1, sizeof(token_opr_t));
        t2->base.type = TK_OPR;
        t2->base.line = ctx->line_number;
        t2->type = OP_GT;
        close_token = (token_t*)t2;

        push_tnode((token_t*)t);
    }
    else if (*ctx->buffer == '\"') {
        ctx->buffer++;
        token_t* t = calloc(1, sizeof(token_t));
        t->type = TK_DQUOTE;
        t->line = ctx->line_number;
        end = '\"';

        close_token = calloc(1, sizeof(token_t));
        close_token->type = TK_DQUOTE;
        close_token->line = ctx->line_number;
        push_tnode(t);
    }
    else {
        NOT_YET_IMPLEMETED;
    }

    char* start = ctx->buffer;
    int len = 0;
    while (*ctx->buffer != end) {
        ctx->buffer++;
        len++;
    }

    token_id_t* tid = calloc(1, sizeof(token_id_t));
    tid->base.type = TK_ID;
    tid->base.line = ctx->line_number;
    tid->id = calloc(len + 1, sizeof(char));
    memcpy(tid->id, start, len);

    push_tnode((token_t*)tid);

    if (*ctx->buffer == end) {
        ctx->buffer++;
        push_tnode(close_token);
    }
    else {
        NOT_YET_IMPLEMETED;
    }
}

static token_t* eof_token = NULL;

static token_t* _next_token(context_t* ctx)
{
    if (eof_token) {
        return eof_token;
    }

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
                eof_token = t;
                goto found;
            }
            done = false;
        }
    }

    if (is_number(*ctx->buffer)) {
        t = parse_value(ctx);
        goto found;
    }

    t = try_to_parse_reserved(ctx);
    if (t) {
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
                t->line = ctx->line_number;
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
                t->line = ctx->line_number;
                goto found;
            }
            else {
                t = calloc(1, sizeof(token_unary_t));
                ((token_unary_t*)t)->base.type = TK_UNARY;
                ((token_unary_t*)t)->type = UNARY_NOT;
                t->line = ctx->line_number;
                goto found;
            }
        }
        case '+': {
            ctx->buffer++;
            if (*ctx->buffer == '=') {
                t = calloc(1, sizeof(operator_type_t));
                ctx->buffer++;
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_ADDEQ;
                t->line = ctx->line_number;
                goto found;
            }
            else if (*ctx->buffer == '+') {
                t = calloc(1, sizeof(token_unary_t));
                ((token_unary_t*)t)->base.type = TK_UNARY;
                ((token_unary_t*)t)->type = UNARY_INC;
                t->line = ctx->line_number;
                ctx->buffer++;
                goto found;
            }
            else {
                t = calloc(1, sizeof(operator_type_t));
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_PLUS;
                t->line = ctx->line_number;
                goto found;
            }
        }
        case '-': {
            ctx->buffer++;
            if (*ctx->buffer == '=') {
                t = calloc(1, sizeof(token_opr_t));
                ctx->buffer++;
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_MINEQ;
                t->line = ctx->line_number;
                goto found;
            }
            else if (*ctx->buffer == '-') {
                t = calloc(1, sizeof(token_unary_t));
                ((token_unary_t*)t)->base.type = TK_UNARY;
                ((token_unary_t*)t)->type = UNARY_DEC;
                t->line = ctx->line_number;
                ctx->buffer++;
                goto found;
            }
            else {
                t = calloc(1, sizeof(token_opr_t));
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_MINUS;
                t->line = ctx->line_number;
                goto found;
            }
        }
        case '*': {
            ctx->buffer++;
            t = calloc(1, sizeof(operator_type_t));
            if (*ctx->buffer == '=') {
                ctx->buffer++;
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_MULEQ;
                t->line = ctx->line_number;
                goto found;
            }
            else {
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_MUL;
                t->line = ctx->line_number;
                goto found;
            }
        }
        case '/': {
            ctx->buffer++;
            t = calloc(1, sizeof(operator_type_t));
            if (*ctx->buffer == '=') {
                ctx->buffer++;
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_DIVEQ;
                t->line = ctx->line_number;
                goto found;
            }
            else {
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_DIV;
                t->line = ctx->line_number;
                goto found;
            }
        }
        case '<': {
            ctx->buffer++;
            t = calloc(1, sizeof(operator_type_t));
            if (*ctx->buffer == '=') {
                ctx->buffer++;
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_LTEQ;
                t->line = ctx->line_number;
                goto found;
            }
            else {
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_LT;
                t->line = ctx->line_number;
                goto found;
            }
        }
        case '>': {
            ctx->buffer++;
            t = calloc(1, sizeof(operator_type_t));
            if (*ctx->buffer == '=') {
                ctx->buffer++;
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_GTEQ;
                t->line = ctx->line_number;
                goto found;
            }
            else {
                ((token_opr_t*)t)->base.type = TK_OPR;
                ((token_opr_t*)t)->type = OP_GT;
                t->line = ctx->line_number;
                goto found;
            }
        }
        case ';': {
            ctx->buffer++;
            t = calloc(1, sizeof(token_t));
            t->type = TK_SEM;
            t->line = ctx->line_number;
            goto found;
        }
        case '{': {
            ctx->buffer++;
            t = calloc(1, sizeof(token_t));
            t->type = TK_L_PAR;
            t->line = ctx->line_number;
            goto found;
        }
        case '}': {
            ctx->buffer++;
            t = calloc(1, sizeof(token_t));
            t->type = TK_R_PAR;
            t->line = ctx->line_number;
            goto found;
        }
        case '(': {
            ctx->buffer++;
            t = calloc(1, sizeof(token_t));
            t->type = TK_L_R_PAR;
            t->line = ctx->line_number;
            goto found;
        }
        case ')': {
            ctx->buffer++;
            t = calloc(1, sizeof(token_t));
            t->type = TK_R_R_PAR;
            t->line = ctx->line_number;
            goto found;
        }
        case '#': {
            ctx->buffer++;
            t = calloc(1, sizeof(token_t));
            t->type = TK_SHARP;
            t->line = ctx->line_number;
            parse_include(ctx);
            goto found;
        }
    }

    t = parse_id(ctx);
    if (t) {
        goto found;
    }

    err(EXIT_FAILURE, "cannot parse a input that is %s\n", ctx->buffer);
found:
    return t;
}

static token_t* front = NULL;
token_t* get_next_token(context_t* ctx)
{
    if (front) {
        token_t* ret = front;
        front = NULL;
        return ret;
    }

    return pop_tnode(ctx);
}

token_t* get_front_token(context_t* ctx)
{
    if (!front) {
        front = pop_tnode(ctx);
    }

    return front;
}
