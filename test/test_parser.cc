#include <gtest/gtest.h>
#include <tr1/memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include "../src/parser.hh"
#include "test_basic_input.hh"
#include "../src/event_handler.hh"

using namespace std;
using namespace std::tr1;
using namespace yaml;
using namespace yaml::syn;

struct test_handler : public event_handler {
    void on_string(const string&);
    void on_integer(const string&);
    void on_sequence_begin();
    void on_mapping_begin();
    void on_sequence_end();
    void on_mapping_end();
    void on_document();
    void on_pair();
    void on_eos();

    string str;
};

void test_handler::on_string(const string& str)
{
    this->str += str;
    this->str += ' ';
}

void test_handler::on_integer(const string& str)
{
    this->str += str;
    this->str += ' ';
}

void test_handler::on_sequence_begin()
{
    this->str += "[ ";
}

void test_handler::on_mapping_begin()
{
    this->str += "{ ";
}

void test_handler::on_sequence_end()
{
    this->str += "] ";
}

void test_handler::on_mapping_end()
{
    this->str += "} ";
}

void test_handler::on_document()
{
    this->str += "---\n";
}

void test_handler::on_pair()
{
    this->str += "<pair> ";
}

void test_handler::on_eos()
{
    this->str += "\nEND";
}

TEST(test_parser, parse_document_begin)
{
    istringstream is("--- ");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    p.parse();
    EXPECT_EQ("---\n\nEND", handler->str);
}

TEST(test_parser, parse_document1)
{
    istringstream is("---\n[a, b, and c]");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    p.parse();
    EXPECT_EQ("---\n[ a b and c ] \nEND", handler->str);
}

TEST(test_parser, parse_string)
{
    istringstream is("abc");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_integer)
{
    istringstream is("44");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_sequence_begin)
{
    istringstream is("[");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_THROW(p.parse(), runtime_error);
}

TEST(test_parser, parse_empty_sequence)
{
    istringstream is("[]");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping_begin)
{
    istringstream is("{");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_THROW(p.parse(), runtime_error);
}

TEST(test_parser, parse_empty_mapping)
{
    istringstream is("{}");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_sequence)
{
    istringstream is("[a, b, c]");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_nested_sequence)
{
    istringstream is("[[b], c]");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_nested_sequence2)
{
    istringstream is("[a, [b], c]");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping)
{
    istringstream is("{a: b, c: d}");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping2)
{
    istringstream is("{a: b, c: [a, b, c]}");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping3)
{
    istringstream is("{a: b, c: [A, [B], {a: z}, X]}");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping4)
{
    istringstream is("{nice: {hello: world, goodbye: world}, a: b, c: [A, [B], {a: z}, X]}");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping5)
{
    istringstream is("{a: b, c: [A, [1, 2, B], {a: z}, X, 012, {0: a, 1: 2, a: 12, le: gt}]}");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping_with_number_values)
{
    istringstream is("{a: 0, b: 1}");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping_with_number_keys)
{
    istringstream is("{0: a, 1: b}");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_mapping_with_number_keys_values)
{
    istringstream is("{0: 1, 1: 2}");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_numbers_sequence)
{
    istringstream is("[0, 1, 2, 3]");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    p.parse();
}

TEST(test_parser, parse_nested_document)
{
    istringstream is("---\nhi\n---\nbye");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_nested_document_empty)
{
    istringstream is("---\n---\nbye");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}

TEST(test_parser, parse_nested_document_no_first_document_begin)
{
    istringstream is("hi\n---\nbye");
    shared_ptr<test_handler> handler(new test_handler());
    parser p(is, handler);
    EXPECT_NO_THROW(p.parse());
}
