#include "token.hh"

token::token() : tag(NONE)
{
}

token::token(lexical_tag tag) : tag(tag)
{
}
