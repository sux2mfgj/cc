#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct _context_t {
    bool (*read_line)(struct _context_t* ctx);
    char* buffer;
    char* _buffer;
    char* _buf_base;
    bool reach_to_end;
    uint64_t line_number;
} context_t;

context_t* init_context(int fd);
