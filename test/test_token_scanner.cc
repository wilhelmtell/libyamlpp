#include <gtest/gtest.h>
#include <sstream>
#include "../src/token_scanner.hh"
#include "../src/flow_sequence_begin_scanner.hh"
#include "../src/flow_sequence_end_scanner.hh"
#include "../src/presentation_input.hh"

using namespace std;

TEST(TokenScanner, CanScanStream)
{
    stringstream ss("[");
    presentation_input* input = new presentation_input(ss);
    token_scanner* scanner = new flow_sequence_begin_scanner(
                             new flow_sequence_end_scanner(0));

    EXPECT_TRUE(scanner->scan(input));
}

TEST(TokenScanner, ScanSequenceBegin)
{
    stringstream ss("[");
    presentation_input* input = new presentation_input(ss);
    token_scanner* scanner = new flow_sequence_begin_scanner(
                             new flow_sequence_end_scanner(0));
    scanner->scan(input);
    token result(scanner->previous());

    // TODO:  s1, s2 should be of different classes.
    // we have only one subclass of token_scanner right now.
    EXPECT_EQ(token::FLOW_SEQUENCE_BEGIN, result.tag);
}
