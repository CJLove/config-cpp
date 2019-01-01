#pragma once

#include <string>
#include <functional>
#include <memory>

#if defined(YAML_SUPPORT)
    // Helper function for unmarshaling yaml
    #include "config-cpp/unmarshal-yaml.h"
#endif

#if defined(JSON_SUPPORT)
    // Helper function for unmarshaling json
    #include "config-cpp/unmarshal-json.h"
#endif

namespace ConfigCpp {
    class ConfigCpp;

    /**
     * @brief Function pointer type for callbacks
     * 
     */
    using Callback = std::function<void(ConfigCpp &)>;

    /**
     * @brief Configuration data types
     * 
     */
    enum class ConfigType { YAML, JSON, UNKNOWN };

    /**
     * @brief The ConfigCpp class is the primary interface to config data coming from a 
     *          variety of sources.  The order of precedence from highest to lowest:
     * 
     * 1. Command-line arguments
     * 2. Environment variables
     * 3. Config files
     * 4. Default values
     * 
     */
    class ConfigCpp {
    public:
        /**
         * @brief Construct a new Config Cpp object
         * 
         * @param argc - number of command line args as passed to main()
         * @param argv - array of command line args as passed to main()
         */
        ConfigCpp(int argc, char **argv);

        /**
         * @brief Construct a new Config Cpp object
         * 
         * @param argc - number of command line args as passed to main()
         * @param argv - array of command line args as passed to main()
         * @param name - configuration name, used as the basename for config files
         * @param path - initial search directory for config files
         */
        ConfigCpp(int argc, char **argv, const std::string &name, const std::string &path);

        /**
         * @brief Destroy the Config Cpp object
         * 
         */
        ~ConfigCpp();

        /**
         * @brief Set the Config Name object
         * 
         * @param name - configuration name, used as the basename for config files
         */
        void SetConfigName(const std::string &name);

        /**
         * @brief Add a directory to the search path for config files
         * 
         * @param path - absolute path to a directory
         */
        void AddConfigPath(const std::string &path);

        /**
         * @brief Watch for config file changes
         * 
         */
        void WatchConfig();

        /**
         * @brief Specify the callback function to be invoked upon config file changes
         * 
         * @param function - std::function argument
         */
        void OnConfigChange(Callback function);

        /**
         * @brief Read in config file located anywhere in the search path
         * 
         * @return true - configuration file was read in successfully
         * @return false - configuration file was not read in successfully
         */
        bool ReadInConfig();

        /**
         * @brief Get the Config Type object
         * 
         * @return ConfigType - UNKNOWN, JSON, or YAML
         */
        ConfigType GetConfigType() const;

        /**
         * @brief Get the Config Data object
         * 
         * @return std::string - Configuration data as read from config files
         */
        std::string GetConfigData() const;

        /**
         * @brief Unmarshal YAML configuration data to a user-provided type
         * 
         * @tparam T - user-provided type
         * @param t - reference to variable of the user-provided type
         * @return true - unmarshal was successful
         * @return false - unmarshal was unsuccessful
         */
        template<typename T>
        bool UnmarshalYaml(T &t) {
#if defined(YAML_SUPPORT)            
            switch(GetConfigType()) {
                case ConfigType::UNKNOWN: return false;
                case ConfigType::YAML: return YamlImpl::Unmarshal(t, GetConfigData());
                case ConfigType::JSON: return false;
            }
#endif            
            return false;
        }

        /**
         * @brief Unmarshal JSON configuration data to a user-provided type
         * 
         * @tparam T - user-provided type
         * @param t - reference to variable of the user-provided type 
         * @return true - unmarshal was successful
         * @return false - unmarshal was unsuccessful
         */
        template<typename T>
        bool UnmarshalJson(T &t) {
#if defined(JSON_SUPPORT)            
            switch(GetConfigType()) {
                case ConfigType::UNKNOWN: return false;
                case ConfigType::YAML: return false;
                case ConfigType::JSON: return JsonImpl::Unmarshal(t, GetConfigData());
            }
#endif            
            return false;
        }

        /**
         * @brief Return indication of whether a particular key is in the config data
         * 
         * @param key - full path to a key
         * @return true - key is available in the config data
         * @return false - key is not available in the config data
         */
        bool IsSet(const std::string &key) const;

        /**
         * @brief Retrieve a particular key as a boolean value
         * 
         * @param key - full path to a key
         * @return boolean
         */
        bool GetBool(const std::string &key) const;

        /**
         * @brief Retrieve a particular key as an integer value
         * 
         * @param key - full path to a key
         * @return int 
         */
        int GetInt(const std::string &key) const;

        /**
         * @brief Retrieve a particular key as a double value
         * 
         * @param key - full path to a key
         * @return double 
         */
        double GetDouble(const std::string &key) const;

        /**
         * @brief Retrieve a particular key as a string value
         * 
         * @param key - full path to a key
         * @return std::string 
         */
        std::string GetString(const std::string &key) const;

        /**
         * @brief Set the default value for a particular key as a boolean
         * 
         * @param key 
         * @param boolVal 
         */
        void SetDefault(const std::string &key, const bool &boolVal);

        /**
         * @brief Set the default value for a particular key as an integer value
         * 
         * @param key 
         * @param intVal 
         */
        void SetDefault(const std::string &key, const int &intVal);

        /**
         * @brief Set the default value for a particular key as a double value
         * 
         * @param key 
         * @param doubleVal 
         */
        void SetDefault(const std::string &key, const double &doubleVal);

        /**
         * @brief Set the default value for a particular key as a string value
         * 
         * @param key 
         * @param stringVal 
         */
        void SetDefault(const std::string &key, const std::string &stringVal);

    private:
        struct st_impl;
        std::unique_ptr<st_impl> m_pImpl;
    };

}