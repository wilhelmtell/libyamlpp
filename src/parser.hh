#ifndef SRC_PARSER_HH_
#define SRC_PARSER_HH_

#include "scanner.hh"
#include <iosfwd>

class parser {
public:
    parser();
    parser(std::istream& is);

    void parse();

private:
    void sip();
    void parse_document();
    void parse_sequence();
    void parse_mapping();
    void parse_string();
    void parse_sequence_nodes();
    void parse_mapping_nodes();
    void parse_pair();
    void parse_eos();

    scanner s;
    token peek;
};

#endif // SRC_PARSER_HH_
