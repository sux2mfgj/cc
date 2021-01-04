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
static token_number_t num_1_token = {
    .base =
        {
            .type = TK_NUM,
        },
    .uint64 = 1,
};
static token_number_t num_2_token = {
    .base =
        {
            .type = TK_NUM,
        },
    .uint64 = 2,
};


LEXER_TEST(null)
{
    PREPARE_CTX("");

    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_EOF));
}

LEXER_TEST(sem_null)
{
    PREPARE_CTX(";");
    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t, is_equal_to_contents_of(&sem_token, sizeof sem_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

LEXER_TEST(number_0)
{
    PREPARE_CTX("12;");

    token_number_t num_12_token = {
        .base =
            {
                .type = TK_NUM,
            },
        .uint64 = 12,
    };

    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t, is_equal_to_contents_of(&num_12_token, sizeof num_12_token));
    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t, is_equal_to_contents_of(&sem_token, sizeof sem_token));
    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t, is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

LEXER_TEST(op_0)
{
    PREPARE_CTX("1 + 1;");

    token_t* t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&num_1_token, sizeof num_1_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&plus_token, sizeof plus_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&num_1_token, sizeof num_1_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&sem_token, sizeof sem_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

LEXER_TEST(op_1)
{
    PREPARE_CTX("1 - 2;");

    token_t* t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&num_1_token, sizeof num_1_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&minus_op, sizeof minus_op));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&num_2_token, sizeof num_2_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&sem_token, sizeof sem_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&eof_token, sizeof eof_token));
}

LEXER_TEST(parentheses_0)
{
    PREPARE_CTX("{}");
    token_t* t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&l_t, sizeof l_t));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&r_t, sizeof r_t));
}

LEXER_TEST(front_0)
{
    PREPARE_CTX("{1 + 2;}");

    token_t* t;
    t = get_front_token(ctx);
    assert_that(t, is_equal_to_contents_of(&l_t, sizeof l_t));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&l_t, sizeof l_t));

    token_number_t num_1_token = {
        .base =
            {
                .type = TK_NUM,
            },
        .uint64 = 1,
    };

    token_number_t num_2_token = {
        .base =
            {
                .type = TK_NUM,
            },
        .uint64 = 2,
    };

    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&num_1_token, sizeof num_1_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&plus_token, sizeof plus_token));
    t = get_front_token(ctx);
    assert_that(t, is_equal_to_contents_of(&num_2_token, sizeof num_2_token));
    t = get_next_token(ctx);
    assert_that(t, is_equal_to_contents_of(&num_2_token, sizeof num_2_token));
    // TODO
}

LEXER_TEST(uint64_t_0)
{
    PREPARE_CTX("uint64_t");

    token_ctype_t t = {
        .base =
            {
                .type = TK_TYPE,
            },
        .type = TYPE_UINT64,
    };

    token_t* tt = get_next_token(ctx);
    assert_that(tt, is_equal_to_contents_of(&t, sizeof t));
}

LEXER_TEST(uint64_t_1)
{
    PREPARE_CTX("uint64_t a;");

    token_t* t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_TYPE));
    token_ctype_t* ctype = (token_ctype_t*)t;
    assert_that(ctype->type, is_equal_to(TYPE_UINT64));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_ID));
    token_id_t* id = (token_id_t*)t;
    assert_that(id->id, is_equal_to_contents_of("a", strlen("a") + 1));
    // TODO
}

LEXER_TEST(multi_node)
{
    PREPARE_CTX(
        "{"
        "   uint64_t a;"
        "}");

    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_L_PAR));

    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_TYPE));

    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_ID));

    assert_that(((token_id_t*)t)->id,
                is_equal_to_contents_of("a", strlen("a") + 1));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_SEM));

    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_R_PAR));
}

LEXER_TEST(return_0)
{
    PREPARE_CTX("return 1;");

    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_RET));
}

LEXER_TEST(void_0)
{
    PREPARE_CTX("void");
    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_TYPE));
    assert_that(((token_ctype_t*)t)->type, is_equal_to(TYPE_VOID));
}

LEXER_TEST(round_par_0)
{
    PREPARE_CTX("()");

    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_L_R_PAR));
    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_R_R_PAR));
}

LEXER_TEST(round_par_1)
{
    PREPARE_CTX("main()");

    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_ID));

    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_L_R_PAR));
    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_R_R_PAR));
}

TestSuite* lexer_tests()
{
    TestSuite* suite = create_test_suite();

    LEXER_ADDTEST(null);
    LEXER_ADDTEST(sem_null);
    LEXER_ADDTEST(number_0);
    LEXER_ADDTEST(op_0);
    LEXER_ADDTEST(op_1);
    LEXER_ADDTEST(parentheses_0);
    LEXER_ADDTEST(front_0);
    LEXER_ADDTEST(uint64_t_0);
    LEXER_ADDTEST(uint64_t_1);
    LEXER_ADDTEST(multi_node);
    LEXER_ADDTEST(return_0);
    LEXER_ADDTEST(void_0);
    LEXER_ADDTEST(round_par_0);
    LEXER_ADDTEST(round_par_1);

    return suite;
}
