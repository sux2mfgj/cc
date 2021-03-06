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

PARSER_TEST(def_val_4)
{
    PREPARE_CTX("uint64_t hello = 1 * 2 - 3 / 4;");

    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_DEF_VAR));

    node_def_val_t* def_val = (node_def_val_t*)n;
    assert_that(def_val->type, is_equal_to(TYPE_UINT64));
    const char* id = "hello";
    assert_that(def_val->id, is_equal_to_contents_of(id, strlen(id) + 1));

    assert_that(def_val->init, is_non_null);
    assert_that(def_val->init->type, is_equal_to(NODE_OP));

    node_op_t* op_top = (node_op_t*)def_val->init;
    assert_that(op_top->opr, is_equal_to(OP_MINUS));
    assert_that(op_top->left, is_non_null);
    assert_that(op_top->left->type, is_equal_to(NODE_OP));
    assert_that(op_top->right->type, is_equal_to(NODE_OP));

    node_op_t* op_left = (node_op_t*)op_top->left;
    assert_that(op_left->opr, is_equal_to(OP_MUL));
    assert_that(op_left->left, is_non_null);
    assert_that(op_left->left->type, is_equal_to(NODE_VAL));
    assert_that(op_left->right, is_non_null);
    assert_that(op_left->right->type, is_equal_to(NODE_VAL));

    node_op_t* op_right = (node_op_t*)op_top->right;
    assert_that(op_right->opr, is_equal_to(OP_DIV));
    assert_that(op_right->left, is_non_null);
    assert_that(op_right->left->type, is_equal_to(NODE_VAL));
    assert_that(op_right->right, is_non_null);
    assert_that(op_right->right->type, is_equal_to(NODE_VAL));
}

PARSER_TEST(def_func_0)
{
    PREPARE_CTX("void main(){}");

    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_DEF_FUNC));

    node_func_t* func = (node_func_t*)n;
    assert_that(func->ret_type, is_equal_to(TYPE_VOID));
    assert_that(func->id, is_equal_to_contents_of("main", sizeof("main")));
    assert_that(func->args, is_null);
    assert_that(func->proc, is_non_null);
    assert_that(func->proc->type, is_equal_to(NODE_PAR));

    node_par_t* par = (node_par_t*)func->proc;
    assert_that(par->contents, is_null);
}

PARSER_TEST(def_func_1)
{
    PREPARE_CTX(
        "void main()"
        "{"
        "   uint64_t a;"
        "}");
    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_DEF_FUNC));

    node_func_t* func = (node_func_t*)n;
    assert_that(func->ret_type, is_equal_to(TYPE_VOID));
    assert_that(func->id, is_equal_to_contents_of("main", sizeof("main")));
    assert_that(func->args, is_null);
    assert_that(func->proc, is_non_null);
    assert_that(func->proc->type, is_equal_to(NODE_PAR));

    node_par_t* par = (node_par_t*)func->proc;
    assert_that(par->contents, is_non_null);
    assert_that(par->contents->type, is_equal_to(NODE_DEF_VAR));

    node_def_val_t* def = (node_def_val_t*)par->contents;
    assert_that(def->type, is_equal_to(TYPE_UINT64));
    assert_that(def->id, is_equal_to_contents_of("a", sizeof("a")));
    assert_that(def->init, is_null);

    assert_that(def->base.next, is_null);
}

PARSER_TEST(def_func_2)
{
    PREPARE_CTX(
        "void main()"
        "{"
        "   uint64_t a;"
        "   a = 12 * 3;"
        "}");

    node_t* n = parse(ctx);
    assert_that(n, is_non_null);
    assert_that(n->type, is_equal_to(NODE_DEF_FUNC));

    node_func_t* func = (node_func_t*)n;
    assert_that(func->ret_type, is_equal_to(TYPE_VOID));
    assert_that(func->id, is_equal_to_contents_of("main", sizeof("main")));
    assert_that(func->args, is_null);
    assert_that(func->proc, is_non_null);
    assert_that(func->proc->type, is_equal_to(NODE_PAR));

    node_par_t* par = (node_par_t*)func->proc;
    assert_that(par->contents, is_non_null);
    assert_that(par->contents->type, is_equal_to(NODE_DEF_VAR));

    node_def_val_t* def = (node_def_val_t*)par->contents;
    assert_that(def->type, is_equal_to(TYPE_UINT64));
    assert_that(def->id, is_equal_to_contents_of("a", sizeof("a")));
    assert_that(def->init, is_null);

    assert_that(def->base.next, is_non_null);
    assert_that(def->base.next->type, is_equal_to(NODE_ASSIGN));

    node_assign_t* assign = (node_assign_t*)def->base.next;
    assert_that(assign->id, is_equal_to_contents_of("a", sizeof("a")));
    assert_that(assign->expr, is_non_null);
    assert_that(assign->expr->type, is_equal_to(NODE_OP));

    node_op_t* op = (node_op_t*)assign->expr;
    assert_that(op->opr, is_equal_to(OP_MUL));
    assert_that(op->left->type, is_equal_to(NODE_VAL));
    assert_that(op->right->type, is_equal_to(NODE_VAL));
    // TODO
}

PARSER_TEST(include_0)
{
    PREPARE_CTX("#include <stdio.h>");

    node_t* n = parse(ctx);

    assert_that(n->type, is_equal_to(NODE_INCLUDE));
    node_include_t* inc = (node_include_t*)n;
    assert_that(inc->filename,
                is_equal_to_contents_of("stdio.h", sizeof "stdio.h"));
}

TestSuite* parser_tests(void)
{
    TestSuite* suite = create_test_suite();

    PARSER_ADDTEST(null);
    PARSER_ADDTEST(def_val_0);
    PARSER_ADDTEST(def_val_1);
    PARSER_ADDTEST(def_val_2);
    PARSER_ADDTEST(def_val_3);
    PARSER_ADDTEST(def_val_4);

    PARSER_ADDTEST(def_func_0);
    PARSER_ADDTEST(def_func_1);
    PARSER_ADDTEST(def_func_2);
    PARSER_ADDTEST(include_0);

    return suite;
}
