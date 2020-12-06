#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include "ir.h"
#include "lexer.h"
#include "parser.h"

node_t* node_head;

void readline(char* buffer)
{
    char* result = fgets(buffer, 81, stdin);
    if (!result) {
        errx(1, "failed to read from stdin");
    }
}

/*
static void gen_asm(void)
{
    const char* output_base_head =
        "\
.globl main\n\
.text\n\
main:\n";
    const char* output_base_end = "ret\n";

    printf("%s", output_base_head);

    uint64_t result;
    if (!node_head) {
        result = 0;
    }
    else {
        if (token_head->type != TK_NUM) {
            errx(1, "invalid operator found");
        }
        token_number_t* result_token = (token_number_t*)token_head;
        result = result_token->uint64;
    }

    printf("mov $%ld, %%rax\n", result);
    printf("%s", output_base_end);
}
*/

int main(int argc, char** argv)
{
    char buffer[81] = {0};
    readline(buffer);

    parse_context_t context = {
        .text = buffer,
    };

    node_t* node = node_head = parse(&context);
    while (node) {
        node->next = parse(&context);
        node = node->next;
    }

    gen_ir(node_head, stdout);

    // gen_asm();
}
