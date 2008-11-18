#include "flow_sequence_begin_scanner.hh"

typedef flow_sequence_begin_scanner fsbs;

fsbs::flow_sequence_begin_scanner(presentation_input* input,
                                  token_scanner* successor_scanner) :
    token_scanner(successor_scanner), input(input)
{
}

bool flow_sequence_begin_scanner::recognize()
{
    if( input->peek() == '[' ) {
        input->sip();
        previous(token(token::FLOW_SEQUENCE_BEGIN));
        return true;
    }
    return false;
}
