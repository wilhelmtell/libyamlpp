#include "token.hh"

token::token() : tag(token::EOS)
{
}

token::token(lexical_tag tag) : tag(tag)
{
}

bool operator==(const token& t1, const token& t2)
{
    return t1.tag == t2.tag;
}

bool operator!=(const token& t1, const token& t2)
{
    return ! (t1 == t2);
}
