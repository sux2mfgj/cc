#include "parser.h"
#include "test.h"

PARSER_TEST(null)
{
    PREPARE_CTX("");
    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_EOF));
}

PARSER_TEST(def_val_0)
{
    PREPARE_CTX("uint64_t hello;");

    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_DEF_VAR));

    node_def_val_t* def_val = (node_def_val_t*)n;
    assert_that(def_val->type, is_equal_to(TYPE_UINT64));
    const char* id = "hello";
    assert_that(def_val->id, is_equal_to_contents_of(id, strlen(id) + 1));
    assert_that(def_val->init, is_null);
}

PARSER_TEST(def_val_1)
{
    PREPARE_CTX("uint64_t hello = 12;");

    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_DEF_VAR));

    node_def_val_t* def_val = (node_def_val_t*)n;
    assert_that(def_val->type, is_equal_to(TYPE_UINT64));
    const char* id = "hello";
    assert_that(def_val->id, is_equal_to_contents_of(id, strlen(id) + 1));
    assert_that(def_val->init, is_non_null);
    assert_that(def_val->init->type, is_equal_to(NODE_VAL));
}

TestSuite* parser_tests(void)
{
    TestSuite* suite = create_test_suite();

    PARSER_ADDTEST(null);
    PARSER_ADDTEST(def_val_0);
    PARSER_ADDTEST(def_val_1);

    return suite;
}
