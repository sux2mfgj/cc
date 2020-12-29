#include "context.h"
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

static context_t ctx;
static int read_fd;

#define READ_SIZE 80
#define BUF_SIZE (READ_SIZE + 1)

static bool read_from_fd(context_t *ctx)
{
    ssize_t size = read(read_fd, ctx->buffer, READ_SIZE);

    if (size < 0) {
        warn("failed the read");
        return false;
    }

    if (size == 0) {
        return false;
    }

    ctx->buffer[size] = '\0';
    return true;
}

context_t* init_context(int fd)
{
    read_fd = fd;
    ctx.read_line = read_from_fd;
    ctx.buffer = calloc(BUF_SIZE, sizeof(char));

    return &ctx;
}
