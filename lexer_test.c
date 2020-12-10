#include "test.h"
#include "lexer.h"

Ensure(null_test) {
    parse_context_t context = {
        .text = "",
    };

    token_t eof_token = {
        .next = NULL,
        .type = TK_EOF,
    };
    assert_that(get_next_token (&context), is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

TestSuite* lexer_tests() {
    TestSuite *suite = create_test_suite();

    add_test(suite, null_test);
    return suite;
}
