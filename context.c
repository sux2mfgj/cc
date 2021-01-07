#include "context.h"
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

static context_t ctx;
static int read_fd;

#define READ_SIZE 80
#define BUF_SIZE (READ_SIZE + 1)

static bool read_line_from_fd(context_t* ctx)
{
    char *start = ctx->_buffer;

    while(true)
    {
        if(ctx->_buffer == '\n')
        {
            break;
        }

        ctx->_buffer++;
    }
    ctx->_buffer = '\0';
    ctx->_buffer++;

    size_t len = ctx->_buffer - start;
    memcopy(ctx->buffer, start, ctx->_buffer - start);

    memcopy(start, ctx->_buffer, READ_SIZE - len);
    if(!ctx->reach_to_end)
    {
        ssize_t size = read(read_fd, ctx->_buffer, len);
        if(size < len)
        {
            ctx->reach_to_end = true;
        }
    }
}

context_t* init_context(int fd)
{
    read_fd = fd;
    ctx.read_line = read_from_fd;
    ctx.buffer = calloc(BUF_SIZE, sizeof(char));
    ctx._buffer = calloc(BUF_SIZE, sizeof(char));
    ctx._buf_base = ctx._buffer;
    ctx.reach_to_end = false;
    ctx.line_number = 1;

    return &ctx;
}
