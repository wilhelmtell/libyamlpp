#ifndef SRC_TOKEN_SCANNER_HH_
#define SRC_TOKEN_SCANNER_HH_

#include "token.hh"

class token_scanner {
public:
    virtual ~token_scanner() { }
    virtual bool scanned() = 0;

protected:
    void previous(const token& previous_token);

private:
    token previously_scanned_token;
};

#endif // SRC_TOKEN_SCANNER_HH_
