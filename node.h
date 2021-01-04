#pragma once

#include "token.h"

typedef enum {
    NODE_EOF,
    NODE_DEF_VAL,
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

node_t* generate_simple_node(node_type_t type);
