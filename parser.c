#include "parser.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "lexer.h"
#include "util.h"

static node_t* parse_op(parse_context_t* ctx,
                        node_t* left,
                        token_opr_t* op_token)
{
    node_op_t* node = calloc(1, sizeof(node_op_t));
    node->base.type = NODE_OP;
    node->token = op_token;
    node->left = left;
    node->right = parse(ctx);
    assert(node->right && "detect invalid token (EOF)");

    return (node_t*)node;
}

static node_t* parse_val(token_number_t* token)
{
    node_val_t* node = calloc(1, sizeof(node_op_t));
    node->base.type = NODE_VAL;
    node->token = token;

    return (node_t*)node;
}

static node_t* _parse(parse_context_t* ctx, node_t* node)
{
    token_t* t1 = get_next_token(ctx);

    if (t1->type == TK_SEM) {
        return node;
    }

    if (t1->type == TK_NUM) {
        return parse_val((token_number_t*)t1);
    }

    if (t1->type == TK_EOF)
    {
        return NULL;
    }

    if (node) {
        if (t1->type == TK_OPR) {
            return parse_op(ctx, node, (token_opr_t*)t1);
        }

        NOT_YET_IMPLEMETED;
    }

    NOT_YET_IMPLEMETED;
}

node_t* parse(parse_context_t* ctx)
{
    node_t* node = NULL;
    while (true) {
        node_t* pre = node;
        node = _parse(ctx, node);
        if (pre == node) {
            return node;
        }
    }
}
