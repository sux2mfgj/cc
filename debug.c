#ifndef NDEBUG
#include "debug.h"
#include <stdio.h>
#include <stdbool.h>

static bool enable = false;
void debug_enable(void)
{
    enable = true;
}

void debug(const char* format, ...)
{
    if(!enable)
    {
        return;
    }

    va_list ap;

    va_start(ap, format);
    fprintf(stderr, format, ap);
    va_end(ap);
}


#endif
