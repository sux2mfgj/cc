#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include "context.h"
//#include "debug.h"
#include "ir.h"
#include "lexer.h"
#include "parser.h"
#include <errno.h>

int main(int argc, char** argv)
{
    //debug_enable();
    context_t* ctx = init_context(0);

    node_t* node_head;
    node_t* node = node_head = parse(ctx);
    while (node && node->type != NODE_EOF) {
        node->next = parse(ctx);
        node = node->next;
    }

    gen_ir(node_head, stdout);
}
