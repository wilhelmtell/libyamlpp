#include "flow_sequence_begin_scanner.hh"
#include "presentation_input.hh"

typedef flow_sequence_begin_scanner fsbs;

fsbs::flow_sequence_begin_scanner(token_scanner* successor_scanner) :
    token_scanner(successor_scanner)
{
}

bool flow_sequence_begin_scanner::recognize(presentation_input* input)
{
    if( input->peek() == '[' ) {
        input->sip();
        previous(token(token::FLOW_SEQUENCE_BEGIN));
        return true;
    }
    return false;
}
