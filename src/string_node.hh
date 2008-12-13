#ifndef SRC_STRING_NODE_HH_
#define SRC_STRING_NODE_HH_

#include "node.hh"
#include <string>

namespace yaml {
namespace syn {

/// Represent a string node in the syntax tree of YAML data.
struct string_node : public node {
    string_node(const std::string& value) : value(value) { }
    std::string value;
};

} // namespace syn
} // namespace yaml

#endif /* SRC_STRING_NODE_HH_ */
