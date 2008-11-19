#include <gtest/gtest.h>
#include <sstream>
#include "../src/token_scanner.hh"
#include "../src/flow_sequence_begin_scanner.hh"
#include "../src/flow_sequence_end_scanner.hh"
#include "../src/flow_mapping_begin_scanner.hh"
#include "../src/flow_mapping_end_scanner.hh"
#include "../src/presentation_input.hh"

using namespace std;

struct token_scanner_test {
    unsigned int chain_length() const;
    token_scanner* scanner;
};

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

TEST(TokenScanner, ChainLength)
{
    stringstream ss("[");
    presentation_input* input = new presentation_input(ss);
    token_scanner* scanner = new flow_sequence_begin_scanner(
                             new flow_sequence_end_scanner(
                             new flow_mapping_begin_scanner(
                             new flow_mapping_end_scanner(
                             0))));
    token_scanner_test tst;
    tst.scanner = scanner;
    EXPECT_EQ(4, tst.chain_length());
    delete input;
    delete scanner;
}

TEST(TokenScanner, CanScanFirstToken)
{
    stringstream ss("[");
    presentation_input* input = new presentation_input(ss);
    token_scanner* scanner = new flow_sequence_begin_scanner(
                             new flow_sequence_end_scanner(
                             new flow_mapping_begin_scanner(
                             new flow_mapping_end_scanner(
                             0))));
    // scanner->scan() throws if it can't find an appropriate token scanner
    EXPECT_NO_THROW(scanner->scan(input));
    delete input;
    delete scanner;
}

TEST(TokenScanner, CanScanNthToken)
{
    stringstream ss("{");
    presentation_input* input = new presentation_input(ss);
    token_scanner* scanner = new flow_sequence_begin_scanner(
                             new flow_sequence_end_scanner(
                             new flow_mapping_begin_scanner(
                             new flow_mapping_end_scanner(
                             0))));
    // scanner->scan() throws if it can't find an appropriate token scanner
    EXPECT_NO_THROW(scanner->scan(input));
    delete input;
    delete scanner;
}

TEST(TokenScanner, ScanSequenceBegin)
{
    stringstream ss("[");
    presentation_input* input = new presentation_input(ss);
    token_scanner* scanner = new flow_sequence_begin_scanner(
                             new flow_sequence_end_scanner(
                             new flow_mapping_begin_scanner(
                             new flow_mapping_end_scanner(
                             0))));
    token result = scanner->scan(input);
    EXPECT_EQ(token::FLOW_SEQUENCE_BEGIN, result.tag);
    delete input;
    delete scanner;
}

TEST(TokenScanner, ScanSequenceEnd)
{
    stringstream ss("]");
    presentation_input* input = new presentation_input(ss);
    token_scanner* scanner = new flow_sequence_begin_scanner(
                             new flow_sequence_end_scanner(
                             new flow_mapping_begin_scanner(
                             new flow_mapping_end_scanner(
                             0))));
    token result = scanner->scan(input);
    EXPECT_EQ(token::FLOW_SEQUENCE_END, result.tag);
    delete input;
    delete scanner;
}
