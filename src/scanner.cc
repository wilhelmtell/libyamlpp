#include "scanner.hh"
#include <cctype>
#include <iostream>
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
    const queue<char>::size_type BUFFER_SIZE = 4096;
    if( buf.empty() )
        for( queue<char>::size_type i = 0; i < BUFFER_SIZE; ++i ) {
            char ch = is.get();
            if( ! is ) {
                buf.push(-1);
                break;
            }
            buf.push(ch);
        }
    peek = buf.front();
    buf.pop();
    assert(buf.size() <= BUFFER_SIZE - 1);
}

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
        if( is.eof() ) return token(token::EOS);
    }

    if( peek == '[' ) {
        sip();
        previous = token(token::FLOW_SEQUENCE_BEGIN);
    }
    else if( peek == ']' ) {
        sip();
        previous = token(token::FLOW_SEQUENCE_END);
    }
    else if( peek == '{' ) {
        sip();
        previous = token(token::FLOW_MAPPING_BEGIN);
    }
    else if( peek == '}' ) {
        sip();
        previous = token(token::FLOW_MAPPING_END);
    }

    return previous;
}
