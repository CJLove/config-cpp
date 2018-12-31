#include "json-config.h"


bool JsonConfig::operator==(const JsonConfig &rhs) const {
    return (top_string == rhs.top_string &&
        top_int == rhs.top_int &&
        top_bool == rhs.top_bool &&
        top_list == rhs.top_list &&
        top_dict == rhs.top_dict &&
        nested_list == rhs.nested_list &&
        nested_dict == rhs.nested_dict
    );
}

std::ostream &operator<<(std::ostream &os, const JsonConfig &s)
{
    os << "top_string: " << s.top_string << "\n"
    << "top_int: " << s.top_int << "\n"
    << "top_bool: " << s.top_bool << "\n"
    << "top_list:\n";
    for (const auto &item: s.top_list) {
        os << "  " << item << "\n";
    }
    os << "top_dict:\n";
    for (const auto &item: s.top_dict) {
        os << "  " << item.first << ": " << item.second << "\n";
    }
    for (const auto &item: s.nested_list) {
        os << "  ListElt" << "\n";
        for (const auto &subitem: item) {
            os << "    " << subitem << "\n";
        }
    }
    for (const auto &item: s.nested_dict) {
        os << "  " << item.first << "\n";
        for (const auto &subitem: item.second) {
            os << "    " << subitem.first << ": " << subitem.second << "\n";
        }
    }
    return os;
}

void to_json(json &j, const JsonConfig &s) {
    j = json{
        {"top-string",s.top_string}, 
        {"top-int", s.top_int}, 
        {"top-bool", s.top_bool},
        {"top-list", s.top_list},
        {"top-dict", s.top_dict}
    };
}

void from_json(const json &j, JsonConfig &s) {
    j.at("top-string").get_to(s.top_string);
    j.at("top-int").get_to(s.top_int);
    j.at("top-bool").get_to(s.top_bool);
    j.at("top-list").get_to(s.top_list);
    j.at("top-dict").get_to(s.top_dict);
    j.at("nested-list").get_to(s.nested_list);
    j.at("nested-dict").get_to(s.nested_dict);
}