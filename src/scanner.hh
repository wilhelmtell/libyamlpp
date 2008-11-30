#ifndef SRC_SCANNER_HH_
#define SRC_SCANNER_HH_

#include "token.hh"
#include <deque>
#include <stack>
#include <iosfwd>

namespace yaml {
namespace lex {

/// The scanner class provides the lexical-analysis component of libyaml++.
/// The user should not be dealing with the scanner at all;  instead, the user
/// should only read YAML streams with the load() function.
class scanner {
public:
    scanner();
    scanner(std::istream& is);
    token scan();
    void sip();
    void putback(char new_peek);

private:
    typedef std::deque<char> input_buffer;

    std::istream &is;
    int line_number;
    char peek;
    input_buffer buf;
    token previous;
    std::stack<int> indentation_level;
    int sequence_depth;
    int mapping_depth;
};

} // namespace lex
} // namespace yaml

#endif // SRC_SCANNER_HH_
