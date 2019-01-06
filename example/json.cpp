// Sample application using ConfigCpp to handle json-based configuration file

#include <iostream>
#include "config-cpp/config-cpp.h"

// User-provided type corresponding to json configuration data

#include "json-config.h"

int main(int argc, char**argv)
{
    ConfigCpp::ConfigCpp config(argc,argv);
    
    config.SetConfigName("config");
    config.AddConfigPath("../inputs/json/");
    config.AddConfigPath("../../inputs/json/");

    config.AddBoolOption("b,top-bool","Bool option");
    config.AddIntOption("t,top-int","Integer option");
    config.AddStringOption("s,top-string","String option");
    config.AddDoubleOption("d,top-double","Double option");

    if (config.ReadInConfig()) {
        std::cout << "Read in config successfully:\n" << config.GetConfigData() << "\n";

        // Unmarshal all config data to user-provided type
        JsonConfig configData;
        if (config.UnmarshalJson<JsonConfig>(configData)) {
            std::cout << "Unmarshalled config successfully:\n" << configData;
        } else {
            std::cout << "Failed to ummarshal config\n";
        }

        // Retrieve specific config values read from yaml:
        auto stringVal = config.GetString("top-string");
        std::cout << "StringVal: " << stringVal << "\n";
        auto intVal = config.GetInt("top-int");
        std::cout << "IntVal: " << intVal << "\n";
        auto dictVal = config.GetString("nested-dict.key2.key2-subkey2");
        std::cout << "DictVal: " << dictVal << "\n";
    } else {
        std::cout << "Failed to read in config\n";
    }

}