#include <chrono>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

#include "config-cpp/config-cpp.h"

#include "configCppData.h"
#include "inotify.h"
#include "notification.h"
#include "util.h"
#include "value.h"

#if defined(JSON_SUPPORT)
#include "jsonHandler.h"
#endif

#if defined(YAML_SUPPORT)
#include "yamlHandler.h"
#endif

#if defined(TOML_SUPPORT)
#include "tomlHandler.h"
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
    Values m_defaults;
    cxxopts::Options m_options;
    std::map<std::string, Value::ValueType> m_optionTypes;
    Values m_cmdLineArgs;
    bool m_cmdLineParsed;

    void handleNotification(const Notification &notification);
    // void handleUnexpectedNotification(Notification notification);

    st_impl(ConfigCpp &config, int argc, char **argv)
        : m_config(config),
          m_argc(argc),
          m_argv(argv),
          m_type(ConfigType::UNKNOWN),
          m_inotify(std::make_unique<Inotify>()),
          m_options(argv != nullptr ? argv[0] : ""),
          m_cmdLineParsed(false) {
        m_options.add_options()("help", "Print help");
    }

    st_impl(ConfigCpp &config, int argc, char **argv, std::string name, const std::string &path)
        : m_config(config),
          m_argc(argc),
          m_argv(argv),
          m_name(std::move(name)),
          m_type(ConfigType::UNKNOWN),
          m_inotify(std::make_unique<Inotify>()),
          m_options(argv != nullptr ? argv[0] : ""),
          m_cmdLineParsed(false) {
        m_path.push_back(normalizePath(path));
        m_options.add_options()("help", "Print help");
    }

    st_impl(const st_impl &rhs) = delete;
    st_impl(st_impl &&rhs) noexcept = delete;

    ~st_impl() = default;

    st_impl &operator=(const st_impl &rhs) = delete;
    st_impl &operator=(st_impl &&rhs) noexcept = delete;
};

void ConfigCpp::st_impl::handleNotification(const Notification &notification) {
    // std::cout << "Registered Event " << notification.m_event << " on " << notification.m_path << " at "
    //           << notification.m_time.time_since_epoch().count() << " was triggered\n";

    if (m_callback) {
        m_callback(m_config);
    }

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

ConfigCpp::ConfigCpp(int argc, char **argv, std::string &name, std::string &path)
    : m_pImpl(std::make_unique<ConfigCpp::st_impl>(*this, argc, argv, name, std::move(path))) {}

ConfigCpp::~ConfigCpp()
{
        // Stop the inotify thread if it is running
        if (!m_pImpl->m_inotify->hasStopped()) {
            m_pImpl->m_inotify->stop();
        }
}

void ConfigCpp::SetConfigName(std::string name) { m_pImpl->m_name = std::move(name); }

void ConfigCpp::AddConfigPath(const std::string &path) { m_pImpl->m_path.push_back(std::move(normalizePath(path))); }

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
    auto handler = [this](const Notification &note) { this->m_pImpl->handleNotification(note); };

    for (const auto &event : events) {
        // m_pImpl->m_inotify->setEventMask(m_pImpl->m_inotify->getEventMask() | static_cast<std::uint32_t>(event));
        m_pImpl->m_inotify->onEvent(event, handler);
    }
    // For troubleshooting:
    // auto unexpectedHandler = [this](Notification note) { this->m_pImpl->handleUnexpectedNotification(note); };
    // m_pImpl->m_inotify->onUnexpectedEvent(unexpectedHandler);

    m_pImpl->m_inotify->start();
}

void ConfigCpp::OnConfigChange(Callback callback) { m_pImpl->m_callback = std::move(callback); }

bool ConfigCpp::ReadInConfig() {
    if (!m_pImpl->m_cmdLineParsed && !m_pImpl->m_optionTypes.empty()) {
        try {
            auto result = m_pImpl->m_options.parse(m_pImpl->m_argc, m_pImpl->m_argv);
            if (result.count("help") != 0) {
                std::cout << m_pImpl->m_options.help({""}) << std::endl;
                return false;
            }
            auto args = result.arguments();
            for (const auto &arg : args) {
                switch (m_pImpl->m_optionTypes[arg.key()]) {
                    case Value::ValueType::BOOL:
                        m_pImpl->m_cmdLineArgs.push_back(Value(arg.key(), arg.as<bool>()));
                        break;
                    case Value::ValueType::INT:
                        m_pImpl->m_cmdLineArgs.push_back(Value(arg.key(), arg.as<int>()));
                        break;
                    case Value::ValueType::STRING:
                        m_pImpl->m_cmdLineArgs.push_back(Value(arg.key(), arg.as<std::string>()));
                        break;
                    case Value::ValueType::DOUBLE:
                        m_pImpl->m_cmdLineArgs.push_back(Value(arg.key(), arg.as<double>()));
                        break;
                }
            }
            m_pImpl->m_cmdLineParsed = true;
        } catch (cxxopts::OptionParseException &e) {
            // Exception from cxxopts parsing the arguments.  Output the error and usage.
            std::cout << "Error: " << e.what() << "\n";
            std::cout << m_pImpl->m_options.help({""}) << std::endl;
            return false;
        } catch (std::exception &e) {
            // Anything else is in the subsequent processing of arguments
            std::cout << "Caught " << e.what() << "\n";
            return  false;
        }
    }
    static std::vector<std::string> yamlExtensions = {".yml", ".yaml"};
    static std::vector<std::string> jsonExtensions = {".json"};
    static std::vector<std::string> tomlExtensions = {".toml"};

    for (const auto &path : m_pImpl->m_path) {
        auto name = path + m_pImpl->m_name;
#if defined(YAML_SUPPORT)
        if (m_pImpl->m_type == ConfigType::UNKNOWN || m_pImpl->m_type == ConfigType::YAML) {
            for (const auto &ext : yamlExtensions) {
                std::ifstream file(name + ext);
                if (file.good()) {
                    try {
                        std::stringstream stream;
                        stream << file.rdbuf();
                        m_pImpl->m_configFileName = name + ext;
                        m_pImpl->m_type = ConfigType::YAML;
                        m_pImpl->m_data = std::make_unique<ConfigCppData<YamlHandler>>(stream.str(), m_pImpl->m_defaults, m_pImpl->m_cmdLineArgs);
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
            for (const auto &ext : jsonExtensions) {
                std::ifstream file(name + ext);
                if (file.good()) {
                    try {
                        std::stringstream stream;
                        stream << file.rdbuf();
                        m_pImpl->m_configFileName = name + ext;
                        m_pImpl->m_type = ConfigType::JSON;
                        m_pImpl->m_data = std::make_unique<ConfigCppData<JsonHandler>>(stream.str(), m_pImpl->m_defaults, m_pImpl->m_cmdLineArgs);
                        return true;
                    } catch (...) {
                        std::cout << "Failed to read " << name + ext << "\n";
                    }
                }
            }
        }
#endif
#if defined(TOML_SUPPORT)
        if (m_pImpl->m_type == ConfigType::UNKNOWN || m_pImpl->m_type == ConfigType::TOML) {
            for (const auto &ext : tomlExtensions) {
                std::ifstream file(name + ext);
                if (file.good()) {
                    try {
                        std::stringstream stream;
                        stream << file.rdbuf();
                        m_pImpl->m_configFileName = name + ext;
                        m_pImpl->m_type = ConfigType::TOML;
                        m_pImpl->m_data = std::make_unique<ConfigCppData<TomlHandler>>(stream.str(), m_pImpl->m_defaults, m_pImpl->m_cmdLineArgs);
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
    if (m_pImpl->m_data) {
        return m_pImpl->m_data->GetConfig();
    }
    return "";
}

ConfigType ConfigCpp::GetConfigType() const { return m_pImpl->m_type; }

bool ConfigCpp::IsSet(const std::string &key) const {
    if (m_pImpl->m_data) {
        return m_pImpl->m_data->IsSet(key);
    }
    return false;
}

bool ConfigCpp::GetBool(const std::string &key) const {
    if (m_pImpl->m_data) {
        return m_pImpl->m_data->GetBool(key);
    }
    return false;
}

int ConfigCpp::GetInt(const std::string &key) const {
    if (m_pImpl->m_data) {
        return m_pImpl->m_data->GetInt(key);
    }
    return 0;
}

double ConfigCpp::GetDouble(const std::string &key) const {
    if (m_pImpl->m_data) {
        return m_pImpl->m_data->GetDouble(key);
    }
    return 0.0;
}

std::string ConfigCpp::GetString(const std::string &key) const {
    if (m_pImpl->m_data) {
        return m_pImpl->m_data->GetString(key);
    }
    return "";
}

void ConfigCpp::SetDefault(const std::string &key, const bool &boolVal) {
    if (m_pImpl) {
        Value def(key, boolVal);
        m_pImpl->m_defaults.push_back(def);
    }
}

void ConfigCpp::SetDefault(const std::string &key, const int &intVal) {
    if (m_pImpl) {
        Value def(key, intVal);
        m_pImpl->m_defaults.push_back(def);
    }
}

void ConfigCpp::SetDefault(const std::string &key, const double &doubleVal) {
    if (m_pImpl) {
        Value def(key, doubleVal);
        m_pImpl->m_defaults.push_back(def);
    }
}

void ConfigCpp::SetDefault(const std::string &key, const char *stringVal) {
    if (m_pImpl) {
        Value def(key, std::string(stringVal));
        m_pImpl->m_defaults.push_back(def);
    }
}

void ConfigCpp::SetDefault(const std::string &key, const std::string &stringVal) {
    if (m_pImpl) {
        Value def(key, stringVal);
        m_pImpl->m_defaults.push_back(def);
    }
}

void ConfigCpp::AddBoolOption(const std::string &name, const std::string &helpString) {
    if (m_pImpl) {
        try {
            m_pImpl->m_options.add_options()(name, helpString);
            m_pImpl->m_optionTypes[longOption(name)] = Value::ValueType::BOOL;
        } catch (std::exception &e) {
        }
    }
}

void ConfigCpp::AddIntOption(const std::string &name, const std::string &helpString) {
    if (m_pImpl) {
        try {
            m_pImpl->m_options.add_options()(name, helpString, cxxopts::value<int>());
            m_pImpl->m_optionTypes[longOption(name)] = Value::ValueType::INT;
        } catch (std::exception &e) {
        }
    }
}

void ConfigCpp::AddStringOption(const std::string &name, const std::string &helpString) {
    if (m_pImpl) {
        try {
            m_pImpl->m_options.add_options()(name, helpString, cxxopts::value<std::string>());
            m_pImpl->m_optionTypes[longOption(name)] = Value::ValueType::STRING;
        } catch (std::exception &e) {
        }
    }
}

void ConfigCpp::AddDoubleOption(const std::string &name, const std::string &helpString) {
    if (m_pImpl) {
        try {
            m_pImpl->m_options.add_options()(name, helpString, cxxopts::value<double>());
            m_pImpl->m_optionTypes[longOption(name)] = Value::ValueType::DOUBLE;
        } catch (std::exception &e) {
        }
    }
}

void ConfigCpp::Version(uint32_t &major, uint32_t &minor, uint32_t &patch) {
    major = CONFIG_CPP_VERSION_MAJOR;
    minor = CONFIG_CPP_VERSION_MINOR;
    patch = CONFIG_CPP_VERSION_PATCH;
}
}  // namespace ConfigCpp