#include <err.h>
#include <stdlib.h>
#include <assert.h>

#include "ir.h"
#include "util.h"

typedef enum {
    IR_INVALID,
    IR_EOF,
} ir_type_t;

typedef struct {
    ir_type_t type;
} ir_t;

static ir_t* _gen_ir(node_t* node, FILE* stream)
{
    assert(node);

    switch (node->type) {
        case NODE_INVALID:
            err(EXIT_FAILURE, "invalid node found");
        case NODE_EOF:
        {
            ir_t* ir = calloc(1, sizeof(ir_t));
            ir->type = IR_EOF;
            return ir;
        }
        case NODE_DEF_VAR:
        case NODE_DEF_FUNC:
        case NODE_VAL:
        case NODE_OP:
        case NODE_PAR:
        case NODE_ASSIGN:
        case NODE_INCLUDE:
            NOT_YET_IMPLEMETED;
    }
}

void gen_ir(node_t* node, FILE* stream)
{
    assert(node);
    while(node->type != NODE_EOF)
    {
        ir_t* ir = _gen_ir(node, stream);
        if(ir->type == IR_EOF)
        {
            break;
        }
        node = node->next;
    }
}
