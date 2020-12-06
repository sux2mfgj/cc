#include "parser.h"
#include <stddef.h>
#include <stdlib.h>
#include "lexer.h"
#include "util.h"

static node_t* parse_op(parse_context_t* ctx, node_t* left, token_opr_t* token)
{
    node_op_t* node = calloc(1, sizeof(node_op_t));
    node->base.type = NODE_OP;
    node->token = token;
    node->left = left;
    node->right = parse(ctx);

    return (node_t*)node;
}

static node_t* parse_val(parse_context_t* ctx, token_number_t* token)
{
    node_val_t* node = calloc(1, sizeof(node_op_t));
    node->base.type = NODE_VAL;
    node->token = token;

    return (node_t*)node;
}

node_t* parse(parse_context_t* ctx)
{
    token_t* token = get_next_token(ctx);

    if (token->type == TK_NUM) {
        node_t* n1 = parse_val(ctx, (token_number_t*)token);
        token_t* t2 = get_next_token(ctx);
        if (t2->type == TK_EOF) {
            return n1;
        }

        if (t2->type == TK_OPR) {
            return parse_op(ctx, n1, (token_opr_t*)t2);
        }

        NOT_YET_IMPLEMETED;
    }

    if (token->type == TK_EOF) {
        return NULL;
    }

    NOT_YET_IMPLEMETED;
    return NULL;
}
