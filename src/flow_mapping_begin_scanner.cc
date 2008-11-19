#include "flow_mapping_begin_scanner.hh"
#include "presentation_input.hh"

typedef flow_mapping_begin_scanner fmbs;

fmbs::flow_mapping_begin_scanner(token_scanner* successor) :
    token_scanner(successor)
{
}

token fmbs::scan(presentation_input* input)
{
    if( input->peek() == '{' ) {
        input->sip();
        token result(token::FLOW_MAPPING_BEGIN);
        previous(result);
        return result;
    }
    return token_scanner::scan(input);
}
