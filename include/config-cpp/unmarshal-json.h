#pragma once

#include "nlohmann/json.hpp"

using nlohmann::json;

namespace ConfigCpp {
namespace JsonImpl {

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