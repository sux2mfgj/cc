#include "parser.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "lexer.h"
#include "util.h"

static node_t* _parse(context_t* ctx, node_t* node);

static node_t* parse_op(context_t* ctx, node_t* left, token_opr_t* op_token)
{
    node_op_t* node = calloc(1, sizeof(node_op_t));
    node->base.type = NODE_OP;
    node->base.next = NULL;
    node->token = op_token;
    node->left = left;
    switch (op_token->type) {
        case OP_PLUS:
        case OP_MINUS: {
            node_t* n1 = _parse(ctx, NULL);
            node_t* n2 = _parse(ctx, n1);
            node->right = n2->type != NODE_SEM ? n2 : n1;
            break;
        }
        case OP_MUL:
        case OP_DIV:
            // node->right = parse(ctx);
            node->right = _parse(ctx, NULL);
            break;
        default:
            NOT_YET_IMPLEMETED;
    }
    assert(node->right && "detect invalid token (EOF)");

    return (node_t*)node;
}

static bool skip_semicolon(context_t* ctx)
{
    node_t* sem_node = parse(ctx);
    get_next_token(ctx);

    return sem_node->type == NODE_SEM;
}

static node_t* parse_parenthe(context_t* ctx)
{
    node_par_t* node = calloc(1, sizeof(node_op_t));
    node->base.type = NODE_PAR;
    node->base.next = NULL;

    node_t* head = NULL;
    node_t* cur;

    while (true) {
        node_t* content = parse(ctx);
        assert(content->type != NODE_EOF);

        if (content->type == NODE_R_PAR) {
            break;
        }

        if (content->type == NODE_SEM) {
            get_next_token(ctx);
        }
        else {
            if (!skip_semicolon(ctx)) {
                assert("wtf");
            }

            if (!head) {
                head = content;
                cur = head;
            }
            else {
                cur->next = content;
                cur = cur->next;
            }
        }
    }
    node->contents = head;

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

static node_t* generate_node(node_type_t type)
{
    node_t* node = calloc(1, sizeof(node_t));
    node->type = type;
    node->next = NULL;

    return node;
}

static node_t* parse_def_val(context_t* ctx,
                             token_ctype_t* ctype,
                             token_id_t* id)
{
    node_def_val_t* node = calloc(1, sizeof(node_def_val_t));

    node->base.type = NODE_DEF_VAL;
    node->base.next = NULL;

    node->type = ctype->type;
    node->id = id->id;

    token_t* next = get_front_token(ctx);
    if (next->type == TK_ASSIGN) {
        get_next_token(ctx);
        node->init = parse(ctx);
    }

    return (node_t*)node;
}

static node_t* parse_def_func(context_t* ctx,
                              token_ctype_t* ctype,
                              token_id_t* id)
{
    token_t* t = get_next_token(ctx);
    assert(t->type == TK_L_R_PAR);

    node_func_t* node = calloc(1, sizeof(node_func_t));

    node->base.type = NODE_FUNC;
    node->ret_type = ctype->type;
    node_def_val_t* args = NULL;

    while (true) {
        t = get_next_token(ctx);
        if (t->type == TK_R_R_PAR) {
            node->args = args;
            break;
        }

        NOT_YET_IMPLEMETED;
    }

    node->proc = parse(ctx);

    return (node_t*)node;
}

static node_t* parse_ret(context_t* ctx)
{
    token_t* t = get_front_token(ctx);

    node_ret_t* ret = calloc(1, sizeof(node_ret_t));
    ret->base.type = NODE_RET;

    if (t->type == TK_SEM) {
        ret->regexp = NULL;
    }
    else {
        ret->regexp = parse(ctx);
    }

    return (node_t*)ret;
}

static node_t* parse_id(context_t* ctx, token_id_t* id)
{
    token_t* t = get_front_token(ctx);
    if (t->type == TK_SEM) {
        node_assign_t* node = calloc(1, sizeof(node_assign_t));
        node->base.type = NODE_ASSIGN;
        node->id = id->id;
        node->right = NULL;
        return (node_t*)node;
    }
    else if (t->type == TK_ASSIGN) {
        node_assign_t* node = calloc(1, sizeof(node_assign_t));
        node->base.type = NODE_ASSIGN;
        node->id = id->id;
        get_next_token(ctx);
        node->right = parse(ctx);
        return (node_t*)node;
    }
    else if (t->type == TK_UNARY) {
        node_unary_t* node = calloc(1, sizeof(node_unary_t));
        node->base.type = NODE_UNARY;
        node->id = id->id;
        node->type = ((token_unary_t*)t)->type;
        get_next_token(ctx);
        return (node_t*)node;
    }

    NOT_YET_IMPLEMETED;
    return NULL;
}

static node_t* _parse(context_t* ctx, node_t* node)
{
    token_t* t1 = get_front_token(ctx);

    if (t1->type == TK_SEM) {
        return generate_node(NODE_SEM);
    }

    if (t1->type == TK_EOF) {
        get_next_token(ctx);
        return generate_node(NODE_EOF);
    }

    if (t1->type == TK_R_PAR) {
        get_next_token(ctx);
        return generate_node(NODE_R_PAR);
    }

    if (t1->type == TK_NUM) {
        assert(!node && "wtf");
        return parse_val((token_number_t*)get_next_token(ctx));
    }
    else if (t1->type == TK_OPR) {
        assert(node && "wtf");
        return parse_op(ctx, node, (token_opr_t*)get_next_token(ctx));
    }
    else if (t1->type == TK_L_PAR) {
        get_next_token(ctx);
        return parse_parenthe(ctx);
    }
    else if (t1->type == TK_TYPE) {
        token_ctype_t* type = (token_ctype_t*)get_next_token(ctx);

        token_t* t2 = get_next_token(ctx);
        assert(t2->type == TK_ID);
        token_id_t* id = (token_id_t*)t2;

        t2 = get_front_token(ctx);
        if (t2->type == TK_L_R_PAR) {
            return parse_def_func(ctx, type, id);
        }
        else {
            return parse_def_val(ctx, type, id);
        }
    }
    else if (t1->type == TK_RET) {
        get_next_token(ctx);
        return parse_ret(ctx);
    }
    else if (t1->type == TK_ID) {
        token_id_t* id = (token_id_t*)t1;
        get_next_token(ctx);
        return parse_id(ctx, id);
    }

    NOT_YET_IMPLEMETED;
}

static node_t* eof_node = NULL;
node_t* parse(context_t* ctx)
{
    if (eof_node) {
        return eof_node;
    }

    node_t* node = NULL;
    while (true) {
        node_t* next = _parse(ctx, node);

        if (next->type == NODE_EOF) {
            eof_node = next;
        }

        if (next->type == NODE_SEM || next->type == NODE_EOF) {
            return node ? node : next;
        }

        node = next;
    }

    return node;
}
