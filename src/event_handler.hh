#ifndef SRC_EVENT_HANDLER_HH_
#define SRC_EVENT_HANDLER_HH_

#include <string>

namespace yaml {

// NOTE:  would it make sense for a handler to handle syntax errors too?
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
