#include <gtest/gtest.h>
#include <algorithm>
#include <sstream>
#include "../src/token_scanner.hh"
#include "../src/flow_sequence_begin_scanner.hh"
#include "../src/presentation_input.hh"

using namespace std;

TEST(TokenScanner, ScanStream)
{
    stringstream ss("[");
    presentation_input* pi = new presentation_input(ss);
    token_scanner* s1 = new flow_sequence_begin_scanner(pi);
    token_scanner* s2 = new flow_sequence_begin_scanner(pi);

    // TODO:  s1, s2 should be of different classes.
    // we have only one subclass of token_scanner right now.
    token_scanner* scanners[] = {s1, s2};
    token_scanner** p = find_if(scanners, scanners+2,
                                mem_fun(&token_scanner::recognize));
    EXPECT_EQ(*(scanners), *p);
}
