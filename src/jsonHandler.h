#pragma once

#include "nlohmann/json.hpp"
#include "value.h"

namespace ConfigCpp {

class JsonHandler {
public:
    JsonHandler(const std::string &data, const Values &defaults, const Values &cmdLineArgs);

    JsonHandler(const JsonHandler &rhs) = default;

    JsonHandler(JsonHandler &&rhs) = default;

    ~JsonHandler() = default;

    JsonHandler &operator=(const JsonHandler &rhs) = default;

    JsonHandler &operator=(JsonHandler &&rhs) = default;

    bool IsSet(const std::string &key) const;

    bool GetBool(const std::string &key) const;
    int GetInt(const std::string &key) const;
    double GetDouble(const std::string &key) const;
    std::string GetString(const std::string &key) const;

    std::string GetConfig() const;

private:
    bool GetNode(const std::string &key, nlohmann::basic_json<> &node) const;

    bool AddDefaultNode(const Value &def);

    nlohmann::basic_json<> m_json;
};

}  // namespace ConfigCpp