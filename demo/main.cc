#include <yaml++.hh>
#include <parser.hh>
#include <event_handler.hh>
#include <iostream>
#include <sstream>

using namespace std;

class my_handler : public yaml::event_handler {
    void on_string(const std::string&) { cout << "parsed string." << endl; }
    void on_integer(const std::string&) { cout << "parsed integer." << endl; }
    void on_sequence_begin() { cout << "parsed sequence_begin." << endl; }
    void on_mapping_begin() { cout << "parsed mapping_begin." << endl; }
    void on_sequence_end() { cout << "parsed sequence_end." << endl; }
    void on_mapping_end() { cout << "parsed mapping_end." << endl; }
    void on_document() { cout << "parsed document." << endl; }
    void on_pair() { cout << "parsed pair." << endl; }
    void on_eos() { cout << "parsed eos." << endl; }
};

int main(int argc, char* argv[])
{
    string yaml_input("---  [a, b, c]");
    cout << "input:  " << yaml_input << endl;
    stringstream s(yaml_input);
    my_handler* h = new my_handler();
    yaml::syn::parser p(s, h);
    p.parse();
    return 0;
}
