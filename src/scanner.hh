#ifndef SRC_SCANNER_HH_
#define SRC_SCANNER_HH_

#include "token.hh"
#include <iosfwd>

class scanner {
public:
    scanner();
    token scan();

private:
    std::istream &is;
    int line_number;
    char peek;
};

#endif // SRC_SCANNER_HH_
