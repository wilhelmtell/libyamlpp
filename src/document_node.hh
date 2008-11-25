#ifndef SRC_DOCUMENT_NODE_HH_
#define SRC_DOCUMENT_NODE_HH_

#include "node.hh"
#include <tr1/memory>

namespace yaml {

struct document_node : public node {
    document_node() { }
    document_node(std::tr1::shared_ptr<node> contents) : contents(contents) { }

    std::tr1::shared_ptr<node> contents;
};

} // namespace yaml

#endif /* SRC_DOCUMENT_NODE_HH_ */
