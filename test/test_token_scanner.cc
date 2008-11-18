#include <gtest/gtest.h>
#include <sstream>
#include "../src/token_scanner.hh"
#include "../src/flow_sequence_begin_scanner.hh"
#include "../src/presentation_input.hh"

using namespace std;

TEST(TokenScanner, CanScanStream)
{
    stringstream ss("[");
    presentation_input* pi = new presentation_input(ss);
    token_scanner* s2 = new flow_sequence_begin_scanner(pi, 0);
    token_scanner* s1 = new flow_sequence_begin_scanner(pi, s2);

    // TODO:  s1, s2 should be of different classes.
    // we have only one subclass of token_scanner right now.
    EXPECT_TRUE(s1->scan());
}
