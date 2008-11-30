#ifndef SRC_PAIR_NODE_HH_
#define SRC_PAIR_NODE_HH_

#include "node.hh"
#include "string_node.hh"
#include <tr1/memory>

namespace yaml {
namespace syn {

struct pair_node : public node {
    pair_node(std::tr1::shared_ptr<string_node> key_node,
              std::tr1::shared_ptr<node> value_node) :
        first(key_node), second(value_node) { }

    std::tr1::shared_ptr<string_node> first;
    std::tr1::shared_ptr<node> second;
};

} // namespace syn
} // namespace yaml

#endif /* SRC_PAIR_NODE_HH_ */
