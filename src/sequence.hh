#ifndef SRC_SEQUENCE_HH_
#define SRC_SEQUENCE_HH_

#include "node.hh"
#include <vector>
#include <tr1/memory>

namespace yaml {

class sequence : public node {
private:
    std::vector<std::tr1::shared_ptr<node> > elements;
};

} // namespace yaml

#endif // SRC_SEQUENCE_HH_
