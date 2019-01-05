#pragma once

#include <string>
#include <vector>

namespace ConfigCpp {

struct Value {
    enum DefaultType { BOOL, INT, DOUBLE, STRING };
    std::string m_key;
    DefaultType m_type;
    bool m_bool;
    int m_int;
    double m_double;
    std::string m_string;

    Value(const Value &rhs);

    Value(const std::string &key, const bool &boolVal);

    Value(const std::string &key, const int &intVal);

    Value(const std::string &key, const double &doubleVal);

    Value(const std::string &key, const std::string &stringVal);

    Value(const std::string &key, const char *stringVal);

    ~Value();
};

using Values = std::vector<Value>;

}  // namespace ConfigCpp