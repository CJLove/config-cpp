#pragma once
#include <string>

#include "value.h"

namespace ConfigCpp {

class ConfigCppBase {
public:
    ConfigCppBase() = default;

    ConfigCppBase(const ConfigCppBase &rhs) = delete;

    ConfigCppBase(ConfigCppBase &&rhs) = delete;

    virtual ~ConfigCppBase() = default;

    ConfigCppBase &operator=(const ConfigCppBase &rhs) = delete;

    ConfigCppBase &operator=(ConfigCppBase &&rhs) noexcept = delete;

    virtual bool IsSet(const std::string &key) const = 0;

    virtual bool GetBool(const std::string &key) const = 0;
    virtual int GetInt(const std::string &key) const = 0;
    virtual double GetDouble(const std::string &key) const = 0;
    virtual std::string GetString(const std::string &key) const = 0;

    virtual std::string GetConfig() const = 0;
};

template <typename T>
class ConfigCppData : public ConfigCppBase {
public:
    ConfigCppData(const std::string &data, const Values &defaults, const Values &cmdLineArgs)
        : ConfigCppBase(), m_handler(data, defaults, cmdLineArgs) {}

    ConfigCppData(const ConfigCppData &rhs) = delete;

    ConfigCppData(ConfigCppData &&rhs) = delete;

    ~ConfigCppData() override = default;

    ConfigCppData &operator=(const ConfigCppData &rhs) = delete;

    ConfigCppData &operator=(ConfigCppData &&rhs) = delete;

    bool IsSet(const std::string &key) const override { return m_handler.IsSet(key); }

    bool GetBool(const std::string &key) const override { return m_handler.GetBool(key); }
    int GetInt(const std::string &key) const override { return m_handler.GetInt(key); }
    double GetDouble(const std::string &key) const override { return m_handler.GetDouble(key); }
    std::string GetString(const std::string &key) const override { return m_handler.GetString(key); }

    std::string GetConfig() const override { return m_handler.GetConfig(); }

private:
    T m_handler;
};

}  // namespace ConfigCpp