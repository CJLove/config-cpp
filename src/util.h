#pragma once

#include <string>
#include <vector>

namespace ConfigCpp {

std::vector<std::string> split(const std::string &str, const char delim);

std::string prefix(const std::string &str);

std::string symlinkName(const std::string &str);

std::string normalizePath(const std::string &path);

std::string longOption(const std::string &option);
}  // namespace ConfigCpp
