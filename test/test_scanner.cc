#include <UnitTest++.h>
#include <sstream>
#include "../src/scanner.hh"
#include "../src/token.hh"

using namespace std;

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
