#pragma once

#include "lexer.h"

typedef enum {
    NODE_OP,
    NODE_VAL,
    NODE_EOF,
    NODE_PAR,
    NODE_R_PAR,
} node_type_t;

typedef struct _node_t {
    node_type_t type;
    struct _node_t* next;
} node_t;

typedef struct {
    node_t base;
    struct _node_t *left, *right;
    token_opr_t* token;
} node_op_t;

typedef struct {
    node_t base;
    union {
        token_number_t* token;
    };
} node_val_t;

typedef struct {
    node_t base;
    node_t* contents;
} node_par_t;

node_t* parse(parse_context_t* ctx);
