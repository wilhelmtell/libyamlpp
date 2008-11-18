#ifndef SRC_FLOW_SEQUENCE_BEGIN_SCANNER_HH_
#define SRC_FLOW_SEQUENCE_BEGIN_SCANNER_HH_

#include <tr1/memory>
#include "token_scanner.hh"
#include "token.hh"

class flow_sequence_begin_scanner : public token_scanner {
public:
    flow_sequence_begin_scanner(token_scanner* successor_scanner);
    bool recognize(presentation_input* input);
};

#endif // SRC_FLOW_SEQUENCE_BEGIN_SCANNER_HH_
