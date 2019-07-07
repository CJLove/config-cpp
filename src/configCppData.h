#pragma once
#include <string>

#include "value.h"

namespace ConfigCpp {

class ConfigCppBase {
public:
    ConfigCppBase() = default;

    ConfigCppBase(const ConfigCppBase &rhs) = default;

    ConfigCppBase(ConfigCppBase &&rhs) = default;

    virtual ~ConfigCppBase() = default;

    ConfigCppBase &operator=(const ConfigCppBase &rhs) = default;

    ConfigCppBase &operator=(ConfigCppBase &&rhs) noexcept = default;

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

    ConfigCppData(const ConfigCppData &rhs) { m_handler = rhs.m_handler; }

    ConfigCppData(ConfigCppData &&rhs) noexcept { m_handler = std::move(rhs.m_handler); }

    ~ConfigCppData() override = default;

    ConfigCppData &operator=(const ConfigCppData &rhs) {
        if (this != &rhs) {
            m_handler = rhs.m_handler;
        }
        return *this;
    }

    ConfigCppData &operator=(ConfigCppData &&rhs) noexcept {
        if (this != &rhs) {
            m_handler = std::move(rhs.m_handler);
        }
        return *this;
    }

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