#pragma once

#include "toml.hpp"

namespace ConfigCpp {

namespace TomlImpl {

/**
 * @brief Unmarshal TOML data to a native datatype
 *
 * @tparam T - User-provided datatype
 * @param t - reference to a variable of user-provided datatype
 * @param str - TOML data
 * @return true - Unmarshal is successful
 * @return false - Unmarshal is unsuccessful
 */
template <typename T>
bool Unmarshal(T &t, const std::string &str) {

    try {
        std::istringstream streamData(str, std::ios_base::binary | std::ios_base::in );

        const auto data = toml::parse(streamData,"string");
        t = toml::get<T>(data);
        return true;
    } catch (std::exception &e) {
        return false;
    }
}

}  // namespace TomlImpl
}  // namespace ConfigCpp