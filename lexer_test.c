#include "lexer.h"
#include "test.h"

static token_t eof_token = {
    .type = TK_EOF,
};
static token_t sem_token = {
    .type = TK_SEM,
};
static token_t l_t = {
    .type = TK_L_PAR,
};
static token_t r_t = {
    .type = TK_R_PAR,
};

#define LEXER_TEST(name) Ensure(lexer_##name)
#define LEXER_ADDTEST(name) add_test(suite, lexer_##name)

LEXER_TEST(null)
{
    parse_context_t context = {
        .text = "",
    };

    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

LEXER_TEST(sem_null)
{
    parse_context_t context = {
        .text = ";",
    };

    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&sem_token, sizeof sem_token));
    assert_that(get_next_token(&context),
                is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

LEXER_TEST(number_0)
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

LEXER_TEST(op_0)
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

LEXER_TEST(parentheses_0)
{
    parse_context_t context = {
        .text = "{}",
    };



    assert_that(get_next_token(&context), is_equal_to_contents_of(&l_t, sizeof l_t));
    assert_that(get_next_token(&context), is_equal_to_contents_of(&r_t, sizeof r_t));
}

TestSuite* lexer_tests()
{
    TestSuite* suite = create_test_suite();

    LEXER_ADDTEST(null);
    LEXER_ADDTEST(sem_null);
    LEXER_ADDTEST(number_0);
    LEXER_ADDTEST(op_0);
    LEXER_ADDTEST(parentheses_0);

    return suite;
}
