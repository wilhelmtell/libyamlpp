#ifndef SRC_TOKEN_HH_
#define SRC_TOKEN_HH_

// TODO:  maybe parameterize token, so that some tokens have values?  for
// example, token<token::INTEGER> will hold the integer value.
class token {
public:
    enum lexical_tag {
        UNDEFINED = -1,
        // ALIAS,
        // ANCHOR,
        BLOCK_SEQUENCE_ELEMENT_BEGIN,
        // COMPLEX_MAPPING_KEY,
        // DOCUMENT_BEGIN,
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

    lexical_tag tag;
};

bool operator==(const token& t1, const token& t2);
bool operator!=(const token& t1, const token& t2);

#endif // SRC_TOKEN_HH_
