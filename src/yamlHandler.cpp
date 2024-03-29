#include <iostream>

#include "util.h"
#include "yamlHandler.h"

namespace ConfigCpp {

YamlHandler::YamlHandler(const std::string &data, const Values &defaults, const Values &cmdLineArgs)
    : m_yaml(YAML::Load(data)) {
    try {
        m_yaml = YAML::Load(data);
    } catch (...) {
        throw std::runtime_error("Invalid YAML received");
    }
    // For defaults, only integrate default values where the specific key isn't
    // part of the config data already
    for (const auto &def : defaults) {
        YAML::Node node;
        auto keys = split(def.m_key, '.');
        if (!GetNode(keys, m_yaml, node)) {
            AddDefaultNode(def);
        }
    }
    // For command-line arguments these values supercede any values read from
    // config data
    for (const auto &arg : cmdLineArgs) {
        AddDefaultNode(arg);
    }
}

bool YamlHandler::IsSet(const std::string &key) const {
    YAML::Node node;
    auto keys = split(key, '.');
    return GetNode(keys, m_yaml, node);
}

bool YamlHandler::GetBool(const std::string &key) const {
    YAML::Node node;
    auto keys = split(key, '.');
    if (GetNode(keys, m_yaml, node)) {
        try {
            return node.as<bool>();
        } catch (...) {
        }
    }
    return false;
}

int YamlHandler::GetInt(const std::string &key) const {
    YAML::Node node;
    auto keys = split(key, '.');
    if (GetNode(keys, m_yaml, node)) {
        try {
            return node.as<int>();
        } catch (...) {
        }
    }
    return 0;
}

double YamlHandler::GetDouble(const std::string &key) const {
    YAML::Node node;
    auto keys = split(key, '.');
    if (GetNode(keys, m_yaml, node)) {
        try {
            return node.as<double>();
        } catch (...) {
        }
    }
    return 0.0;
}

std::string YamlHandler::GetString(const std::string &key) const {
    YAML::Node node;
    auto keys = split(key, '.');
    if (GetNode(keys, m_yaml, node)) {
        try {
            return node.as<std::string>();
        } catch (...) {
        }
    }
    return "";
}

// Note: here there be dragons!
// As noted here (https://stackoverflow.com/questions/43597237/yaml-cpp-modifies-underlying-container-even-for-const-nodes)
// YAML::Node is a reference type, so recursion must be used when navigating over a hierarchy of nodes to avoid modifying
// the nodes as we go.
bool YamlHandler::GetNode(std::vector<std::string> &keys, const YAML::Node &cur, YAML::Node &node) const {
    if (keys.empty()) {
        node = cur;
        return true;
    }
    auto key = keys[0];
    if (cur[key]) {
        keys.erase(keys.begin());
        return GetNode(keys, cur[key], node);
    }
    return false;
}

bool YamlHandler::AddDefaultNode(const Value &def) {
    auto keys = split(def.m_key, '.');
    if (keys.size() == 1) {
        switch (def.m_type) {
            case Value::BOOL:
                m_yaml[keys[keys.size() - 1]] = def.m_bool;
                break;
            case Value::INT:
                m_yaml[keys[keys.size() - 1]] = def.m_int;
                break;
            case Value::DOUBLE:
                m_yaml[keys[keys.size() - 1]] = def.m_double;
                break;
            case Value::STRING:
                m_yaml[keys[keys.size() - 1]] = def.m_string;
                break;
        }
        return true;
    }
    return false;
}

std::string YamlHandler::GetConfig() const {
    std::stringstream stream;
    stream << m_yaml;
    return stream.str();
}

}  // namespace ConfigCpp