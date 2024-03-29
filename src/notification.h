#pragma once
#include "event.h"
#include <chrono>
#include <string>

namespace ConfigCpp {

struct Notification {
    Notification(const Event& event, std::string path, std::chrono::steady_clock::time_point time);

    Event m_event;
    std::string m_path;
    std::chrono::steady_clock::time_point m_time;
};

}  // namespace ConfigCpp