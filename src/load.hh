#ifndef SRC_LOAD_HH_
#define SRC_LOAD_HH_

#include <iosfwd>
#include <tr1/memory>
#include "event_handler.hh" // #include for user, to allow subclassing

namespace yaml {

class event_handler;

void load(std::istream& is, std::tr1::shared_ptr<event_handler> handler);

} // namespace yaml

#endif // SRC_LOAD_HH_
