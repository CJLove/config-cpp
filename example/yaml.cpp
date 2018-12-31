#include <iostream>
#include "config-cpp/config-cpp.h"

#include "yaml-config.h"

int main(int argc, char**argv)
{
    ConfigCpp::ConfigCpp config(argc,argv);
    
    config.SetConfigName("config");
    config.AddConfigPath("../inputs/yaml/");
    config.AddConfigPath("../../inputs/yaml/");

    if (config.ReadInConfig()) {
        std::cout << "Read in config successfully:\n" << config.GetConfigData() << "\n";

        YamlConfig configData;
        if (config.UnmarshalYaml<YamlConfig>(configData)) {
            std::cout << "Unmarshalled config successfully:\n" << configData;
        } else {
            std::cout << "Failed to ummarshal config\n";
        }
    } else {
        std::cout << "Failed to read in config\n";
    }

    std::cout << "IsSet(top-string) " << config.IsSet("top-string") << "\n";
    std::cout << "IsSet(nested-dict.key2) " << config.IsSet("nested-dict.key2") << "\n";

}