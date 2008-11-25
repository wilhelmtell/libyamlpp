#ifndef SRC_MAPPING_HH_
#define SRC_MAPPING_HH_

#include "node.hh"
#include <string>
#include <map>
#include <tr1/memory>

namespace yaml {

class mapping : public node {
private:
    std::map<std::string,std::tr1::shared_ptr<node> > pairs;
};

} // namespace yaml

#endif // SRC_MAPPING_HH_
