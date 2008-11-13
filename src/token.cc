#include "token.hh"

token::token() : tag(token::EOS)
{
}

token::token(lexical_tag tag) : tag(tag)
{
}
