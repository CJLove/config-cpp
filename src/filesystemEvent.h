#pragma once

#include <chrono>
#include <string>

namespace ConfigCpp {
class FileSystemEvent {
public:
    FileSystemEvent() = default;

    FileSystemEvent(int wd, uint32_t mask, std::string path, const std::chrono::steady_clock::time_point &eventTime);

    FileSystemEvent(const FileSystemEvent &rhs) = default;

    FileSystemEvent(FileSystemEvent &&rhs) = default;

    ~FileSystemEvent() = default;

    FileSystemEvent &operator=(const FileSystemEvent &rhs) = default;

    FileSystemEvent &operator=(FileSystemEvent &&rhs) = default;

public:
    int m_wd = 0;
    uint32_t m_mask = 0;
    std::string m_path;
    std::chrono::steady_clock::time_point m_eventTime;
};

}  // namespace ConfigCpp