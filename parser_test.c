#include "parser.h"
#include "test.h"

#define PARSER_TEST(name) Ensure(parser_##name)
#define PARSER_ADDTEST(name) add_test(suite, parser_##name)

PARSER_TEST(null) {
    parse_context_t context = {
        .text = "",
    };

    assert_that(parse(&context), is_null);
}

PARSER_TEST(sem) {
    parse_context_t context = {
        .text = ";",
    };

    assert_that(parse(&context), is_null);
}

PARSER_TEST(num_0) {
    parse_context_t context = {
        .text = "0;",
    };

    node_val_t node = {
        .base = {
            .type = NODE_VAL,
        },
    };

    token_number_t token = {
        .base = {
            .type = TK_NUM,
        },
        .uint64 = 0,
    };

    node_t* result = parse(&context);
    assert_that(result->type, is_equal_to(NODE_VAL));
    assert_that(((node_val_t*)result)->token,
                is_equal_to_contents_of(&token, sizeof token));
    assert_that(parse(&context), is_null);
}

PARSER_TEST(op_0) {
    parse_context_t context = {
        .text = "1 + 12;",
    };

    token_number_t token_1 = {
        .base = {
            .type = TK_NUM,
        },
        .uint64 = 1,
    };
    token_opr_t op = {
        .base = {
            .type = TK_OPR,
        },
        .type = OP_PLUS,
    };

    token_number_t token_12 = {
        .base = {
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
    //TODO check the values
}

TestSuite* parser_tests(void)
{
    TestSuite* suite = create_test_suite();

    PARSER_ADDTEST(null);
    PARSER_ADDTEST(sem);
    PARSER_ADDTEST(num_0);
    PARSER_ADDTEST(op_0);

    return suite;
}
