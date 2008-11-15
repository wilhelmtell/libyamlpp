#include <gtest.h>
#include <sstream>
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
    token t1(s.scan());
    token t2(s.scan());
    EXPECT_EQ(token::FLOW_MAPPING_BEGIN, t1.tag);
    EXPECT_EQ(token::FLOW_MAPPING_END, t2.tag);
}
