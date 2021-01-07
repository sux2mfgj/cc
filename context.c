#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "context.h"

static context_t ctx;
static int read_fd;

#define READ_SIZE 80
#define BUF_SIZE (READ_SIZE + 1)

static bool read_line_from_fd(context_t* ctx)
{
    char* start = ctx->_buffer;

    if (*start == '\0') {
        return false;
    }

    while (true) {
        if (*ctx->_buffer == '\n') {
            break;
        }
        if (*ctx->_buffer == '\0') {
            break;
        }

        ctx->_buffer++;
    }
    *ctx->_buffer = '\0';
    ctx->_buffer++;

    size_t len = ctx->_buffer - start;
    memcpy(ctx->buffer, start, ctx->_buffer - start);

    memcpy(start, ctx->_buffer, READ_SIZE - len);

    return true;
}

context_t* init_context(int fd)
{
    read_fd = fd;
    ctx.read_line = read_line_from_fd;
    ctx.buffer = calloc(BUF_SIZE, sizeof(char));
    ctx._buffer = calloc(BUF_SIZE, sizeof(char));
    ctx._buf_base = ctx._buffer;
    ctx.reach_to_end = false;
    ctx.line_number = 1;

    ssize_t size = read(read_fd, ctx._buffer, READ_SIZE);
    if (size < READ_SIZE) {
        ctx.reach_to_end = true;
    }

    return &ctx;
}
