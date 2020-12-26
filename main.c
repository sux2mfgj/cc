#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include "ir.h"
#include "lexer.h"
#include "parser.h"
#include "debug.h"

node_t* node_head;

void readline(char* buffer)
{
    char* result = fgets(buffer, 81, stdin);
    if (!result) {
        errx(1, "failed to read from stdin");
    }
}

static char* read_line(void)
{
    static char buffer[81];
    readline(buffer);

    return buffer;
}

int main(int argc, char** argv)
{

    parse_context_t context = {
        .read_line = read_line,
    };

    debug_enable();

    node_t* node = node_head = parse(&context);
    while (node && node->type != NODE_EOF) {
        node->next = parse(&context);
        node = node->next;
    }

    gen_ir(node_head, stdout);
}
