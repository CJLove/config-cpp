#pragma once

#include "default.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

namespace ConfigCpp {

class JsonHandler {
public:
    JsonHandler(const std::string &data, const DefaultValues &defaults);

    ~JsonHandler();

    bool IsSet(const std::string &key) const;

    bool GetBool(const std::string &key) const;
    int GetInt(const std::string &key) const;
    double GetDouble(const std::string &key) const;
    std::string GetString(const std::string &key) const;

private:
    bool GetNode(const std::string &key, json &node) const;

    bool AddDefaultNode(const DefaultValue &def);

    json m_json;
};

}