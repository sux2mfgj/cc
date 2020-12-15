#include "parser.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "lexer.h"
#include "util.h"

static node_t* _parse(parse_context_t* ctx, node_t* node);

static node_t* parse_op(parse_context_t* ctx,
                        node_t* left,
                        token_opr_t* op_token)
{
    node_op_t* node = calloc(1, sizeof(node_op_t));
    node->base.type = NODE_OP;
    node->base.next = NULL;
    node->token = op_token;
    node->left = left;
    switch (op_token->type) {
        case OP_PLUS:
        case OP_MINUS:
        {
            node_t* n1 = _parse(ctx, NULL);
            node_t* n2 = _parse(ctx, n1);
            node->right = n2 ? n2 : n1;
            break;
        }
        case OP_MUL:
        case OP_DIV:
            //node->right = parse(ctx);
            node->right = _parse(ctx, NULL);
            break;
    }
    assert(node->right && "detect invalid token (EOF)");

    return (node_t*)node;
}

static node_t* parse_parenthe(parse_context_t* ctx)
{
    node_par_t *node = calloc(1, sizeof(node_op_t));
    node->base.type = NODE_PAR;
    node->base.next = NULL;

    node_t *prev = NULL;

    while(true)
    {
        node_t* content = parse(ctx);
        if(content->type == NODE_R_PAR)
        {
            break;
        }

        if(!prev)
        {
            prev = content;
        }
        else
        {
            prev->next = content;
            prev = prev->next;
        }
    }
    node->contents = prev;

    return (node_t*)node;
}

static node_t* parse_val(token_number_t* token)
{
    node_val_t* node = calloc(1, sizeof(node_op_t));
    node->base.type = NODE_VAL;
    node->base.next = NULL;
    node->token = token;

    return (node_t*)node;
}

static node_t* parse_eof(token_t* token)
{
    node_t* node = calloc(1, sizeof(node_t));
    node->type = NODE_EOF;
    node->next = NULL;

    return node;
}

static node_t* parse_r_par(void)
{
    node_t* node = calloc(1, sizeof(node_t));
    node->type = NODE_R_PAR;
    node->next = NULL;

    return node;
}

static node_t* _parse(parse_context_t* ctx, node_t* node)
{
    token_t* t1 = get_next_token(ctx);

    if (t1->type == TK_SEM) {
        return NULL;
    }

    if (t1->type == TK_EOF) {
        return parse_eof(t1);
    }

    if (t1->type == TK_R_PAR) {
        return parse_r_par();
    }

    node_t* n1;

    if (t1->type == TK_NUM) {
        n1 = parse_val((token_number_t*)t1);
    }
    else if (t1->type == TK_OPR) {
        assert(node && "wtf");
        return parse_op(ctx, node, (token_opr_t*)t1);
    }
    else if (t1->type == TK_L_PAR) {
        return parse_parenthe(ctx);
    }
    else if (t1->type == TK_R_PAR) {
        return parse_r_par();
    }
    else {
        NOT_YET_IMPLEMETED;
    }

    if (!node) {
        return n1;
    }

    NOT_YET_IMPLEMETED;
}

static node_t *eof_node = NULL;
node_t* parse(parse_context_t* ctx)
{
    if(eof_node)
    {
        return eof_node;
    }

    node_t* node = _parse(ctx, NULL);
    while(true)
    {
        node_t* next;
        next = _parse(ctx, node);
        if(!next || next->type == NODE_EOF)
        {
            if(next && next->type == NODE_EOF)
            {
                eof_node = next;
            }

            return node;
        }
        node = next;
    }

    return node;
}
