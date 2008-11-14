#ifndef SRC_SCANNER_HH_
#define SRC_SCANNER_HH_

#include "token.hh"
#include <queue>
#include <stack>
#include <iosfwd>

class scanner {
public:
    scanner();
    scanner(std::istream& is);
    token scan();
    void sip();

private:
    typedef std::queue<char> input_buffer;

    std::istream &is;
    int line_number;
    char peek;
    input_buffer buf;
    token previous;
    std::stack<int> indentation_level;
};

#endif // SRC_SCANNER_HH_
