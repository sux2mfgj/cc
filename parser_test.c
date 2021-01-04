#include "parser.h"
#include "test.h"

PARSER_TEST(null)
{
    PREPARE_CTX("");
    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_EOF));
}

TestSuite* parser_tests(void)
{
    TestSuite* suite = create_test_suite();

    PARSER_ADDTEST(null);

    return suite;
}
