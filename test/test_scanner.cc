#include <gtest/gtest.h>
#include <sstream>
#include <list>
#include <vector>
#include "../src/scanner.hh"
#include "../src/token.hh"

using namespace std;

struct test_scanner : public testing::Test {
    test_scanner(const string& input);
    istringstream ss;
    scanner s;
    vector<token> scanned_tokens;
};

test_scanner::test_scanner(const string& input) :
    ss(input), s(ss)
{
    token t = token::UNDEFINED;
    while( t != token::EOS ) {
        t = s.scan();
        scanned_tokens.push_back(t);
    }
}

struct test_scan_empty_string : public test_scanner {
    test_scan_empty_string() : test_scanner("") { }
};

TEST_F(test_scan_empty_string, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[0].tag);
}

struct test_scan_whitespace : public test_scanner {
    test_scan_whitespace() : test_scanner("  ") { }
};

TEST_F(test_scan_whitespace, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[0].tag);
}

struct test_scan_sequence_begin : public test_scanner {
    test_scan_sequence_begin() : test_scanner("[") { }
};

TEST_F(test_scan_sequence_begin, scan_sequence_begin)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_BEGIN, scanned_tokens[0].tag);
}

struct test_scan_sequence_end : public test_scanner {
    test_scan_sequence_end() : test_scanner("]") { }
};

TEST_F(test_scan_sequence_end, scan_sequence_end)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_END, scanned_tokens[0].tag);
}

struct test_scan_mapping_begin : public test_scanner {
    test_scan_mapping_begin() : test_scanner("{") { }
};

TEST_F(test_scan_mapping_begin, scan_mapping_begin)
{
    EXPECT_EQ(token::FLOW_MAPPING_BEGIN, scanned_tokens[0].tag);
}

struct test_scan_mapping_end : public test_scanner {
    test_scan_mapping_end() : test_scanner("}") { }
};

TEST_F(test_scan_mapping_end, scan_mapping_end)
{
    EXPECT_EQ(token::FLOW_MAPPING_END, scanned_tokens[0].tag);
}

struct test_scan_mapping_begin_end : public test_scanner {
    test_scan_mapping_begin_end() : test_scanner("{  }") { }
};

TEST_F(test_scan_mapping_begin_end, test_scanned_token1)
{
    EXPECT_EQ(token::FLOW_MAPPING_BEGIN, scanned_tokens[0].tag);
}

TEST_F(test_scan_mapping_begin_end, test_scanned_token2)
{
    EXPECT_EQ(token::FLOW_MAPPING_END, scanned_tokens[1].tag);
}

struct test_scan_mapping_sequence_begin_end : public test_scanner {
    test_scan_mapping_sequence_begin_end() : test_scanner("{ 	[ ]}") { }
};

TEST_F(test_scan_mapping_sequence_begin_end, test_size)
{
    EXPECT_EQ(5, scanned_tokens.size());
}

TEST_F(test_scan_mapping_sequence_begin_end, scan_mapping_begin)
{
    EXPECT_EQ(token::FLOW_MAPPING_BEGIN, scanned_tokens[0].tag);
}

TEST_F(test_scan_mapping_sequence_begin_end, scan_sequence_begin)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_BEGIN, scanned_tokens[1].tag);
}

TEST_F(test_scan_mapping_sequence_begin_end, scan_sequence_end)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_END, scanned_tokens[2].tag);
}

TEST_F(test_scan_mapping_sequence_begin_end, scan_mapping_end)
{
    EXPECT_EQ(token::FLOW_MAPPING_END, scanned_tokens[3].tag);
}

TEST_F(test_scan_mapping_sequence_begin_end, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[4].tag);
}

struct test_scan_digit : public test_scanner {
    test_scan_digit() : test_scanner("0") { }
};

TEST_F(test_scan_digit, scan_digit)
{
    EXPECT_EQ(token::INTEGER, scanned_tokens[0].tag);
}

struct test_scan_integer : public test_scanner {
    test_scan_integer() : test_scanner("1337042") { }
};

TEST_F(test_scan_integer, scan_integer)
{
    EXPECT_EQ(token::INTEGER, scanned_tokens[0].tag);
}

struct test_scan_pair_separator : public test_scanner {
    test_scan_pair_separator() : test_scanner(": ") { }
};

TEST_F(test_scan_pair_separator, scan_pair_separator)
{
    EXPECT_EQ(token::PAIR_SEPARATOR, scanned_tokens[0].tag);
}

struct test_scan_sequence_separator : public test_scanner {
    test_scan_sequence_separator() : test_scanner(", ") { }
};

TEST_F(test_scan_sequence_separator, scan_sequence_separator)
{
    EXPECT_EQ(token::SEQUENCE_SEPARATOR, scanned_tokens[0].tag);
}

struct test_scan_quoted_string : public test_scanner {
    test_scan_quoted_string() : test_scanner("'Grokking the grammar'") { }
};

TEST_F(test_scan_quoted_string, scan_quoted_string)
{
    EXPECT_EQ(token::STRING, scanned_tokens[0].tag);
}

TEST_F(test_scan_quoted_string, scan_quoted_string_value)
{
    EXPECT_EQ("Grokking the grammar", scanned_tokens[0].value);
}

struct test_scan_unquoted_string : public test_scanner {
    test_scan_unquoted_string() : test_scanner("email address") { }
};

TEST_F(test_scan_unquoted_string, scan_unquoted_string)
{
    EXPECT_EQ(token::STRING, scanned_tokens[0].tag);
}

TEST_F(test_scan_unquoted_string, scan_unquoted_string_value)
{
    EXPECT_EQ("email address", scanned_tokens[0].value);
}

struct test_scan_pair : public test_scanner {
    test_scan_pair() : test_scanner("email: ee@mail.box") { }
};

TEST_F(test_scan_pair, test_size)
{
    EXPECT_EQ(4, scanned_tokens.size());
}

TEST_F(test_scan_pair, scan_key)
{
    EXPECT_EQ(token::STRING, scanned_tokens[0].tag);
}

TEST_F(test_scan_pair, scan_pair_separator)
{
    EXPECT_EQ(token::PAIR_SEPARATOR, scanned_tokens[1].tag);
}

TEST_F(test_scan_pair, scan_value)
{
    EXPECT_EQ(token::STRING, scanned_tokens[2].tag);
}

TEST_F(test_scan_pair, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[3].tag);
}

struct test_scan_string_with_colon : public test_scanner {
    test_scan_string_with_colon() : test_scanner("email:ee@mail.box") { }
};

TEST_F(test_scan_string_with_colon, scan_string_with_colon)
{
    EXPECT_EQ(2, scanned_tokens.size());
}

TEST_F(test_scan_string_with_colon, scan_string)
{
    EXPECT_EQ(token::STRING, scanned_tokens[0].tag);
}

TEST_F(test_scan_string_with_colon, scan_string_value)
{
    EXPECT_EQ("email:ee@mail.box", scanned_tokens[0].value);
}

TEST_F(test_scan_string_with_colon, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[1].tag);
}

struct test_scan_string_with_command_and_space : public test_scanner {
    test_scan_string_with_command_and_space() : test_scanner("hello, world") { }
};

TEST_F(test_scan_string_with_command_and_space, test_size)
{
    EXPECT_EQ(2, scanned_tokens.size());
}

TEST_F(test_scan_string_with_command_and_space, scan_string)
{
    EXPECT_EQ(token::STRING, scanned_tokens[0].tag);
}

TEST_F(test_scan_string_with_command_and_space, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[1].tag);
}

struct test_scan_sequence_elements : public test_scanner {
    test_scan_sequence_elements() : test_scanner("[hello, world]") { }
};

TEST_F(test_scan_sequence_elements, scan_sequence_elements)
{
    EXPECT_EQ(6, scanned_tokens.size());
}

TEST_F(test_scan_sequence_elements, scan_sequence_begin)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_BEGIN, scanned_tokens[0].tag);
}

TEST_F(test_scan_sequence_elements, scan_element1)
{
    EXPECT_EQ(token::STRING, scanned_tokens[1].tag);
}

TEST_F(test_scan_sequence_elements, scan_sequence_separator)
{
    EXPECT_EQ(token::SEQUENCE_SEPARATOR, scanned_tokens[2].tag);
}

TEST_F(test_scan_sequence_elements, scan_element2)
{
    EXPECT_EQ(token::STRING, scanned_tokens[3].tag);
}

TEST_F(test_scan_sequence_elements, scan_sequence_end)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_END, scanned_tokens[4].tag);
}

TEST_F(test_scan_sequence_elements, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[5].tag);
}

struct test_scan_mapping_pairs : public test_scanner {
    test_scan_mapping_pairs() : test_scanner("{ np: hard }") { }
};

TEST_F(test_scan_mapping_pairs, scan_mapping_pairs)
{
    EXPECT_EQ(6, scanned_tokens.size());
}

TEST_F(test_scan_mapping_pairs, scan_mapping_begin)
{
    EXPECT_EQ(token::FLOW_MAPPING_BEGIN, scanned_tokens[0].tag);
}

TEST_F(test_scan_mapping_pairs, scan_key)
{
    EXPECT_EQ(token::STRING, scanned_tokens[1].tag);
}

TEST_F(test_scan_mapping_pairs, scan_pair_separator)
{
    EXPECT_EQ(token::PAIR_SEPARATOR, scanned_tokens[2].tag);
}

TEST_F(test_scan_mapping_pairs, scan_value)
{
    EXPECT_EQ(token::STRING, scanned_tokens[3].tag);
}

TEST_F(test_scan_mapping_pairs, scan_mapping_end)
{
    EXPECT_EQ(token::FLOW_MAPPING_END, scanned_tokens[4].tag);
}

struct test_scan_nested_sequence : public test_scanner {
    test_scan_nested_sequence() : test_scanner("[a, [b]]") { }
};

TEST_F(test_scan_nested_sequence, test_size)
{
    EXPECT_EQ(8, scanned_tokens.size());
}

TEST_F(test_scan_nested_sequence, scan_sequence_begin1)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_BEGIN, scanned_tokens[0].tag);
}

TEST_F(test_scan_nested_sequence, scan_sequence_element1)
{
    EXPECT_EQ(token::STRING, scanned_tokens[1].tag);
}

TEST_F(test_scan_nested_sequence, scan_sequence_separator)
{
    EXPECT_EQ(token::SEQUENCE_SEPARATOR, scanned_tokens[2].tag);
}

TEST_F(test_scan_nested_sequence, scan_sequence_begin2)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_BEGIN, scanned_tokens[3].tag);
}

TEST_F(test_scan_nested_sequence, scan_sequence_element2)
{
    EXPECT_EQ(token::STRING, scanned_tokens[4].tag);
}

TEST_F(test_scan_nested_sequence, scan_sequence_end1)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_END, scanned_tokens[5].tag);
}

TEST_F(test_scan_nested_sequence, scan_sequence_end2)
{
    EXPECT_EQ(token::FLOW_SEQUENCE_END, scanned_tokens[6].tag);
}

TEST_F(test_scan_nested_sequence, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[7].tag);
}
struct test_scan_nested_sequence2 : public test_scanner {
    test_scan_nested_sequence2() :
        test_scanner("{name: Richard Stallman, email: rms@gnu.org}") { }
};

TEST_F(test_scan_nested_sequence2, test_size)
{
    EXPECT_EQ(10, scanned_tokens.size());
}

TEST_F(test_scan_nested_sequence2, scan_mapping_begin)
{
    EXPECT_EQ(token::FLOW_MAPPING_BEGIN, scanned_tokens[0].tag);
}

TEST_F(test_scan_nested_sequence2, scan_key1)
{
    EXPECT_EQ(token::STRING, scanned_tokens[1].tag);
}

TEST_F(test_scan_nested_sequence2, scan_pair_separator1)
{
    EXPECT_EQ(token::PAIR_SEPARATOR, scanned_tokens[2].tag);
}

TEST_F(test_scan_nested_sequence2, scan_value1)
{
    EXPECT_EQ(token::STRING, scanned_tokens[3].tag);
}

TEST_F(test_scan_nested_sequence2, scan_sequence_separator)
{
    EXPECT_EQ(token::SEQUENCE_SEPARATOR, scanned_tokens[4].tag);
}

TEST_F(test_scan_nested_sequence2, scan_key2)
{
    EXPECT_EQ(token::STRING, scanned_tokens[5].tag);
}

TEST_F(test_scan_nested_sequence2, scan_pair_separator2)
{
    EXPECT_EQ(token::PAIR_SEPARATOR, scanned_tokens[6].tag);
}

TEST_F(test_scan_nested_sequence2, scan_value2)
{
    EXPECT_EQ(token::STRING, scanned_tokens[7].tag);
}

TEST_F(test_scan_nested_sequence2, scan_mapping_end)
{
    EXPECT_EQ(token::FLOW_MAPPING_END, scanned_tokens[8].tag);
}

TEST_F(test_scan_nested_sequence2, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[9].tag);
}

struct test_scan_string_with_comma : public test_scanner {
    test_scan_string_with_comma() : test_scanner("no,space") { }
};

TEST_F(test_scan_string_with_comma, test_size)
{
    EXPECT_EQ(2, scanned_tokens.size());
}

TEST_F(test_scan_string_with_comma, scan_string)
{
    EXPECT_EQ(token::STRING, scanned_tokens[0].tag);
}

TEST_F(test_scan_string_with_comma, scan_eos)
{
    EXPECT_EQ(token::EOS, scanned_tokens[1].tag);
}

struct test_scan_document_begin : public test_scanner {
    test_scan_document_begin() : test_scanner("--- ") { }
};

TEST_F(test_scan_document_begin, scan_document_begin)
{
    EXPECT_EQ(token::DOCUMENT_BEGIN, scanned_tokens[0].tag);
}

struct test_scan_three_dashes_string : public test_scanner {
    test_scan_three_dashes_string() : test_scanner("---") { }
};

TEST_F(test_scan_three_dashes_string, scan_three_dashes_string)
{
    EXPECT_EQ(token::STRING, scanned_tokens[0].tag);
}

TEST_F(test_scan_three_dashes_string, scan_three_dashes_string_value)
{
    EXPECT_EQ("---", scanned_tokens[0].value);
}
