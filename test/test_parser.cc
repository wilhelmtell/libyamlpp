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

TEST(test_parser, parse_document1)
{
    istringstream is("---\n[a, b, and c]");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_string)
{
    istringstream is("abc");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_integer)
{
    istringstream is("44");
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

TEST(test_parser, parse_nested_sequence)
{
    istringstream is("[[b], c]");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_nested_sequence2)
{
    istringstream is("[a, [b], c]");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping)
{
    istringstream is("{a: b, c: d}");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping2)
{
    istringstream is("{a: b, c: [a, b, c]}");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping3)
{
    istringstream is("{a: b, c: [A, [B], {a: z}, X]}");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping4)
{
    istringstream is("{nice: {hello: world, goodbye: world}, a: b, c: [A, [B], {a: z}, X]}");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping5)
{
    istringstream is("{a: b, c: [A, [1, 2, B], {a: z}, X, 012, {0: a, 1: 2, a: 12, le: gt}]}");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping_with_number_values)
{
    istringstream is("{a: 0, b: 1}");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping_with_number_keys)
{
    istringstream is("{0: a, 1: b}");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping_with_number_keys_values)
{
    istringstream is("{0: 1, 1: 2}");
    parser p(is);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_numbers_sequence)
{
    istringstream is("[0, 1, 2, 3]");
    parser p(is);
    p.parse();
}
