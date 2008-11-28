#include "parser.hh"
#include "node.hh"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include <tr1/memory>
#include <list>
#include <map>
#include "document_node.hh"
#include "null_node.hh"
#include "string_node.hh"
#include "sequence_node.hh"
#include "mapping_node.hh"
#include "pair_node.hh"

using namespace std;
using namespace std::tr1;
using namespace yaml::lex;

namespace yaml {
namespace syn {

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
    tree = parse_document();
}

shared_ptr<document_node> parser::parse_document()
{
    // TODO:  currently we support a single document only (and no more than a
    //        single document-begin token in the stream).  we should allow for
    //        multiple documents.
    if( peek == token::EOS ) {
        shared_ptr<null_node> nil(new null_node());
        shared_ptr<document_node> null_document(new document_node(nil));
        assert(null_document);
        return null_document;
    }

    // if the stream is valid yaml but it has no data then we'll return a
    // default document of null.
    shared_ptr<null_node> null_node_document(new null_node());
    shared_ptr<document_node> document(new document_node(null_node_document));

    if( peek == token::DOCUMENT_BEGIN ) sip();
    if( peek == token::FLOW_SEQUENCE_BEGIN ) {
        shared_ptr<node> sequence(parse_sequence());
        document.reset(new document_node(sequence));
    }
    else if( peek == token::FLOW_MAPPING_BEGIN ) {
        shared_ptr<node> mapping(parse_mapping());
        document.reset(new document_node(mapping));
    }
    else if( peek == token::STRING || peek == token::INTEGER ) {
        shared_ptr<node> the_string(parse_string());
        document.reset(new document_node(the_string));
    }
    else if( peek != token::EOS )
        throw runtime_error("Syntax error:  expected a document-begin, "
                            "sequence, mapping or scalar."); // TODO:  be more descriptive.  (line number etc.)

    parse_eos();

    // document should hold something, although it may be a "null node"
    assert(document);
    return document;
}

shared_ptr<sequence_node> parser::parse_sequence()
{
    if( peek == token::FLOW_SEQUENCE_BEGIN ) {
        sip();
        shared_ptr<sequence_node> the_sequence(new sequence_node());
        while( peek != token::FLOW_SEQUENCE_END ) {
            assert(the_sequence);
            if( peek == token::FLOW_SEQUENCE_BEGIN )
                the_sequence->elements.push_back(parse_sequence());
            else if( peek == token::FLOW_MAPPING_BEGIN )
                the_sequence->elements.push_back(parse_mapping());
            else if( peek == token::STRING || peek == token::INTEGER ) {
                shared_ptr<string_node> a_string(new string_node(peek.value));
                the_sequence->elements.push_back(a_string);
                sip();
            }
            else if( peek != token::SEQUENCE_SEPARATOR &&
                     peek != token::FLOW_SEQUENCE_END )
                throw runtime_error("Syntax error:  expected sequence-"
                                    "separator or sequence-end.");
            if( peek == token::SEQUENCE_SEPARATOR )
                sip();
        }
        sip(); // token::FLOW_SEQUENCE_END
        assert(the_sequence);
        return the_sequence;
    }
    else
        throw runtime_error("Syntax error:  expected sequence-begin.");
}

shared_ptr<mapping_node> parser::parse_mapping()
{
    if( peek == token::FLOW_MAPPING_BEGIN ) {
        sip();
        shared_ptr<mapping_node> the_mapping(new mapping_node());
        while( peek != token::FLOW_MAPPING_END ) {
            shared_ptr<pair_node> pair(parse_pair());
            the_mapping->elements.push_back(pair);
            if( peek == token::SEQUENCE_SEPARATOR )
                sip();
            else if( peek != token::FLOW_MAPPING_END )
                throw runtime_error("Syntax error:  expected sequence-"
                                    "separator or mapping-end.");
        }
        sip(); // token::FLOW_MAPPING_END
        assert(the_mapping);
        return the_mapping;
    }
    else
        throw runtime_error("Syntax error:  expected mapping-begin.");
}

shared_ptr<pair_node> parser::parse_pair()
{
    if( peek == token::STRING || peek == token::INTEGER ) {
        string key(peek.value);
        shared_ptr<string_node> key_node(new string_node(key));
        sip();
        if( peek == token::PAIR_SEPARATOR ) sip();
        else throw runtime_error("Syntax error:  expected a pair-separator.");
        shared_ptr<pair_node> pair;
        if( peek == token::STRING || peek == token::INTEGER ) {
            string value(peek.value);
            shared_ptr<string_node> value_node(new string_node(value));
            sip();
            pair.reset(new pair_node(key_node, value_node));
        }
        else if( peek == token::FLOW_SEQUENCE_BEGIN )
            pair.reset(new pair_node(key_node, parse_sequence()));
        else if( peek == token::FLOW_MAPPING_BEGIN )
            pair.reset(new pair_node(key_node, parse_mapping()));
        else throw runtime_error("Syntax error:  expected a scalar, a "
                                 "sequence-begin or a mapping-begin in pair "
                                 "value");
        assert(pair);
        return pair;
    }
    else throw runtime_error("Syntax error:  expected a scalar for key.");
}

shared_ptr<string_node> parser::parse_string()
{
    if( peek == token::STRING || peek == token::INTEGER ) {
        string value(peek.value);
        shared_ptr<string_node> the_string(new string_node(value));
        sip();
        assert(the_string);
        return the_string;
    }
    else throw runtime_error("Syntax error:  expected a scalar.");
}

void parser::parse_eos()
{
    return;
}

} // namespace syn
} // namespace yaml
