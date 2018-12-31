#pragma once

#include "nlohmann/json.hpp"

using nlohmann::json;

namespace ConfigCpp {
namespace JsonImpl {

/**
 * @brief Unmarshal JSON data to a native datatype
 *
 * @tparam T - user-provided data type
 * @param t - reference to a variable of user-provided data type
 * @param str - string containing JSON data
 * @return true - unmarshal is successful
 * @return false - unmarshal is unsuccessful
 */
template <typename T>
bool Unmarshal(T &t, const std::string &str) {
    auto jsonData = json::parse(str);
    try {
        t = jsonData;
        return true;
    } catch (std::exception &e) {
        return false;
    }
}

}  // namespace JsonImpl
}  // namespace ConfigCpp