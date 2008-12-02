#include "yaml_handler.hh"
#include <stdexcept>
#include <iostream>

using namespace std;

void yaml_handler::on_string(const string& s)
{
    if( key.empty() ) key = s;
    else {
        conf.insert(conf_t::value_type(key, s));
        key.clear();
    }
}

void yaml_handler::on_integer(const string& s)
{
    on_string(s);
}

void yaml_handler::on_eos()
{
    if( ! key.empty() ) {
        string error_message("Error:  ");
        error_message += "The configuration variable ";
        error_message += key;
        error_message += " hasn't been assigned a value.";
        throw runtime_error(error_message);
    }
    for( conf_t::const_iterator i(conf.begin()); i != conf.end(); ++i ) {
        conf_t::key_type key = i->first;
        conf_t::mapped_type value = i->second;
        cout << "The value of " << key << " is " << value << endl;
    }
}
