#include "flow_sequence_end_scanner.hh"
#include "presentation_input.hh"

typedef flow_sequence_end_scanner fses;

fses::flow_sequence_end_scanner(token_scanner* successor) :
    token_scanner(successor)
{
}

bool fses::recognize(presentation_input* input)
{
    if( input->peek() == ']' ) {
        input->sip();
        previous(token(token::FLOW_SEQUENCE_END));
        return true;
    }
    return false;
}
