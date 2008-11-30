#ifndef SRC_EVENT_HANDLER_HH_
#define SRC_EVENT_HANDLER_HH_

#include <string>

namespace yaml {

// NOTE:  would it make sense for a handler to handle syntax errors too?

/// The event_handler is the basis for the YAML events dispatching mechanism.
/// When a YAML entity is encountered during the parsing procedure, an
/// appropriate event is fired.  The user then has the chance to handle this
/// event.  For instance, if a sequence is encountered the on_sequence_begin()
/// event is fired.  When the sequence ends the on_sequence_end() event is
/// fired.
///
/// The event_handler base class is designed to be subclassed.  Each event,
/// however, does have a default behaviour, so the user need only handle the
/// events the user is interested in handling.  The default behaviour for
/// unhandled events is to ignore them.
class event_handler {
public:
    virtual ~event_handler() { }

    virtual void on_string(const std::string&) { }
    virtual void on_integer(const std::string&) { }
    virtual void on_sequence_begin() { }
    virtual void on_mapping_begin() { }
    virtual void on_sequence_end() { }
    virtual void on_mapping_end() { }
    virtual void on_document() { }
    virtual void on_pair() { }
    virtual void on_eos() { }
};

} // namespace yaml

#endif // SRC_EVENT_HANDLER_HH_
