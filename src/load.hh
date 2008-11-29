#ifndef SRC_LOAD_HH_
#define SRC_LOAD_HH_

#include <iosfwd>
#include <tr1/memory>
#include "event_handler.hh" // #include for user, to allow subclassing

namespace yaml {

class event_handler;

/// Load a stream of YAML data using a handler to handle YAML events.
///
/// The load() function is a convenience function that wraps calls to the
/// libyaml++ parser.
///
/// \param is Input stream of YAML data.
/// \param handler Event handler to handle YAML events.
///
/// \see event_handler
void load(std::istream& is, std::tr1::shared_ptr<event_handler> handler);

} // namespace yaml

#endif // SRC_LOAD_HH_
