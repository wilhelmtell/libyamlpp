#ifndef SRC_FLOW_SEQUENCE_BEGIN_SCANNER_HH_
#define SRC_FLOW_SEQUENCE_BEGIN_SCANNER_HH_

#include <tr1/memory>
#include "presentation_input.hh"
#include "token_scanner.hh"
#include "token.hh"

class flow_sequence_begin_scanner : public token_scanner {
public:
    flow_sequence_begin_scanner(presentation_input* input);
    bool scanned();

private:
    std::tr1::shared_ptr<presentation_input> input;
};

#endif // SRC_FLOW_SEQUENCE_BEGIN_SCANNER_HH_
