#ifndef SRC_SCALAR_HH_
#define SRC_SCALAR_HH_

#include "node.hh"
#include <string>

namespace yaml {

class scalar : public node {
private:
    std::string value;
};

} // namespace yaml

#endif // SRC_SCALAR_HH_
