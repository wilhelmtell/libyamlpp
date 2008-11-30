#ifndef SRC_TOKEN_HH_
#define SRC_TOKEN_HH_

#include <string>

namespace yaml {
namespace lex {

// TODO:  maybe parameterize token, so that some tokens have values?  for
// example, token<token::INTEGER> will hold the integer value.

/// The token class holds token identification data for the scanner.  The
/// parser later uses tokens to do the parsing task.  The user should not be
/// exposed to this internal detail;  instead, the user should only read YAML
/// streams using the load() function.
class token {
public:
    // all tokens i plan on implementing are listed below.  those already
    // implemented are uncommented.
    enum lexical_tag { ///< holds the token identification data
        UNDEFINED = -1,
        // ALIAS,
        // ANCHOR,
        BLOCK_SEQUENCE_ELEMENT_BEGIN,
        // COMPLEX_MAPPING_KEY,
        DOCUMENT_BEGIN,
        // DOCUMENT_END,
        EOS,
        // ESCAPE_SEQUENCE,
        FLOAT,
        FLOW_MAPPING_BEGIN,
        FLOW_MAPPING_END,
        FLOW_SEQUENCE_BEGIN,
        FLOW_SEQUENCE_END,
        // FOLDED_STYLE,
        // HANDLE,
        // IDENTIFIER,
        INTEGER,
        // LITERAL_STYLE,
        // LOCAL_TAG,
        // NIL,
        PAIR_SEPARATOR,
        SEQUENCE_SEPARATOR,
        // STANDARD_TAG,
        STRING
        // TAG,
        // TAG_DIRECTIVE,
        // URI,
        // VERSION_DIRECTIVE
    };

    token();
    token(lexical_tag tag);
    token(lexical_tag tag, const std::string& value);

    lexical_tag tag;

    /// Some tokens have a value (STRING, INTEGER, ...).  Tokens for which the
    /// value makes no sense (DOCUMENT_BEGIN, PAIR_SEPARATOR, ...) merely
    /// ignore the value.
    std::string value;
};

/// Convenience comparison operators.  Allow for comparing tokens, token tags
/// and mixtures thereof.
bool operator==(const token& t1, const token& t2);
bool operator!=(const token& t1, const token& t2);

} // namespace lex
} // namespace yaml

#endif // SRC_TOKEN_HH_
