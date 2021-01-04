#pragma once

#include "token.h"

typedef enum {
    NODE_INVALID,
    NODE_EOF,
    NODE_DEF_VAR,
    NODE_DEF_FUNC,
    NODE_VAL,
    NODE_OP,
    NODE_PAR,
    NODE_ASSIGN,
    NODE_INCLUDE,
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

typedef struct {
    node_t base;
    ctype_t ret_type;
    char* id;
    node_def_val_t* args;
    node_t* proc;
} node_func_t;

typedef struct {
    node_t base;
    node_t* contents;
} node_par_t;

typedef struct {
    node_t base;
    char* id;  // it should be node_t*
    node_t* expr;
} node_assign_t;

typedef struct {
    node_t base;
    char* filename;
} node_include_t;

node_t* generate_simple_node(node_type_t type);
