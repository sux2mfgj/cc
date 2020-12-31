#include "ir.h"
#include <assert.h>
#include <stdbool.h>
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

typedef struct _variable_list_t {
    char* id;
    struct _variable_list_t* next;
} variable_list_t;

variable_list_t* variable_list_head;

static void push_variable(char* id)
{
    variable_list_t* node = calloc(1, sizeof(variable_list_t));
    node->id = id;
    node->next = NULL;

    if (!variable_list_head) {
        variable_list_head = node;
        return;
    }

    variable_list_t* current = variable_list_head;
    while (current->next) {
        current = current->next;
    }

    current->next = node;
}

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
        case NODE_PAR: {
            node_par_t* par = (node_par_t*)node;
            node_t* cur = par->contents;
            for (; cur; cur = cur->next) {
                ir_t* ir = _gen_ir(cur, stream);
            }

            break;
        }
        case NODE_SEM:
        case NODE_R_PAR:
            assert(false && "wtf!?");
        case NODE_EOF:
            break;
        case NODE_DEF_VAL: {
            node_def_val_t* n = (node_def_val_t*)node;
            push_variable(n->id);

            if (!n->init) {
                fprintf(stream, "u_%s = 0\n", n->id);
            }
            else {
                ir_t* result = _gen_ir(n->init, stream);
                fprintf(stream, "u_%s = ", n->id);
                put_ir(result, stream);
                fprintf(stream, "\n");
            }
            break;
        }
        case NODE_RET: {
            break;
        }
        case NODE_ASSIGN: {
            node_assign_t* n = (node_assign_t*)node;
            if (!n->right) {
                NOT_YET_IMPLEMETED;
            }
            ir_t* result = _gen_ir(n->right, stream);
            fprintf(stream, "u_%s = ", n->id);
            put_ir(result, stream);
            fprintf(stream, "\n");
            break;
        }
        case NODE_UNARY: {
            node_unary_t* n = (node_unary_t*)node;
            if (n->type == UNARY_NOT) {
                NOT_YET_IMPLEMETED;
            }
            fprintf(stream, "u_%s = u_%s", n->id, n->id);
            if (n->type == UNARY_INC) {
                fprintf(stream, " + u_%s\n", n->id);
            }
            else if (n->type == UNARY_DEC) {
                fprintf(stream, " - u_%s\n", n->id);
            }
            break;
        }
        default: {
            printf("oh my gosh %d\n", node->type);
            NOT_YET_IMPLEMETED;
        }
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
