// Sample application using ConfigCpp to handle json-based configuration file
// and handling updates to the config file.

#include <iostream>
#include <unistd.h>

#include "config-cpp/config-cpp.h"

void onConfigChange(ConfigCpp::ConfigCpp &config) {
    config.ReadInConfig();
    std::cout << "in onConfigChange().  Updated config is:\n" << config.GetConfigData() << "\n";
}

int main(int argc, char**argv)
{
    ConfigCpp::ConfigCpp config(argc,argv);
    
    config.SetConfigName("config");
    config.AddConfigPath("/tmp/config/");
    config.AddConfigPath("/etc/config/");
    
    if (config.ReadInConfig()) {
        std::cout << "Read in config successfully:\n" << config.GetConfigData() << "\n";

        config.OnConfigChange(onConfigChange);
        config.WatchConfig();
        std::cout << "Waiting for config changes\n";

        while (true) {
            sleep(1);
        }
    } else {
        std::cout << "Failed to read in config\n";
    }

}