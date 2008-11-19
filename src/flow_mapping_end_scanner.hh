#ifndef SRC_FLOW_MAPPING_END_SCANNER_HH_
#define SRC_FLOW_MAPPING_END_SCANNER_HH_

#include "token_scanner.hh"

class flow_mapping_end_scanner : public token_scanner {
public:
    flow_mapping_end_scanner(token_scanner* successor_scanner);
    token scan(presentation_input* input);
};

#endif // SRC_FLOW_MAPPING_END_SCANNER_HH_
