#pragma once

#include <string>
#include <functional>
#include <memory>

// Helper function for unmarshaling yaml
#include "config-cpp/unmarshal-yaml.h"

// Helper function for unmarshaling json
#include "config-cpp/unmarshal-json.h"

namespace ConfigCpp {
    class ConfigCpp;

    using Callback = std::function<void(ConfigCpp &)>;

    enum class ConfigType { YAML, JSON, UNKNOWN };

    class ConfigCpp {
    public:
        ConfigCpp(int argc, char **argv);

        ConfigCpp(int argc, char **argv, const std::string &name, const std::string &path);

        ~ConfigCpp();

        void SetConfigName(const std::string &name);

        void AddConfigPath(const std::string &path);

        void WatchConfig();

        void OnConfigChange(Callback &);

        bool ReadInConfig();

        ConfigType GetConfigType() const;

        std::string GetConfigData() const;

        template<typename T>
        bool UnmarshalYaml(T &t) {
            switch(GetConfigType()) {
                case ConfigType::UNKNOWN: return false;
                case ConfigType::YAML: return YamlImpl::Unmarshal(t, GetConfigData());
                case ConfigType::JSON: return false;
            }
            return false;
        }

        template<typename T>
        bool UnmarshalJson(T &t) {
            switch(GetConfigType()) {
                case ConfigType::UNKNOWN: return false;
                case ConfigType::YAML: return false;
                case ConfigType::JSON: return JsonImpl::Unmarshal(t, GetConfigData());
            }
            return false;
        }

        bool IsSet(const std::string &key) const;

        bool GetBool(const std::string &key) const;
        int GetInt(const std::string &key) const;
        double GetDouble(const std::string &key) const;
        std::string GetString(const std::string &key) const;
        

    private:
        struct st_impl;
        std::unique_ptr<st_impl> m_pImpl;
    };

}