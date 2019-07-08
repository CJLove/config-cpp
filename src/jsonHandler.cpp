#include <iostream>

#include "jsonHandler.h"
#include "util.h"

using nlohmann::json;

namespace ConfigCpp {

JsonHandler::JsonHandler(const std::string &data, const Values &defaults, const Values &cmdLineArgs) {
    try {
        m_json = json::parse(data);
    } catch (...) {
        throw std::runtime_error("Invalid json received");
    }
    // For defaults, only integrate default values where the specific key isn't
    // part of the config data already
    for (const auto &def : defaults) {
        json node;
        if (!GetNode(def.m_key, node)) {
            AddDefaultNode(def);
        }
    }
    // For command-line arguments these values supercede any values read from
    // config data
    for (const auto &arg : cmdLineArgs) {
        AddDefaultNode(arg);
    }
}

bool JsonHandler::IsSet(const std::string &key) const {
    // return (m_json.count(key) > 0);
    json node;
    return GetNode(key, node);
}

bool JsonHandler::GetBool(const std::string &key) const {
    json node;
    try {
        if (GetNode(key, node)) {
            return node.get<bool>();
        }
    } catch (...) {
    }
    return false;
}

int JsonHandler::GetInt(const std::string &key) const {
    json node;
    try {
        if (GetNode(key, node)) {
            return node.get<int>();
        }
    } catch (...) {
    }
    return 0;
}

double JsonHandler::GetDouble(const std::string &key) const {
    json node;
    try {
        if (GetNode(key, node)) {
            return node.get<double>();
        }
    } catch (...) {
    }
    return 0.0;
}

std::string JsonHandler::GetString(const std::string &key) const {
    json node;
    try {
        if (GetNode(key, node)) {
            return node.get<std::string>();
        }
    } catch (...) {
    }
    return "";
}

bool JsonHandler::GetNode(const std::string &key, json &node) const {
    auto keys = split(key, '.');
    auto cur = m_json;
    for (const auto &k : keys) {
        if (cur.count(k) > 0) {
            auto next = cur[k];
            cur = next;
        } else {
            return false;
        }
    }
    node = cur;
    return true;
}

bool JsonHandler::AddDefaultNode(const Value &def) {
    auto keys = split(def.m_key, '.');
    if (keys.size() == 1) {
        auto key = keys[0];

        switch (def.m_type) {
            case Value::BOOL:
                m_json[key] = def.m_bool;
                break;
            case Value::INT:
                m_json[key] = def.m_int;
                break;
            case Value::DOUBLE:
                m_json[key] = def.m_double;
                break;
            case Value::STRING:
                m_json[key] = def.m_string;
                break;
        }
    } else {
        // json node;

        // if (GetNode(prefix(def.m_key),node)) {
        //     switch (def.m_type) {
        //         case Value::BOOL:
        //             node[keys[keys.size()-1]] = def.m_bool;
        //             break;
        //         case Value::INT:
        //             node[keys[keys.size()-1]] = def.m_int;
        //             break;
        //         case Value::DOUBLE:
        //             node[keys[keys.size()-1]] = def.m_double;
        //             break;
        //         case Value::STRING:
        //             node[keys[keys.size()-1]] = def.m_string;
        //             break;
        //     }
        //     return true;
        // }
    }
    return false;
}

std::string JsonHandler::GetConfig() const { return m_json.dump(2); }

}  // namespace ConfigCpp