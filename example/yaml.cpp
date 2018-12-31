// Sample application using ConfigCpp to handle yaml-based configuration file

#include <iostream>
#include "config-cpp/config-cpp.h"

// User-provided type corresponding to yaml configuration data
#include "yaml-config.h"

int main(int argc, char**argv)
{
    ConfigCpp::ConfigCpp config(argc,argv);
    
    // Look for "config.{yaml|yml}" in the following directories
    config.SetConfigName("config");
    config.AddConfigPath("../inputs/yaml/");
    config.AddConfigPath("../../inputs/yaml/");

    if (config.ReadInConfig()) {
        std::cout << "Read in config successfully:\n" << config.GetConfigData() << "\n";

        // Unmarshal all config data to user-provided type
        YamlConfig configData;
        if (config.UnmarshalYaml<YamlConfig>(configData)) {
            std::cout << "Unmarshalled config successfully:\n" << configData;
        } else {
            std::cout << "Failed to ummarshal config\n";
        }

        std::cout << "IsSet(top-string) " << config.IsSet("top-string") << "\n";

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