#pragma once

#include <stdint.h>

typedef enum {
    TK_INV,      //
    TK_OPR,      // +, -, *, /, ==, !=, +=,
    TK_NUM,      // 1 ~ 9
    TK_SEM,      // ;
    TK_L_PAR,    // { //TODO gather the parenthes {, }, (, ), [, ]
    TK_R_PAR,    // }
    TK_L_R_PAR,  // (
    TK_R_R_PAR,  // )
    TK_ASSIGN,   // =
    TK_UNARY,    // !, ++, --
    TK_ID,
    TK_EOF,
    TK_TYPE,  // uint64_t, void
    TK_RET,   // return
    TK_IF,
    TK_ELSE,
    TK_SHARP,   // #
    TK_DQUOTE,  // "
} token_type_t;

typedef struct _token_base {
    token_type_t type;
    uint64_t line;
} token_t;

typedef struct {
    token_t base;
    union {
        uint64_t uint64;
        // TODO
    };
} token_number_t;

typedef enum {
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_EQ,
    OP_NEQ,
    OP_ADDEQ,
    OP_MINEQ,
    OP_MULEQ,
    OP_DIVEQ,
    OP_LT,  // <
    OP_GT,  // >
    OP_LTEQ,
    OP_GTEQ,
} operator_type_t;

typedef enum {
    UNARY_NOT,
    UNARY_INC,
    UNARY_DEC,
} unary_opr_t;

typedef struct {
    token_t base;
    operator_type_t type;
} token_opr_t;

typedef enum { TYPE_UINT64, TYPE_INT, TYPE_VOID } ctype_t;

typedef struct {
    token_t base;
    ctype_t type;
} token_ctype_t;

typedef struct {
    token_t base;
    char* id;
} token_id_t;

typedef struct {
    token_t base;
    unary_opr_t type;
} token_unary_t;

char* token_to_str(token_t* t);
