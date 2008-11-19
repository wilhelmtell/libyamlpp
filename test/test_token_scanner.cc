#include <gtest/gtest.h>
#include <sstream>
#include "../src/token_scanner.hh"
#include "../src/flow_sequence_begin_scanner.hh"
#include "../src/flow_sequence_end_scanner.hh"
#include "../src/flow_mapping_begin_scanner.hh"
#include "../src/flow_mapping_end_scanner.hh"
#include "../src/presentation_input.hh"

using namespace std;

struct token_scanner_test : testing::Test {
    token_scanner_test();
    ~token_scanner_test();

    void reset_input_string_to(const string& input_string);
    unsigned int chain_length() const;

    istringstream ss;
    presentation_input* input;
    token_scanner* scanner;
};

token_scanner_test::token_scanner_test() :
    input(new presentation_input(ss)),
    scanner(new flow_sequence_begin_scanner(
            new flow_sequence_end_scanner(
            new flow_mapping_begin_scanner(
            new flow_mapping_end_scanner(
            0)))))
{
}

token_scanner_test::~token_scanner_test()
{
    delete input;
    delete scanner;
}

void token_scanner_test::reset_input_string_to(const string& input_string)
{
    ss.str(input_string);
    ss.clear(); // in re-initialization it is possible that e.g. ss.eof()

    // FIXME:  i shouldn't need to do the following.  but for some reason if i
    // don't refresh the input pointer then updating the stream has no effect
    // (or doesn't have the effect i want)
    delete input;
    input = new presentation_input(ss);
}

unsigned int token_scanner_test::chain_length() const
{
    if( scanner == 0 ) return 0;
    unsigned int length = 1;
    token_scanner* scanner = this->scanner;
    while( scanner->successor() ) {
        ++length;
        scanner = scanner->successor();
    }
    return length;
}

TEST_F(token_scanner_test, ChainLength)
{
    EXPECT_EQ(4, chain_length());
}

TEST_F(token_scanner_test, CanScanFirstToken)
{
    reset_input_string_to("[");

    // scanner->scan() throws if it can't find an appropriate token scanner
    EXPECT_NO_THROW(scanner->scan(input));
}

TEST_F(token_scanner_test, CanScanNthToken)
{
    reset_input_string_to("{");

    // scanner->scan() throws if it can't find an appropriate token scanner
    EXPECT_NO_THROW(scanner->scan(input));
}

TEST_F(token_scanner_test, ScanSequenceBegin)
{
    reset_input_string_to("[");
    token result = scanner->scan(input);
    EXPECT_EQ(token::FLOW_SEQUENCE_BEGIN, result.tag);
}

TEST_F(token_scanner_test, ScanSequenceEnd)
{
    reset_input_string_to("]");
    token result = scanner->scan(input);
    EXPECT_EQ(token::FLOW_SEQUENCE_END, result.tag);
}
