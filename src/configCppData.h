#pragma once
#include <string>

#include "default.h"

namespace ConfigCpp {

class ConfigCppBase {
public:
    virtual ~ConfigCppBase() { }

    virtual bool IsSet(const std::string &key) const = 0;
    
    virtual bool GetBool(const std::string &key) const = 0;
    virtual int GetInt(const std::string &key) const = 0;
    virtual double GetDouble(const std::string &key) const = 0;
    virtual std::string GetString(const std::string &key) const = 0;

    virtual std::string GetConfig() const = 0;
};


template<typename T>
    class ConfigCppData: public ConfigCppBase {
    public:
        ConfigCppData(const std::string &data, const DefaultValues &defaults): ConfigCppBase(), m_handler(data,defaults) { }

        virtual ~ConfigCppData() { }

        bool IsSet(const std::string &key) const override {
            return m_handler.IsSet(key);
        }

        bool GetBool(const std::string &key) const override {
            return m_handler.GetBool(key);
        }
        int GetInt(const std::string &key) const override {
            return m_handler.GetInt(key);
        }
        double GetDouble(const std::string &key) const override {
            return m_handler.GetDouble(key);
        }
        std::string GetString(const std::string &key) const override {
            return m_handler.GetString(key);
        }

        std::string GetConfig() const override {
            return m_handler.GetConfig();
        }

    private:
        T m_handler;
    };

}