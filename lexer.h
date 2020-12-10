#pragma once

#include <stdint.h>

typedef enum {
    TK_OPR, // +, -, *, /
    TK_NUM, // 1 ~ 9
    TK_SEM, // ;
    TK_EOF,
} token_type_t;

typedef struct _token_base {
    token_type_t type;
    char* word;
} token_t;

typedef struct {
    token_t base;
    union {
        uint64_t uint64;
        // TODO
    };
} token_number_t;

typedef enum { OP_PLUS, OP_MINUS, OP_MUL, OP_DIV } operator_type_t;

typedef struct {
    token_t base;
    operator_type_t type;
} token_opr_t;

typedef struct {
    char* text;
} parse_context_t;

token_t* get_next_token(parse_context_t* context);
