#ifndef DEMO_CONFIGURATION_YAML_HANDLER_HH_
#define DEMO_CONFIGURATION_YAML_HANDLER_HH_

#include <iyaml++.hh>
#include <string>
#include <map>

class yaml_handler : public yaml::event_handler {
    void on_string(const std::string& s);
    void on_integer(const std::string& s);
    void on_eos();

    typedef std::map<std::string,std::string> conf_t;

    std::string key;
    conf_t conf;
};

#endif // DEMO_CONFIGURATION_YAML_HANDLER_HH_
