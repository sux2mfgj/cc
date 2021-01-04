#include <assert.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "util.h"

static node_t* parse_root(context_t* ctx)
{
    token_t* t = get_next_token(ctx);

    switch (t->type) {
        case TK_EOF: {
            return generate_simple_node(NODE_EOF);
        }
        case TK_TYPE: {
            NOT_YET_IMPLEMETED;
        }
        default:
            assert(false && "wtf!");
    }
}

node_t* parse(context_t* ctx)
{
    return parse_root(ctx);
}

