#include <iostream>

#include "util.h"
#include "yamlHandler.h"

using namespace YAML;

namespace ConfigCpp {

YamlHandler::YamlHandler(const std::string &data) : m_yaml(YAML::Load(data)) {}

YamlHandler::~YamlHandler() {}

bool YamlHandler::IsSet(const std::string &key) const {
    YAML::Node node;
    auto keys = split(key, '.');
    return GetNode(keys, m_yaml, node);
}

bool YamlHandler::GetBool(const std::string &key) const {
    Node node;
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
    Node node;
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
    Node node;
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
    Node node;
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

}  // namespace ConfigCpp