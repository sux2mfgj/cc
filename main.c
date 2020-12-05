#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_YET_IMPLEMETED errx(1, "not yet implemented at line %d", __LINE__)

typedef enum {
    TK_OPR,
    TK_NUM,
    TK_EOF,
} token_type_t;

typedef struct _token_base {
    struct _token_base* next;
    token_type_t type;
} token_t;

typedef struct {
    token_t base;
    union {
        uint64_t uint64;
        // TODO
    };
} token_number_t;

typedef enum { OP_PLUS, OP_MINUS } operator_type_t;

typedef struct {
    token_t base;
    operator_type_t type;
} token_opr_t;

token_t* token_head;

typedef struct {
    char* text;
} parse_context_t;

static void skip_separators(parse_context_t* context)
{
    while (*context->text == ' ' || *context->text == '\n' ||
           *context->text == '\t') {
        context->text++;
    }
}

bool parse_value(parse_context_t* context, uint64_t* result, char** word)
{
    char* start = context->text;

    int len = 0;
    // decimal
    while (*context->text != ' ' && *context->text != '\n' &&
           *context->text != '\t' && *context->text) {
        len++;
        context->text++;
    }

    *word = calloc(1, len);
    memcpy(*word, start, len);

    //*text = '\0';

    long value = strtol(*word, NULL, 10);
    if (errno) {
        warn("detect invalid value (cannot parse by strtol)\n");
        ;
        return false;
    }

    *result = value;
    return true;
}

token_t* lexical_analyze(parse_context_t* context)
{
    skip_separators(context);

    // parse number
    if ('0' <= *context->text && *context->text <= '9') {
        uint64_t value;
        char* word;
        if (!parse_value(context, &value, &word)) {
            return NULL;
        }

        token_number_t* number_token = calloc(1, sizeof(token_number_t));
        number_token->base.next = NULL;
        number_token->base.type = TK_NUM;
        number_token->uint64 = value;

        return (token_t*)number_token;
    }

    if (*context->text == '+') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.next = NULL;
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_PLUS;

        return (token_t*)opr_token;
    }

    if (*context->text == '-') {
        context->text++;
        token_opr_t* opr_token = calloc(1, sizeof(token_opr_t));
        opr_token->base.next = NULL;
        opr_token->base.type = TK_OPR;
        opr_token->type = OP_MINUS;

        return (token_t*)opr_token;
    }

    // reach to end of line
    if (*context->text == 0) {
        token_t* eof_token = calloc(1, sizeof(token_t));
        eof_token->next = NULL;
        eof_token->type = TK_EOF;
        return eof_token;
    }

    return NULL;
}

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

    token_t* cur_token;
    do {
        token_t* next = lexical_analyze(&context);
        if (!next) {
            errx(1, "invalid token found");
        }

        if (token_head == NULL) {
            token_head = next;
            cur_token = next;
        }
        else {
            cur_token->next = next;
            cur_token = next;
        }
    } while (cur_token->type != TK_EOF);

    const char* output_base_head =
        "\
.globl main\n\
.text\n\
main:\n";
    const char* output_base_end = "ret\n";

    printf("%s", output_base_head);

    uint64_t result;
    if (!token_head) {
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
