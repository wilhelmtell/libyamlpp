#include "scanner.hh"
#include <cctype>
#include <iostream>
#include <cassert>

using namespace std;

scanner::scanner() : is(cin), line_number(1), peek(' ')
{
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
    assert(buf.size() <= BUFFER_SIZE);
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
        if( ! is ) return token(); // TODO:  indicate to caller that no-token can be returned
    }

    char ch = peek;
    return previous;
}
