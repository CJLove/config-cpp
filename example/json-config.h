#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "nlohmann/json.hpp"

using nlohmann::json;

// Sample user-provided datatype matching ../inputs/json/config.json
//
// See https://github.com/nlohmann/json/blob/develop/README.md for converting to/from arbitrary types

struct JsonConfig
{
    std::string top_string;
    int top_int;
    bool top_bool;

    std::vector<std::string> top_list;

    std::map<std::string, std::string> top_dict;

    std::vector<std::vector<std::string>> nested_list;

    std::map<std::string, std::map<std::string, std::string>> nested_dict;

    bool operator==(const JsonConfig &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const JsonConfig &s);

void to_json(json &j, const JsonConfig &s);

void from_json(const json &j, JsonConfig &s);






