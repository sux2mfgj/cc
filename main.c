#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include "context.h"
//#include "debug.h"
//#include "ir.h"
#include "lexer.h"
//#include "parser.h"
#include <errno.h>

//node_t* node_head;

static bool read_from_stdin(char* buf)
{
    char* result = fgets(buf, 81, stdin);
    if (!result && errno) {
        warn("failed to read fron stdin");
    }

    return !!result & !errno;
}

int main(int argc, char** argv)
{
    //debug_enable();
    context_t* ctx = init_context(0);

    //node_t* node = node_head = parse(&context);
    //while (node && node->type != NODE_EOF) {
    //    node->next = parse(&context);
    //    node = node->next;
    //}

    //gen_ir(node_head, stdout);
}
