#pragma once

#include <stdint.h>

#include "context.h"
#include "token.h"

token_t* get_next_token(context_t* ctx);
token_t* get_front_token(context_t* ctx);
