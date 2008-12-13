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

    /// Event fired when a string has just been parsed.
    ///
    /// \param string The string parameter is the string parsed.
    virtual void on_string(const std::string&) { }

    /// Event fired when an integer has just been parsed.
    ///
    /// \param string The string parameter is a string representation of the
    /// integer parsed.
    virtual void on_integer(const std::string&) { }

    /// Event fired when a sequence is about to be parsed.
    ///
    /// When the on_sequence_begin() event is fired, the sequence-begin token,
    /// '[', has just been parsed.
    virtual void on_sequence_begin() { }

    /// Event fired when a mapping is about to be parsed.
    ///
    /// When the on_mapping_begin() event is fired, the mapping-begin token,
    /// '{', has just been parsed.
    virtual void on_mapping_begin() { }

    /// Event fired when a sequence has just been parsed.
    ///
    /// When the on_sequence_end() event is fired, the sequence-end token,
    /// ']', has just been parsed.
    virtual void on_sequence_end() { }

    /// Event fired when a mapping has just been parsed.
    ///
    /// When the on_mapping_end() event is fired, the mapping-end token,
    /// '}', has just been parsed.
    virtual void on_mapping_end() { }

    /// Event fired when a document is about to be parsed.
    ///
    /// When the on_document() event is fired, the document-begin token,
    /// '---', has just been parsed, or a the first token of a document is
    /// about to be parsed.
    virtual void on_document() { }

    /// Event fired when a pair is about to be parsed.
    ///
    /// When the on_pair() event is fired, the first element of a pair is about
    /// to be parsed.
    virtual void on_pair() { }

    /// Event fired when a YAML stream has ended.
    ///
    /// This event may be triggered, for instance, when an end-of-file mark was
    /// encountered, or a stream is otherwise ended.
    virtual void on_eos() { }
};

} // namespace yaml

#endif // SRC_EVENT_HANDLER_HH_
