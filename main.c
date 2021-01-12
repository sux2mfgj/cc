#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

#include "context.h"
#include "ir.h"
#include "lexer.h"
#include "node.h"
#include "parser.h"

int main(int argc, char** argv)
{
    context_t* ctx = init_context(0);

    node_t *node_head = NULL, *node;

    while (true) {
        node_t* tmp = parse(ctx);
        if (!node_head) {
            node_head = tmp;
            node = tmp;
        }
        else {
            node->next = tmp;
            node = node->next;
        }

        if (tmp->type == NODE_EOF) {
            break;
        }
    }

    gen_ir(node_head, stdout);
}
