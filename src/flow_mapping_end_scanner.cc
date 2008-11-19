#include "flow_mapping_end_scanner.hh"
#include "presentation_input.hh"

typedef flow_mapping_end_scanner fmes;

fmes::flow_mapping_end_scanner(token_scanner* successor) :
    token_scanner(successor)
{
}

token fmes::scan(presentation_input* input)
{
    if( input->peek() == '}' ) {
        input->sip();
        token result(token::FLOW_MAPPING_END);
        previous(result);
        return result;
    }
    return token_scanner::scan(input);
}
