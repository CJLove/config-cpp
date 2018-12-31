#include "jsonHandler.h"
#include "util.h"

namespace ConfigCpp {

JsonHandler::JsonHandler(const std::string &data) { m_json = json::parse(data); }

JsonHandler::~JsonHandler() {}

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
    auto next = cur;
    for (const auto &k : keys) {
        if (cur.count(k)) {
            next = cur[k];
            cur = next;
        } else {
            return false;
        }
    }
    node = cur;
    return true;
}
}  // namespace ConfigCpp