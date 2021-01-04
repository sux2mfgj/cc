#include <assert.h>
#include <err.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "util.h"

// static node_t* parse_func_def(context_t* ctx, token_ctype_t* type,
// token_id_t* id) { }

static node_t* parse_var_def(context_t* ctx,
                             token_ctype_t* type,
                             token_id_t* id)
{
    token_t* t = get_next_token(ctx);

    node_def_val_t* node = calloc(1, sizeof(node_def_val_t));
    node->base.type = NODE_DEF_VAL;
    node->type = type->type;
    node->id = id->id;

    if (t->type == TK_SEM) {
        return (node_t*)node;
    }
    else if (t->type == TK_ASSIGN) {
        NOT_YET_IMPLEMETED;
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
                NOT_YET_IMPLEMETED;
                // return parse_func_def(ctx, ctype, id);
            }
            // variable definition
            else {
                result = parse_var_def(ctx, ctype, id);
            }
            break;
        }
        default:
            assert(false && "wtf!");
    }

    return result;
}

node_t* parse(context_t* ctx)
{
    return parse_root(ctx);
}

