#ifndef SRC_SEQUENCE_NODE_HH_
#define SRC_SEQUENCE_NODE_HH_

#include "node.hh"
#include <tr1/memory>
#include <list>

namespace yaml {
namespace syn {

/// Represent a sequence node in the syntax tree of YAML data.
struct sequence_node : public node {
    typedef std::list<std::tr1::shared_ptr<node> > value_type;
    value_type elements;
};

} // namespace syn
} // namespace yaml

#endif /* SRC_SEQUENCE_NODE_HH_ */
