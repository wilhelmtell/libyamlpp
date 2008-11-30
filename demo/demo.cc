#include <iyaml++.hh>
#include <tr1/memory>
#include <iostream>
#include <stdexcept>

using namespace std;

// What should libyaml++ do when a YAML entity is parsed?
// NOTE:  if any of the event handlers is not defined, a respective default
// no-op handler will be used.  For example, not defining on_eos() is
// equivalent to defining void on_eos() { }.
class my_handler : public yaml::event_handler {
    void on_string(const std::string& s) { cout << "parsed string:  " << s << endl; }
    void on_integer(const std::string& s) { cout << "parsed integer:  " << s << endl; }
    void on_sequence_begin() { cout << "parsed sequence-begin." << endl; }
    void on_mapping_begin() { cout << "parsed mapping-begin." << endl; }
    void on_sequence_end() { cout << "parsed sequence-end." << endl; }
    void on_mapping_end() { cout << "parsed mapping-end." << endl; }
    void on_document() { cout << "parsed document." << endl; }
    void on_pair() { cout << "parsed pair." << endl; }
    void on_eos() { cout << "parsed eos." << endl; }
};

// ok then, now that i know how to behave on each YAML entity encountered, just
// give me a stream to parse!
int main(int argc, char* argv[])
{
    tr1::shared_ptr<my_handler> handler(new my_handler());
    while( cin ) {
        try { yaml::load(cin, handler); } // throws on syntax error

        catch( const runtime_error& e ) {
            cerr << e.what() << endl;
        }
    }
    return 0;
}
