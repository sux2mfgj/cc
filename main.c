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

int main(int argc, char** argv)
{
    char buffer[81] = {0};
    readline(buffer);

    parse_context_t context = {
        .text = buffer,
    };

    debug_enable();

    node_t* node = node_head = parse(&context);
    while (node) {
        node->next = parse(&context);
        node = node->next;
    }

    gen_ir(node_head, stdout);
}
