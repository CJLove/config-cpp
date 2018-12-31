#pragma once

#include "default.h"
#include "yaml-cpp/yaml.h"

namespace ConfigCpp {

class YamlHandler {
public:
    YamlHandler(const std::string &data, const DefaultValues &defaults);

    ~YamlHandler();

    bool IsSet(const std::string &key) const;

    bool GetBool(const std::string &key) const;
    int GetInt(const std::string &key) const;
    double GetDouble(const std::string &key) const;
    std::string GetString(const std::string &key) const;

private:
    // Recursive function to find a node in a "path"
    bool GetNode(std::vector<std::string> &keys, const YAML::Node &cur, YAML::Node &node) const;

    bool AddDefaultNode(const DefaultValue &def);

    YAML::Node m_yaml;
};

}