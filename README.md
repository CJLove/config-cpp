[![ci](https://github.com/CJLove/config-cpp/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/CJLove/config-cpp/actions/workflows/ci.yml)
![CodeQL](https://github.com/CJLove/config-cpp/workflows/CodeQL/badge.svg)



# config-cpp

C++ Configuration library patterned after [`viper`](https://github.com/spf13/viper) package for golang.  It is designed to work within an application and handle configuration needs.

- framework for companion libraries which can read from JSON, YAML, TOML
- unmarshalling configuration data to native C++ types where supported
- setting defaults
- TODO: reading from environment variables
- reading from command line flags
- live watching and re-reading of config files or Kubernetes ConfigMaps

ConfigCpp uses the following precedence order.  Each item takes precedence over the item(s) below it:

- command-line flags
- TODO: environment variables
- configuration file
- default

## Dependencies
- C++14-capable compiler
- GoogleTest (unit testing)
- CMake
- cxxopts (https://github.com/jarro2783/cxxopts) version 3.1.0 or later

Also one or more of the following:
- Json for Modern C++ (https://github.com/nlohmann/json)
- Yaml-cpp (https://github.com/jbeder/yaml-cpp)
- Toml11 (https://github.com/ToruNiina/toml11)

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
Command line flags can be added as follows, following the `cxxopts` convention of specifying the long name optionally preceded by the short name separated by a comma.

```c++
config.AddBoolOption("b,top-bool","Bool option");
config.AddIntOption("t,top-int","Integer option");
config.AddStringOption("s,top-string","String option");
config.AddDoubleOption("d,top-double","Double option");
config.AddStringOption("long-only","String option w/no short name");
```
By default a `--help` option is added, which will result in displaying the help output from `cxxopts` and then exiting with return code 1.  Errors encountered while parsing command-line arguments will result displaying the error message and help output from `cxxopts` and then exiting with return code 1.


### Retrieving Configuration Values

```c++
std::string stringVal = config.GetString("key");
bool flagVal = config.GetBool("key.subkey");
int intVal = config.GetInt("key.intVal");
double doubleVal = config.GetDouble("key.double");
```

### Unmarshalling to native types
See the JSON, YAML and TOML libraries for requirements.
```c++
JsonType myJsonConfig;
if (config.UnmarshalJson<JsonType>(myJsonConfig)) {
    ...
}

YamlType myYamlConfig;
if (config.UnmarshalYaml<YamlType>(myYamlConfig)) {
    ...
}

TomlType myTomlConfig;
if (config.UnmarshalToml<TomlType>(myTomlConfig)) {
    ...
}
```
