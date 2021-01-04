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

PARSER_TEST(def_val_2)
{
    PREPARE_CTX("uint64_t hello = 12 + 2;");

    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_DEF_VAR));

    node_def_val_t* def_val = (node_def_val_t*)n;
    assert_that(def_val->type, is_equal_to(TYPE_UINT64));
    const char* id = "hello";
    assert_that(def_val->id, is_equal_to_contents_of(id, strlen(id) + 1));

    assert_that(def_val->init, is_non_null);
    assert_that(def_val->init->type, is_equal_to(NODE_OP));

    node_op_t* op = (node_op_t*)def_val->init;
    assert_that(op->left, is_non_null);
    assert_that(op->right, is_non_null);
    assert_that(op->left->type, is_equal_to(NODE_VAL));
    assert_that(op->right->type, is_equal_to(NODE_VAL));

    node_val_t* left = (node_val_t*)op->left;
    assert_that(left->uint64, is_equal_to(12));

    node_val_t* right = (node_val_t*)op->right;
    assert_that(right->uint64, is_equal_to(2));
}

PARSER_TEST(def_val_3)
{
    PREPARE_CTX("uint64_t hello = 12 + 2 * 3;");

    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_DEF_VAR));

    node_def_val_t* def_val = (node_def_val_t*)n;
    assert_that(def_val->type, is_equal_to(TYPE_UINT64));
    const char* id = "hello";
    assert_that(def_val->id, is_equal_to_contents_of(id, strlen(id) + 1));

    assert_that(def_val->init, is_non_null);
    assert_that(def_val->init->type, is_equal_to(NODE_OP));

    node_op_t* op = (node_op_t*)def_val->init;
    assert_that(op->opr, is_equal_to(OP_PLUS));
    assert_that(op->left, is_non_null);
    assert_that(op->right, is_non_null);
    assert_that(op->left->type, is_equal_to(NODE_VAL));

    node_val_t* left = (node_val_t*)op->left;
    assert_that(left->uint64, is_equal_to(12));

    assert_that(op->right->type, is_equal_to(NODE_OP));
    node_op_t* op2 = (node_op_t*)op->right;
    assert_that(op2->opr, is_equal_to(OP_MUL));
    assert_that(op2->left->type, is_equal_to(NODE_VAL));
    assert_that(op2->right->type, is_equal_to(NODE_VAL));

    left = (node_val_t*)op2->left;
    node_val_t* right = (node_val_t*)op2->right;

    assert_that(left->uint64, is_equal_to(2));
    assert_that(right->uint64, is_equal_to(3));
}

TestSuite* parser_tests(void)
{
    TestSuite* suite = create_test_suite();

    PARSER_ADDTEST(null);
    PARSER_ADDTEST(def_val_0);
    PARSER_ADDTEST(def_val_1);
    PARSER_ADDTEST(def_val_2);
    PARSER_ADDTEST(def_val_3);

    return suite;
}
