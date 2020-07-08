#include "tomlHandler.h"
#include "util.h"

namespace ConfigCpp {

TomlHandler::TomlHandler(const std::string &data, const Values &defaults, const Values &cmdLineArgs) {
    try {
        std::istringstream streamData(data, std::ios_base::binary | std::ios_base::in );

        m_toml = toml::parse(streamData,"string");

    } catch (...) {
        throw std::runtime_error("Invalid toml received");
    }
    // For defaults, only integrate default values where the specific key isn't
    // part of the config data already
    for (const auto &def : defaults) {
        toml::value value;
        if (!GetNode(def.m_key, value)) {
            AddDefaultNode(def);
        }
    }
    // For command-line arguments these values supercede any values read from
    // config data
    for (const auto &arg : cmdLineArgs) {
         AddDefaultNode(arg);
    }

}

bool TomlHandler::IsSet(const std::string &key) const
{
    toml::value value;
    return GetNode(key,value);
}

bool TomlHandler::GetBool(const std::string &key) const
{
    toml::value value;
    try {
        if (GetNode(key, value)) {
            if (value.is_boolean()) {
                return value.as_boolean();
            }
        }
    } catch (...) {
    }
    return false;
}
    
int TomlHandler::GetInt(const std::string &key) const
{
    toml::value value;
    try {
        if (GetNode(key, value)) {
            if (value.is_integer()) {
                return static_cast<int>(value.as_integer());
            } else if (value.is_boolean()) {
                return value.as_boolean() ? 1 : 0;
            } else if (value.is_floating()) {
                return static_cast<int>(value.as_floating());
            }
        }
    } catch (...) {
    }
    return 0;
}

double TomlHandler::GetDouble(const std::string &key) const
{
    toml::value value;
    try {
        if (GetNode(key, value)) {
            if (value.is_floating()) {
                return value.as_floating();
            } else if (value.is_integer()) {
                return static_cast<double>(value.as_integer());
            }
        }
    } catch (...) {
    }
    return 0.0;
}
    
std::string TomlHandler::GetString(const std::string &key) const
{
    toml::value value;
    try {
        if (GetNode(key, value)) {
            if (value.is_string()) {
                return value.as_string();
            }
        }
    } catch (...) {
    }
    return "";
}

bool TomlHandler::GetNode(const std::string &key, toml::value &value) const
{
    auto keys = split(key, '.');
    auto cur = m_toml;
    for (const auto &k: keys) {
        try {
            auto next = toml::find(cur,k);
            cur = next;
        }
        catch (...) {
            return false;
        }
    }
    value = cur;
    return true;
}

bool TomlHandler::AddDefaultNode(const Value &def)
{
   auto keys = split(def.m_key, '.');
    if (keys.size() == 1) {
        auto key = keys[0];

        switch (def.m_type) {
            case Value::BOOL:
                m_toml[key] = def.m_bool;
                break;
            case Value::INT:
                m_toml[key] = def.m_int;
                break;
            case Value::DOUBLE:
                m_toml[key] = def.m_double;
                break;
            case Value::STRING:
                m_toml[key] = def.m_string;
                break;
        }
        return true;
    }
    return false;     
}

std::string TomlHandler::GetConfig() const
{ 
    return toml::format(m_toml); 
}

} // Namespace ConfigCpp