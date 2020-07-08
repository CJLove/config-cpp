#include "toml-config.h"

bool TomlConfig::operator==(const TomlConfig &rhs) const {
    return (top_string == rhs.top_string &&
        top_int == rhs.top_int &&
        top_bool == rhs.top_bool &&
        top_array == rhs.top_array &&
        top_table == rhs.top_table &&
        nested_array == rhs.nested_array &&
        nested_table == rhs.nested_table
    );
}

std::ostream &operator<<(std::ostream &os, const TomlConfig &s)
{
    os << "top_string: " << s.top_string << "\n"
    << "top_int: " << s.top_int << "\n"
    << "top_bool: " << s.top_bool << "\n"
    << "top_array:\n";
    for (const auto &item: s.top_array) {
        os << "  " << item << "\n";
    }
    os << "top_table:\n";
    for (const auto &item: s.top_table) {
        os << "  " << item.first << ": " << item.second << "\n";
    }
    for (const auto &item: s.nested_array) {
        os << "  ListElt" << "\n";
        for (const auto &subitem: item) {
            os << "    " << subitem << "\n";
        }
    }
    for (const auto &item: s.nested_table) {
        os << "  " << item.first << "\n";
        for (const auto &subitem: item.second) {
            os << "    " << subitem.first << ": " << subitem.second << "\n";
        }
    }
    return os;
}
