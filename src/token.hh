#ifndef SRC_TOKEN_HH_
#define SRC_TOKEN_HH_

class token {
public:
    enum lexical_tag {
        NONE = 0,
        ALIAS,                        ANCHOR,
        BLOCK_SEQUENCE_ELEMENT_BEGIN, COMPLEX_MAPPING_KEY,
        DOCUMENT_BEGIN,               DOCUMENT_END,
        ESCAPE_SEQUENCE,              FLOW_MAPPING_END,
        FLOW_MAPPING_START,           FLOW_SEQUENCE_END,
        FLOW_SEQUENCE_START,          FOLDED_STYLE,
        HANDLE,                       IDENTIFIER,
        INTEGER,                      LITERAL_STYLE,
        LOCAL_TAG,                    NIL,
        PAIR_SEPARATOR,               STANDARD_TAG,
        STRING,                       TAG,
        TAG_DIRECTIVE,                URI,
        VERSION_DIRECTIVE
    };

    token();
    token(lexical_tag tag);

    lexical_tag tag;
};

#endif // SRC_TOKEN_HH_
