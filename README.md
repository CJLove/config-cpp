# config-cpp

C++ Configuration library patterned after [`viper`](https://github.com/spf13/viper) package for golang.  It is designed to work within an application and handle configuration needs.

- framework for companion libraries which can read from JSON, YAML
- unmarshaling configuration data to native C++ types
- TODO: setting defaults
- TODO: reading from environment variables
- TODO: reading from command line flags
- TODO: live watching and re-reading of config files or Kubernetes ConfigMaps

## Dependencies
- C++14-capable compiler
- GoogleTest (unit testing)
- CMake

Also one or both of the following:
- Json for Modern C++ (https://github.com/nlohmann/json)
- Yaml-cpp (https://github.com/jbeder/yaml-cpp)



