#pragma once

#include <stdbool.h>

typedef struct _context_t{
    bool (*read_line)(struct _context_t *ctx);
    char* buffer;
} context_t;

context_t* init_context(int fd);
