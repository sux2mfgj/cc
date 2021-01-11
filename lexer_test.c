#include "lexer.h"
#include "test.h"

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
    assert_that(t->type, is_equal_to(TK_SEM));
    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_EOF));
}

LEXER_TEST(number_0)
{
    PREPARE_CTX("12;");

    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_NUM));
    assert_that(((token_number_t*)t)->uint64, is_equal_to(12));
    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_SEM));
    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_EOF));
}

LEXER_TEST(op_0)
{
    PREPARE_CTX("1 + 1;");

    token_t* t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_NUM));
    assert_that(((token_number_t*)t)->uint64, is_equal_to(1));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_OPR));
    assert_that(((token_opr_t*)t)->type, is_equal_to(OP_PLUS));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_NUM));
    assert_that(((token_number_t*)t)->uint64, is_equal_to(1));

    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_SEM));
    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_EOF));
}

LEXER_TEST(op_1)
{
    PREPARE_CTX("1 - 2;");

    token_t* t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_NUM));
    assert_that(((token_number_t*)t)->uint64, is_equal_to(1));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_OPR));
    assert_that(((token_opr_t*)t)->type, is_equal_to(OP_MINUS));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_NUM));
    assert_that(((token_number_t*)t)->uint64, is_equal_to(2));

    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_SEM));
    t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_EOF));
}

LEXER_TEST(parentheses_0)
{
    PREPARE_CTX("{}");
    token_t* t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_L_PAR));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_R_PAR));
}

LEXER_TEST(front_0)
{
    PREPARE_CTX("{1 + 2;}");

    token_t* t;
    t = get_front_token(ctx);

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_L_PAR));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_NUM));
    assert_that(((token_number_t*)t)->uint64, is_equal_to(1));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_OPR));
    assert_that(((token_opr_t*)t)->type, is_equal_to(OP_PLUS));

    t = get_front_token(ctx);
    assert_that(t->type, is_equal_to(TK_NUM));
    assert_that(((token_number_t*)t)->uint64, is_equal_to(2));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_NUM));
    assert_that(((token_number_t*)t)->uint64, is_equal_to(2));
    // TODO
}

LEXER_TEST(uint64_t_0)
{
    PREPARE_CTX("uint64_t");

    token_t* t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_TYPE));
    assert_that(((token_ctype_t*)t)->type, is_equal_to(TYPE_UINT64));
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

LEXER_TEST(if_0)
{
    PREPARE_CTX("if");
    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_IF));
}

LEXER_TEST(else_0)
{
    PREPARE_CTX("else");
    token_t* t = get_next_token(ctx);
    assert_that(t, is_non_null);
    assert_that(t->type, is_equal_to(TK_ELSE));
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

LEXER_TEST(macro_0)
{
    PREPARE_CTX("#include <stdio.h>");

    token_t* t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_SHARP));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_ID));

    t = get_next_token(ctx);
    token_opr_t* opr = (token_opr_t*)t;
    assert_that(t->type, is_equal_to(TK_OPR));
    assert_that(opr->type, is_equal_to(OP_LT));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_ID));

    t = get_next_token(ctx);
    opr = (token_opr_t*)t;
    assert_that(t->type, is_equal_to(TK_OPR));
    assert_that(opr->type, is_equal_to(OP_GT));
}

LEXER_TEST(macro_1)
{
    PREPARE_CTX("#include \"stdio.h\"");

    token_t* t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_SHARP));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_ID));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_DQUOTE));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_ID));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_DQUOTE));
}

LEXER_TEST(func_0)
{
    PREPARE_CTX(
        "int main()"
        "{"
        "}");

    token_t* t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_TYPE));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_ID));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_L_R_PAR));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_R_R_PAR));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_L_PAR));

    t = get_next_token(ctx);
    assert_that(t->type, is_equal_to(TK_R_PAR));
}

LEXER_TEST(func_1)
{
    PREPARE_CTX(
        "int main()"
        "{"
        "}");

    token_t* pop_tnode(context_t* ctx);
    token_t* t = pop_tnode(ctx);
    assert_that(t->type, is_equal_to(TK_TYPE));

    t = pop_tnode(ctx);
    assert_that(t->type, is_equal_to(TK_ID));

    t = pop_tnode(ctx);
    assert_that(t->type, is_equal_to(TK_L_R_PAR));

    t = pop_tnode(ctx);
    assert_that(t->type, is_equal_to(TK_R_R_PAR));

    t = pop_tnode(ctx);
    assert_that(t->type, is_equal_to(TK_L_PAR));

    t = pop_tnode(ctx);
    assert_that(t->type, is_equal_to(TK_R_PAR));
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
    LEXER_ADDTEST(if_0);

    LEXER_ADDTEST(macro_0);
    LEXER_ADDTEST(macro_1);
    LEXER_ADDTEST(func_0);
    LEXER_ADDTEST(func_1);

    return suite;
}
