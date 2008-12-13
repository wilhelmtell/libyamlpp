#ifndef SRC_MAPPING_NODE_HH_
#define SRC_MAPPING_NODE_HH_

#include "node.hh"
#include <tr1/memory>
#include <list>

namespace yaml {
namespace syn {

/// Represent a mapping node in the syntax tree of YAML data.
struct mapping_node : public node {
    // typedef map<shared_ptr<node>, shared_ptr<node> > value_type;
    typedef std::list<std::tr1::shared_ptr<node> > value_type; // serialization
    value_type elements;
};

} // namespace syn
} // namespace yaml

#endif /* SRC_MAPPING_NODE_HH_ */
