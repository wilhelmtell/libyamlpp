#include <iyaml++.hh>
#include "yaml_handler.hh"
#include <tr1/memory>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

///////////////////////////////////////////////////////////////////////////////
// Usage:  demo-configuration [conf_file]
//
// If conf_file is not given or if conf_file is "-" then configuration input is
// taken from stdin.
//
// Try passing in configuration.yaml, supplied in the directory of the source
// files of this binary.
///////////////////////////////////////////////////////////////////////////////

using namespace std;

void process(istream& in)
{
    tr1::shared_ptr<yaml_handler> handler(new yaml_handler());
    if( ! in )
        throw runtime_error("Error:  Can't open configuration stream.");
    yaml::load(in, handler);
}

int main(int argc, char* argv[])
{
    // open file if user supplied one in argv[1], stdin otherwise.
    try { // throws on syntax error or file-not-found
        if( argc < 2 || string(argv[1]) == "-" ) process(cin);
        else {
            ifstream configuration_file(argv[1]);
            process(configuration_file);
        }
    }
    catch( const runtime_error& e ) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
