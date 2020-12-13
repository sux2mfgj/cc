#include "ir.h"
#include <stdlib.h>
#include "util.h"

typedef enum { IR_VAR, IR_VAL } ir_type_t;

typedef struct {
    ir_type_t type;
    union {
        uint64_t val;
        uint64_t var_index;
    };
} ir_t;

// $t1 = 1 + 2
// ret $t1

static void put_ir(ir_t* ir, FILE* stream)
{
    switch (ir->type) {
        case IR_VAL: {
            fprintf(stream, "$%ld", ir->val);
            break;
        }
        case IR_VAR: {
            fprintf(stream, "t%ld", ir->var_index);
            break;
        }
    }
}

static void put_op(node_op_t* node, FILE* stream)
{
    switch (node->token->type) {
        case OP_PLUS: {
            fputs(" + ", stream);
            break;
        }
        case OP_MINUS: {
            fputs(" - ", stream);
            break;
        }
        case OP_MUL: {
            fputs(" * ", stream);
            break;
        }
        case OP_DIV: {
            fputs(" / ", stream);
            break;
        }
    }
}

int var_index;

static ir_t* _gen_ir(node_t* node, FILE* stream)
{
    if (!node) {
        return NULL;
    }

    switch (node->type) {
        case NODE_VAL: {
            node_val_t* node_val = (node_val_t*)node;
            ir_t* ir = calloc(1, sizeof(ir_t));
            ir->type = IR_VAL;
            ir->val = node_val->token->uint64;
            return ir;
        }
        case NODE_OP: {
            node_op_t* node_op = (node_op_t*)node;
            ir_t* left = _gen_ir(node_op->left, stream);
            ir_t* right = _gen_ir(node_op->right, stream);

            ir_t* ir_result = calloc(1, sizeof(ir_t));
            ir_result->type = IR_VAR;
            ir_result->var_index = var_index++;

            put_ir(ir_result, stream);
            fputs(" = ", stream);
            put_ir(left, stream);
            put_op(node_op, stream);
            put_ir(right, stream);
            fprintf(stream, "\n");
            return ir_result;
        }
        case NODE_EOF:
            break;
    }

    return NULL;
}

void gen_ir(node_t* node, FILE* stream)
{
    var_index = 0;
    while (_gen_ir(node, stream)) {
        node = node->next;
    }
}
