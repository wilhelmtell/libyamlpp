#ifndef SRC_FLOW_SEQUENCE_END_SCANNER_HH_
#define SRC_FLOW_SEQUENCE_END_SCANNER_HH_

#include "token_scanner.hh"

class flow_sequence_end_scanner : public token_scanner {
public:
    flow_sequence_end_scanner(token_scanner* successor_scanner);
    bool recognize(presentation_input* input);
};

#endif // SRC_FLOW_SEQUENCE_END_SCANNER_HH_
