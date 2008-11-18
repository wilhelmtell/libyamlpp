#ifndef SRC_SCANNER_HH_
#define SRC_SCANNER_HH_

#include "token.hh"
#include <deque>
#include <stack>
#include <iosfwd>

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
};

#endif // SRC_SCANNER_HH_
