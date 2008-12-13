#ifndef SRC_NULL_NODE_HH_
#define SRC_NULL_NODE_HH_

#include "node.hh"

namespace yaml {
namespace syn {

/// Represent a null node in the syntax tree of YAML data.
struct null_node : public node {
};

} // namespace syn
} // namespace yaml

#endif /* SRC_NULL_NODE_HH_ */
