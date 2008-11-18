#ifndef SRC_TOKEN_SCANNER_HH_
#define SRC_TOKEN_SCANNER_HH_

#include "token.hh"
#include <tr1/memory>

class presentation_input;

class token_scanner {
public:
    token_scanner();
    token_scanner(token_scanner* successor_scanner);
    bool scan(presentation_input* input);
    virtual ~token_scanner() { }

protected:
    virtual bool recognize(presentation_input* input) = 0;
    void previous(const token& previous_token);
    token_scanner* successor() const;
    void successor(token_scanner* successor_scanner);

private:
    token previously_scanned_token;
    std::tr1::shared_ptr<token_scanner> successor_scanner;
};

#endif // SRC_TOKEN_SCANNER_HH_
