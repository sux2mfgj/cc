#include "test.h"

int main(int argc, char* argv[])
{
    TestSuite* suite = create_test_suite();

    add_suite(suite, lexer_tests());
    //add_suite(suite, parser_tests());

    return run_test_suite (suite, create_text_reporter());
}
