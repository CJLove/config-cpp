#include <fstream>
#include <iostream>
#include <vector>

#include "config-cpp/config-cpp.h"

#include "configCppData.h"

#include "jsonHandler.h"

#include "yamlHandler.h"

namespace ConfigCpp {

struct ConfigCpp::st_impl {
    int m_argc;
    char **m_argv;
    std::string m_name;
    std::vector<std::string> m_path;
    Callback *m_callback;
    ConfigType m_type;
    std::unique_ptr<ConfigCppBase> m_data;

    std::string m_config;

    st_impl(int argc, char **argv) : m_argc(argc), m_argv(argv), m_type(ConfigType::UNKNOWN) {}

    st_impl(int argc, char **argv, const std::string &name, const std::string &path)
        : m_argc(argc), m_argv(argv), m_name(name), m_type(ConfigType::UNKNOWN) {
        m_path.push_back(path);
    }
};

ConfigCpp::ConfigCpp(int argc, char **argv) : m_pImpl(std::make_unique<ConfigCpp::st_impl>(argc, argv)) {}

ConfigCpp::ConfigCpp(int argc, char **argv, const std::string &name, const std::string &path)
    : m_pImpl(std::make_unique<ConfigCpp::st_impl>(argc, argv, name, path)) {}

ConfigCpp::~ConfigCpp() {}

void ConfigCpp::SetConfigName(const std::string &name) { m_pImpl->m_name = name; }

void ConfigCpp::AddConfigPath(const std::string &path) { m_pImpl->m_path.push_back(path); }

void ConfigCpp::WatchConfig() {}

void ConfigCpp::OnConfigChange(Callback &callback) { m_pImpl->m_callback = &callback; }

bool ConfigCpp::ReadInConfig() {
    static std::vector<std::string> yamlExtensions = {".yml", ".yaml"};
    static std::vector<std::string> jsonExtensions = {".json"};

    for (const auto &path : m_pImpl->m_path) {
        auto name = path + m_pImpl->m_name;

        if (m_pImpl->m_type == ConfigType::UNKNOWN || m_pImpl->m_type == ConfigType::YAML) {
            for (const auto ext : yamlExtensions) {
                std::cout << "Trying " << name + ext << "\n";
                std::ifstream file(name + ext);
                if (file.good()) {
                    std::stringstream stream;
                    stream << file.rdbuf();
                    m_pImpl->m_type = ConfigType::YAML;
                    m_pImpl->m_config = stream.str();
                    m_pImpl->m_data.reset(new ConfigCppData<YamlHandler>(m_pImpl->m_config));
                    return true;
                }
            }
        }
        if (m_pImpl->m_type == ConfigType::UNKNOWN || m_pImpl->m_type == ConfigType::JSON) {
            for (const auto ext : jsonExtensions) {
                std::cout << "Trying " << name + ext << "\n";
                std::ifstream file(name + ext);
                if (file.good()) {
                    std::stringstream stream;
                    stream << file.rdbuf();
                    m_pImpl->m_type = ConfigType::JSON;
                    m_pImpl->m_config = stream.str();
                    m_pImpl->m_data.reset(new ConfigCppData<JsonHandler>(m_pImpl->m_config));
                    return true;
                }
            }
        }
    }

    return false;
}

std::string ConfigCpp::GetConfigData() const { return m_pImpl->m_config; }

ConfigType ConfigCpp::GetConfigType() const { return m_pImpl->m_type; }

bool ConfigCpp::IsSet(const std::string &key) const {
    if (m_pImpl->m_data) return m_pImpl->m_data->IsSet(key);
    return false;
}


bool ConfigCpp::GetBool(const std::string &key) const {
    if (m_pImpl->m_data) return m_pImpl->m_data->GetBool(key);
    return false;
}

int ConfigCpp::GetInt(const std::string &key) const {
    if (m_pImpl->m_data) return m_pImpl->m_data->GetInt(key);
    return 0;
}

double ConfigCpp::GetDouble(const std::string &key) const {
    if (m_pImpl->m_data) return m_pImpl->m_data->GetDouble(key);
    return 0.0;
}

std::string ConfigCpp::GetString(const std::string &key) const {
    if (m_pImpl->m_data) return m_pImpl->m_data->GetString(key);
    return "";
}

}  // namespace ConfigCpp