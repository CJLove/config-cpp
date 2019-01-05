#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "config-cpp/config-cpp.h"

#include "configCppData.h"
#include "default.h"
#include "inotify.h"
#include "notification.h"
#include "util.h"

#if defined(JSON_SUPPORT)
#include "jsonHandler.h"
#endif

#if defined(YAML_SUPPORT)
#include "yamlHandler.h"
#endif

namespace ConfigCpp {

/**
 * @brief Private implementation struct
 */
struct ConfigCpp::st_impl {
    ConfigCpp &m_config;
    int m_argc;
    char **m_argv;
    std::string m_name;
    std::vector<std::string> m_path;
    Callback m_callback;
    ConfigType m_type;
    std::string m_configFileName;
    std::unique_ptr<ConfigCppBase> m_data;
    std::unique_ptr<Inotify> m_inotify;
    DefaultValues m_defaults;

    void handleNotification(Notification notification);
    //void handleUnexpectedNotification(Notification notification);

    st_impl(ConfigCpp &config, int argc, char **argv)
        : m_config(config), m_argc(argc), m_argv(argv), m_type(ConfigType::UNKNOWN), m_inotify(std::make_unique<Inotify>()) {}

    st_impl(ConfigCpp &config, int argc, char **argv, const std::string &name, const std::string &path)
        : m_config(config),
          m_argc(argc),
          m_argv(argv),
          m_name(name),
          m_type(ConfigType::UNKNOWN),
          m_inotify(std::make_unique<Inotify>()) {
        m_path.push_back(normalizePath(path));
    }

    ~st_impl() {
        // Stop the inotify thread if it is running
        if (!m_inotify->hasStopped())
            m_inotify->stop();
    }
};

void ConfigCpp::st_impl::handleNotification(Notification notification) {
    std::cout << "Registered Event " << notification.m_event << " on " << notification.m_path << " at "
              << notification.m_time.time_since_epoch().count() << " was triggered\n";

    if (m_callback)
        m_callback(m_config);

    // (re)-register watch for the symlink
    if (notification.m_event == Event::remove) {
        m_inotify->unwatchFile(symlinkName(m_configFileName));
        m_inotify->watchFile(symlinkName(m_configFileName));
    }
}

// For Troubleshooting
// void ConfigCpp::st_impl::handleUnexpectedNotification(Notification notification) {
//     std::cout << "Unregistered Event " << notification.m_event << " on " << notification.m_path << " at "
//               << notification.m_time.time_since_epoch().count() << " was triggered\n";
// }

ConfigCpp::ConfigCpp(int argc, char **argv) : m_pImpl(std::make_unique<ConfigCpp::st_impl>(*this, argc, argv)) {}

ConfigCpp::ConfigCpp(int argc, char **argv, const std::string &name, const std::string &path)
    : m_pImpl(std::make_unique<ConfigCpp::st_impl>(*this, argc, argv, name, path)) {}

ConfigCpp::~ConfigCpp() {}

void ConfigCpp::SetConfigName(const std::string &name) { m_pImpl->m_name = name; }

void ConfigCpp::AddConfigPath(const std::string &path) { m_pImpl->m_path.push_back(normalizePath(path)); }

void ConfigCpp::WatchConfig() {
    std::cout << "Adding watch for " << m_pImpl->m_configFileName << "\n";
    m_pImpl->m_inotify->watchFile(m_pImpl->m_configFileName);
    // In case this is a Kubernetes ConfigMap, also register a watch for the symlink, which gets
    // removed and replaced when the ConfigMap is updated.
    m_pImpl->m_inotify->watchFile(symlinkName(m_pImpl->m_configFileName));
    auto events = {
        Event::close_write,
        Event::remove,
    };
    auto handler = [this](Notification note) { this->m_pImpl->handleNotification(note); };

    for (const auto &event : events) {
        // m_pImpl->m_inotify->setEventMask(m_pImpl->m_inotify->getEventMask() | static_cast<std::uint32_t>(event));
        m_pImpl->m_inotify->onEvent(event, handler);
    }
    // For troubleshooting:
    // auto unexpectedHandler = [this](Notification note) { this->m_pImpl->handleUnexpectedNotification(note); };
    // m_pImpl->m_inotify->onUnexpectedEvent(unexpectedHandler);

    m_pImpl->m_inotify->start();
}

void ConfigCpp::OnConfigChange(Callback callback) { m_pImpl->m_callback = callback; }

bool ConfigCpp::ReadInConfig() {
    static std::vector<std::string> yamlExtensions = {".yml", ".yaml"};
    static std::vector<std::string> jsonExtensions = {".json"};

    for (const auto &path : m_pImpl->m_path) {
        auto name = path + m_pImpl->m_name;
#if defined(YAML_SUPPORT)
        if (m_pImpl->m_type == ConfigType::UNKNOWN || m_pImpl->m_type == ConfigType::YAML) {
            for (const auto ext : yamlExtensions) {
                std::cout << "Trying " << name + ext << "\n";
                std::ifstream file(name + ext);
                if (file.good()) {
                    try {
                        std::stringstream stream;
                        stream << file.rdbuf();
                        m_pImpl->m_configFileName = name + ext;
                        m_pImpl->m_type = ConfigType::YAML;
                        m_pImpl->m_data.reset(new ConfigCppData<YamlHandler>(stream.str(), m_pImpl->m_defaults));
                        return true;
                    } catch (...) {
                        std::cout << "Failed to read " << name + ext << "\n";
                    }
                }
            }
        }
#endif
#if defined(JSON_SUPPORT)
        if (m_pImpl->m_type == ConfigType::UNKNOWN || m_pImpl->m_type == ConfigType::JSON) {
            for (const auto ext : jsonExtensions) {
                std::cout << "Trying " << name + ext << "\n";
                std::ifstream file(name + ext);
                if (file.good()) {
                    try {
                        std::stringstream stream;
                        stream << file.rdbuf();
                        m_pImpl->m_configFileName = name + ext;
                        m_pImpl->m_type = ConfigType::JSON;
                        m_pImpl->m_data.reset(new ConfigCppData<JsonHandler>(stream.str(), m_pImpl->m_defaults));
                        return true;
                    } catch (...) {
                        std::cout << "Failed to read " << name + ext << "\n";
                    }
                }
            }
        }
#endif
    }
    return false;
}

std::string ConfigCpp::GetConfigData() const {
    if (m_pImpl->m_data)
        return m_pImpl->m_data->GetConfig();
    return "";
}

ConfigType ConfigCpp::GetConfigType() const { return m_pImpl->m_type; }

bool ConfigCpp::IsSet(const std::string &key) const {
    if (m_pImpl->m_data)
        return m_pImpl->m_data->IsSet(key);
    return false;
}

bool ConfigCpp::GetBool(const std::string &key) const {
    if (m_pImpl->m_data)
        return m_pImpl->m_data->GetBool(key);
    return false;
}

int ConfigCpp::GetInt(const std::string &key) const {
    if (m_pImpl->m_data)
        return m_pImpl->m_data->GetInt(key);
    return 0;
}

double ConfigCpp::GetDouble(const std::string &key) const {
    if (m_pImpl->m_data)
        return m_pImpl->m_data->GetDouble(key);
    return 0.0;
}

std::string ConfigCpp::GetString(const std::string &key) const {
    if (m_pImpl->m_data)
        return m_pImpl->m_data->GetString(key);
    return "";
}

void ConfigCpp::SetDefault(const std::string &key, const bool &boolVal) {
    if (m_pImpl) {
        DefaultValue def(key, boolVal);
        m_pImpl->m_defaults.push_back(def);
    }
}

void ConfigCpp::SetDefault(const std::string &key, const int &intVal) {
    if (m_pImpl) {
        DefaultValue def(key, intVal);
        m_pImpl->m_defaults.push_back(def);
    }
}

void ConfigCpp::SetDefault(const std::string &key, const double &doubleVal) {
    if (m_pImpl) {
        DefaultValue def(key, doubleVal);
        m_pImpl->m_defaults.push_back(def);
    }
}

void ConfigCpp::SetDefault(const std::string &key, const char *stringVal) {
    if (m_pImpl) {
        DefaultValue def(key,std::string(stringVal));
        m_pImpl->m_defaults.push_back(def);
    }
}

void ConfigCpp::SetDefault(const std::string &key, const std::string &stringVal) {
    if (m_pImpl) {
        DefaultValue def(key, stringVal);
        m_pImpl->m_defaults.push_back(def);
    }
}

}  // namespace ConfigCpp