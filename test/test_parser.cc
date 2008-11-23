#include <gtest/gtest.h>
#include <sstream>
#include "parser.hh"

using namespace std;

TEST(test_parser, test_parser)
{
    istringstream is("--- ");
    parser p(is);

    p.parse();
}

TEST(test_parser, test_parser2)
{
    istringstream is("[a, b, c]");
    parser p(is);

    p.parse();
}
