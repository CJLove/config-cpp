#pragma once

#include <chrono>
#include <string>

namespace ConfigCpp {
class FileSystemEvent {
public:
    FileSystemEvent();

    FileSystemEvent(int wd, uint32_t mask, const std::string& path, const std::chrono::steady_clock::time_point& eventTime);

    ~FileSystemEvent();

public:
    int m_wd;
    uint32_t m_mask;
    std::string m_path;
    std::chrono::steady_clock::time_point m_eventTime;
};

}  // namespace ConfigCpp