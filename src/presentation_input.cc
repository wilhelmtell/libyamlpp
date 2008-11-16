#include "presentation_input.hh"
#include <cassert>
#include <deque>
#include <iostream>

using namespace std;

struct presentation_input::impl {
    impl() : is(cin), peek(' ') { }
    impl(istream& is) : is(is), peek(' ') { }

    typedef deque<char> input_buffer;

    std::istream &is;
    char peek;
    input_buffer buf;
};

presentation_input::presentation_input() : pimpl(new impl())
{
}

presentation_input::~presentation_input()
{
    delete pimpl;
}

presentation_input::presentation_input(istream& is) : pimpl(new impl(is))
{
}

// Fill peek with the next character from the input stream
void presentation_input::sip()
{
    // maintain a buffer so we do io less often
    const impl::input_buffer::size_type BUFFER_SIZE = 4096;
    if( pimpl->buf.empty() )
        for( impl::input_buffer::size_type i = 0; i < BUFFER_SIZE; ++i ) {
            char ch = pimpl->is.get();
            if( ! pimpl->is ) {
                pimpl->buf.push_back(' ');
                break;
            }
            pimpl->buf.push_back(ch);
        }
    pimpl->peek = pimpl->buf.front();
    pimpl->buf.pop_front();
    assert(pimpl->buf.size() <= BUFFER_SIZE - 1);
}
