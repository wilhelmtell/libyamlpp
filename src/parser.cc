#include "parser.hh"
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

parser::parser(istream& is) : s(is), peek(token::UNDEFINED)
{
    sip();
}

parser::parser() : peek(token::UNDEFINED)
{
    sip();
}

void parser::sip()
{
    peek = s.scan();
}

void parser::parse()
{
    parse_document();
}

void parser::parse_document()
{
    if( peek == token::EOS ) parse_eos();
    if( peek == token::DOCUMENT_BEGIN ) sip();
    if( peek == token::FLOW_SEQUENCE_BEGIN ) parse_sequence();
    else if( peek == token::FLOW_MAPPING_BEGIN ) parse_mapping();
    else if( peek == token::STRING ) parse_string();
    else if( peek != token::EOS ) throw runtime_error("Syntax error."); // TODO:  be more descriptive.  (line number etc.)
    parse_eos();
}

void parser::parse_sequence()
{
    if( peek == token::FLOW_SEQUENCE_BEGIN ) {
        sip();
        if( peek == token::FLOW_SEQUENCE_BEGIN ) parse_sequence();
        else if( peek == token::FLOW_MAPPING_BEGIN ) parse_mapping();
        else if( peek == token::STRING ) parse_sequence_nodes();
        else if( peek == token::FLOW_SEQUENCE_END ) {
            sip();
            return;
        }
    }
    else throw runtime_error("Syntax error:  expected '['.");
}

void parser::parse_sequence_nodes()
{
    if( peek == token::STRING ) {
        sip();
        if( peek == token::SEQUENCE_SEPARATOR ) {
            sip();
            parse_sequence_nodes();
        }
    }
    else throw runtime_error("Syntax error:  expected a scalar after ','.");
}

void parser::parse_mapping()
{
    if( peek == token::FLOW_MAPPING_BEGIN ) {
        sip();
        if( peek == token::FLOW_MAPPING_BEGIN ) parse_mapping();
        else if( peek == token::FLOW_SEQUENCE_BEGIN ) parse_sequence();
        else if( peek == token::STRING ) parse_mapping_nodes();
        else if( peek == token::FLOW_MAPPING_END ) {
            sip();
            return;
        }
    }
    else throw runtime_error("Syntax error:  expected '{'.");
}

void parser::parse_mapping_nodes()
{
    if( peek == token::STRING ) {
        parse_pair();
        if( peek == token::SEQUENCE_SEPARATOR ) {
            sip();
            parse_mapping_nodes();
        }
    }
    else throw runtime_error("Syntax error:  expected a scalar after ', '.");
}

void parser::parse_pair()
{
    if( peek == token::STRING ) {
        sip();
        if( peek == token::PAIR_SEPARATOR ) sip();
        else throw runtime_error("Syntax error:  expected a ': '.");
        if( peek == token::STRING ) sip();
        else throw runtime_error("Syntax error:  expected a scalar.");
    }
    else throw runtime_error("Syntax error:  expected a scalar.");
}

void parser::parse_string()
{
    if( peek == token::STRING ) sip();
    else throw runtime_error("Syntax error:  expected a scalar.");
}

void parser::parse_eos()
{
    return;
}
