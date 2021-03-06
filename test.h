#pragma once
#include <cgreen/cgreen.h>

#include <unistd.h>
#include "lexer.h"

#define PREPARE_CTX(text)                \
    int fds[2];                          \
    int status = pipe(fds);              \
    assert_that(status, is_equal_to(0)); \
    write(fds[1], text, sizeof text);    \
    close(fds[1]);                       \
    context_t* ctx = init_context(fds[0]);

#define LEXER_TEST(name) Ensure(lexer_##name)
#define LEXER_ADDTEST(name) add_test(suite, lexer_##name)

#define PARSER_TEST(name) Ensure(parser_##name)
#define PARSER_ADDTEST(name) add_test(suite, parser_##name)

TestSuite* lexer_tests(void);
TestSuite* parser_tests(void);
