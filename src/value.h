#pragma once

#include <string>
#include <vector>

namespace ConfigCpp {

struct Value {
    enum ValueType { BOOL, INT, DOUBLE, STRING };
    double m_double;
    int m_int;
    std::string m_key;
    std::string m_string;
    ValueType m_type;
    bool m_bool;

    Value(const Value &rhs) = default;

    Value(Value &&rhs) = default;

    Value(std::string key, const bool &boolVal);

    Value(std::string key, const int &intVal);

    Value(std::string key, const double &doubleVal);

    Value(std::string key, std::string stringVal);

    Value(std::string key, const char *stringVal);

    ~Value() = default;

    Value &operator=(const Value &rhs) = default;

    Value &operator=(Value &&rhs) = default;
};

using Values = std::vector<Value>;

}  // namespace ConfigCpp
