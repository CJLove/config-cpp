#pragma once
#include <string>

namespace ConfigCpp {

class ConfigCppBase {
public:
    virtual ~ConfigCppBase() { }

    virtual bool IsSet(const std::string &key) const = 0;
    
    virtual bool GetBool(const std::string &key) const = 0;
    virtual int GetInt(const std::string &key) const = 0;
    virtual double GetDouble(const std::string &key) const = 0;
    virtual std::string GetString(const std::string &key) const = 0;
};


template<typename T>
    class ConfigCppData: public ConfigCppBase {
    public:
        ConfigCppData(const std::string &data): ConfigCppBase(), m_handler(data) { }

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


    private:
        T m_handler;
    };

}