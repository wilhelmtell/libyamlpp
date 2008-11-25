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

using namespace std;
using namespace std::tr1;

namespace yaml {

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
    else if( peek == token::STRING ) {
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
        shared_ptr<sequence_node> sequence(new sequence_node());
        if( peek == token::FLOW_SEQUENCE_BEGIN ) {
            shared_ptr<sequence_node> nested_sequence(parse_sequence());
            assert(sequence);
            sequence_node::value_type& elements = sequence->elements;
            elements.push_back(nested_sequence);
        }
        if( peek == token::FLOW_MAPPING_BEGIN ) {
            shared_ptr<mapping_node> nested_mapping(parse_mapping());
            assert(sequence);
            sequence_node::value_type& elements = sequence->elements;
            elements.push_back(nested_mapping);
        }
        if( peek == token::STRING ) {
            assert(sequence);
            sequence_node::value_type& target_elements = sequence->elements;
            shared_ptr<sequence_node> sequence_contents(parse_sequence_nodes());
            assert(sequence);
            target_elements.splice(target_elements.end(),
                                   sequence_contents->elements);
        }
        if( peek != token::FLOW_SEQUENCE_END )
            throw runtime_error("Syntax error:  expected sequence-end.");
        sip(); // token::FLOW_SEQUENCE_END
        if( ! sequence || sequence->elements.empty() ) { // none of the branches above filled sequence
            sequence.reset(new sequence_node());
            assert(sequence);
            sequence_node::value_type& elements = sequence->elements;
            shared_ptr<null_node> null(new null_node());
            elements.push_back(null);
        }
        assert(sequence);
        return sequence;
    }
    else throw runtime_error("Syntax error:  expected sequence-begin.");
}

shared_ptr<sequence_node> parser::parse_sequence_nodes()
{
    shared_ptr<sequence_node> sequence(new sequence_node());
    assert(sequence);
    sequence_node::value_type& elements = sequence->elements;
    if( peek == token::STRING ) {
        string element_value = peek.value;
        shared_ptr<string_node> new_element(new string_node(element_value));
        assert(sequence);
        sequence->elements.push_back(new_element);
        sip();
        if( peek == token::SEQUENCE_SEPARATOR ) {
            sip();
            shared_ptr<sequence_node> remaining(parse_sequence_nodes());
            assert(remaining);
            elements.splice(elements.end(), remaining->elements);
        }
    }
    else if( peek == token::FLOW_SEQUENCE_BEGIN ) {
        shared_ptr<sequence_node> sequence_element(parse_sequence());
        elements.push_back(sequence_element);
    }
    else if( peek == token::FLOW_MAPPING_BEGIN ) {
        shared_ptr<mapping_node> map_element(parse_mapping());
        elements.push_back(map_element);
    }
    else throw runtime_error("Syntax error:  expected a scalar.");
    assert(sequence);
    return sequence;
}

shared_ptr<mapping_node> parser::parse_mapping()
{
    if( peek == token::FLOW_MAPPING_BEGIN ) {
        sip();
        shared_ptr<mapping_node> mapping(new mapping_node());

        // we only support simple keys at the moment, no complex mappings
        if( peek == token::STRING ) {
            shared_ptr<sequence_node> remaining(parse_mapping_nodes());
            assert(mapping);
            mapping_node::value_type& elements = mapping->elements;
            assert(remaining);
            elements.splice(elements.end(), remaining->elements);
        }
        else if( peek == token::FLOW_MAPPING_END ) sip();
        else
            throw runtime_error("Syntax error:  expected key or mapping-end.");
        assert(mapping);
        return mapping;
    }
    else throw runtime_error("Syntax error:  expected '{'.");
}

shared_ptr<sequence_node> parser::parse_mapping_nodes()
{
    if( peek == token::STRING ) {
        shared_ptr<sequence_node> sequence(new sequence_node());
        assert(sequence);
        sequence_node::value_type& elements = sequence->elements;
        shared_ptr<pair_node> pair = parse_pair();
        elements.push_back(pair);
        if( peek == token::SEQUENCE_SEPARATOR ) {
            sip();
            shared_ptr<sequence_node> remaining(parse_mapping_nodes());
            assert(remaining);
            elements.splice(elements.end(), remaining->elements);
        }
        assert(sequence);
        return sequence;
    }
    else throw runtime_error("Syntax error:  expected a key-value pair.");
}

shared_ptr<pair_node> parser::parse_pair()
{
    if( peek == token::STRING ) {
        string key(peek.value);
        shared_ptr<string_node> key_node(new string_node(key));
        sip();
        if( peek == token::PAIR_SEPARATOR ) sip();
        else throw runtime_error("Syntax error:  expected a pair-separator.");
        if( peek == token::STRING ) {
            string value(peek.value);
            shared_ptr<string_node> value_node(new string_node(key));
            sip();
            shared_ptr<pair_node> pair(new pair_node(key_node, value_node));
            assert(pair);
            return pair;
        }
        else throw runtime_error("Syntax error:  expected a scalar.");
    }
    else throw runtime_error("Syntax error:  expected a scalar.");
}

shared_ptr<string_node> parser::parse_string()
{
    if( peek == token::STRING ) {
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

} // namespace yaml
