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

    DefaultValue(const DefaultValue &rhs);

    DefaultValue(const std::string &key, const bool &boolVal);

    DefaultValue(const std::string &key, const int &intVal);

    DefaultValue(const std::string &key, const double &doubleVal);

    DefaultValue(const std::string &key, const std::string &stringVal);

    DefaultValue(const std::string &key, const char *stringVal);

    ~DefaultValue();
};

using DefaultValues = std::vector<DefaultValue>;

}  // namespace ConfigCpp