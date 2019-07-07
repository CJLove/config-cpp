#include "value.h"

namespace ConfigCpp {

Value::Value(std::string key, const bool &boolVal)
    : m_double(0), m_int(0), m_key(std::move(key)), m_type(BOOL), m_bool(boolVal) {}

Value::Value(std::string key, const int &intVal)
    : m_double(0), m_int(intVal), m_key(std::move(key)), m_type(INT), m_bool(false) {}

Value::Value(std::string key, const double &doubleVal)
    : m_double(doubleVal), m_int(0), m_key(std::move(key)), m_type(DOUBLE), m_bool(false) {}

Value::Value(std::string key, std::string stringVal)
    : m_double(0), m_int(0), m_key(std::move(key)), m_string(std::move(stringVal)), m_type(STRING), m_bool(false) {}

Value::Value(std::string key, const char *stringVal)
    : m_double(0), m_int(0), m_key(std::move(key)), m_string(stringVal), m_type(STRING), m_bool(false) {}

}  // namespace ConfigCpp
