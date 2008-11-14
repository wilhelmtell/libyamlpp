#include "scanner.hh"
#include <string>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <cassert>

using namespace std;

scanner::scanner() : is(cin), line_number(1), peek(' ')
{
    sip();
}

scanner::scanner(istream& is) : is(is), line_number(1), peek(' ')
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
                buf.push_back(' ');
                break;
            }
            buf.push_back(ch);
        }
    peek = buf.front();
    buf.pop_front();
    assert(buf.size() <= BUFFER_SIZE - 1);
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
        if( buf.empty() && is.eof() ) return previous = token(token::EOS);
    }

    if( peek == '[' ) {
        sip();
        return previous = token(token::FLOW_SEQUENCE_BEGIN);
    }
    if( peek == ']' ) {
        sip();
        return previous = token(token::FLOW_SEQUENCE_END);
    }
    if( peek == '{' ) {
        sip();
        return previous = token(token::FLOW_MAPPING_BEGIN);
    }
    if( peek == '}' ) {
        sip();
        return previous = token(token::FLOW_MAPPING_END);
    }
    if( peek == ':' ) {
        sip();
        return previous = token(token::PAIR_SEPARATOR);
    }
    if( peek == ',' ) {
        sip();
        return previous = token(token::SEQUENCE_SEPARATOR);
    }
    if( peek == '-' || isdigit(peek) ) { // a number
        string number(1, peek); // TODO:  can use istringstream to get int
        sip();
        while( isdigit(peek) ) {
            number += peek;
            sip();
        }
        if( peek == '.' ) {
            number += peek;
            while( isdigit(peek) ) {
                number += peek;
                sip();
            }
            // done parsing number.  now, is it really a number?
            if( number == "-" ) { // NaN.  restore input.
                buf.push_front(peek);
                peek = '-';
            }
            else // TODO:  wrap number value in token
                return previous = token(token::FLOAT);
        }
        else // TODO:  wrap number value in token
            return previous = token(token::INTEGER);
    }
    if( peek == '\'' ) { // quoted string
        string the_string;
        sip();
        while( peek != '\'' ) { // consume string until closing quote
            the_string += peek;
            sip();
        }
        sip(); // closing quote
        return previous = token(token::STRING);
    }
    if( !is )
        return previous = token(token::EOS);

    throw runtime_error(string("not a valid token: ") + peek);  // TODO:  there must be a better way
}
