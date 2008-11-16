#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include "../src/scanner.hh"
#include "../src/token.hh"

using namespace std;

TEST(Scanner, ScanEmptyString)
{
    istringstream ss("");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::EOS, t.tag);
}

TEST(Scanner, ScanWhitespace)
{
    istringstream ss("  ");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::EOS, t.tag);
}

TEST(Scanner, ScanSequenceBegin)
{
    istringstream ss("[");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::FLOW_SEQUENCE_BEGIN, t.tag);
}

TEST(Scanner, ScanSequenceEnd)
{
    istringstream ss("]");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::FLOW_SEQUENCE_END, t.tag);
}

TEST(Scanner, ScanMappingBegin)
{
    istringstream ss("{");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::FLOW_MAPPING_BEGIN, t.tag);
}

TEST(Scanner, ScanMappingEnd)
{
    istringstream ss("}");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::FLOW_MAPPING_END, t.tag);
}

TEST(Scanner, ScanMappingBeginEnd)
{
    istringstream ss("{  }");
    scanner s(ss);
    token expected[] = {
        token(token::FLOW_MAPPING_BEGIN),
        token(token::FLOW_MAPPING_END),
        token(token::EOS)
    };
    vector<token> actual;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        actual.push_back(t);
    actual.push_back(token::EOS);
    size_t expected_count = sizeof(expected) / sizeof(token);
    EXPECT_TRUE(expected_count == actual.size() &&
                equal(expected, expected+expected_count, actual.begin()));
}

TEST(Scanner, ScanMappingSequenceBeginEnd)
{
    istringstream ss("{ 	[ ]}");
    scanner s(ss);
    token expected[] = {
        token(token::FLOW_MAPPING_BEGIN),
        token(token::FLOW_SEQUENCE_BEGIN),
        token(token::FLOW_SEQUENCE_END),
        token(token::FLOW_MAPPING_END),
        token(token::EOS)
    };
    vector<token> actual;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        actual.push_back(t);
    actual.push_back(token::EOS);
    size_t expected_count = sizeof(expected) / sizeof(token);
    EXPECT_TRUE(expected_count == actual.size() &&
                equal(expected, expected+expected_count, actual.begin()));
}

TEST(Scanner, ScanDigit)
{
    istringstream ss("0");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::INTEGER, t.tag);
}

TEST(Scanner, ScanInteger)
{
    istringstream ss("1337042");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::INTEGER, t.tag);
}
