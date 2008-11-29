#ifndef SRC_PARSER_HH_
#define SRC_PARSER_HH_

#include "scanner.hh"
#include <iosfwd>
#include <tr1/memory>

namespace yaml {

class event_handler;

namespace syn {

class node;
class document_node;
class sequence_node;
class mapping_node;
class string_node;
class pair_node;

class parser {
public:
    parser();
    parser(std::istream& is, std::tr1::shared_ptr<event_handler> handler);

    void parse();

private:
    void sip();
    std::tr1::shared_ptr<document_node> parse_document();
    std::tr1::shared_ptr<sequence_node> parse_sequence();
    std::tr1::shared_ptr<mapping_node> parse_mapping();
    std::tr1::shared_ptr<string_node> parse_string();
    std::tr1::shared_ptr<pair_node> parse_pair();
    void parse_eos();

    lex::scanner s;
    lex::token peek;
    std::tr1::shared_ptr<node> tree;
    std::tr1::shared_ptr<event_handler> handler;
};

} // namespace syn
} // namespace yaml

#endif // SRC_PARSER_HH_
