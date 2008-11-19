#include "flow_sequence_end_scanner.hh"
#include "presentation_input.hh"

typedef flow_sequence_end_scanner fses;

fses::flow_sequence_end_scanner(token_scanner* successor) :
    token_scanner(successor)
{
}

token fses::scan(presentation_input* input)
{
    if( input->peek() == ']' ) {
        input->sip();
        token result(token::FLOW_SEQUENCE_END);
        previous(result);
        return result;
    }
    return token_scanner::scan(input);
}
