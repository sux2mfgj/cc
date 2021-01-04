#pragma once

typedef enum {
    NODE_EOF,
} node_type_t;

typedef struct _node_t {
    node_type_t type;
    struct _node_t* next;
} node_t;
