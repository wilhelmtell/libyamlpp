#include "token.hh"

using namespace std;

namespace yaml {
namespace lex {

token::token() : tag(token::EOS)
{
}

token::token(lexical_tag tag) : tag(tag)
{
}

token::token(lexical_tag tag, const string& value) : tag(tag), value(value)
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

} // namespace lex
} // namespace yaml
