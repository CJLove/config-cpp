#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

// Sample user-provided datatype matching ../inputs/yaml/config.yaml
//
// See https://github.com/jbeder/yaml-cpp/wiki/Tutorial for converting to/from native types

struct YamlConfig {
    std::string top_string;
    int top_int = 0;
    bool top_bool = false;

    std::vector<std::string> top_list;

    std::map<std::string, std::string> top_dict;

    std::vector<std::vector<std::string>> nested_list;

    std::map<std::string, std::map<std::string, std::string>> nested_dict;

    bool operator==(const YamlConfig &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const YamlConfig &s);

namespace YAML {
template <>
struct convert<YamlConfig> {
    static Node encode(__attribute__((unused)) const YamlConfig &rhs) {
        const Node node;
        return node;
    }

    static bool decode(const Node &node, YamlConfig &rhs) {

        if (node["top-string"]) {
            try {
                rhs.top_string = node["top-string"].as<std::string>();
            } catch (...) {
            }
        }
        if (node["top-int"]) {
            try {
                rhs.top_int = node["top-int"].as<int>();
            } catch (...) {
            }
        }
        if (node["top-bool"]) {
            try {
                rhs.top_bool = node["top-bool"].as<bool>();
            } catch (...) {
            }
        }
        if (node["top-list"]) {
            try {
                for (const auto &item : node["top-list"]) {
                    rhs.top_list.push_back(item.as<std::string>());
                }
            } catch (...) {
            }
        }
        if (node["top-dict"]) {
            try {
                for (const auto &item : node["top-dict"]) {
                    rhs.top_dict[item.first.as<std::string>()] = item.second.as<std::string>();
                }
            } catch (...) {
            }
        }
        if (node["nested-list"]) {
            try {
                for (const auto &item : node["nested-list"]) {
                    std::vector<std::string> l;
                    for (const auto &subitem : item) {
                        l.push_back(subitem.as<std::string>());
                    }
                    rhs.nested_list.push_back(l);
                }
            } catch (...) {
            }
        }
        if (node["nested-dict"]) {
            try {
                for (const auto &item : node["nested-dict"]) {
                    std::map<std::string, std::string> m;
                    for (const auto &subitem : item.second) {
                        m[subitem.first.as<std::string>()] = subitem.second.as<std::string>();
                    }
                    rhs.nested_dict[item.first.as<std::string>()] = m;
                }
            } catch (...) {
            }
        }
        return true;
    }
};
}  // namespace YAML