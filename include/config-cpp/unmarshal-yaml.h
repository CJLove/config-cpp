#pragma once

#include "yaml-cpp/yaml.h"

namespace ConfigCpp {

namespace YamlImpl {

/**
 * @brief Unmarshal YAML data to a native datatype
 *
 * @tparam T - User-provided datatype
 * @param t - reference to a variable of user-provided datatype
 * @param str - YAML data
 * @return true - Unmarshal is successful
 * @return false - Unmarshal is unsuccessful
 */
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