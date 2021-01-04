#pragma once

#include "token.h"

typedef enum {
    NODE_INVALID,
    NODE_EOF,
    NODE_DEF_VAR,
    NODE_VAL,
    NODE_OP,
} node_type_t;

typedef struct _node_t {
    node_type_t type;
    struct _node_t* next;
} node_t;

typedef struct {
    node_t base;
    ctype_t type;
    char* id;
    node_t* init;
} node_def_val_t;

typedef struct {
    node_t base;
    union {
        uint64_t uint64;
    };
} node_val_t;

typedef struct {
    node_t base;
    node_t *left, *right;
    operator_type_t opr;
} node_op_t;

node_t* generate_simple_node(node_type_t type);