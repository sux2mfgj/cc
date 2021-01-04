#pragma once
#include <cgreen/cgreen.h>

#include <unistd.h>
#include "lexer.h"

static token_opr_t plus_token = {
    .base =
        {
            .type = TK_OPR,
        },
    .type = OP_PLUS,
};

static token_opr_t minus_op = {
    .base =
        {
            .type = TK_OPR,
        },
    .type = OP_MINUS,
};

static token_opr_t mul_op = {
    .base =
        {
            .type = TK_OPR,
        },
    .type = OP_MUL,
};

static token_opr_t div_op = {
    .base =
        {
            .type = TK_OPR,
        },
    .type = OP_DIV,
};

#define PREPARE_CTX(text)                  \
    int fds[2];                            \
    int status = pipe(fds);                \
    assert_that(status, is_equal_to(0));   \
    context_t* ctx = init_context(fds[0]); \
    write(fds[1], text, sizeof text);      \
    close(fds[1]);

TestSuite* lexer_tests(void);
