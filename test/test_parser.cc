#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include "parser.hh"

using namespace std;

TEST(test_parser, parse_document_begin)
{
    istringstream is("--- ");
    yaml::syn::parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_string)
{
    istringstream is("abc");
    yaml::syn::parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_sequence_begin)
{
    istringstream is("[");
    yaml::syn::parser p(is);
    EXPECT_THROW(p.parse(), runtime_error);
}

TEST(test_parser, parse_empty_sequence)
{
    istringstream is("[]");
    yaml::syn::parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping_begin)
{
    istringstream is("{");
    yaml::syn::parser p(is);
    EXPECT_THROW(p.parse(), runtime_error);
}

TEST(test_parser, parse_empty_mapping)
{
    istringstream is("{}");
    yaml::syn::parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_sequence)
{
    istringstream is("[a, b, c]");
    yaml::syn::parser p(is);
    EXPECT_NO_THROW(p.parse());
}
