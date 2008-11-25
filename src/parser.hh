#ifndef SRC_PARSER_HH_
#define SRC_PARSER_HH_

#include "scanner.hh"
#include <iosfwd>
#include <tr1/memory>

namespace yaml {

class node;
class document_node;
class sequence_node;
class mapping_node;
class string_node;
class pair_node;

class parser {
public:
    parser();
    parser(std::istream& is);

    void parse();

private:
    void sip();
    std::tr1::shared_ptr<document_node> parse_document();
    std::tr1::shared_ptr<sequence_node> parse_sequence();
    std::tr1::shared_ptr<mapping_node> parse_mapping();
    std::tr1::shared_ptr<string_node> parse_string();
    std::tr1::shared_ptr<sequence_node> parse_sequence_nodes();
    std::tr1::shared_ptr<sequence_node> parse_mapping_nodes(); // serialization
    std::tr1::shared_ptr<pair_node> parse_pair();
    void parse_eos();

    scanner s;
    token peek;
    std::tr1::shared_ptr<node> tree;
};

} // namespace yaml

#endif // SRC_PARSER_HH_
