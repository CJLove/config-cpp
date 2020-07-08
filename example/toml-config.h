#pragma once
#include <string>
#include <vector>
#include <map>

#include "toml.hpp"

// TomlConfig user-provided datatype matching ../inputs/TomlConfig.toml

// See https://github.com/ToruNiina/toml11#conversion-between-toml-value-and-arbitrary-types

struct TomlConfig {
    std::string top_string;
    int top_int;
    bool top_bool;

    std::vector<std::string> top_array;

    std::map<std::string, std::string> top_table;

    std::vector<std::vector<std::string>> nested_array;

    std::map<std::string, std::map<std::string, std::string>> nested_table;

    bool operator==(const TomlConfig &rhs) const;

};

std::ostream &operator<<(std::ostream &os, const TomlConfig &s);


// Implement specialization of toml::from for struct TomlConfig
namespace toml
{
    template<>
    struct from<TomlConfig>
    {
        static TomlConfig from_toml(const value& v)
        {
            TomlConfig s;
            s.top_string = find<std::string>(v,"top-string");
            s.top_int    = find<int>(v,"top-int");
            s.top_bool   = find<bool>(v,"top-bool");

            s.top_array   = find<std::vector<std::string>>(v,"top-array");

            s.nested_array = find<std::vector<std::vector<std::string>>>(v,"nested-array");

            s.top_table = find<std::map<std::string,std::string>>(v,"top-table");

            s.nested_table = find<std::map<std::string,std::map<std::string,std::string>>>(v,"nested-table");

            return s;           
        }
    };
}