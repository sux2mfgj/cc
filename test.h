#pragma once
#include <cgreen/cgreen.h>

#include "lexer.h"
#include "parser.h"

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

static node_t eof_node = {
    .type = NODE_EOF,
};

TestSuite* lexer_tests(void);
TestSuite* parser_tests(void);

