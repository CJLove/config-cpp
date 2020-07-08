#pragma once

#include "toml.hpp"
#include "value.h"

namespace ConfigCpp {

class TomlHandler {
public:
    TomlHandler(const std::string &data, const Values &defaults, const Values &cmdLineArgs);

    TomlHandler(const TomlHandler &rhs) = default;

    TomlHandler(TomlHandler &&rhs) = default;

    ~TomlHandler() = default;

    TomlHandler &operator=(const TomlHandler &rhs) = default;

    TomlHandler &operator=(TomlHandler &&rhs) = default;

    bool IsSet(const std::string &key) const;

    bool GetBool(const std::string &key) const;
    int GetInt(const std::string &key) const;
    double GetDouble(const std::string &key) const;
    std::string GetString(const std::string &key) const;

    std::string GetConfig() const;

private:
    // // Recursive function to find a node in a "path"
    // bool GetNode(std::vector<std::string> &keys, const YAML::Node &cur, YAML::Node &node) const;
    bool GetNode(const std::string &key, toml::value &value) const;
    
    bool AddDefaultNode(const Value &def);

    toml::value m_toml;
};

}  // namespace ConfigCpp
