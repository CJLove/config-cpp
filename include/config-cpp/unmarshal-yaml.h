#pragma once

#include "yaml-cpp/yaml.h"

namespace ConfigCpp {

namespace YamlImpl {

template <typename T>
bool Unmarshal(T &t, const std::string &str) {

    auto yaml = YAML::Load(str);
    try {
        t = yaml.as<T>();
        return true;
    } catch (std::exception &e) {
        return false;
    }
}

}  // namespace YamlImpl
}  // namespace ConfigCpp