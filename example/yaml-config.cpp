#include "yaml-config.h"

bool YamlConfig::operator==(const YamlConfig &rhs) const {
    return (top_string == rhs.top_string && top_int == rhs.top_int && top_bool == rhs.top_bool && top_list == rhs.top_list &&
            top_dict == rhs.top_dict && nested_list == rhs.nested_list && nested_dict == rhs.nested_dict);
}

std::ostream &operator<<(std::ostream &os, const YamlConfig &s) {
    os << "top_string: " << s.top_string << "\n"
       << "top_int: " << s.top_int << "\n"
       << "top_bool: " << s.top_bool << "\n"
       << "top_list:\n";
    for (const auto &item : s.top_list) {
        os << "  " << item << "\n";
    }
    os << "top_dict:\n";
    for (const auto &item : s.top_dict) {
        os << "  " << item.first << ": " << item.second << "\n";
    }
    for (const auto &item : s.nested_list) {
        os << "  ListElt"
           << "\n";
        for (const auto &subitem : item) {
            os << "    " << subitem << "\n";
        }
    }
    for (const auto &item : s.nested_dict) {
        os << "  " << item.first << "\n";
        for (const auto &subitem : item.second) {
            os << "    " << subitem.first << ": " << subitem.second << "\n";
        }
    }
    return os;
}
