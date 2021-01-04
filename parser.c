#include <assert.h>
#include <err.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "util.h"

static node_t* parse_node_in_par(context_t* ctx)
{
    token_t* t = get_front_token(ctx);
    assert(t->type != TK_R_PAR);

    switch (t->type) {
        case TK_TYPE:
        case TK_ID:
        case TK_NUM:
        case TK_RET:
        case TK_L_R_PAR:
            NOT_YET_IMPLEMETED;
        default:
            errx(EXIT_FAILURE, "invalid token found: %d", __LINE__);
    }

    NOT_YET_IMPLEMETED;
}

static node_t* parse_parenthes(context_t* ctx)
{
    token_t* t = get_next_token(ctx);
    assert(t->type == TK_L_PAR);

    node_par_t* par = calloc(1, sizeof(node_func_t));
    par->base.type = NODE_PAR;

    node_t* head = NULL;
    node_t* cur;

    while (true) {
        token_t* t = get_front_token(ctx);
        if (t->type == TK_R_PAR) {
            get_next_token(ctx);
            break;
        }

        node_t* context = parse_node_in_par(ctx);
        assert(context->type != NODE_EOF);

        if (!head) {
            head = context;
            cur = context;
        }
        else {
            cur->next = context;
            cur = cur->next;
        }
    }

    par->contents = head;

    return (node_t*)par;
}

static node_t* parse_func_def(context_t* ctx,
                              token_ctype_t* type,
                              token_id_t* id)
{
    token_t* t = get_next_token(ctx);
    assert(t->type == TK_L_R_PAR);

    node_func_t* node = calloc(1, sizeof(node_func_t));
    node->base.type = NODE_DEF_FUNC;
    node->ret_type = type->type;
    node->id = id->id;

    node_def_val_t* args = NULL;

    while (true) {
        t = get_front_token(ctx);
        if (t->type == TK_R_R_PAR) {
            get_next_token(ctx);
            node->args = args;
            break;
        }

        NOT_YET_IMPLEMETED;
    }

    node_t* proc = parse_parenthes(ctx);
    node->proc = proc;

    return (node_t*)node;
}

static node_t* parse_single_node(context_t* ctx)
{
    token_t* t = get_next_token(ctx);
    switch (t->type) {
        case TK_NUM: {
            node_val_t* n = calloc(1, sizeof(node_val_t));
            n->base.type = NODE_VAL;
            n->uint64 = ((token_number_t*)t)->uint64;
            return (node_t*)n;
        }
        case TK_ID:
        case TK_L_R_PAR:
            NOT_YET_IMPLEMETED;
        default:
            errx(EXIT_FAILURE, "invalid token found: %d", __LINE__);
    }
}

static node_t* parse_expr(context_t* ctx);

static node_t* _parse_expr(context_t* ctx, node_t* left)
{
    token_t* t = get_front_token(ctx);

    if (t->type == TK_OPR) {
        token_opr_t* opr = (token_opr_t*)t;
        get_next_token(ctx);
        node_op_t* n = calloc(1, sizeof(node_op_t));
        n->base.type = NODE_OP;
        n->opr = opr->type;
        switch (opr->type) {
            case OP_PLUS:
            case OP_MINUS: {
                node_t* right = parse_expr(ctx);
                n->left = left;
                n->right = right;
                break;
            }
            case OP_MUL:
            case OP_DIV: {
                n->left = left;
                n->right = (node_t*)parse_single_node(ctx);
                break;
            }
            default:
                NOT_YET_IMPLEMETED;
        }

        return (node_t*)n;
    }
    else {
        return left;
    }
}

static node_t* parse_expr(context_t* ctx)
{
    node_t* left = parse_single_node(ctx);

    while (true) {
        token_t* t = get_front_token(ctx);
        if (t->type != TK_OPR) {
            return left;
        }
        left = _parse_expr(ctx, left);
    }
}

static void skip_semicolon(context_t* ctx)
{
    token_t* t = get_next_token(ctx);
    if (t->type != TK_SEM) {
        errx(EXIT_FAILURE, "invalid token found: %d", __LINE__);
    }
}

static node_t* parse_var_def(context_t* ctx,
                             token_ctype_t* type,
                             token_id_t* id)
{
    token_t* t = get_next_token(ctx);

    node_def_val_t* node = calloc(1, sizeof(node_def_val_t));
    node->base.type = NODE_DEF_VAR;
    node->type = type->type;
    node->id = id->id;

    if (t->type == TK_SEM) {
        return (node_t*)node;
    }
    else if (t->type == TK_ASSIGN) {
        node->init = parse_expr(ctx);
        skip_semicolon(ctx);
        return (node_t*)node;
    }
    else {
        errx(EXIT_FAILURE, "invalid input");
    }
}

static node_t* parse_root(context_t* ctx)
{
    token_t* t = get_next_token(ctx);

    node_t* result;
    switch (t->type) {
        case TK_EOF: {
            result = generate_simple_node(NODE_EOF);
            break;
        }
        case TK_TYPE: {
            token_ctype_t* ctype = (token_ctype_t*)t;

            t = get_next_token(ctx);
            assert(t->type == TK_ID);
            token_id_t* id = (token_id_t*)t;

            t = get_front_token(ctx);
            // function definition
            if (t->type == TK_L_R_PAR) {
                return parse_func_def(ctx, ctype, id);
            }
            // variable definition
            else {
                result = parse_var_def(ctx, ctype, id);
            }
            break;
        }
        default:
            errx(EXIT_FAILURE, "invalid token found: %d", __LINE__);
    }

    return result;
}

node_t* parse(context_t* ctx)
{
    return parse_root(ctx);
}
