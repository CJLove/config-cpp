# config-cpp

C++ Configuration library patterned after [`viper`](https://github.com/spf13/viper) package for golang.  It is designed to work within an application and handle configuration needs.

- framework for companion libraries which can read from JSON, YAML
- unmarshalling configuration data to native C++ types
- setting defaults
- TODO: reading from environment variables
- TODO: reading from command line flags
- live watching and re-reading of config files or Kubernetes ConfigMaps

ConfigCpp uses the following precedence order.  Each item takes precedence over the item(s) below it:

- TODO: command-line flags
- TODO: environment variables
- configuration file
- default

## Dependencies
- C++14-capable compiler
- GoogleTest (unit testing)
- CMake

Also one or both of the following:
- Json for Modern C++ (https://github.com/nlohmann/json)
- Yaml-cpp (https://github.com/jbeder/yaml-cpp)

## Basic Usage

### Reading Config Files

```c++
ConfigCpp::ConfigCpp config;
config.SetConfigName("config"); // Name of config file minus extension
config.AddConfigPath("/etc/appname/"); // Path to look for config files
config.AddConfigPath("/home/.appname/"); // call as many times as needed
config.AddConfigPath(".");
if (!config.ReadInConfig()) {
    std::cerr << "Failed to read in config\n";
}
```

### Watching and Re-reading Config Files

```c++
// Callback function called upon config file change
void onConfigChange(ConfigCpp::ConfigCpp &config) {
    std::cout << "Config changed\n";
    config.ReadInConfig();
    ...
}

config.OnConfigChange(onConfigChange);
config.WatchConfig();
```

### Setting Defaults

Default values can be specified for keys in case values aren't specified in config file(s), environment variables, or command-line flags.

```c++
config.SetDefault("key1",false);
config.SetDefault("key2",255);
config.SetDefault("key3",0.95);
config.SetDefault("logLevel", "trace");
```

### Environment Variables
TBD

### Command-line Flags
TBD

### Retrieving Configuration Values

```c++
std::string stringVal = config.GetString("key");
bool flagVal = config.GetBool("key.subkey");
int intVal = config.GetInt("key.intVal");
double doubleVal = config.GetDouble("key.double");
```

### Unmarshalling to native types
See the JSON and YAML libraries for requirements.
```c++
JsonType myJsonConfig;
if (config.UnmarshalJson<JsonType>(myJsonConfig)) {
    ...
}

YamlType myYamlConfig;
if (config.UnmarshalYaml<YamlType>(myYamlConfig)) {
    ...
}
```