#pragma once

#include <string>
#include <vector>

namespace ConfigCpp {

struct DefaultValue {
    enum DefaultType { BOOL, INT, DOUBLE, STRING };
    std::string m_key;
    DefaultType m_type;
    bool m_bool;
    int m_int;
    double m_double;
    std::string m_string;

    DefaultValue(const DefaultValue &rhs) {
        m_key = rhs.m_key;
        m_type = rhs.m_type;
        m_bool = rhs.m_bool;
        m_int = rhs.m_int;
        m_double = rhs.m_double;
        m_string = rhs.m_string;
    }

    DefaultValue(const std::string &key, const bool &boolVal)
        : m_key(key), m_type(BOOL), m_bool(boolVal), m_int(0), m_double(0) {}

    DefaultValue(const std::string &key, const int &intVal)
        : m_key(key), m_type(INT), m_bool(false), m_int(intVal), m_double(0) {}

    DefaultValue(const std::string &key, const double &doubleVal)
        : m_key(key), m_type(DOUBLE), m_bool(false), m_int(0), m_double(doubleVal) {}

    DefaultValue(const std::string &key, const std::string &stringVal)
        : m_key(key), m_type(STRING), m_bool(false), m_int(0), m_double(0), m_string(stringVal) {}

    DefaultValue(const std::string &key, const char *stringVal)
        : m_key(key), m_type(STRING), m_bool(false), m_int(0), m_double(0), m_string(stringVal) {}

    ~DefaultValue() {}
};

using DefaultValues = std::vector<DefaultValue>;

}  // namespace ConfigCpp