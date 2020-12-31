#pragma once

#include "context.h"
#include "lexer.h"

typedef enum {
    NODE_OP,
    NODE_VAL,
    NODE_EOF,
    NODE_PAR,
    NODE_DEF_VAL,
    NODE_R_PAR,
    NODE_SEM,
    NODE_RET,
    NODE_ASSIGN,
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

typedef struct {
    node_t base;
    type_type_t type;
    char* id;
    node_t* init;
} node_def_val_t;

typedef struct {
    node_t base;
    node_t* regexp;
} node_ret_t;

typedef struct {
    node_t base;
    char* id;
    node_t* right;
} node_assign_t;

node_t* parse(context_t* ctx);
