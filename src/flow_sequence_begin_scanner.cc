#include "flow_sequence_begin_scanner.hh"
#include "presentation_input.hh"

typedef flow_sequence_begin_scanner fsbs;

fsbs::flow_sequence_begin_scanner(token_scanner* successor) :
    token_scanner(successor)
{
}

token fsbs::scan(presentation_input* input)
{
    if( input->peek() == '[' ) {
        input->sip();
        token result(token::FLOW_SEQUENCE_BEGIN);
        previous(result);
        return result;
    }
    return token_scanner::scan(input);
}
