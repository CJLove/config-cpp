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

std::string symlinkName(const std::string &str) {
    auto symlink = str;
    auto pos = symlink.find_last_of('/');
    if (pos != std::string::npos) {
        symlink.erase(pos);
        symlink += "/";
        symlink += "..data";
    }
    return symlink;
}

std::string normalizePath(const std::string &path) {
    auto ret = path;
    if (!ret.empty()) {
        if (ret[ret.size()-1] != '/') {
            ret += '/';
        }
    }
    return ret;
}

std::string longOption(const std::string &option) {
    auto longOption = option;
    auto pos = option.find(',');
    if (pos != std::string::npos) {
        longOption = option.substr(pos+1);
    }
    return longOption;
}

}  // namespace ConfigCpp