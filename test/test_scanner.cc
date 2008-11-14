#include <UnitTest++.h>
#include <sstream>
#include "../src/scanner.hh"
#include "../src/token.hh"

using namespace std;

TEST(ScanEmptyString)
{
    istringstream ss("");
    scanner s(ss);
    token t(s.scan());
    CHECK_EQUAL(token::EOS, t.tag);
}

TEST(ScanWhitespace)
{
    istringstream ss("  ");
    scanner s(ss);
    token t(s.scan());
    CHECK_EQUAL(token::EOS, t.tag);
}

TEST(ScanSequenceBegin)
{
    istringstream ss("[");
    scanner s(ss);
    token t(s.scan());
    CHECK_EQUAL(token::FLOW_SEQUENCE_BEGIN, t.tag);
}

TEST(ScanSequenceEnd)
{
    istringstream ss("]");
    scanner s(ss);
    token t(s.scan());
    CHECK_EQUAL(token::FLOW_SEQUENCE_END, t.tag);
}

TEST(ScanMappingBegin)
{
    istringstream ss("{");
    scanner s(ss);
    token t(s.scan());
    CHECK_EQUAL(token::FLOW_MAPPING_BEGIN, t.tag);
}

TEST(ScanMappingEnd)
{
    istringstream ss("}");
    scanner s(ss);
    token t(s.scan());
    CHECK_EQUAL(token::FLOW_MAPPING_END, t.tag);
}

TEST(ScanMappingBeginEnd)
{
    istringstream ss("{  }");
    scanner s(ss);
    token t1(s.scan());
    token t2(s.scan());
    CHECK_EQUAL(token::FLOW_MAPPING_BEGIN, t1.tag);
    CHECK_EQUAL(token::FLOW_MAPPING_END, t2.tag);
}
