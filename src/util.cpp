#include "util.h"
#include <sstream>

namespace ConfigCpp {
std::vector<std::string> split(const std::string &str, const char delim) {
    std::vector<std::string> strings;
    std::istringstream stream(str);
    std::string s;
    while (std::getline(stream, s, delim)) {
        strings.push_back(s);
    }
    return strings;
}

std::string prefix(const std::string &str) {
    auto pos = str.find_last_of('.');
    if (pos != std::string::npos) {
        return str.substr(0,pos);
    }
    return str;
}

}  // namespace ConfigCpp