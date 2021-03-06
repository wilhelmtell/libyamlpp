#include "scanner.hh"
#include <string>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <algorithm>

using namespace std;

namespace yaml {
namespace lex {

scanner::scanner() :
    is(cin), line_number(1), peek(' '), sequence_depth(0), mapping_depth(0)
{
    sip();
}

scanner::scanner(istream& is) :
    is(is), line_number(1), peek(' '), sequence_depth(0), mapping_depth(0)
{
    sip();
}

// Fill peek with the next character from the input stream
void scanner::sip()
{
    // maintain a buffer so we do io less often
    const input_buffer::size_type BUFFER_SIZE = 4096;
    if( buf.empty() )
        for( input_buffer::size_type i = 0; i < BUFFER_SIZE; ++i ) {
            char ch = is.get();
            if( ! is ) {
                buf.push_back(-1);
                break;
            }
            buf.push_back(ch);
        }
    peek = buf.front();
    buf.pop_front();
    assert(buf.size() <= BUFFER_SIZE - 1);
}

void scanner::putback(char new_peek)
{
    buf.push_front(peek);
    peek = new_peek;
}

/// Get the token representation of a scalar.
///
/// \return token::INTEGER if the string resembles a number.  Otherwise,
/// returns token::STRING.
token scalar_type(const string& scalar)
{
    int (*isdigit_ref)(int) = &std::isdigit; // resolve function type
    pointer_to_unary_function<int,int> isdigit = ptr_fun(isdigit_ref);
    if( find_if(scalar.begin(), scalar.end(), not1(isdigit)) == scalar.end() )
        return token::INTEGER;
    return token::STRING;
}

// TODO:  can we apply some design patter to break this huge function into
// something manageable and malable?  how about a chain of responsibility?
token scanner::scan()
{
    // skip whitespace
    // TODO:  wrong.  we need to account for indentation.
    while( true ) {
        if( peek == '\n' )
            ++line_number;
        else if( ! isspace(peek) )
            break;
        sip();
    }

    if( peek == '-' ) { // DOCUMENT_BEGIN
        string document_begin;
        const int LENGTH_OF_DOCUMENT_BEGIN = 3; // "---"
        for( int i = 0; i < LENGTH_OF_DOCUMENT_BEGIN; ++i ) {
            document_begin += peek;
            sip();
        }
        if( document_begin == "---" && isspace(peek) )
            return previous = token::DOCUMENT_BEGIN;
        else for( int i = LENGTH_OF_DOCUMENT_BEGIN; i > 0; --i )
            putback(document_begin[i - 1]);
    }
    if( peek == '[' ) {
        sip();
        ++sequence_depth;
        return previous = token::FLOW_SEQUENCE_BEGIN;
    }
    if( peek == ']' ) {
        sip();
        if( sequence_depth > 0 ) --sequence_depth;
        return previous = token::FLOW_SEQUENCE_END;
    }
    if( peek == '{' ) {
        sip();
        ++mapping_depth;
        return previous = token::FLOW_MAPPING_BEGIN;
    }
    if( peek == '}' ) {
        sip();
        if( mapping_depth > 0 ) --mapping_depth;
        return previous = token::FLOW_MAPPING_END;
    }
    if( peek == ':' ) {
        sip();
        if( isspace(peek) ) {
            sip();
            return previous = token::PAIR_SEPARATOR;
        }
        else putback(':');
    }
    if( peek == ',' ) { // ", " is a sequence element separator
        sip();
        if( isspace(peek) ) {
            sip();
            return previous = token::SEQUENCE_SEPARATOR;
        }
        else putback(',');
    }
    if( peek == '\'' ) { // quoted string
        string the_string;
        sip();
        while( peek != '\'' ) { // consume string until closing quote
            the_string += peek;
            sip();
        }
        sip(); // closing quote
        previous.tag = token::STRING;
        previous.value = the_string;
        return previous;
    }
    if( isprint(peek) && peek != '\n' ) { // strings come unquoted as well
        string the_string;
        do {
            the_string += peek;
            sip();
            if( peek == ':' ) { // pair separator ahead?  must match /:(?=\s)/
                sip();
                if( isspace(peek) ) { // pair separator ahead
                    putback(':');
                    previous = scalar_type(the_string);
                    previous.value = the_string;
                    return previous;
                }
                else putback(':'); // false alarm, still lexing a string
            }
            // FIXME:  in YAML, this: "hello, world" is in fact a string
            //         the following would consider the comma as a sequence sep
            if( peek == ',' ) { // seq separator ahead?  must match /,(?=\s)/
                sip();
                if( isspace(peek) &&
                    (sequence_depth > 0 || mapping_depth > 0) ) {
                    putback(',');
                    previous = scalar_type(the_string);
                    previous.value = the_string;
                    return previous;
                }
                else putback(','); // false alarm, still lexing a string
            }
            if( peek == '[' &&
                (sequence_depth > 0 || mapping_depth > 0) &&
                (previous.tag == token::SEQUENCE_SEPARATOR ||
                previous.tag == token::PAIR_SEPARATOR) ) {
                previous = scalar_type(the_string);
                previous.value = the_string;
                return previous;
            }
            if( peek == ']' && sequence_depth > 0 ) {
                previous = scalar_type(the_string);
                previous.value = the_string;
                return previous;
            }
            if( peek == '{' &&
                (sequence_depth > 0 || mapping_depth > 0) &&
                (previous.tag == token::SEQUENCE_SEPARATOR ||
                previous.tag == token::PAIR_SEPARATOR) ) {
                previous = scalar_type(the_string);
                previous.value = the_string;
                return previous;
            }
            if( peek == '}' && mapping_depth > 0 ) {
                previous = scalar_type(the_string);
                previous.value = the_string;
                return previous;
            }
        } while( isprint(peek) && peek != '\n' );
        if( the_string.empty() ) putback('\n');
        else {
            previous = scalar_type(the_string);
            previous.value = the_string;
            return previous;
        }
    }
    if( !is )
        return previous = token::EOS;

    throw runtime_error(string("not a valid token: ") + peek);  // TODO:  there must be a better way
}

} // namespace lex
} // namespace yaml
