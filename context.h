#pragma once

#include <stdbool.h>

typedef struct _context_t {
    bool (*read_line)(struct _context_t* ctx);
    char* buffer;
    char* _buffer;
    int buf_index;
    uint64_t line_number;
} context_t;

context_t* init_context(int fd);
