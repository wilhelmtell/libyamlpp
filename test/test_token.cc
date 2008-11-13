#include <UnitTest++.h>
#include "../src/token.hh"

using namespace std;

TEST(TokenDefaultConstructor)
{
    token t;
    CHECK_EQUAL(token::EOS, t.tag);
}

TEST(TokenLexicalTagConstructor)
{
    token t(token::FLOW_SEQUENCE_BEGIN);
    CHECK_EQUAL(token::FLOW_SEQUENCE_BEGIN, t.tag);
}
