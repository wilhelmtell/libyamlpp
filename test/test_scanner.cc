#include <gtest/gtest.h>
#include <sstream>
#include <list>
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
    token expected_tokens[] = {
        token(token::FLOW_MAPPING_BEGIN),
        token(token::FLOW_MAPPING_END),
        token(token::EOS)
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    EXPECT_TRUE(equal(expected_tokens, expected_tokens+expected_scan_count,
                      scanned_input.begin()));
}

TEST(Scanner, ScanMappingSequenceBeginEnd)
{
    istringstream ss("{ 	[ ]}");
    scanner s(ss);
    token expected_tokens[] = {
        token(token::FLOW_MAPPING_BEGIN),
        token(token::FLOW_SEQUENCE_BEGIN),
        token(token::FLOW_SEQUENCE_END),
        token(token::FLOW_MAPPING_END),
        token(token::EOS)
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    EXPECT_TRUE(equal(expected_tokens, expected_tokens+expected_scan_count,
                      scanned_input.begin()));
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

TEST(Scanner, ScanPairSeparator)
{
    istringstream ss(": ");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::PAIR_SEPARATOR, t.tag);
}

TEST(Scanner, ScanSequenceSeperator)
{
    istringstream ss(", ");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::SEQUENCE_SEPARATOR, t.tag);
}

TEST(Scanner, ScanQuotedString)
{
    istringstream ss("'Grokking the grammar'");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::STRING, t.tag);
}

TEST(Scanner, ScanUnquotedString)
{
    istringstream ss("email");
    scanner s(ss);
    token t(s.scan());
    EXPECT_EQ(token::STRING, t.tag);
}

TEST(Scanner, ScanPair)
{
    istringstream ss("email: ee@mail.box");
    scanner s(ss);
    token expected_tokens[] = {
        token(token::STRING),
        token(token::PAIR_SEPARATOR),
        token(token::STRING),
        token(token::EOS)
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    EXPECT_TRUE(equal(expected_tokens, expected_tokens+expected_scan_count,
                      scanned_input.begin()));
}

TEST(Scanner, ScanStringWithColon)
{
    istringstream ss("email:ee@mail.box");
    scanner s(ss);
    token expected_tokens[] = {
        token(token::STRING),
        token(token::EOS)
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    EXPECT_TRUE(equal(expected_tokens, expected_tokens+expected_scan_count,
                      scanned_input.begin()));
}

TEST(Scanner, ScanStringWithCommaAndSpace)
{
    istringstream ss("hello, world");
    scanner s(ss);
    token expected_tokens[] = {
        token(token::STRING),
        token(token::EOS)
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    EXPECT_TRUE(equal(expected_tokens, expected_tokens+expected_scan_count,
                      scanned_input.begin()));
}

TEST(Scanner, ScanSequenceElements)
{
    istringstream ss("[hello, world]");
    scanner s(ss);
    token expected_tokens[] = {
        token(token::FLOW_SEQUENCE_BEGIN),
        token(token::STRING),
        token(token::SEQUENCE_SEPARATOR),
        token(token::STRING),
        token(token::FLOW_SEQUENCE_END),
        token(token::EOS)
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    list<token>::const_iterator i(scanned_input.begin());
    EXPECT_EQ(expected_tokens[0].tag, i->tag);
    EXPECT_EQ(expected_tokens[1].tag, (++i)->tag);
    EXPECT_EQ(expected_tokens[2].tag, (++i)->tag);
    EXPECT_EQ(expected_tokens[3].tag, (++i)->tag);
    EXPECT_EQ(expected_tokens[4].tag, (++i)->tag);
    EXPECT_EQ(expected_tokens[5].tag, (++i)->tag);
}

TEST(Scanner, ScanMappingPairs)
{
    istringstream ss("{ np: hard }");
    scanner s(ss);
    token expected_tokens[] = {
        token(token::FLOW_MAPPING_BEGIN),
        token(token::STRING),
        token(token::PAIR_SEPARATOR),
        token(token::STRING),
        token(token::FLOW_MAPPING_END),
        token(token::EOS)
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    list<token>::const_iterator i(scanned_input.begin());
    EXPECT_EQ(expected_tokens[0].tag, i->tag);
    EXPECT_EQ(expected_tokens[1].tag, (++i)->tag);
    EXPECT_EQ(expected_tokens[2].tag, (++i)->tag);
    EXPECT_EQ(expected_tokens[3].tag, (++i)->tag);
    EXPECT_EQ(expected_tokens[4].tag, (++i)->tag);
    EXPECT_EQ(expected_tokens[5].tag, (++i)->tag);
}

TEST(Scanner, ScanNestedSequence)
{
    istringstream ss("[a, [b]]");
    scanner s(ss);
    token expected_tokens[] = {
        token(token::FLOW_SEQUENCE_BEGIN),
        token(token::STRING),
        token(token::SEQUENCE_SEPARATOR),
        token(token::FLOW_SEQUENCE_BEGIN),
        token(token::STRING),
        token(token::FLOW_SEQUENCE_END),
        token(token::FLOW_SEQUENCE_END),
        token(token::EOS)
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    list<token>::const_iterator i(scanned_input.begin());
    for( size_t index = 0; index < expected_scan_count; ++index ) {
        EXPECT_EQ(expected_tokens[index].tag, i->tag);
        ++i;
    }
}

TEST(Scanner, ScanNestedSequence2)
{
    istringstream ss("{name: Richard Stallman, email: rms@gnu.org}");
    scanner s(ss);
    token expected_tokens[] = {
        token::FLOW_MAPPING_BEGIN,
        token::STRING,
        token::PAIR_SEPARATOR,
        token::STRING,
        token::SEQUENCE_SEPARATOR,
        token::STRING,
        token::PAIR_SEPARATOR,
        token::STRING,
        token::FLOW_MAPPING_END,
        token::EOS
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    list<token>::const_iterator i(scanned_input.begin());
    for( size_t index = 0; index < expected_scan_count; ++index ) {
        EXPECT_EQ(expected_tokens[index].tag, i->tag);
        ++i;
    }
}

TEST(Scanner, ScanStringWithComma)
{
    istringstream ss("no,space");
    scanner s(ss);
    token expected_tokens[] = {
        token(token::STRING),
        token(token::EOS)
    };
    list<token> scanned_input;
    for( token t = s.scan(); t.tag != token::EOS; t = s.scan() )
        scanned_input.push_back(t);
    scanned_input.push_back(token::EOS);
    size_t expected_scan_count = sizeof(expected_tokens) / sizeof(token);
    EXPECT_EQ(expected_scan_count, scanned_input.size());
    EXPECT_TRUE(equal(expected_tokens, expected_tokens+expected_scan_count,
                      scanned_input.begin()));
}
