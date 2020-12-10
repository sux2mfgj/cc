#include "lexer.h"
#include "test.h"

static token_t eof_token = {
    .type = TK_EOF,
};
static token_t sem_token = {
    .type = TK_SEM,
};

Ensure(null)
{
    parse_context_t context = {
        .text = "",
    };

    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

Ensure(sem_null)
{
    parse_context_t context = {
        .text = ";",
    };

    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&sem_token, sizeof sem_token));
    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

Ensure(number_0)
{
    parse_context_t context = {
        .text = "12;",
    };

    token_number_t num_12_token = {
        .base =
            {
                .type = TK_NUM,
            },
        .uint64 = 12,
    };

    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&num_12_token, sizeof num_12_token));
    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&sem_token, sizeof sem_token));
    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

Ensure(op_0)
{
    parse_context_t context = {
        .text = "1 + 1;",
    };

    token_opr_t plus_token = {
        .base =
            {
                .type = TK_OPR,
            },
        .type = OP_PLUS,
    };

    token_number_t num_1_token = {
        .base =
            {
                .type = TK_NUM,
            },
        .uint64 = 1,
    };

    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&num_1_token, sizeof num_1_token));
    assert_that(get_next_token(&context),
            is_equal_to_contents_of(&plus_token, sizeof plus_token));
    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&num_1_token, sizeof num_1_token));
    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&sem_token, sizeof sem_token));
    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

TestSuite* lexer_tests()
{
    TestSuite* suite = create_test_suite();

    add_test(suite, null);
    add_test(suite, sem_null);
    add_test(suite, number_0);
    add_test(suite, op_0);

    return suite;
}
