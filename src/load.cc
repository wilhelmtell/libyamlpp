#include "load.hh"
#include "parser.hh"
#include <iosfwd>

using namespace std;
using namespace std::tr1;

namespace yaml {

void load(std::istream& input_stream, shared_ptr<event_handler> handler)
{
    syn::parser parser(input_stream, handler);
    parser.parse();
}

} // namespace yaml
