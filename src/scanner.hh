#ifndef SRC_SCANNER_HH_
#define SRC_SCANNER_HH_

#include "token.hh"
#include <queue>
#include <stack>
#include <iosfwd>

class scanner {
public:
    scanner();
    token scan();
    void sip();

private:
    std::istream &is;
    int line_number;
    char peek;
    std::queue<char> buf;
    token previous;
    std::stack<int> indentation_level;
};

#endif // SRC_SCANNER_HH_
