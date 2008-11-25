#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include "parser.hh"

using namespace std;
using namespace yaml;
using namespace yaml::syn;

TEST(test_parser, parse_document_begin)
{
    istringstream is("--- ");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_string)
{
    istringstream is("abc");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_sequence_begin)
{
    istringstream is("[");
    parser p(is);
    EXPECT_THROW(p.parse(), runtime_error);
}

TEST(test_parser, parse_empty_sequence)
{
    istringstream is("[]");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping_begin)
{
    istringstream is("{");
    parser p(is);
    EXPECT_THROW(p.parse(), runtime_error);
}

TEST(test_parser, parse_empty_mapping)
{
    istringstream is("{}");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_sequence)
{
    istringstream is("[a, b, c]");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}
