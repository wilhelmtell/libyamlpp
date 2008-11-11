#include "scanner.hh"
#include <iostream>

using namespace std;

scanner::scanner() : is(cin), line_number(1), peek(' ')
{
}

token scanner::scan()
{
    // skip whitespace
    while( true ) {
        if( peek == '\n' )
            ++line_number;
        else if( peek != ' ' && peek != '\t' )
            break;
        peek = is.get();
        if( ! is ) break;
    }

    char ch = peek;
    return token();
}
