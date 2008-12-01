#include <iyaml++.hh>
#include "yaml_handler.hh"
#include <tr1/memory>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    if( argc < 2 ) {
        cout << "Usage:  " << argv[0] << " <conf-file>" << endl;
        cout << endl;
        cout << "Try passing in configuration.yaml, supplied in the" << endl;
        cout << "directory of the source files of this binary." << endl;
        return 1;
    }

    tr1::shared_ptr<yaml_handler> handler(new yaml_handler());
    ifstream configuration_file(argv[1]);
    try {
        if( ! configuration_file.is_open() )
            throw runtime_error("Error:  Can't open configration file.");
        yaml::load(configuration_file, handler); // throws on syntax error
    }
    catch( const runtime_error& e ) {
        cerr << e.what() << endl;
    }

    return 0;
}
