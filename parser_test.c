#include "parser.h"
#include "test.h"

#define PARSER_TEST(name) Ensure(parser_##name)
#define PARSER_ADDTEST(name) add_test(suite, parser_##name)

PARSER_TEST(null)
{
    parse_context_t context = {
        .text = "",
    };

    assert_that(parse(&context),
                is_equal_to_contents_of(&eof_node, sizeof eof_node));
}

PARSER_TEST(sem)
{
    parse_context_t context = {
        .text = ";",
    };

    assert_that(parse(&context), is_equal_to_contents_of(&sem_node, sizeof sem_node));
}

PARSER_TEST(num_0)
{
    parse_context_t context = {
        .text = "0;",
    };

    node_val_t node = {
        .base =
            {
                .type = NODE_VAL,
            },
    };

    token_number_t token = {
        .base =
            {
                .type = TK_NUM,
            },
        .uint64 = 0,
    };

    node_t* n = parse(&context);
    assert_that(n->type, is_equal_to(NODE_VAL));
    assert_that(((node_val_t*)n)->token,
                is_equal_to_contents_of(&token, sizeof token));
    n = parse(&context);
    assert_that(n->type , is_equal_to(NODE_SEM));
    get_next_token(&context);
    n = parse(&context);
    assert_that(n, is_equal_to_contents_of(&eof_node, sizeof eof_node));
}

PARSER_TEST(op_0)
{
    parse_context_t context = {
        .text = "1 + 12;",
    };

    token_number_t token_1 = {
        .base =
            {
                .type = TK_NUM,
            },
        .uint64 = 1,
    };
    token_opr_t op = {
        .base =
            {
                .type = TK_OPR,
            },
        .type = OP_PLUS,
    };

    token_number_t token_12 = {
        .base =
            {
                .type = TK_NUM,
            },
        .uint64 = 12,
    };

    node_t* result = parse(&context);
    assert_that(result, is_non_null);
    assert_that(result->type, is_equal_to(NODE_OP));
    node_op_t* op_node = (node_op_t*)result;
    assert_that(op_node->token, is_equal_to_contents_of(&op, sizeof op));
    assert_that(op_node->left, is_non_null);
    assert_that(op_node->right, is_non_null);
    // TODO check the values
}

PARSER_TEST(op_1)
{
    parse_context_t context = {
        .text = "1 - 2 * 3;",
    };

    node_t* n1 = parse(&context);
    assert_that(n1, is_non_null);
    assert_that(n1->type, is_equal_to(NODE_OP));
    node_op_t* n1_op = (node_op_t*)n1;

    assert_that(n1_op->token, is_equal_to_contents_of(&minus_op, sizeof minus_op));
    assert_that(n1_op->left, is_non_null);
    assert_that(n1_op->left->type, is_equal_to(NODE_VAL));

    assert_that(n1_op->right, is_non_null);
    // TODO
}

PARSER_TEST(op_2)
{
    parse_context_t context = {
        .text = "1 * 2 - 3 / 4;",
    };

    node_t* n1 = parse(&context);
    assert_that(n1, is_non_null);
    assert_that(n1->type, is_equal_to(NODE_OP));

    node_op_t* no_op = (node_op_t*)n1;

    assert_that(no_op->token, is_equal_to_contents_of(&minus_op, sizeof minus_op));

    assert_that(no_op->left, is_non_null);
    assert_that(no_op->left->type, is_equal_to(NODE_OP));

    node_op_t* left_op = (node_op_t*)no_op->left;
    assert_that(left_op->token, is_equal_to_contents_of(&mul_op, sizeof mul_op));

    assert_that(no_op->right, is_non_null);
    assert_that(no_op->right->type, is_equal_to(NODE_OP));
    node_op_t* right_op = (node_op_t*)no_op->right;
    assert_that(right_op->token, is_equal_to_contents_of(&div_op, sizeof div_op));
}

PARSER_TEST(parentheses_0)
{
    parse_context_t context = {
        .text = "{}",
    };

    node_t* n1 = parse(&context);
    assert_that (n1, is_non_null);
    assert_that (n1->type, is_equal_to(NODE_PAR));

    node_par_t* np = (node_par_t*)n1;
    assert_that (np->contents, is_null);
    assert_that (parse(&context), is_equal_to_contents_of(&eof_node, sizeof eof_node));
}

PARSER_TEST(parentheses_1)
{
    parse_context_t context = {
        .text = "{1 + 2;}",
    };

    node_t* n1 = parse(&context);
    assert_that (n1, is_non_null);
    assert_that (n1->type, is_equal_to(NODE_PAR));

    node_par_t* np = (node_par_t*)n1;
    assert_that (np->contents, is_non_null);
    assert_that (np->contents->type, is_equal_to(NODE_OP));

    token_number_t token_1 = {
        .base =
            {
                .type = TK_NUM,
            },
        .uint64 = 1,
    };

    node_op_t *op = (node_op_t*)np->contents;
    assert_that (op->left->type, is_equal_to(NODE_VAL));
    assert_that (op->right->type, is_equal_to(NODE_VAL));
    assert_that (op->base.next, is_null);
    assert_that (parse(&context), is_equal_to_contents_of(&eof_node, sizeof eof_node));
}

TestSuite* parser_tests(void)
{
    TestSuite* suite = create_test_suite();

    PARSER_ADDTEST(null);
    PARSER_ADDTEST(sem);
    PARSER_ADDTEST(num_0);
    PARSER_ADDTEST(op_0);
    PARSER_ADDTEST(op_1);
    PARSER_ADDTEST(op_2);
    PARSER_ADDTEST(parentheses_0);
    PARSER_ADDTEST(parentheses_1);

    return suite;
}
