#ifndef TEST_TEST_BASIC_INPUT_HH_
#define TEST_TEST_BASIC_INPUT_HH_

#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <string>
#include "../src/scanner.hh"
#include "../src/token.hh"
#include "../src/parser.hh"

namespace yaml {

struct test_basic_input : public testing::Test {
    test_basic_input(const std::string& input);
    std::istringstream ss;
    yaml::lex::scanner s;
    yaml::syn::parser p;
    std::vector<yaml::lex::token> scanned_tokens;
};

} // namespace yaml

#endif // TEST_TEST_BASIC_INPUT_HH_
